/*
    WIZARDS.EXE
        ovr067
        ovr068
        ovr150
*/
/*

// WZD o68p09
// AKA IDK_Draw_MiniMap_s5B828()
void IDK_Draw_MiniMap_s5B828()

// WZD o150p16
// "Reduced Map"/"World Window"
// AKA Draw_Minimap()
void Create_Reduced_Map_Picture(int16_t minimap_start_x, int16_t minimap_start_y, int16_t wp, byte_ptr minimap_pict_seg, int16_t minimap_width, int16_t minimap_height, int16_t mark_x, int16_t mark_y, int16_t mark_flag)

*/

#include "../../STU/src/STU_CHK.h"

#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/MOX_BITS.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/MOX_DAT.h"  /* _players[] */
#include "../../MoX/src/MOX_SET.h"  /* magic_set */
#include "../../MoX/src/MOX_T4.h"

#include "Explore.h"
#include "MainScr.h"
#include "MOM_SCR.h"
#include "SCastScr.h"  /* World_To_Screen() */
#include "Terrain.h"
#include "UnitMove.h"
#include "UNITTYPE.h"

#include <assert.h>
#include <string.h>

#include "MainScr_Maps.h"



enum e_SCOUT_BITS
{
    SCT_BottomLeft   = 1,
    SCT_TopLeft      = 2,
    SCT_TopRight     = 4,
    SCT_BottomRight  = 8
};



// WZD dseg:3278                                                 BEGIN:  ovr068 - Initialized Data

// WZD dseg:3278
/*

SCT_BottomLeft   0x01  00000001
SCT_TopLeft      0x02  00000010
SCT_TopRight     0x04  00000100
SCT_BottomRight  0x08  00001000

*/
uint8_t NoSide_Explores[] =
{
    SCT_BottomLeft                || SCT_TopRight,
    SCT_BottomLeft || SCT_TopLeft || SCT_TopRight,
                      SCT_TopLeft                 || SCT_BottomRight,
    SCT_BottomLeft || SCT_TopLeft                 || SCT_BottomRight,
    SCT_BottomLeft                || SCT_TopRight || SCT_BottomRight,
                      SCT_TopLeft || SCT_TopRight || SCT_BottomRight
};
// WZD dseg:3278                                                 END:  ovr068 - Initialized Data



// WZD dseg:6FF4                                                 BEGIN: ovr150

// WZD dseg:6FF4
// drake178: OVL_TileAnim_Stage
int16_t terrain_anim_ctr = 0;
// WZD dseg:6FF6
// drake178: OVL_EnchRoad_Stage
int16_t road_anim_ctr = 0;
// WZD dseg:6FF8
// drake178: OVL_NodeSprkl_Stage
int16_t node_anim_ctr = 0;
// WZD dseg:6FFA db    0
// WZD dseg:6FFB db    0

// WZD dseg:6FFC
// drake178: OVL_Anim_Stepper
int16_t map_anim_cycle = 0;
// WZD dseg:6FFE
int16_t map_draw_prev_x = ST_UNDEFINED;
// WZD dseg:7000
int16_t map_draw_prev_y = ST_UNDEFINED;
// WZD dseg:7002
// AKA OVL_NewMapSustain
int16_t map_draw_sustain = 2;

// WZD dseg:7002                                                 END: ovr150

// WZD dseg:7004                                                 ovr057
// WZD dseg:7004                                                 ovr150
// WZD dseg:7004
int16_t DBG_ShowTileInfo = ST_FALSE;

// WZD dseg:7006 00                                              db    0
// WZD dseg:7007 00                                              db    0
// WZD dseg:7008 00                                              db    0
// WZD dseg:7009 00                                              db    0

// WZD dseg:700A                                                 BEGIN: Draw_Minimap()
// WZD dseg:700A
/*
    // banner colors: {171, 216, 205, 201, 210, 50}
    // 171 ~ blue
    // 216 ~ green
    // 205 ~ purple
    // 201 ~ red
    // 210 ~ yellow
    //  50 ~ brown
*/
uint8_t COL_MinimapBanners[NUM_BANNER_COLORS] = {
    BANNER_COLOR_BLUE_171, 
    BANNER_COLOR_GREEN,
    BANNER_COLOR_PURPLE_205,
    BANNER_COLOR_RED,
    BANNER_COLOR_YELLOW_210,
    BANNER_COLOR_BROWN
};
// WZD dseg:7010
uint8_t COL_MinimapNeutral = NEUTRAL_PLAYER_BANNER_COLOR;
// END: Draw_Minimap()

// ; the colors to replace indices $D6-DA in the city
// ; images with for the corresponding banner colors;
// ; this is actually a single array of 7 * 5 bytes, the
// ; last two groups of fives not being used in the game
// WZD dseg:7011 60 61 62 63 64                                  COL_City_Banner0 db 60h, 61h, 62h, 63h, 64h
// WZD dseg:7016 48 49 4A 4B 4C                                  COL_City_Banner1 db 48h, 49h, 4Ah, 4Bh, 4Ch
// WZD dseg:701B CD CE CF D0 D1                                  COL_City_Banner2 db 0CDh, 0CEh, 0CFh, 0D0h, 0D1h
// WZD dseg:7020 C9 A5 CB A6 2D                                  COL_City_Banner3 db 0C9h, 0A5h, 0CBh, 0A6h, 2Dh
// WZD dseg:7025 D1 D2 D3 D4 D5                                  COL_City_Banner4 db 0D1h, 0D2h, 0D3h, 0D4h, 0D5h
// WZD dseg:702A 1D 1D 1C 1C 1B                                  UU_COL_City_Banner5 db 1Dh, 1Dh, 1Ch, 1Ch, 1Bh
// WZD dseg:702F 1D 1D 1C 1C 1B                                  UU_COL_City_Banner6 db 1Dh, 1Dh, 1Ch, 1Ch, 1Bh
// WZD dseg:7011
/*
DEDU  ¿ could be uint8_t COL_City_Banner[7][5] ?
// MGC; 1-byte, signed;
enum enum_BANNER_COLOR_ID
    BNR_Blue  = 0,
    BNR_Green  = 1,
    BNR_Purple  = 2,
    BNR_Red  = 3,
    BNR_Yellow  = 4,
    BNR_Brown  = 5

*/
uint8_t COL_City_Banner_7x5[7][5] = 
{
    {  96,  97,  98,  99, 100 },
    {  72,  73,  74,  75,  76 },
    { 205, 206, 207, 208, 209 },  // BANNER_COLOR_PURPLE_205
    { 201, 165, 203, 166,  45 },  // BANNER_COLOR_RED
    { 209, 210, 211, 212, 213 },  // BANNER_COLOR_YELLOW_209
    {  13,  13,  12,  12,  11 },
    {  13,  13,  12,  12,  11 }
};
uint8_t COL_City_Banner[] = {
    0x60, 0x61, 0x62, 0x63, 0x64, 
    0x48, 0x49, 0x4A, 0x4B, 0x4C, 
    0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 
    0xC9, 0xA5, 0xCB, 0xA6, 0x2D, 
    0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 
    0x1D, 0x1D, 0x1C, 0x1C, 0x1B, 
    0x1D, 0x1D, 0x1C, 0x1C, 0x1B, 
};
/*
WZD dseg:7011
; DATA XREF: DEPR_MainScr_Draw_Map_Cities:loc_5AD6Er ...
; the colors to replace indices $D6-DA in the city
; images with for the corresponding banner colors;
; this is actually a single array of 7 * 5 bytes, the
; last two groups of fives not being used in the game

COL_City_Banner0    db  96,  97,  98,  99, 100 
COL_City_Banner1    db  72,  73,  74,  75,  76
COL_City_Banner2    db 205, 206, 207, 208, 209
COL_City_Banner3    db 201, 165, 203, 166,  45
COL_City_Banner4    db 209, 210, 211, 212, 213
UU_COL_City_Banner5 db  29,  29,  28,  28,  27
UU_COL_City_Banner6 db  29,  29,  28,  28,  27
*/

// WZD dseg:7034
int8_t TBL_Warp_GFX_Lines[20] = {0};              // ; 20 bytes randomized between -1, 0, or +1
// WZD dseg:7048  9B 9F        WarpNode_SeedSave_LO dw   9F9Bh
// WZD dseg:704A  0F 00        WarpNode_SeedSave_HO dw     0Fh
// WZD dseg:7048  9B 9F 0F 00  WarpNode_SeedSave    dd 0F9F9Bh
uint32_t WarpNode_SeedSave = 0x000F9F9B;

// WZD dseg:704A                                                 ? END:  ovr150 ?



// MoM_Data  // WZD dseg:974A
// MoM_Data  int16_t _prev_world_y;
// MoM_Data  // WZD dseg:974C
// MoM_Data  int16_t _prev_world_x;

// NIU?  // WZD dseg:9746
// NIU?  int16_t CRP_OVL_MapWindowY;
// NIU?  // ; would be the top Y pixel position if the map was direct copied from another part of the VGA memory as it stands, it's repurposable
// NIU?  // set to 0 in GAME_Overland_Init()
// NIU?  // WZD dseg:9748
// NIU?  int16_t CRP_OVL_MapWindowX;
// NIU?  // ; would be the left X pixel position if the map was direct copied from another part of the VGA memory as it stands, it's repurposable
// NIU?  // set to 0 in GAME_Overland_Init()


// WZD dseg:9CA8
SAMB_ptr m_terrain_lbx_002;  // 2 * 762 color map indecies, by terrain type, for minimap



//                                          ¿ BEGIN: ~ Reduced/World Map  - Uninitialized Data ?

// WZD dseg:C11C
int16_t minimap_height;
int16_t reduced_map_height;

// WZD dseg:C11E
int16_t minimap_width;
int16_t reduced_map_width;

// WZD dseg:C120
int16_t minimap_y;
int16_t reduced_map_y;

// WZD dseg:C122
int16_t minimap_x;
int16_t reduced_map_x;

// WZD dseg:C124 00 00                                           dw 0

// WZD dseg:C126
int16_t reduced_map_mark_cycle;

//                                          ¿ END: ~ Reduced/World Map  - Uninitialized Data ?





// WZD dseg:CC22
/*
¿ "Terrain Type Record [Data]" ?
TERRAIN.LBX, 001
3048 bytes, 1524 values, 762 terain types
2-byte, unsigned
1 bit value for {F,T} terrain is animated
7 bit value for index of EMM Logical Page
8 bit value for index of picture record in EMM Page Frame

00 02
...
80 14
...
...
...
A4 3F
...
27 62
*/
uint16_t * m_terrain_lbx_001;                   // load in Terrain_Init() ovr052
// WZD dseg:CC26
byte_ptr m_terrain_001_1;                     // load in Terrain_Init() ovr052
// WZD dseg:CC28
// g_EmmHndl_TERRAIN dw 0  ; EMM_Load_LBX handle
SAMB_ptr m_terrain_lbx_000;                   // load in Terrain_Init() ovr052





/*
    WIZARDS.EXE  ovr067
*/

// WZD o67p01
/*

screen x,y                   0,20
map w,h                     12,10
(current) map x,y           _map_x,y
(current) plane             _map_plane
target x,y                  _prev_world_x,y
player idx                  _human_player_idx

    Main_Screen_Draw() |-> Main_Screen_Draw_Do_Draw(&_map_x, &_map_y, _map_plane, _prev_world_x, _prev_world_y, _human_player_idx);



; draws the visible sections of the overland into the
; current draw segment, and the minimap into the
; Minimap_IMG_Seg allocation, according to the passed
; parameters:
;   x/y - main display pixel coordinates
;   H/map_height - horizontal/vertical map size, in squares
;   XPos/YPos/Plane - top left map coordinates, in squares, of world
;   MapX@/MapY@ - return values to set
; resets the draw window after finishing
*/
// void Draw_Maps(int16_t x, int16_t y, int16_t map_width, int16_t map_height, int16_t * map_xpos, int16_t * map_ypos, int16_t map_plane, int16_t xpos, int16_t ypos, int16_t player_idx)
void Draw_Maps(int16_t screen_x, int16_t screen_y, int16_t map_width, int16_t map_height, int16_t * map_x, int16_t * map_y, int16_t map_plane, int16_t xpos, int16_t ypos, int16_t player_idx)
{
    int16_t l_map_x;
    int16_t l_map_y;
    int16_t map_moved_flag;
    int16_t shift_right_flag;
    int16_t half_swap_flag;

    assert(*map_x >= WORLD_XMIN && *map_x <= WORLD_XMAX);  /*  0 & 59 */
    assert(*map_y >= WORLD_YMIN && *map_y <= WORLD_YMAX);  /*  0 & 39 */

    l_map_x = *map_x;
    l_map_y = *map_y;


    /*
        BEGIN: Map-Moved!!
    */
    map_moved_flag = ST_FALSE;

    if(xpos != l_map_x)
    {
        map_moved_flag = ST_TRUE;
        shift_right_flag = ST_FALSE;
        half_swap_flag = ST_FALSE;

        // Which direction to scroll the movement map?
        // if the new x is greater than the old x, scroll right
        // if the new x is less than the old x, scroll left
        if(xpos < l_map_x)
        {
            shift_right_flag = ST_FALSE;
        }
        else
        {
            shift_right_flag = ST_TRUE;
        }

        // World-Wrap:
        // 
        if(xpos > 50 && l_map_x < 20)
        {
            shift_right_flag = ST_FALSE;
            half_swap_flag = ST_TRUE;
        }
        if(l_map_x > 50 && xpos < 20)
        {
            shift_right_flag = ST_TRUE;
            half_swap_flag = ST_TRUE;
        }

        /* ¿ shift 1 map square to the right or left ? */
        if(shift_right_flag != ST_FALSE)
        {
            // NIU?  CRP_OVL_MapWindowX += 20;
            l_map_x++;
            if(l_map_x == WORLD_WIDTH)
            {
                l_map_x = 0;
                // NIU?  CRP_OVL_MapWindowX = 0;
            }
            if(half_swap_flag == ST_FALSE && l_map_x > xpos)
            {
                l_map_x = xpos;
                // NIU?  CRP_OVL_MapWindowX = l_map_x * 20;
            }
        }
        else  /* if(shift_right_flag == ST_FALSE) */
        {
            // NIU?  CRP_OVL_MapWindowX -= 20;
            l_map_x--;
            if(l_map_x == 0)
            {
                l_map_x = WORLD_WIDTH - 1;  // ? MoO2 _MAP_MAX_X ?
                // NIU?  CRP_OVL_MapWindowX = l_map_x * 20;
            }
            if(half_swap_flag == ST_FALSE && l_map_x < xpos)
            {
                l_map_x = xpos;
                // NIU?  CRP_OVL_MapWindowX = l_map_x * 20;
            }
        }
    }

    if(ypos != l_map_y)
    {
        map_moved_flag = ST_TRUE;

        if(ypos < l_map_y)
        {
            // NIU?  CRP_OVL_MapWindowY -= 18;
            l_map_y--;
            if(l_map_y < ypos)
            {
                l_map_y = ypos;
            }
        }
        else
        {
            // NIU?  CRP_OVL_MapWindowY += 18;
            l_map_y++;
            if(l_map_y > ypos)
            {
                l_map_y = ypos;
            }
        }
    }

    if(map_moved_flag == ST_TRUE)
    {
        Set_Unit_Draw_Priority();
        Reset_Stack_Draw_Priority();
        Set_Entities_On_Map_Window(l_map_x, l_map_y, map_plane);

        if(player_idx == _human_player_idx)
        {
            _map_x = l_map_x;
            _map_y = l_map_y;
        }
    }
    /*
        END: Map-Moved!!
    */

    Reduced_Map_Coords(&minimap_x, &minimap_y, ((l_map_x + (MAP_WIDTH/2)) % WORLD_WIDTH), (l_map_y + (MAP_HEIGHT/2)), minimap_width, minimap_height);

    // Draw_Minimap(minimap_x, minimap_y, map_plane, _reduced_map_seg, minimap_width, minimap_height, 0, 0, 0);
    Create_Reduced_Map_Picture(minimap_x, minimap_y, map_plane, _reduced_map_seg, minimap_width, minimap_height, 0, 0, 0);  // mark_x = 0, mark_y = 0, mark_flag = 0


    Draw_Map_Window(screen_x, screen_y, map_width, map_height, l_map_x, l_map_y, map_plane);


    if(map_moved_flag == ST_FALSE)
    {
        assert(*map_x >= WORLD_XMIN && *map_x <= WORLD_XMAX);  /*  0 & 59 */
        assert(*map_y >= WORLD_YMIN && *map_y <= WORLD_YMAX);  /*  0 & 39 */
        Draw_Active_Unit_Stack(*map_x, *map_y, map_plane);
    }


    // j_OVL_RedrawScouting(x, y, map_width, map_height, l_map_x, l_map_y, map_plane);
    Redraw_Map_Unexplored_Area(screen_x, screen_y, map_width, map_height, l_map_x, l_map_y, map_plane);

    Reset_Window();

    *map_x = l_map_x;
    *map_y = l_map_y;

}

// WZD o67p02
// AKA IDK_CheckSet_MapDisplay_XY
void IDK_CheckSet_MapDisplay_XY(void)
{
    // bind Y on the bottom
    if(_prev_world_y + MAP_HEIGHT >= WORLD_HEIGHT)
    {
        _prev_world_y = WORLD_HEIGHT - MAP_HEIGHT;
    }

    // wrap X around to the right
    if(_prev_world_x >= WORLD_WIDTH)
    {
        _prev_world_x = _prev_world_x - WORLD_WIDTH;
    }

    // wrap X around to the left
    if(_prev_world_x < 0)
    {
        _prev_world_x = _prev_world_x + WORLD_WIDTH;
    }

    // bind Y on the top
    if(_prev_world_y < 0)
    {
        _prev_world_y = 0;
    }

    // ¿ reorient X if scroll will wrap ?
    if(_map_x == 0 && _prev_world_x > 49)
    {
        _map_x = WORLD_WIDTH;
    }


}


// WZD o67p03
// void sub_59DF7(int16_t arg_0);



// WZD o67p04
// AKA OVL_ShowActiveStack
void Set_Draw_Active_Stack_Always(void)
{
    draw_active_stack_flag = -1;
}


// WZD o67p05
// AKA OVL_HideActiveStack
void Set_Draw_Active_Stack_Never(void)
{
    draw_active_stack_flag = -2;
}

// WZD o67p06
// Reset_Active_Stack_Draw
void Reset_Draw_Active_Stack(void)
{
    if(
        (all_units_moved == ST_FALSE)
        &&
        (_unit_stack_count > 0)
    )
    {

        draw_active_stack_flag = 0;

    }

    Reset_Map_Draw();

}

// WZD o67p07
// AKA OVL_DrawActiveStack
void Draw_Active_Unit_Stack(int16_t mmap_xw, int16_t mmap_yw, int16_t wp)
{
/*
draws the top movable unit's card from the active
stack into the current draw segment based on the
specified starting map tile parameters, unless stack
drawing is disabled, or the view is on the opposite
plane; also redraws the minimap with a white marker

contains a BUG that may unintentionally draw the
stack over other units/cities already on the tile
when always draw (-1) is set for OVL_ActiveStackDraw
*/

    int16_t in_view;
    int16_t first_active_stack_unit_idx;
    int16_t unit_xw;
    int16_t unit_yw;
    int16_t unit_idx;
    int16_t itr_unit_stack_count;

    assert(mmap_xw >= WORLD_XMIN && mmap_xw <= WORLD_XMAX);  /*  0 & 59 */
    assert(mmap_yw >= WORLD_YMIN && mmap_yw <= WORLD_YMAX);  /*  0 & 39 */

    if(draw_active_stack_flag != -2)  /* -2: never draw*/
    {
        if((_UNITS[_unit_stack[0].unit_idx].owner_idx != ST_UNDEFINED))
        {
            unit_idx = _unit_stack[0].unit_idx;
            if( (_UNITS[_unit_stack[0].unit_idx].wp == wp) || (_UNITS[_unit_stack[0].unit_idx].in_tower == ST_TRUE) )
            {
                unit_xw = _UNITS[unit_idx].wx;
                unit_yw = _UNITS[unit_idx].wy;
                in_view = World_To_Screen(mmap_xw, mmap_yw, &unit_xw, &unit_yw);

                // not never, not always, so must be set for draw-cycling, so increment the cycle
                if(draw_active_stack_flag != -1)  /* -1: always draw */
                {
                    draw_active_stack_flag = ((draw_active_stack_flag + 1) % 8);
                }

                if((draw_active_stack_flag == -1) || (draw_active_stack_flag > 3))  /* always draw OR draw if counter is {4,5,6,7} */
                {
                    /*
                        WTF?
                        
                        draw_active_stack_flag != -1
                        && draw_active_stack_flag != -1
                        OR
                        draw_active_stack_flag == -1
                        && entities_on_movement_map != -1
                        OR
                        draw_active_stack_flag == -1
                        && entities_on_movement_map == -1
                        && draw_active_stack_flag == -1

                        if ... entities_on_movement_map[ ( ((unit_yw - mmap_yw) * MAP_WIDTH) + unit_xw ) ]; ==/!= ST_UNDEFINED
                    */

                    if(draw_active_stack_flag != -1)
                    {

                        if(in_view == ST_TRUE)
                        {
                            first_active_stack_unit_idx = 0;
                            for(itr_unit_stack_count = 0; itr_unit_stack_count < _unit_stack_count; itr_unit_stack_count++)
                            {
                                if(_unit_stack[itr_unit_stack_count].active == ST_TRUE)
                                {
                                    first_active_stack_unit_idx = itr_unit_stack_count;
                                    break;
                                }
                            }
                            unit_idx = _unit_stack[first_active_stack_unit_idx].unit_idx;
                            Draw_Unit_Picture(unit_xw, unit_yw, unit_idx, 2);  /* 2: Nay status text; Nay Grayscale; Yay owner background banner color; enchantment outline? invisible? */
                        }

                        unit_idx = _unit_stack[0].unit_idx;
                        // Draw_Minimap(minimap_x, minimap_y, wp, _reduced_map_seg, minimap_width, minimap_height, _UNITS[unit_idx].wx, _UNITS[unit_idx].wy, ST_TRUE);
                        Create_Reduced_Map_Picture(minimap_x, minimap_y, wp, _reduced_map_seg, minimap_width, minimap_height, _UNITS[unit_idx].wx, _UNITS[unit_idx].wy, ST_TRUE);

                    }
                }
            }
        }
    }

}

// WZD o67p08
// AKA Minimap_Set_Dims()
void Reduced_Map_Set_Dims(int16_t width, int16_t height)
{
    minimap_width = width;
    minimap_height = height;
}


// WZD o67p09
// "Reduced Map"/"World Window"
// AKA Draw_Minimap_Window()
// void Draw_Minimap_Window(int16_t start_x, int16_t start_y, int16_t width, int16_t height)
void Draw_World_Window(int16_t start_x, int16_t start_y, int16_t width, int16_t height)
{
    int16_t color;
    int16_t line_x;
    int16_t line_y;

    color = REDUCED_MAP_BOX_COLOR;  // 0Dh  13d  Light Magenta / Bright Magenta

    Reset_Window();

    Draw_Picture(start_x, start_y, _reduced_map_seg);

    line_x = width / 2;
    line_y = height / 2;
    line_x = line_x - (MAP_WIDTH  / 2);
    line_y = line_y - (MAP_HEIGHT / 2);
    line_x = line_x + start_x;
    line_y = line_y + start_y;

    Line(line_x - 1            , line_y - 1         , line_x + 1            , line_y - 1             , color);  // top left
    Line(line_x - 1            , line_y - 1         , line_x - 1            , line_y + 1             , color);  // left upper
    Line(line_x + MAP_WIDTH - 2, line_y - 1         , line_x + MAP_WIDTH    , line_y - 1             , color);  // top right
    Line(line_x + MAP_WIDTH    , line_y - 1         , line_x + MAP_WIDTH    , line_y + 1             , color);  // upper right
    Line(line_x - 1            , line_y + MAP_HEIGHT, line_x - 1            , line_y + MAP_HEIGHT - 2, color);  // left lower
    Line(line_x - 1            , line_y + MAP_HEIGHT, line_x + 1            , line_y + MAP_HEIGHT    , color);  // bottom left
    Line(line_x + MAP_WIDTH    , line_y + MAP_HEIGHT, line_x + MAP_WIDTH    , line_y + MAP_HEIGHT - 2, color);  // right lower
    Line(line_x + MAP_WIDTH    , line_y + MAP_HEIGHT, line_x + MAP_WIDTH - 2, line_y + MAP_HEIGHT    , color);  // bottom right

}


// WZD o67p10
void Set_Entities_On_Map_Window(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t city_in_view;
    int16_t entity_owner_idx;
    int16_t entity_wy;
    int16_t entity_wx;
    int16_t entity_wp;
    int16_t square_is_explored;
    int16_t square_is_scouted;
    int16_t itr_map_x;
    int16_t itr_map_y;
    int16_t itr_units;
    int16_t itr_cities;
    int16_t entity_mx;  // DNE in Dasm
    int16_t entity_my;  // DNE in Dasm
    // DELETEME  int16_t entity_table_idx;  // DNE in Dasm
    int16_t prior_entity_idx;  // used to check if a Unit has already been placed

    city_in_view = ST_FALSE;
    entity_wy = 0;
    entity_wx = 0;

    for(itr_map_y = 0; itr_map_y < MAP_HEIGHT; itr_map_y++)
    {
        for(itr_map_x = 0; itr_map_x < MAP_WIDTH; itr_map_x++)
        {
            entities_on_movement_map[(itr_map_y * MAP_WIDTH) + itr_map_x] = ST_UNDEFINED;
        }
    }

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        entity_owner_idx = _UNITS[itr_units].owner_idx;
        entity_wx = _UNITS[itr_units].wx;
        entity_wy = _UNITS[itr_units].wy;
        entity_wp = _UNITS[itr_units].wp;
        square_is_scouted = Check_Square_Scouted(entity_wx, entity_wy, wp);

        if(entity_owner_idx != ST_UNDEFINED)
        {
            if(square_is_scouted == ST_TRUE)
            {
                if( (_UNITS[itr_units].wp == wp) || (_UNITS[itr_units].in_tower == ST_TRUE) )
                {
                    // entity_world_y = _UNITS[itr_units].wy;
                    if( (entity_wy >= wy) && (entity_wy < wy + MAP_HEIGHT) )
                    {
                        // entity_world_x = _UNITS[itr_units].wx;
                        if(
                             ((entity_wx >= wx) && (entity_wx < wx + MAP_WIDTH)) ||
                             ((entity_wx + WORLD_WIDTH >= wx) && (entity_wx + WORLD_WIDTH < wx + MAP_WIDTH))
                        )
                        {
                            if((entity_wx + WORLD_WIDTH >= wx) && (entity_wx + WORLD_WIDTH < wx + MAP_WIDTH))
                            {
                                entity_wx = (entity_wx + WORLD_WIDTH);
                            }

                            entity_mx = (entity_wx - wx);
                            entity_my = (entity_wy - wy);
                            // DELETEME  entity_table_idx = ((entity_map_y * MAP_WIDTH) + entity_map_x);
                            // DELETEME  
                            // DELETEME  // prior_entity_idx = (entities_on_movement_map[((entity_map_y * MAP_WIDTH) + entity_map_x)]);
                            // DELETEME  // // entities_on_movement_map[((my * MAP_WIDTH) + mx)];
                            // DELETEME  // prior_entity_idx = entities_on_movement_map[entity_table_idx];
                            prior_entity_idx = GET_MAP_ENTITY(entity_mx,entity_my);

                            /*
                                Add UNIT to entities_on_movement_map[]
                            */
                            if(prior_entity_idx != ST_UNDEFINED)
                            {
                                if(_UNITS[itr_units].Draw_Priority > _UNITS[prior_entity_idx].Draw_Priority)
                                {
                                    // Add UNIT to entities_on_movement_map[]
                                    // DELETEME  entities_on_movement_map[entity_table_idx] = itr_units;
                                    SET_MAP_ENTITY(entity_mx, entity_my, itr_units);
                                }
                            }
                            else
                            {
                                if(_UNITS[itr_units].Draw_Priority > 0)
                                {
                                    // Add UNIT to entities_on_movement_map[]
                                    // DELETEME  entities_on_movement_map[entity_table_idx] = itr_units;
                                    SET_MAP_ENTITY(entity_mx, entity_my, itr_units);
                                }
                                else
                                {
                                    if( (_UNITS[itr_units].Draw_Priority == 0) && (draw_active_stack_flag == -1) )  /* ALWAYS draw active stack */
                                    {
                                        // Add UNIT to entities_on_movement_map[]
                                        // DELETEME  entities_on_movement_map[entity_table_idx] = itr_units;
                                        SET_MAP_ENTITY(entity_mx, entity_my, itr_units);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {

        entity_wx = _CITIES[itr_cities].wx;
        entity_wy = _CITIES[itr_cities].wy;
        entity_wp = _CITIES[itr_cities].wp;

        // DELETEME  square_is_explored = _square_explored[((entity_world_p * WORLD_SIZE) + (entity_world_y * WORLD_WIDTH) + entity_world_x)];
        square_is_explored = GET_SQUARE_EXPLORED(entity_wx, entity_wy, entity_wp);

        if(
            (
                (entity_wp == wp)
                ||
                (entity_wp == 2)
            )
            &&
            (square_is_explored != ST_FALSE)
        )
        {

            entity_wy = _CITIES[itr_cities].wy;

            if(
                (entity_wy >= wy)
                &&
                (entity_wy < wy + MAP_HEIGHT)
            )
            {

                city_in_view = ST_FALSE;

                entity_my = (entity_wy - wy);

                entity_wx = _CITIES[itr_cities].wx;

                if(
                    ( (entity_wx >= wx) && (entity_wx < wx + MAP_WIDTH) ) ||
                    ( (entity_wx + WORLD_WIDTH >= wx) && (entity_wx + WORLD_WIDTH < wx + MAP_WIDTH) )
                )
                {

                    if( (entity_wx + WORLD_WIDTH >= wx) && (entity_wx + WORLD_WIDTH < wx + MAP_WIDTH) )
                    {
                        entity_wx = (entity_wx + WORLD_WIDTH);
                    }

                    entity_mx = (entity_wx - wx);

                    city_in_view = ST_TRUE;

                }

                if(city_in_view == ST_TRUE)
                {
                    // DELETEME  entity_table_idx = ((entity_map_y * MAP_WIDTH) + entity_map_x);
                    // DELETEME  entities_on_movement_map[entity_table_idx] = (itr_cities + MAX_UNIT_COUNT);
                    SET_MAP_ENTITY(entity_mx, entity_my, (itr_cities + MAX_UNIT_COUNT));
                }

            }

        }

    }

}


// WZD o67p11
void Add_Nodes_To_Entities_On_Map_Window(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t var_4 = 0;
    int16_t node_wy = 0;
    int16_t itr = 0;  // _CX_
    int16_t node_wx = 0;  // _DI_

    for(itr = 0; itr < NUM_NODES; itr++)
    {

        if(_NODES[itr].wp == wp)
        {

            if(SQUARE_EXPLORED(wx,wy,wp))
            {

                if(
                    (node_wy = _NODES[itr].wy >= wy)
                    &&
                    ((wy + MAP_HEIGHT) > node_wy)
                )
                {

                    var_4 = ST_FALSE;

                    node_wy = (_NODES[itr].wy - wy);

                    node_wx = _NODES[itr].wx;

                    if(
                        (node_wx >= wx)
                        &&
                        ((wx + MAP_WIDTH) > node_wx)
                    )
                    {
                        node_wx -= wx;
                        var_4 = ST_TRUE;
                    }
                    else
                    {
                        node_wx += WORLD_WIDTH;
                        if(
                            (node_wx >= wx)
                            &&
                            ((wx + MAP_WIDTH) > node_wx)
                        )
                        {
                            node_wx -= wx;
                            var_4 = ST_TRUE;
                        }

                    }

                    if(var_4 == ST_TRUE)
                    {

                        entities_on_movement_map[((node_wy * MAP_WIDTH) + node_wx)] = (1100 + itr);

                    }

                }

            }

        }

    }

}

// WZD o67p12
// OVL_DrawMapSection

// WZD o67p13
/*
    updates draw priority on ALL units in _UNITS
*/
void Set_Unit_Draw_Priority(void)
{
    int16_t draw_priority = 0;
    int16_t unit_idx = 0;

    for(unit_idx = 0; unit_idx < _units; unit_idx++)
    {

        draw_priority = _unit_type_table[_UNITS[unit_idx].type].Melee + _unit_type_table[_UNITS[unit_idx].type].Ranged;

        if(draw_priority == 0)
        {
            draw_priority = 1;
        }

        if(_unit_type_table[_UNITS[unit_idx].type].Transport > 0)
        {
            draw_priority = 50;
        }

        if(_UNITS[unit_idx].owner_idx == ST_UNDEFINED)
        {
            draw_priority = ST_UNDEFINED;
        }

        if(
            (_UNITS[unit_idx].owner_idx != _human_player_idx)
            &&
            (Unit_Has_Invisibility(unit_idx) == ST_TRUE)
        )
        {
            draw_priority = ST_UNDEFINED;
        }

        UNITS_DRAW_PRIORITY(unit_idx, draw_priority);

    }

}


// WZD o67p14
// drake178: STK_NoUnitDraw()
void Reset_Stack_Draw_Priority(void)
{
    int16_t itr_unit_stack_count = 0;
    int16_t unit_idx = 0;
    for(itr_unit_stack_count = 0; itr_unit_stack_count < _unit_stack_count; itr_unit_stack_count++)
    {
        unit_idx = _unit_stack[itr_unit_stack_count].unit_idx;
        UNITS_DRAW_PRIORITY(unit_idx, 0);
    }
}


// WZD o67p15
/*
; returns 1 if the tile has a tower of wizardry and
; there is no Planar Seal active; or 0 otherwise
*/
/*
OON XREF:
    GAME_Overland_Init()
*/
int16_t IsPassableTower(int16_t wx, int16_t wy)
{
    int16_t itr_towers = 0;  // _SI_
    int16_t is_passible_tower = 0;  // _DI_
    int16_t active_planar_seal = 0;

    is_passible_tower = ST_FALSE;

    for(itr_towers = 0; itr_towers < NUM_TOWERS; itr_towers++)
    {

        if(
            (wx == _TOWERS[itr_towers].wx)
            &&
            (wy == _TOWERS[itr_towers].wy)
        )
        {

            is_passible_tower = ST_TRUE;

        }

    }

    if(is_passible_tower == ST_TRUE)
    {

        if(Check_Planar_Seal() == ST_TRUE)
        {

            is_passible_tower = ST_FALSE;

        }

    }

    return is_passible_tower;

}


/*
    WIZARDS.EXE  ovr068
*/

// WZD o68p01
// ? MoO2  Module: MAINSCR  Center_Map_() ?
/*
    map_x, map_y are the top-left corner of the movement map, in world coordinates
    seeting _prev_world_x, y causes the next map draw to jump to the new coordinates

    in: world_grid_x,ys is new world-map center
    in/out: map_x,map_y are cur/new map start
*/
void Center_Map(int16_t * map_x, int16_t * map_y, int16_t world_grid_x, int16_t world_grid_y, int16_t wp)
{
    int16_t tmp_world_grid_y = 0;
    int16_t tmp_world_grid_x = 0;

    // NOWORKIE  assert(*map_x >= WORLD_X_MIN && *map_x <= WORLD_X_MAX);  /*  0 & 59 */
    // NOWORKIE  assert(*map_y >= WORLD_Y_MIN && *map_y <= WORLD_YMAX);  /*  0 & 39 */

    if(world_grid_x >= WORLD_WIDTH)
    {
        world_grid_x -= WORLD_WIDTH;
    }

    if(world_grid_x < 0)
    {
        world_grid_x += WORLD_WIDTH;
    }

    tmp_world_grid_y = world_grid_y;

    tmp_world_grid_x = (world_grid_x - MAP_WIDTH_HALF);

    if(tmp_world_grid_x > 0)
    {
        *map_x = tmp_world_grid_x;
    }
    else
    {
        *map_x = ((tmp_world_grid_x + WORLD_WIDTH) / WORLD_WIDTH);
    }

    tmp_world_grid_y = (world_grid_y - MAP_HEIGHT_HALF);

    if(tmp_world_grid_y > 0)
    {
        *map_y = tmp_world_grid_y;
    }
    else
    {
        *map_y = 0;
    }

    if((*map_y + MAP_HEIGHT) >= WORLD_HEIGHT)
    {
        *map_y = (WORLD_HEIGHT - MAP_HEIGHT);  // (40 - 10) = 30  (World Height - Map Height)
    }

    assert(*map_x >= WORLD_XMIN && *map_x <= WORLD_XMAX);  /*  0 & 59 */
    assert(*map_y >= WORLD_YMIN && *map_y <= WORLD_YMAX);  /*  0 & 39 */

    _prev_world_x = *map_x;
    _prev_world_y = *map_y;

    // TODO  CRP_OVL_MapWindowX = G_OVL_MapDisplay_X * TILE_WIDTH;
    // TODO  CRP_OVL_MapWindowY = G_OVL_MapDisplay_Y * TILE_HEIGHT;
    
    Set_Unit_Draw_Priority();
    Reset_Stack_Draw_Priority();
    Set_Entities_On_Map_Window(*map_x, *map_y, wp);

}


// WZD o68p02
void City_Center_Map(int16_t * map_x, int16_t * map_y, int16_t world_grid_x, int16_t world_grid_y, int16_t wp)
{
    int16_t city_screen_map_world_grid_y = 0;
    int16_t city_screen_map_world_grid_x = 0;

    city_screen_map_world_grid_y = world_grid_y;
    city_screen_map_world_grid_x = (world_grid_x - 2);

    if(city_screen_map_world_grid_x <= 0)
    {
        *map_x = ((city_screen_map_world_grid_x + WORLD_WIDTH) % WORLD_WIDTH);
    }
    else
    {
        *map_x = city_screen_map_world_grid_x;
    }

    city_screen_map_world_grid_y = (world_grid_y - 2);

    if(city_screen_map_world_grid_y <= 0)
    {
        *map_y = 0;
    }
    else
    {
        *map_y = city_screen_map_world_grid_y;
    }

    if((*map_y + 5) >= WORLD_HEIGHT)
    {
        *map_y = (WORLD_HEIGHT - 5);
    }

    _prev_world_x = *map_x;
    _prev_world_y = *map_y;

    // NIU  CRP_OVL_MapWindowX = (_prev_world_x * SQUARE_WIDTH);
    // NIU  CRP_OVL_MapWindowY = (_prev_world_y * SQUARE_HEIGHT);

    Set_Unit_Draw_Priority();
    Reset_Stack_Draw_Priority();
    Set_Entities_On_Map_Window(*map_x, *map_y, wp);

}



// WZD o68p03
// drake178: OVL_GetMinimapStart()
// AKA Minimap_Coords()
/*
; sets the return values to the top left map tile
; coordinates that should be displayed at the top left
; corner of the minimap
*/
/*
    in-outs the x,y for the top-left corner of the reduced map window

    mid_x   ((_map_x + (MAP_WIDTH  / 2)) % WORLD_WIDTH)
    mid_y   ( _map_y + (MAP_HEIGHT / 2))
        where, _map_x,y are the top left corner of the movement map in world coordinates

    translates the movement map center x,y to the reduced map window x,y, in world coordinates
*/
void Reduced_Map_Coords(int16_t * minimap_x, int16_t * minimap_y, int16_t mid_x, int16_t mid_y, int16_t minimap_width, int16_t minimap_height)
{
    int16_t tmp_minimap_y = 0;
    int16_t tmp_minimap_x = 0;  // _CX_

    tmp_minimap_x = (mid_x - (minimap_width / 2));

    if(tmp_minimap_x > 0)
    {
        *minimap_x = tmp_minimap_x;
    }
    else
    {
        *minimap_x = ((tmp_minimap_x + WORLD_WIDTH) % WORLD_WIDTH);
    }

    tmp_minimap_y = (mid_y - (minimap_height / 2));

    *minimap_y = tmp_minimap_y;

}


// WZD o68p04
// drake178: OVL_RedrawScouting()
void Redraw_Map_Unexplored_Area(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t wp)
{
    int16_t itr_screen_x;
    int16_t itr_screen_y;
    int16_t itr_world_x;
    int16_t itr_world_y;
    int16_t curr_world_x;
    uint16_t terrain_001_index;
    uint8_t terrain_001_0;
    uint8_t animated_terrain_flag;
    uint8_t square_explored_flag;
    SAMB_ptr unexplored_mask_pict_seg;

    itr_screen_y = screen_y;
    itr_world_y = world_grid_y;
    while(world_grid_y + map_grid_height > itr_world_y)
    {
        itr_screen_x = screen_x;
        itr_world_x = world_grid_x;
        while(world_grid_x + map_grid_width > itr_world_x)
        {
            if(itr_world_x < WORLD_WIDTH)
            {
                curr_world_x = itr_world_x;
            }
            else
            {
                curr_world_x = itr_world_x - WORLD_WIDTH;
            }

            terrain_001_index = GET_2B_OFS(_world_maps, ((wp * WORLD_SIZE * 2) + (itr_world_y * WORLD_WIDTH * 2) + (curr_world_x * 2)));
            terrain_001_0 = GET_1B_OFS(m_terrain_lbx_001, (terrain_001_index * 2) + 0);

            animated_terrain_flag = ((terrain_001_0 & 0x80) != 0);  // ¿ prefer ((bitfield >> 7) & 0x01) - saves the CPU doing a full register-sized compare ?

            // if( (map_draw_full == ST_TRUE) || (animated_terrain_flag != ST_TRUE) )
            // {
                square_explored_flag = GET_SQUARE_EXPLORED(curr_world_x, itr_world_y, wp);

                if(
                    (square_explored_flag != 0x0)
                    &&
                    (square_explored_flag != 0xF)
                )
                {
                    unexplored_mask_pict_seg = unexplored_mask_seg[square_explored_flag - 1];
                    FLIC_Draw(itr_screen_x, itr_screen_y, unexplored_mask_pict_seg);
                }
            // }
            // NOTE: not in Draw_Map_Unexplored_Area(), but in Draw_Map_Terrain(), which uses Fill()
            if(square_explored_flag == 0)
            {
                Clipped_Fill(itr_screen_x, itr_screen_y, (itr_screen_x + SQUARE_WIDTH - 1), (itr_screen_y + SQUARE_HEIGHT - 1), 0);
            }


            itr_screen_x += SQUARE_WIDTH;
            itr_world_x += 1;
        }

        itr_screen_y += SQUARE_HEIGHT;
        itr_world_y += 1;
    }

}


// WZD o68p05
// RP_OVL_DrawCities2()


// WZD o68p06
// drake178: TILE_Explore()
// MoO2  Module: PLNTSUM  code (0 bytes) Set_Planet_Explored_Flags_()  Address: 01:0009BB40
/*
    called from Units_In_Tower()
        only for human_player_idx
    called from Set_Map_Square_Explored_Flags_XYP_Range()

*/
void Set_Map_Square_Explored_Flags_XYP(int16_t wx, int16_t wy, int16_t wp)
{
    // DEDU  does this actually exist in the Dasm?  uint8_t * ptr_square_explored;
    int16_t itr_wx;
    int16_t wy_u2;
    int16_t wy_d2;
    int16_t wx_l2;
    int16_t wx_r2;
    int16_t wy_u1;
    int16_t wy_d1;
    int16_t wx_l1;
    int16_t wx_r1;

    if(wy == 0)
    {
        SET_SQUARE_EXPLORED(wx, wy, wp, (SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight));
    }
    else
    {

        /*
            BEGIN: sanitize wy & wx
        */
        if(wy == 0)
        {
            wy_u1 = 0;
            wy_u2 = 0;
        }
        else
        {
            wy_u1 = wy - 1;
            wy_u2 = wy - 2;
            SETMIN(wy_u2, 0);
        }

        if(wy == WORLD_YMAX)
        {
            wy_d1 = WORLD_WIDTH - 1;  // 40 - 1 = 39
            wy_d2 = WORLD_WIDTH - 2;  // 40 - 2 = 38
        }
        else
        {
            wy_d1 = wy + 1;
            wy_d2 = wy + 2;
            SETMAX(wy_d2, WORLD_WIDTH);
        }

        if(wx == 0)
        {
            wx_l1 = WORLD_WIDTH - 1;  // 60 - 1 = 59
            wx_l2 = WORLD_WIDTH - 2;  // 60 - 2 = 58
        }
        else
        {
            wx_l1 = wx - 1;
            wx_l2 = wx - 2;
            if(wx_l2 < 0)
            {
                wx_l2 = WORLD_XMAX;
            }
        }

        if(wx == WORLD_XMAX)
        {
            wx_r1 = 0;
            wx_r2 = 1;
        }
        else
        {
            wx_r1 = wx + 1;
            wx_r2 = wx + 2;
        }
        /*
            END: sanitize wy & wx
        */

        // 
        SET_SQUARE_EXPLORED(wx_l1, wy_u1, wp, (GET_SQUARE_EXPLORED(wx_l1, wy_u1, wp) |                                               SCT_BottomRight));
        // 
        SET_SQUARE_EXPLORED(wx,    wy_u1, wp, (GET_SQUARE_EXPLORED(wx,    wy_u1, wp) | SCT_BottomLeft                              | SCT_BottomRight));
        // 
        SET_SQUARE_EXPLORED(wx_r1, wy_u1, wp, (GET_SQUARE_EXPLORED(wx_r1, wy_u1, wp) | SCT_BottomLeft                                               ));
        // 
        SET_SQUARE_EXPLORED(wx_l1, wy,    wp, (GET_SQUARE_EXPLORED(wx_l1, wy,    wp) |                                SCT_TopRight | SCT_BottomRight));
        // 
        SET_SQUARE_EXPLORED(wx,    wy,    wp, (GET_SQUARE_EXPLORED(wx,    wy,    wp) | SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight));
        // 
        SET_SQUARE_EXPLORED(wx_r1, wy,    wp, (GET_SQUARE_EXPLORED(wx_r1, wy,    wp) | SCT_BottomLeft | SCT_TopLeft                                 ));
        // 
        SET_SQUARE_EXPLORED(wx_l1, wy_d1, wp, (GET_SQUARE_EXPLORED(wx_l1, wy_d1, wp) |                                SCT_TopRight                  ));
        // 
        SET_SQUARE_EXPLORED(wx,    wy_d1, wp, (GET_SQUARE_EXPLORED(wx,    wy_d1, wp) |                  SCT_TopLeft | SCT_TopRight                  ));
        // 
        SET_SQUARE_EXPLORED(wx_r1, wy_d1, wp, (GET_SQUARE_EXPLORED(wx_r1, wy_d1, wp) |                  SCT_TopLeft                                 ));



        Set_Square_Explored_Flags_Fix(wx, wy, wp);



        for(itr_wx = wx_l2; itr_wx < wx_r2; itr_wx++)
        {

            Set_Square_Explored_Flags_Bottom_Right_Corner(itr_wx, wy_u2, wp);

        }

        Set_Square_Explored_Flags_Bottom_Right_Corner(wx_r1, wy_u1, wp);

        Set_Square_Explored_Flags_Bottom_Right_Corner(wx_r1, wy, wp);
        
        for(itr_wx = wx_l2; itr_wx < wx_r2; itr_wx++)
        {

            Set_Square_Explored_Flags_Bottom_Right_Corner(itr_wx, wy_d1, wp);

        }

        Set_Square_Explored_Flags_Bottom_Right_Corner(wx_l2, wy_u1, wp);

        Set_Square_Explored_Flags_Bottom_Right_Corner(wx_l2, wy, wp);

        Set_Square_Explored_Flags_Fix(wx, wy, wp);

    }

}


// WZD o68p07
// drake178: TILE_ExploreCorners()
/*
; compares the tile and it's bottom, right, and
; diagonal bottom-right neighbors to NoSide_Explores,
; and if all match one of them, removes the corner
; corresponding to the original tile's bottom right one
;
; conceptually flawed, it should simply check whether
; there is any exploration on all 4 tiles, rather than
; comparing them against the corner only masks
*/
/*

*/
void Set_Square_Explored_Flags_Bottom_Right_Corner(int16_t wx, int16_t wy, int16_t wp)
{
    uint8_t * ptr_square_explored = 0;
    int16_t wy_d1 = 0;
    int16_t wx_r1 = 0;
    uint8_t CornerOnly_Explores[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t IDK_itr2 = 0;  // _DI_
    int16_t IDK_itr1 = 0;  // _SI_


// lea     ax, [bp+CornerOnly_Explores]
// push    ss
// push    ax                              ; Dest_Struct
// mov     ax, offset NoSide_Explores
// push    ds
// push    ax                              ; src
// mov     cx, 6
// call    F_SCOPY@
    // ...generated by the compiler...
    memcpy(CornerOnly_Explores, NoSide_Explores, sizeof(NoSide_Explores));
    
    ptr_square_explored = &_square_explored[wp];
    
    wx_r1 = (wx + 1);
    
    wy_d1 = (wy + 1);
    
    IDK_itr2 = 0;
    
    for(IDK_itr1 = 0; IDK_itr1 < 6; IDK_itr1++)
    {
        if(ptr_square_explored[((wy * WORLD_WIDTH) + wx)] == CornerOnly_Explores[IDK_itr1])
        {
            IDK_itr2++;
        }
        else
        {
            continue;
        }
    }

    if(IDK_itr2 == 0)
    {
        return;
    }

    for(IDK_itr1 = 0; IDK_itr1 < 6; IDK_itr1++)
    {
        if(ptr_square_explored[((wy * WORLD_WIDTH) + wx_r1)] == CornerOnly_Explores[IDK_itr1])
        {
            IDK_itr2++;
        }
    }

    if(IDK_itr2 == 1)
    {
        return;
    }

    for(IDK_itr1 = 0; IDK_itr1 < 6; IDK_itr1++)
    {
        // ; BUG: at Y=39, this is out of bounds on Myrror!
        // ; (next alloc is the arcanus visibility table)
        if(ptr_square_explored[((wy_d1 * WORLD_WIDTH) + wx)] == CornerOnly_Explores[IDK_itr1])
        {
            IDK_itr2++;
        }
    }

    if(IDK_itr2 == 2)
    {
        return;
    }

    for(IDK_itr1 = 0; IDK_itr1 < 6; IDK_itr1++)
    {
        // ; BUG: at Y=39, this is out of bounds on Myrror!
        if(ptr_square_explored[((wy_d1 * WORLD_WIDTH) + wx_r1)] == CornerOnly_Explores[IDK_itr1])
        {
            IDK_itr2++;
        }
    }

    if(IDK_itr2 == 4)
    {
        ptr_square_explored[((wy    * WORLD_WIDTH) + wx   )] = ptr_square_explored[((wy    * WORLD_WIDTH) + wx   )] | SCT_BottomRight;
        ptr_square_explored[((wy    * WORLD_WIDTH) + wx_r1)] = ptr_square_explored[((wy    * WORLD_WIDTH) + wx_r1)] | SCT_BottomLeft;
        ptr_square_explored[((wy_d1 * WORLD_WIDTH) + wx   )] = ptr_square_explored[((wy_d1 * WORLD_WIDTH) + wx   )] | SCT_BottomLeft;
        ptr_square_explored[((wy_d1 * WORLD_WIDTH) + wx_r1)] = ptr_square_explored[((wy_d1 * WORLD_WIDTH) + wx_r1)] | SCT_BottomLeft;
    }

}


// WZD o68p08
// drake178: RP_TILE_ExploreMatch()
/*
; entirely redundant, TILE_ExploreCorners, if it was
; implemented correctly, would deal with each and every
; one of these cases, and then some
;
; performs exploration matching of neighboring tiles
; in a 5-by-5 rectangle around the selected tile in an
; attempt to remove fog of war from between tiles that
; are already explored
;
; BUG: the array contains many elements that are only
; intended to bugfix artifacts that the developers
; could not track down, but is missing 10 valid and
; occuring configurations, and has wrong values for 4
; of the ones it does include
*/
/*

*/
struct Explore_Matching
{
    int16_t Tile_Value;
    int16_t Adj_Scouting;
    int16_t Mark_Tile;
    int16_t Mark_Adj;
    int16_t Adj_Direction;
   
};
void Set_Square_Explored_Flags_Fix(int16_t wx, int16_t wy, int16_t wp)
{
    struct Explore_Matching Match_Array[38];
    uint8_t * ptr_square_explored;
    int16_t Adj_Tile_Scouting = 0;
    int16_t LoopTile_Scouting = 0;
    int16_t Y_LoopVar = 0;
    int16_t Match_Count = 0;
    int16_t Adjacent_Row = 0;
    int16_t wx_l2 = 0;
    int16_t X_LoopVar = 0;
    int16_t Match_Index = 0;
    int16_t wy_u2 = 0;  // _DI_
    int16_t IDK_wx_l2 = 0;  // _SI_
    int16_t Adjacent_Col = 0;  // _CX_

    Match_Count = 38;
    
    Match_Array[0].Tile_Value = SCT_BottomLeft;
    Match_Array[0].Adj_Scouting = SCT_BottomRight;
    Match_Array[0].Mark_Tile = 9;
    Match_Array[0].Mark_Adj = 9;
    Match_Array[0].Adj_Direction = 0;
    Match_Array[1].Tile_Value = SCT_BottomLeft | SCT_TopLeft | SCT_BottomRight;
    Match_Array[1].Adj_Scouting = SCT_BottomLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[1].Mark_Tile = 15;
    Match_Array[1].Mark_Adj = 15;
    Match_Array[1].Adj_Direction = 0;
    Match_Array[2].Tile_Value = SCT_BottomLeft | SCT_TopLeft | SCT_TopRight;
    Match_Array[2].Adj_Scouting = SCT_TopLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[2].Mark_Tile = 15;
    Match_Array[2].Mark_Adj = 15;
    Match_Array[2].Adj_Direction = 0;
    Match_Array[3].Tile_Value = SCT_TopLeft;
    Match_Array[3].Adj_Scouting = SCT_TopRight | SCT_BottomRight;
    Match_Array[3].Mark_Tile = 6;
    Match_Array[3].Mark_Adj = 15;  // ; BUG: should be 0Eh
    Match_Array[3].Adj_Direction = 0;
    Match_Array[4].Tile_Value = SCT_BottomLeft | SCT_TopLeft | SCT_TopRight;
    Match_Array[4].Adj_Scouting = SCT_BottomLeft | SCT_TopLeft | SCT_BottomRight;
    Match_Array[4].Mark_Tile = 15;
    Match_Array[4].Mark_Adj = 15;
    Match_Array[4].Adj_Direction = 1;
    Match_Array[5].Tile_Value = SCT_TopLeft;
    Match_Array[5].Adj_Scouting = SCT_BottomLeft | SCT_BottomRight;
    Match_Array[5].Mark_Tile = 3;
    Match_Array[5].Mark_Adj = 11;
    Match_Array[5].Adj_Direction = 1;
    Match_Array[6].Tile_Value = SCT_TopLeft | SCT_TopRight;
    Match_Array[6].Adj_Scouting = SCT_BottomLeft | SCT_BottomRight;
    Match_Array[6].Mark_Tile = 15;
    Match_Array[6].Mark_Adj = 15;
    Match_Array[6].Adj_Direction = 1;
    Match_Array[7].Tile_Value = SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[7].Adj_Scouting = SCT_BottomLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[7].Mark_Tile = 15;
    Match_Array[7].Mark_Adj = 15;
    Match_Array[7].Adj_Direction = 1;  // ; bogus configuration, should be fixed elsewhere
    Match_Array[8].Tile_Value = SCT_BottomLeft | SCT_TopLeft;
    Match_Array[8].Adj_Scouting = SCT_BottomRight;
    Match_Array[8].Mark_Tile = 11;
    Match_Array[8].Mark_Adj = 9;
    Match_Array[8].Adj_Direction = 0;
    Match_Array[9].Tile_Value = SCT_TopLeft;
    Match_Array[9].Adj_Scouting = SCT_BottomRight;
    Match_Array[9].Mark_Tile = 6;
    Match_Array[9].Mark_Adj = 9;
    Match_Array[9].Adj_Direction = 0;  // ; concept flaw, this should not yield extra scouting
    Match_Array[10].Tile_Value = SCT_BottomLeft | SCT_TopLeft | SCT_TopRight;
    Match_Array[10].Adj_Scouting = SCT_BottomLeft | SCT_TopLeft;
    Match_Array[10].Mark_Tile = 15;
    Match_Array[10].Mark_Adj = 3;
    Match_Array[10].Adj_Direction = 0;  // ; bogus configuration, should be fixed elsewhere
    Match_Array[11].Tile_Value = SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[11].Adj_Scouting = SCT_TopLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[11].Mark_Tile = 15;
    Match_Array[11].Mark_Adj = 15;
    Match_Array[11].Adj_Direction = 0;  // ; bogus configuration, should be fixed elsewhere
    Match_Array[12].Tile_Value = SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[12].Adj_Scouting = SCT_BottomLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[12].Mark_Tile = 15;
    Match_Array[12].Mark_Adj = 15;
    Match_Array[12].Adj_Direction = 0;  // ; bogus configuration, should be fixed elsewhere
    Match_Array[13].Tile_Value = SCT_BottomLeft | SCT_TopLeft | SCT_BottomRight;
    Match_Array[13].Adj_Scouting = SCT_BottomLeft | SCT_TopLeft | SCT_BottomRight;
    Match_Array[13].Mark_Tile = 11;
    Match_Array[13].Mark_Adj = 15;
    Match_Array[13].Adj_Direction = 1;  // ; bogus configuration, should be fixed elsewhere
    Match_Array[14].Tile_Value = SCT_BottomLeft | SCT_BottomRight;
    Match_Array[14].Adj_Scouting = SCT_BottomLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[14].Mark_Tile = 9;
    Match_Array[14].Mark_Adj = 15;
    Match_Array[14].Adj_Direction = 1;  // ; bogus configuration, should be fixed elsewhere
    Match_Array[15].Tile_Value = SCT_BottomLeft | SCT_TopLeft;
    Match_Array[15].Adj_Scouting = SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[15].Mark_Tile = 15;
    Match_Array[15].Mark_Adj = 15;
    Match_Array[15].Adj_Direction = 0;  // ; bogus configuration, should be fixed elsewhere
    Match_Array[16].Tile_Value = SCT_TopRight;
    Match_Array[16].Adj_Scouting = SCT_BottomLeft | SCT_BottomRight;
    Match_Array[16].Mark_Tile = 15;  // ; BUG: should be 12
    Match_Array[16].Mark_Adj = 15;  // ; BUG: should be 0Dh
    Match_Array[16].Adj_Direction = 1;
    Match_Array[17].Tile_Value = SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[17].Adj_Scouting = SCT_BottomLeft | SCT_TopLeft | SCT_TopRight;
    Match_Array[17].Mark_Tile = 15;
    Match_Array[17].Mark_Adj = 15;
    Match_Array[17].Adj_Direction = 2;  // ; bogus configuration, should be fixed elsewhere
    Match_Array[18].Tile_Value = SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[18].Adj_Scouting = SCT_BottomLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[18].Mark_Tile = 15;
    Match_Array[18].Mark_Adj = 15;
    Match_Array[18].Adj_Direction = 1;  // ; bogus configuration, should be fixed elsewhere
    Match_Array[19].Tile_Value = SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[19].Adj_Scouting = SCT_BottomLeft | SCT_TopLeft | SCT_BottomRight;
    Match_Array[19].Mark_Tile = 15;
    Match_Array[19].Mark_Adj = 15;
    Match_Array[19].Adj_Direction = 2;  // ; bogus configuration, should be fixed elsewhere
    Match_Array[20].Tile_Value = SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[20].Adj_Scouting = SCT_BottomLeft | SCT_TopLeft;
    Match_Array[20].Mark_Tile = 15;
    Match_Array[20].Mark_Adj = 15;
    Match_Array[20].Adj_Direction = 2;  // ; bogus configuration, should be fixed elsewhere
    Match_Array[21].Tile_Value = SCT_TopLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[21].Adj_Scouting = SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[21].Mark_Tile = 15;
    Match_Array[21].Mark_Adj = 15;
    Match_Array[21].Adj_Direction = 1;  // ; bogus configuration, should be fixed elsewhere
    Match_Array[22].Tile_Value = SCT_BottomLeft | SCT_TopLeft | SCT_TopRight;
    Match_Array[22].Adj_Scouting = SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[22].Mark_Tile = 15;
    Match_Array[22].Mark_Adj = 15;
    Match_Array[22].Adj_Direction = 1;  // ; bogus configuration, should be fixed elsewhere
    Match_Array[23].Tile_Value = SCT_TopLeft;
    Match_Array[23].Adj_Scouting = SCT_BottomLeft;
    Match_Array[23].Mark_Tile = 3;
    Match_Array[23].Mark_Adj = 3;
    Match_Array[23].Adj_Direction = 1;
    Match_Array[24].Tile_Value = SCT_TopLeft | SCT_TopRight;
    Match_Array[24].Adj_Scouting = SCT_BottomLeft;
    Match_Array[24].Mark_Tile = 15;  // ; BUG: should be 07h
    Match_Array[24].Mark_Adj = 3;
    Match_Array[24].Adj_Direction = 1;
    Match_Array[25].Tile_Value = SCT_BottomLeft | SCT_TopLeft | SCT_BottomRight;
    Match_Array[25].Adj_Scouting = SCT_BottomLeft | SCT_TopLeft;
    Match_Array[25].Mark_Tile = 15;
    Match_Array[25].Mark_Adj = 3;
    Match_Array[25].Adj_Direction = 0;  // ; bogus configuration, should be fixed elsewhere
    Match_Array[26].Tile_Value = SCT_TopLeft;
    Match_Array[26].Adj_Scouting = SCT_TopRight;
    Match_Array[26].Mark_Tile = 6;
    Match_Array[26].Mark_Adj = 6;
    Match_Array[26].Adj_Direction = 0;
    Match_Array[27].Tile_Value = SCT_TopLeft | SCT_TopRight;
    Match_Array[27].Adj_Scouting = SCT_BottomRight;
    Match_Array[27].Mark_Tile = 14;
    Match_Array[27].Mark_Adj = 12;
    Match_Array[27].Adj_Direction = 1;
    Match_Array[28].Tile_Value = SCT_TopRight;
    Match_Array[28].Adj_Scouting = SCT_BottomRight;
    Match_Array[28].Mark_Tile = 12;
    Match_Array[28].Mark_Adj = 12;
    Match_Array[28].Adj_Direction = 1;
    Match_Array[29].Tile_Value = SCT_TopRight | SCT_BottomRight;
    Match_Array[29].Adj_Scouting = SCT_TopLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[29].Mark_Tile = 12;
    Match_Array[29].Mark_Adj = 15;
    Match_Array[29].Adj_Direction = 0;  // ; bogus configuration, should be fixed elsewhere
    Match_Array[30].Tile_Value = SCT_TopRight | SCT_BottomRight;
    Match_Array[30].Adj_Scouting = SCT_BottomLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[30].Mark_Tile = 12;
    Match_Array[30].Mark_Adj = 15;
    Match_Array[30].Adj_Direction = 0;  // ; bogus configuration, should be fixed elsewhere
    Match_Array[31].Tile_Value = SCT_BottomLeft | SCT_TopLeft;
    Match_Array[31].Adj_Scouting = SCT_TopRight;
    Match_Array[31].Mark_Tile = 7;
    Match_Array[31].Mark_Adj = 6;
    Match_Array[31].Adj_Direction = 0;
    Match_Array[32].Tile_Value = SCT_BottomLeft;
    Match_Array[32].Adj_Scouting = SCT_TopRight | SCT_BottomRight;
    Match_Array[32].Mark_Tile = 9;
    Match_Array[32].Mark_Adj = 14;  // BUG: should be 0Dh;
    Match_Array[32].Adj_Direction = 0;
    Match_Array[33].Tile_Value = SCT_TopLeft | SCT_TopRight;
    Match_Array[33].Adj_Scouting = SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[33].Mark_Tile = 15;
    Match_Array[33].Mark_Adj = 15;
    Match_Array[33].Adj_Direction = 1;  // ; bogus configuration, should be fixed elsewhere
    Match_Array[34].Tile_Value = SCT_BottomLeft | SCT_BottomRight;
    Match_Array[34].Adj_Scouting = SCT_TopRight | SCT_BottomRight;
    Match_Array[34].Mark_Tile = 9;
    Match_Array[34].Mark_Adj = 15;
    Match_Array[34].Adj_Direction = 1;  // ; bogus configuration, should be fixed elsewhere
    Match_Array[35].Tile_Value = SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[35].Adj_Scouting = SCT_TopRight | SCT_BottomRight;
    Match_Array[35].Mark_Tile = 15;
    Match_Array[35].Mark_Adj = 15;
    Match_Array[35].Adj_Direction = 1;  // ; bogus configuration, should be fixed elsewhere
    Match_Array[36].Tile_Value = SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[36].Adj_Scouting = SCT_TopLeft;
    Match_Array[36].Mark_Tile = 15;
    Match_Array[36].Mark_Adj = 6;
    Match_Array[36].Adj_Direction = 1;  // ; bogus configuration, should be fixed elsewhere
    Match_Array[37].Tile_Value = SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight;
    Match_Array[37].Adj_Scouting = SCT_BottomLeft | SCT_TopRight;
    Match_Array[37].Mark_Tile = 15;
    Match_Array[37].Mark_Adj = 15;
    Match_Array[37].Adj_Direction = 1;  // ; bogus configuration, should be fixed elsewhere
    
    
    ptr_square_explored = &_square_explored[wp];
    
    wx_l2 = (wx - 2);

    if(wx_l2 < 0)
    {
        wx_l2 += WORLD_WIDTH;
    }
    
    IDK_wx_l2 = wx_l2;

    wy_u2 = (wy - 2);

    if(wy_u2 < 1)
    {
        wy_u2 = 1;
    }

    for(Y_LoopVar = 0; Y_LoopVar < 5; Y_LoopVar++)
    {

        for(X_LoopVar = 0; X_LoopVar < 5; X_LoopVar++)
        {

            IDK_wx_l2 = (wx_l2 + X_LoopVar);
            
            if(IDK_wx_l2 >= WORLD_WIDTH)
            {
                IDK_wx_l2 -= WORLD_WIDTH;
            }

            LoopTile_Scouting = ptr_square_explored[((wy_u2 * WORLD_WIDTH) + IDK_wx_l2)];

            for(Match_Index = 0; Match_Index < Match_Count; Match_Index++)
            {

                if(Match_Array[Match_Index].Tile_Value == LoopTile_Scouting)
                {

                    Adjacent_Row = wy_u2;

                    Adjacent_Col = IDK_wx_l2;
                    
                    switch(Match_Array[Match_Index].Adj_Direction)
                    {
                        case 0:  // ; Tile_Right
                        {
                            Adjacent_Col = (IDK_wx_l2 + 1);
                            if(Adjacent_Col >= WORLD_WIDTH)
                            {
                                Adjacent_Col -= WORLD_WIDTH;
                            }
                        } break;
                        case 1:  // ; Tile_Below
                        {
                            Adjacent_Row++;
                            if(Adjacent_Row >= WORLD_HEIGHT)
                            {
                                Adjacent_Row = WORLD_YMAX;
                            }
                        } break;
                        case 2:  // ; Tile_Left
                        {
                            Adjacent_Col = (IDK_wx_l2 - 1);
                            if(Adjacent_Col < 0)
                            {
                                Adjacent_Col = WORLD_XMAX;
                            }
                        } break;

                    }

                    Adj_Tile_Scouting = ptr_square_explored[((Adjacent_Row * WORLD_WIDTH) + Adjacent_Col)];
                    
                    if(Match_Array[Match_Index].Adj_Scouting == Adj_Tile_Scouting)
                    {

                        // ; BUG: modifying the tile scouting should also modify
                        // ; its locally stored value, otherwise matches further
                        // ; on will not work properly any more (a single tile
                        // ; can have a match in both directions)
                        ptr_square_explored[((wy_u2 * WORLD_WIDTH) + IDK_wx_l2)] = (Match_Array[Match_Index].Mark_Tile || LoopTile_Scouting);
                        
                        ptr_square_explored[((Adjacent_Row * WORLD_WIDTH) + Adjacent_Col)] = (Match_Array[Match_Index].Mark_Adj || Adj_Tile_Scouting);
                        
                    }

                }

            }

        }

        wy_u2++;

        if(wy_u2 >= WORLD_HEIGHT)
        {

            wy_u2 = WORLD_HEIGHT;

        }

    }

}


// WZD o68p09
/*
ArmyList_Draw_Reduced_Map()
    |-> List_Screen_Draw_Reduced_Map(86, 164, 49, 33, unit_world_p, unit_world_x, unit_world_y)
CityList_Draw_Reduced_Map()
    |-> List_Screen_Draw_Reduced_Map(42, 162, 49, 33,, city_world_p, city_world_x, city_world_y);


x,y only used for call to Draw_Picture()
    so, definitely just screen_x, screen_y

*/
void List_Screen_Draw_Reduced_Map(int16_t x, int16_t y, int16_t w, int16_t h, int16_t wp, int16_t wx, int16_t wy)
{
    int16_t mark_flag;
// minimap_y= word ptr -4
// minimap_x= word ptr -2
// x= word ptr  6
// y= word ptr  8
// minimap_width= word ptr  0Ah
// minimap_height= word ptr  0Ch
// Plane= word ptr  0Eh
// mid_x= word ptr  10h
// mid_y= word ptr  12h

    reduced_map_mark_cycle = ((reduced_map_mark_cycle + 1) % 8);

    mark_flag  = ST_FALSE;

    // NOTE: same logic as cycler on 'Main Screen' for 'Active Stack'
    if(reduced_map_mark_cycle < 4)
    {
        mark_flag = ST_TRUE;
    }

    Reduced_Map_Coords(&reduced_map_x, &reduced_map_y, wx, wy, w, h);
    Create_Reduced_Map_Picture(reduced_map_x, reduced_map_y, wp, _reduced_map_seg, w, h, wx, wy, mark_flag);

    Reset_Window();
    Draw_Picture(x, y, _reduced_map_seg);

}


/*
    WIZARDS.EXE  ovr150
*/

// WZD o150p01
// AKA Undef_Prev_Map_Draw_XY
void Reset_Map_Draw(void)
{

    map_draw_prev_x = ST_UNDEFINED;

    map_draw_prev_y = ST_UNDEFINED;
    
}

// WZD o150p04
void Draw_Map_Window(int16_t screen_x, int16_t screen_y, int16_t map_w, int16_t map_h, int16_t map_x, int16_t map_y, int16_t map_p)
{
    int16_t map_draw_curr_x;
    int16_t map_draw_curr_y;

    /*
        x,y for the last time it thinks we drew the movement map
        x,y for the target of this request to draw the movement
        ~== curr. vs. prev.
    */
    map_draw_curr_x = map_x;
    map_draw_curr_y = map_y;


    if(
        (map_draw_curr_x != map_draw_prev_x)
        ||
        (map_draw_curr_y != map_draw_prev_y)
    )
    {
        map_draw_full = ST_TRUE;
        map_draw_sustain = 1;
    }
    else
    {
        map_draw_sustain--;

        if(map_draw_sustain >= 0)
        {
            map_draw_full = ST_TRUE;
        }
        else
        {
            map_draw_full = ST_FALSE;
        }
    }

    map_draw_prev_x = map_draw_curr_x;
    map_draw_prev_y = map_draw_curr_y;

    Draw_Map_Terrain(screen_x, screen_y, map_w, map_h, map_draw_curr_x, map_draw_curr_y, map_p);
    Draw_Map_Minerals(screen_x, screen_y, map_w, map_h, map_draw_curr_x, map_draw_curr_y, map_p);
    Draw_Map_Biota(screen_x, screen_y, map_w, map_h, map_draw_curr_x, map_draw_curr_y, map_p);
    Draw_Map_Roads(screen_x, screen_y, map_w, map_h, map_draw_curr_x, map_draw_curr_y, map_p);
    Draw_Map_Cities(screen_x, screen_y, map_w, map_h, map_draw_curr_x, map_draw_curr_y, map_p);
    Draw_Map_Towers(screen_x, screen_y, map_w, map_h, map_draw_curr_x, map_draw_curr_y, map_p);
    Draw_Map_Lairs(screen_x, screen_y, map_w, map_h, map_draw_curr_x, map_draw_curr_y, map_p);
    Draw_Map_Units(screen_x, screen_y, map_w, map_h, map_draw_curr_x, map_draw_curr_y);
    Draw_Map_Nodes(screen_x, screen_y, map_w, map_h, map_draw_curr_x, map_draw_curr_y, map_p);
    Draw_Map_Unexplored_Area(screen_x, screen_y, map_w, map_h, map_draw_curr_x, map_draw_curr_y, map_p);

    Cycle_Map_Animations();

}


// WZD o150p05
void Draw_Map_Terrain(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t wp)
{
    int16_t terrain_tile_base;
    int16_t itr_world_x;
    int16_t itr_world_y;
    int16_t itr_screen_y;
    int16_t itr_screen_x;
    int16_t curr_world_x;
    uint8_t unexplored_area_flag;
    int16_t world_maps_offset;
    uint32_t terrain_lbx_000_offset;  // DNE in Dasm
    uint16_t terrain_001_index;  // DNE inm Dasm
    uint8_t l_terrain_001_0;
    uint8_t l_terrain_001_1;  // DNE in Dasm
    byte_ptr terrain_pict_seg;

    if(wp == 0)
    {
        terrain_tile_base = 0;
    }
    else
    {
        terrain_tile_base = 762; // 0x2FA
    }

    // MS-DOS  terrain_000_elpn = ST_UNDEFINED;


    itr_screen_y = screen_y;
    itr_world_y = world_grid_y;
    // while(world_grid_y + map_grid_height > itr_world_y)
    while(itr_world_y < (world_grid_y + map_grid_height))
    {
        itr_screen_x = screen_x;
        itr_world_x = world_grid_x;
        while(itr_world_x < (world_grid_x + map_grid_width))
        {
            if(itr_world_x >= WORLD_WIDTH)
            {
                curr_world_x = itr_world_x - WORLD_WIDTH;
            }
            else
            {
                curr_world_x = itr_world_x;
            }

            // DELETEME  unexplored_area_flag = _square_explored[(wp * WORLD_SIZE) + (itr_world_y * WORLD_WIDTH) + (curr_world_x)];
            unexplored_area_flag = GET_SQUARE_EXPLORED(curr_world_x, itr_world_y, wp);

            if(unexplored_area_flag == ST_FALSE)
            {
                Fill(itr_screen_x, itr_screen_y, (itr_screen_x + 19), (itr_screen_y + 17), 0);
            }
            else
            {
                world_maps_offset = ((wp * WORLD_SIZE * 2) + (itr_world_y * WORLD_WIDTH * 2) + (curr_world_x * 2));
                terrain_001_index = GET_2B_OFS(_world_maps,world_maps_offset);
                terrain_001_index += terrain_tile_base;
                terrain_001_index *= 2;  // because, sizeof(int16_t)
                l_terrain_001_0 = 0;
                l_terrain_001_0 = GET_1B_OFS(m_terrain_lbx_001, terrain_001_index + 0);
                l_terrain_001_1 = GET_1B_OFS(m_terrain_lbx_001, terrain_001_index + 1);

                assert( (l_terrain_001_0 == 0 && l_terrain_001_1 >= 2) || (l_terrain_001_0 != 0) );

                assert((l_terrain_001_0 & 0x7F) <= 41);  // highest  676032 / 16384 = 41.26171875  ...got 55

                map_draw_full = ST_TRUE;
                if( ((l_terrain_001_0 & 0x80) == 0) || (map_draw_full == ST_TRUE) )
                {
                    if((l_terrain_001_0 & 0x80) != 0)
                    {
                        l_terrain_001_0 = (l_terrain_001_0 & 0x7F);  // ~== -128  ... NOTE(JimBalcomb,20240915)  clear the sign-bit
                        l_terrain_001_1 += terrain_anim_ctr;
                    }

                    // MS-DOS  // Dasm  terrain_pict_seg = EMS_PFBA[( (((((m_terrain_lbx_001 * 2) + m_terrain_lbx_001) * 2) * 2) *2) )];
                    // MS-DOS  terrain_pict_seg = EMS_PFBA[(m_terrain_lbx_001 * 24)];  // segments; 24 * 16 = 384, sizeof pict; 180h offset from File, not Entry
                    // MS-DOS  if(terrain_000_elpn != terrain_001_0)
                    // MS-DOS  {
                    // MS-DOS      EMM_Map4Pages(terrain_001_0, g_EmmHndl_TERRAIN); // First Logical Page, EMM Handle Name
                    // MS-DOS  }

                    // map index in m_terrain_lbx_001 to TERRAIN.LBX Entry 0, instead of TERRAIN.LBX entire file in EMM
                    // loading entry 0, instead of whole file
                    // ( EMM Page * 16K ) + (terrain pict index * 384) - (difference in header size)
                    if(l_terrain_001_0 == 0)
                    {
                        terrain_lbx_000_offset = 0xC0 + ((l_terrain_001_1 - 2) * 384);
                        // terrain_lbx_000_offset = (terrain_001_0 * 16384) + ((m_terrain_lbx_001 - 2) * 384) + 0xC0;
                    }
                    else
                    {
                        terrain_lbx_000_offset = (l_terrain_001_0 * 16384) + (l_terrain_001_1 * 384) - 0xC0 - 384;
                        // terrain_lbx_000_offset = (terrain_001_0 * 16384) + (m_terrain_lbx_001 * 384) - 0xC0 - 384;
                    }

                    terrain_pict_seg = m_terrain_lbx_000 + terrain_lbx_000_offset;
                    Draw_Picture(itr_screen_x, itr_screen_y, terrain_pict_seg);
                }

            } /* if(unexplored_area_flag == ST_FALSE) */

            if(DBG_ShowTileInfo == ST_TRUE)
            {
                // TODO  j_EMM_Map_CONTXXX();  // ; maps in the EMM_ContXXX_H handle (all 4 pages), and resets its corresponding global pointers
                // TODO  Set_Font_Style(0,0);
                // Print_Integer(itr_screen_x, itr_screen_y + 12, _landmasses[(_map_plane * WORLD_SIZE) + (itr_world_y * WORLD_WIDTH) + curr_world_x]);
                // TODO  Set_Font_Style(0, 2);
                // Print_Integer(itr_screen_x, itr_screen_y, curr_world_x);
            }

            itr_screen_x += SQUARE_WIDTH;
            itr_world_x++;
        }

        itr_screen_y += SQUARE_HEIGHT;
        itr_world_y++;
    }

}


// WZD o150p06
void Cycle_Map_Animations(void)
{
    // map_anim_cycle = (1 - map_anim_cycle);  // toggle {1-0=1,1-1=0}
    // map_anim_cycle = ((map_anim_cycle + 1) % 2);
    map_anim_cycle = ((map_anim_cycle + 1) % 4);

    if(map_anim_cycle == 0)
    {
        terrain_anim_ctr = (terrain_anim_ctr + 1) % 4;
        node_anim_ctr = (node_anim_ctr + 1) % 6;
        road_anim_ctr = (road_anim_ctr + 1) % 6;
    }
}

// WZD o150p07
void Draw_Map_Unexplored_Area(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t wp)
{
    int16_t itr_screen_x;
    int16_t itr_screen_y;
    int16_t itr_world_x;
    int16_t itr_world_y;
    int16_t curr_world_x;
    uint16_t terrain_001_index;
    uint8_t terrain_001_0;
    uint8_t animated_terrain_flag;
    uint8_t square_explored_flag;
    SAMB_ptr unexplored_mask_pict_seg;

    itr_screen_y = screen_y;
    itr_world_y = world_grid_y;
    while(world_grid_y + map_grid_height > itr_world_y)
    {
        itr_screen_x = screen_x;
        itr_world_x = world_grid_x;
        while(world_grid_x + map_grid_width > itr_world_x)
        {
            if(itr_world_x < WORLD_WIDTH)
            {
                curr_world_x = itr_world_x;
            }
            else
            {
                curr_world_x = itr_world_x - WORLD_WIDTH;
            }

            terrain_001_index = GET_2B_OFS(_world_maps, ((wp * WORLD_SIZE * 2) + (itr_world_y * WORLD_WIDTH * 2) + (curr_world_x * 2)));
            terrain_001_0 = GET_1B_OFS(m_terrain_lbx_001, (terrain_001_index * 2) + 0);
            animated_terrain_flag = ((terrain_001_0 & 0x80) != 0);  // ¿ prefer ((bitfield >> 7) & 0x01) - saves the CPU doing a full register-sized compare ?


            // HERE: Redraw_Map_Unexplored_Area() does not test m_draw_full or animated terrain type bit

            if( (map_draw_full == ST_TRUE) || (animated_terrain_flag != ST_TRUE) )
            {
                // DELETEME  square_explored_flag = _square_explored[(wp * WORLD_SIZE) + (itr_world_y * WORLD_WIDTH) + (curr_world_x)];
                square_explored_flag = GET_SQUARE_EXPLORED(curr_world_x, itr_world_y, wp);

                if( (square_explored_flag != 0) && (square_explored_flag != 15) )
                {
                    unexplored_mask_pict_seg = unexplored_mask_seg[square_explored_flag - 1];
                    assert(unexplored_mask_pict_seg != NULL);
                    FLIC_Draw(itr_screen_x, itr_screen_y, unexplored_mask_pict_seg);
                }
            }

            itr_screen_x += SQUARE_WIDTH;
            itr_world_x += 1;
        }

        itr_screen_y += SQUARE_HEIGHT;
        itr_world_y += 1;
    }

}

// WZD o150p08
void Draw_Map_Cities(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t wp)
{
    int16_t itr_cities;
    int16_t city_x;
    int16_t city_y;
    int16_t city_owner;
    int16_t screen_start_x;
    int16_t screen_start_y;
    SAMB_ptr city_pict_seg;
    int8_t city_size;
    int16_t itr_color_remap;

    if(current_screen == scr_City_Screen)
    {
        Set_Window(215, 4, 454, 183);  // BUGBUG:  should be x2: (215 + (5 * 20) - 1) & y2: (4 + (5 * 18) - 1)
    }
    else
    {
        Set_Window(0, 20, 239, 199);
    }

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].wp == wp)
        {
            city_x = _CITIES[itr_cities].wx;
            city_y = _CITIES[itr_cities].wy;
            // DELETEME  if(_square_explored[((wp * WORLD_SIZE) + (city_y * WORLD_WIDTH) + city_x)] != ST_FALSE)
            if(GET_SQUARE_EXPLORED(city_x, city_y, wp)!= ST_FALSE)
            {
                city_x = city_x - world_grid_x;  // translate to map relative coordinates
                if(city_x < 0)
                {
                    city_x = city_x + WORLD_WIDTH;
                }
                city_y = city_y - world_grid_y;  // translate to map relative coordinates

                if(city_y >= 0)
                {
                    if(city_y - 1 < map_grid_height)
                    {
                        if(city_x >= 0)
                        {
                            if(city_x - 1 < map_grid_width)
                            {
                                city_owner = _CITIES[itr_cities].owner_idx;
                                screen_start_x = screen_x + (city_x * SQUARE_WIDTH) - 6;  // start 6 pixels early, to overdraw the City Picture
                                screen_start_y = screen_y + (city_y * SQUARE_HEIGHT) - 6;  // start 6 pixels early, to overdraw the City Picture

                                if(city_owner != NEUTRAL_PLAYER_IDX)  /* Neutral Player */
                                {
                                    if(_CITIES[itr_cities].bldg_status[CITY_WALLS] == bs_Replaced)  /* ¿ "B_Replaced" or just ST_FALSE ? */
                                    {
                                        city_pict_seg = IMG_OVL_NoWall_City;  // CITYNOWA
                                    }
                                    else
                                    {
                                        city_pict_seg = IMG_OVL_Walled_City;  // MAPCITY
                                    }

                                    // Pop_K: ¿ {0, ..., 25} ?
                                    // {1,4}-1/4=0 {5,8}-1/4=1 {9,12}-1/4=2 {13,16}-1/4=3  {17,20}-1/4=4 {21,24}-1/4=5 {25}-1/4=6
                                    // NOTE: this would seems to indicate than an 'Outpost' is not even in the cities table
                                    // TODO: figure out where Outposts get drawn on the map
                                    city_size = (_CITIES[itr_cities].population - 1) / 4;  
                                    if(city_size > 4)
                                    {
                                        city_size = 4;
                                    }
                                    Set_Animation_Frame(city_pict_seg, city_size);
                                    Draw_Picture_To_Bitmap(city_pict_seg, Map_Square_WorkArea);
                                    for(itr_color_remap = 0; itr_color_remap < 5; itr_color_remap++)
                                    {
                                        Replace_Color(Map_Square_WorkArea, 214 + itr_color_remap, (COL_City_Banner[((_players[city_owner].banner_id * 5) + itr_color_remap)] - 1));
                                    }
                                }
                                else
                                {
                                    if(_CITIES[itr_cities].bldg_status[CITY_WALLS] == bs_Replaced)  /* ¿ "B_Replaced" or just ST_FALSE ? */
                                    {
                                        city_pict_seg = IMG_OVL_NoWall_City;  // CITYNOWA
                                    }
                                    else
                                    {
                                        city_pict_seg = IMG_OVL_Walled_City;
                                    }
                                    city_size = (_CITIES[itr_cities].population - 1) / 4;
                                    if(city_size > 4)
                                    {
                                        city_size = 4;
                                    }
                                    Set_Animation_Frame(city_pict_seg, city_size);
                                    Draw_Picture_To_Bitmap(city_pict_seg, Map_Square_WorkArea);
                                    for(itr_color_remap = 0; itr_color_remap < 5; itr_color_remap++)
                                    {
                                        Replace_Color(Map_Square_WorkArea, 214 + itr_color_remap, 51 + itr_color_remap);
                                    }
                                }

                                Draw_Picture_Windowed(screen_start_x, screen_start_y, Map_Square_WorkArea);

                            }
                        }
                    }
                }
            }
        }
    }

}

// WZD o150p09
void Draw_Map_Towers(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t wp)
{
    int16_t itr_towers;
    int16_t tower_x;
    int16_t tower_y;
    uint8_t unexplored_flag;
    int16_t tower_owner_idx;
    int16_t start_x;
    int16_t start_y;
    int16_t itr_color_remap;

    for(itr_towers = 0; itr_towers < NUM_TOWERS; itr_towers++)
    {
        tower_x = _TOWERS[itr_towers].wx;
        tower_y = _TOWERS[itr_towers].wy;

        // DELETEME  unexplored_flag = _square_explored[(wp * WORLD_SIZE) + (tower_y * WORLD_WIDTH) + tower_x];
        unexplored_flag = GET_SQUARE_EXPLORED(tower_x, tower_y, wp);

        if(unexplored_flag != ST_FALSE)
        {
            tower_x = tower_x - world_grid_x;
            if(tower_x < 0)
            {
                tower_x = tower_x + WORLD_WIDTH;
            }
            tower_y = tower_y - world_grid_y;
            if( !(tower_y < 0) )
            {
                if(tower_y < map_grid_height)
                {
                    if( !(tower_x < 0) )
                    {
                        if(tower_x < map_grid_width)
                        {
                            tower_owner_idx = _TOWERS[itr_towers].owner_idx;
                            start_x = screen_x + (tower_x * SQUARE_WIDTH);
                            start_y = screen_y + (tower_y * SQUARE_HEIGHT);
                            if(tower_owner_idx != ST_UNDEFINED)
                            {
                                // SITES    unowned tower
                                // SITES    owned tower
                                Draw_Picture_To_Bitmap(tower_owned_seg, Map_Square_WorkArea);
                                for(itr_color_remap = 0; itr_color_remap < 5; itr_color_remap++)
                                {
                                    // ; BUG: parameter mismatch, passing a pointer instead of an actual color index!
                                    // ; BUG: this is the index of repeat colors in encoded images (224), the banner replacement colors start at index $D6 instead (214)
                                    // TODO(JimBalcomb,20230701): add this bug to the 'OG MoM v1.31 Bug-List'
                                    // ~== FLIC_Remap_Color(Map_Square_WorkArea, 214 + itr_color_remap, (COL_City_Banner[((_players[city_owner_idx  ].Banner * 5) + itr_color_remap)] - 1));
                                    //     FLIC_Remap_Color(Map_Square_WorkArea, 214 + itr_color_remap, (COL_Banners[((_players[towner_owner_idx].Banner * 5) + itr_color_remap)] - 1));
                                    Replace_Color(Map_Square_WorkArea, 224 + itr_color_remap, *(COL_Banners + (_players[tower_owner_idx].banner_id * 5)));
                                }
                            }
                            else
                            {
                                Draw_Picture_To_Bitmap(tower_unowned_seg, Map_Square_WorkArea);
                            }

                            Draw_Picture(start_x, start_y, Map_Square_WorkArea);

                        }
                    }
                }
            }
        }
    }

}

// WZD o150p10
/*
    Draws Lairs (but, not *Lairs* that Towers or Nodes)
*/
void Draw_Map_Lairs(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t wp)
{
    int16_t itr_lairs;
    int16_t lair_x;
    int16_t lair_y;
    uint8_t unexplored_flag;
    int16_t start_x;
    int16_t start_y;

    for(itr_lairs = 0; itr_lairs < NUM_LAIRS; itr_lairs++)
    {
        if(_LAIRS[itr_lairs].wp == wp)
        {
            if(_LAIRS[itr_lairs].Intact == ST_TRUE)
            {
                if(_LAIRS[itr_lairs].type > lt_Sorcery_Node)
                {
                    lair_x = _LAIRS[itr_lairs].wx;
                    lair_y = _LAIRS[itr_lairs].wy;

                    // DELETEME  unexplored_flag = _square_explored[(wp * WORLD_SIZE) + (lair_y * WORLD_WIDTH) + lair_x];
                    unexplored_flag = GET_SQUARE_EXPLORED(lair_x, lair_y, wp);

                    if(unexplored_flag != ST_FALSE)
                    {
                        lair_x = lair_x - world_grid_x;
                        if(lair_x < 0)
                        {
                            lair_x = lair_x + WORLD_WIDTH;
                        }
                        lair_y = lair_y - world_grid_y;
                        if( !(lair_y < 0) )
                        {
                            if(lair_y < map_grid_height)
                            {
                                if( !(lair_x < 0) )
                                {
                                    if( lair_x < map_grid_width)
                                    {
                                        start_x = screen_x + (lair_x * SQUARE_WIDTH);
                                        start_y = screen_y + (lair_y * SQUARE_HEIGHT);
                                        /*
                                        mov     ax, _SI_itr_lairs
                                        mov     dx, 18h
                                        imul    dx
                                        les     bx, [_LAIRS]
                                        add     bx, ax
                                        mov     al, [es:bx+s_LAIR.Type]
                                        cbw
                                        shl     ax, 1
                                        mov     bx, ax
                                        push    (IMG_OVL_EZ_Cave@ - (2*lt_Cave))[bx]
                                        push    (IMG_OVL_EZ_Cave@ - (2*lt_Cave))[bx] ; sa_FLIC_Header
                                        */
                                        // TODO(JimBalcomb,20230701): figure out what is going on with this approach to the pict seg
                                        // FWIW, they are in the same order in the Data Segment, Terrain_Init(), and enum (though I assume the enum is derived)
                                        // No-Workie  FLIC_Draw(start_x, start_y, (SAMB_ptr)(IMG_OVL_EZ_Cave + ((_LAIRS[itr_lairs].Type - lt_Cave) * sizeof(SAMB_ptr))));
                                        // No-Workie  FLIC_Draw(start_x, start_y, IMG_OVL_EZ_Cave + 1);
                                        // Workie  FLIC_Draw(start_x, start_y, IMG_OVL_EZ_Cave);
                                        switch(_LAIRS[itr_lairs].type)
                                        {
                                            case lt_Cave: {           FLIC_Draw(start_x, start_y, IMG_OVL_EZ_Cave);  } break;
                                            case lt_Dungeon: {        FLIC_Draw(start_x, start_y, IMG_OVL_EZ_Dung);  } break;
                                            case lt_Ancient_Temple: { FLIC_Draw(start_x, start_y, IMG_OVL_EZ_ATmpl); } break;
                                            case lt_Keep: {           FLIC_Draw(start_x, start_y, IMG_OVL_EZ_AKeep); } break;
                                            case lt_Monster_Lair: {   FLIC_Draw(start_x, start_y, IMG_OVL_EZ_MLair); } break;
                                            case lt_Ruins: {          FLIC_Draw(start_x, start_y, IMG_OVL_EZ_Ruins); } break;
                                            case lt_Fallen_Temple: {  FLIC_Draw(start_x, start_y, IMG_OVL_EZ_FTmpl); } break;
                                        }

                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

}

// WZD o150p11
void Draw_Map_Nodes(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t wp)
{
    int16_t itr_nodes;
    int16_t node_owner_idx;
    uint8_t node_owner_banner_idx;
    SAMB_ptr node_anim_seg;
    int8_t node_power;
    int16_t Tile_Index;
    uint8_t unexplored_area;
    int8_t node_aura_world_x;
    int8_t node_aura_world_y;
    int8_t node_aura_map_x;
    int8_t node_aura_map_y;
    int16_t start_x;
    int16_t start_y;
    int16_t node_anim_frame_idx;
// TODO      int16_t Warp_Byte_Index;
// TODO      int16_t Warp_Line_Value;
// TODO      uint32_t tmp_random_seed;

    for(itr_nodes = 0; itr_nodes < NUM_NODES; itr_nodes++)
    {
        if(wp == _NODES[itr_nodes].wp)
        {
            node_owner_idx = _NODES[itr_nodes].owner_idx;

            if(node_owner_idx != ST_UNDEFINED)
            {
                if(magic_set.show_node_owners == ST_TRUE)
                {
                    node_owner_banner_idx = _players[node_owner_idx].banner_id;

                    assert(node_owner_banner_idx <= 5);
                    // //       Severity	Code	Description	Project	File	Line	Suppression State
                    // // TODO  Warning	C6385	Reading invalid data from 'node_auras_seg':  the readable size is '40' bytes, but 'node_owner_banner_idx' bytes may be read.ReMoM	C : \STU\devel\ReMoM\src\MainScr_Maps.C	1860
                    // // Reading invalid data from the readable size is bytes but bytes may be read
                    // // try assert(image_encoders_count * sizeof(ImageCodecInfo) <= image_encoder_array_size);
                    // // or  assert(image_encoders_count <= image_encoder_array_size / sizeof(ImageCodecInfo));
                    // // try to use __analysis_assume() instead of assert()
                    // // node_anim_seg = node_auras_seg[node_owner_banner_idx];
                    // NOWORKIE  if (node_owner_banner_idx < sizeof(node_auras_seg))
                    // {
                    //     node_anim_seg = node_auras_seg[node_owner_banner_idx];
                    // }
                    // else
                    // {
                    //     __debug_break();
                    // }
                    #pragma warning(suppress : 6385)
                    node_anim_seg = node_auras_seg[node_owner_banner_idx];
                    node_power = _NODES[itr_nodes].power;

                    Tile_Index = 0;
                    while(Tile_Index < node_power)
                    {
                        node_aura_world_x = _NODES[itr_nodes].Aura_Xs[Tile_Index];

                        node_aura_world_y = _NODES[itr_nodes].Aura_Ys[Tile_Index];

                        // DELETEME  unexplored_area = _square_explored[(wp * WORLD_SIZE) + (node_aura_world_y * WORLD_WIDTH) + (node_aura_world_x)];
                        unexplored_area = GET_SQUARE_EXPLORED(node_aura_world_x, node_aura_world_y, wp);

                        if(unexplored_area != ST_FALSE)
                        {
                            node_aura_map_x = node_aura_world_x - world_grid_x;

                            if(node_aura_map_x < 0)
                            {
                                node_aura_map_x = node_aura_map_x + WORLD_WIDTH;
                            }
                            node_aura_map_y = node_aura_world_y - world_grid_y;

                            if( (node_aura_map_y >= 0) && (node_aura_map_y < map_grid_height) && (node_aura_map_x >= 0) && (node_aura_map_x < map_grid_width) )
                            {
                                start_x = screen_x + (node_aura_map_x * SQUARE_WIDTH);

                                start_y = screen_y + (node_aura_map_y * SQUARE_HEIGHT);

                                node_anim_frame_idx = ((node_anim_ctr + Tile_Index) % 6);

                                Set_Animation_Frame(node_anim_seg, node_anim_frame_idx);
                                FLIC_Draw(start_x, start_y, node_anim_seg);
                            }
                        }
                        Tile_Index++;
                    }

// SPELLY                      if( (_NODES[itr_nodes].flags & NF_WARPED) != 0 )
// SPELLY                      {
// SPELLY                          node_aura_world_x = _NODES[itr_nodes].Aura_Xs[Tile_Index];
// SPELLY                          node_aura_world_y = _NODES[itr_nodes].Aura_Ys[Tile_Index];
// SPELLY                          unexplored_area = _square_explored[(wp * WORLD_SIZE_DB) + (node_aura_world_y) + (node_aura_world_x)];
// SPELLY                          if(unexplored_area != ST_FALSE)
// SPELLY                          {
// SPELLY                              node_aura_map_x = node_aura_world_x - world_grid_x;
// SPELLY                              if(node_aura_map_x < 0)
// SPELLY                              {
// SPELLY                                  node_aura_map_x = node_aura_map_x + WORLD_WIDTH;
// SPELLY                              }
// SPELLY                              node_aura_map_y = node_aura_world_y - world_grid_y;
// SPELLY                              if( (node_aura_map_y >= 0) && (node_aura_map_y < map_grid_height) && (node_aura_map_x >= 0) && (node_aura_map_x < map_grid_width) )
// SPELLY                              {
// SPELLY                                  start_x = screen_x + (node_aura_map_y * SQUARE_WIDTH) + node_aura_map_x;
// SPELLY                                  start_y = screen_y + (node_aura_map_y * SQUARE_HEIGHT) + node_aura_map_x;
// SPELLY                                  FLIC_Set_CurrentFrame(node_warped_seg, 0);
// SPELLY                                  Draw_Picture_To_Bitmap(node_warped_seg, Map_Square_WorkArea);
// SPELLY                                  FLIC_Set_CurrentFrame(node_warped_seg, 0);
// SPELLY                                  Screen_Picture_Capture(start_x, start_y, start_x + 19, start_y + 17, Warp_Node_WorkArea);
// SPELLY                                  if(terrain_anim_ctr >= 0)
// SPELLY                                  {
// SPELLY                                      WarpNode_SeedSave = Get_Random_Seed();
// SPELLY                                      for(Warp_Byte_Index = 0; Warp_Byte_Index < 20; Warp_Byte_Index++)
// SPELLY                                      {
// SPELLY                                          Warp_Line_Value = (int16_t)TBL_Warp_GFX_Lines[Warp_Byte_Index];
// SPELLY                                          if(Random(4) == 1)
// SPELLY                                          {
// SPELLY                                              Warp_Line_Value++;
// SPELLY                                          }
// SPELLY                                          if(Random(4) == 1)
// SPELLY                                          {
// SPELLY                                              Warp_Line_Value--;
// SPELLY                                          }
// SPELLY                                          if(Warp_Line_Value < -1)
// SPELLY                                          {
// SPELLY                                              Warp_Line_Value = -1;
// SPELLY                                          }
// SPELLY                                          if(Warp_Line_Value > 1)
// SPELLY                                          {
// SPELLY                                              Warp_Line_Value = 1;
// SPELLY                                          }
// SPELLY                                          TBL_Warp_GFX_Lines[Warp_Byte_Index] = Warp_Line_Value;
// SPELLY                                      }
// SPELLY                                  }
// SPELLY                                  tmp_random_seed = Get_Random_Seed();
// SPELLY                                  Set_Random_Seed(WarpNode_SeedSave);
// SPELLY                                  // TODO  LBX_IMG_RandomDelete(Warp_Node_WorkArea, 50);
// SPELLY                                  Set_Random_Seed(tmp_random_seed);
// SPELLY                                  // TODO  LBX_IMG_HorzWarp(&TBL_Warp_GFX_Lines, Warp_Node_WorkArea);
// SPELLY                                  // TODO  LBX_IMG_VertWarp(&TBL_Warp_GFX_Lines, Warp_Node_WorkArea);
// SPELLY                                  // TODO  Clipped_Copy_Mask(0, 0, Warp_Node_WorkArea, Map_Square_WorkArea);
// SPELLY                                  Draw_Picture(start_x, start_y, Warp_Node_WorkArea);
// SPELLY                              }
// SPELLY                          }
// SPELLY                      }
                }
            }
        }
    }

}

// WZD o150p12
/*
    draws Terrain Specials - Biota - Wild Game, Hunters Lodge, Nightshade
*/
void Draw_Map_Biota(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t wp)
{
    int16_t itr_screen_x;
    int16_t itr_screen_y;
    int16_t itr_world_x;
    int16_t itr_world_y;
    int16_t curr_world_x;
    uint8_t unexplored_area;
    int16_t itr_cities;
    int16_t has_city;
    int16_t terrain_special;
    int16_t terrain_flag;
    SAMB_ptr site_pict_seg;

    itr_screen_y = screen_y;
    itr_world_y = world_grid_y;
    while(world_grid_y + map_grid_height > itr_world_y)
    {
        itr_screen_x = screen_x;
        itr_world_x = world_grid_x;
        while(world_grid_x + map_grid_width > itr_world_x)
        {
            if(itr_world_x < WORLD_WIDTH)
            {
                curr_world_x = itr_world_x;
            }
            else
            {
                curr_world_x = itr_world_x - WORLD_WIDTH;
            }

            // DELETEME  unexplored_area = _square_explored[(wp * WORLD_SIZE_DB) + (itr_world_y * WORLD_WIDTH) + (curr_world_x)];
            unexplored_area = GET_SQUARE_EXPLORED(curr_world_x, itr_world_y, wp);

            if(unexplored_area != ST_FALSE)
            {
                terrain_special = _map_square_terrain_specials[(wp * WORLD_SIZE_DB) + (itr_world_y * WORLD_WIDTH) + (curr_world_x)];

                terrain_flag = _map_square_flags[(wp * WORLD_SIZE_DB) + (itr_world_y * WORLD_WIDTH) + (curr_world_x)];

                if((terrain_flag & MSF_CORRUPTION) != 0)
                {
                    site_pict_seg = IMG_OVL_Corruption;
                    FLIC_Draw(itr_screen_x, itr_screen_y, site_pict_seg);
                }

                if((terrain_special & TS_WILDGAME) != 0)
                {
                    has_city = ST_FALSE;
                    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
                    {
                        if((_CITIES[itr_cities].wx == curr_world_x) && (_CITIES[itr_cities].wy == itr_world_y) && (_CITIES[itr_cities].wp == wp))
                        {
                            has_city = ST_TRUE;
                        }
                    }

                    if(has_city == ST_FALSE)
                    {
                        site_pict_seg = IMG_OVL_WildGame;
                        // FLIC_Draw(itr_screen_x, itr_screen_y, site_pict_seg);
                        
                        site_pict_seg = UU_hunters_lodge_seg;
                        FLIC_Draw(itr_screen_x, itr_screen_y, site_pict_seg);

                        if((terrain_special & TS_HUNTERSLODGE) != 0)
                        {
                            site_pict_seg = UU_hunters_lodge_seg;
                            FLIC_Draw(itr_screen_x, itr_screen_y, site_pict_seg);
                        }
                    }
                }

                if((terrain_special & TS_NIGHTSHADE) != 0)
                {
                    site_pict_seg = IMG_OVL_Nightshade;
                    FLIC_Draw(itr_screen_x, itr_screen_y, site_pict_seg);
                }

            }
            itr_screen_x += SQUARE_WIDTH;
            itr_world_x += 1;
        }
        itr_screen_y += SQUARE_HEIGHT;
        itr_world_y += 1;
    }

}

// WZD o150p13
/*
    draws Terrain Specials - Minerals
*/
void Draw_Map_Minerals(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t wp)
{
    // DEDU  does this actually exist in the Dasm?  uint8_t * ptr_TBL_Scouting;
    uint8_t * terrain_specials_ptr;
    int16_t terrain_special;
    int16_t City_Cover;
    int16_t curr_world_x;
    int16_t itr_screen_x;
    int16_t itr_screen_y;
    int16_t itr_world_x;
    int16_t itr_world_y;
    int16_t itr_cities;
    uint8_t unexplored_area;  // DNE in Dasm
    SAMB_ptr site_pict_seg;  // DNE in Dasm

    itr_screen_y = screen_y;

    itr_world_y = world_grid_y;

    while(world_grid_y + map_grid_height > itr_world_y)
    {
        itr_screen_x = screen_x;

        terrain_specials_ptr = (uint8_t *)(_map_square_terrain_specials + (wp * WORLD_SIZE) + (itr_world_y * WORLD_WIDTH));

        // TODO  square_explored_ptr

        itr_world_x = world_grid_x;

        while(world_grid_x + map_grid_width > itr_world_x)
        {

            if(itr_world_x < WORLD_WIDTH)
            {

                curr_world_x = itr_world_x;

            }
            else
            {

                curr_world_x = itr_world_x - WORLD_WIDTH;

            }

            unexplored_area = GET_SQUARE_EXPLORED(curr_world_x, itr_world_y, wp);

            // if(square_explored_ptr + 
            if(unexplored_area != ST_FALSE)
            {

                terrain_special = (((int16_t)*(terrain_specials_ptr + curr_world_x)) & 0x0F);

                if(terrain_special != 0)
                {

                    City_Cover = 0;

                    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
                    {

                        if(
                            (_CITIES[itr_cities].wx == curr_world_x)
                            &&
                            (_CITIES[itr_cities].wy == itr_world_y)
                            &&
                            (_CITIES[itr_cities].wp == wp)
                        )
                        {

                            City_Cover = 1;

                        }

                    }

                    if(City_Cover == 0)
                    {

                        site_pict_seg = mineral_site_segs[terrain_special];

                        FLIC_Draw(itr_screen_x, itr_screen_y, site_pict_seg);

                    }

                }

            }

            itr_screen_x += SQUARE_WIDTH;

            itr_world_x += 1;

        }

        itr_screen_y += SQUARE_HEIGHT;

        itr_world_y += 1;

    }

}

// WZD o150p14
void Draw_Map_Roads(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t wp)
{
    int16_t itr_screen_x = 0;
    int16_t itr_screen_y = 0;
    int16_t itr_world_x = 0;
    int16_t itr_world_y = 0;
    int16_t curr_world_x = 0;
    uint8_t unexplored_area = 0;
    uint8_t * terrain_flags_table_sgmt = 0;
    int16_t terrain_flags_table_ofst = 0;
    uint8_t has_road = 0;
    uint8_t has_eroad = 0;
    int16_t itr_cities = 0;
    int16_t has_city = 0;

    itr_screen_y = screen_y;
    itr_world_y = world_grid_y;
    while(world_grid_y + map_grid_height > itr_world_y)
    {
        itr_screen_x = screen_x;
        itr_world_x = world_grid_x;
        while(world_grid_x + map_grid_width > itr_world_x)
        {
            if(itr_world_x < WORLD_WIDTH)
            {
                curr_world_x = itr_world_x;
            }
            else
            {
                curr_world_x = itr_world_x - WORLD_WIDTH;
            }

            // DELETEME  unexplored_area = _square_explored[(wp * WORLD_SIZE) + (itr_world_y * WORLD_WIDTH) + (curr_world_x)];
            unexplored_area = GET_SQUARE_EXPLORED(curr_world_x, itr_world_y, wp);
            if(unexplored_area != ST_FALSE)
            {
                terrain_flags_table_sgmt = (uint8_t *)_map_square_flags;
                terrain_flags_table_ofst = (wp * WORLD_SIZE) + ((itr_world_y - 1) * WORLD_WIDTH) + (curr_world_x - 1);
                // -------------
                // |  0|  1|  2|
                // -------------
                // | 60| 61| 62|
                // -------------
                // |120|121|122|
                // -------------
                // test order: N, NE, E, SE, S, SW, W, NW  (1, 2, 62, 122, 121, 120, 60, 0)

                has_road = (*(terrain_flags_table_sgmt + terrain_flags_table_ofst + 61) & 0x08); /* Road Flag */
                if(has_road != 0)
                {
                    has_eroad = (*(terrain_flags_table_sgmt + terrain_flags_table_ofst + 61) & 0x10); /* Enchanted Road Flag */
                    if(has_eroad == 0)
                    {
                        if((*(terrain_flags_table_sgmt + terrain_flags_table_ofst +   1) & 0x08) != 0)
                        {
                            FLIC_Draw(itr_screen_x, itr_screen_y, roads_seg[0]);
                        }
                        if((*(terrain_flags_table_sgmt + terrain_flags_table_ofst +   2) & 0x08) != 0)
                        {
                            FLIC_Draw(itr_screen_x, itr_screen_y, roads_seg[1]);
                        }
                        if((*(terrain_flags_table_sgmt + terrain_flags_table_ofst +  62) & 0x08) != 0)
                        {
                            FLIC_Draw(itr_screen_x, itr_screen_y, roads_seg[2]);
                        }
                        if((*(terrain_flags_table_sgmt + terrain_flags_table_ofst + 122) & 0x08) != 0)
                        {
                            FLIC_Draw(itr_screen_x, itr_screen_y, roads_seg[3]);
                        }
                        if((*(terrain_flags_table_sgmt + terrain_flags_table_ofst + 121) & 0x08) != 0)
                        {
                            FLIC_Draw(itr_screen_x, itr_screen_y, roads_seg[4]);
                        }
                        if((*(terrain_flags_table_sgmt + terrain_flags_table_ofst + 120) & 0x08) != 0)
                        {
                            FLIC_Draw(itr_screen_x, itr_screen_y, roads_seg[5]);
                        }
                        if((*(terrain_flags_table_sgmt + terrain_flags_table_ofst +  60) & 0x08) != 0)
                        {
                            FLIC_Draw(itr_screen_x, itr_screen_y, roads_seg[6]);
                        }
                        if((*(terrain_flags_table_sgmt + terrain_flags_table_ofst +   0) & 0x08) != 0)
                        {
                            FLIC_Draw(itr_screen_x, itr_screen_y, roads_seg[7]);
                        }                        
                    }
                    else
                    {
                        if((*(terrain_flags_table_sgmt + terrain_flags_table_ofst +   1) & 0x08) != 0)
                        {
                            Set_Animation_Frame(eroads_seg[0], road_anim_ctr);
                            FLIC_Draw(itr_screen_x, itr_screen_y, eroads_seg[0]);
                        }
                        if((*(terrain_flags_table_sgmt + terrain_flags_table_ofst +   2) & 0x08) != 0)
                        {
                            Set_Animation_Frame(eroads_seg[1], road_anim_ctr);
                            FLIC_Draw(itr_screen_x, itr_screen_y, eroads_seg[1]);
                        }
                        if((*(terrain_flags_table_sgmt + terrain_flags_table_ofst +  62) & 0x08) != 0)
                        {
                            Set_Animation_Frame(eroads_seg[2], road_anim_ctr);
                            FLIC_Draw(itr_screen_x, itr_screen_y, eroads_seg[2]);
                        }
                        if((*(terrain_flags_table_sgmt + terrain_flags_table_ofst + 122) & 0x08) != 0)
                        {
                            Set_Animation_Frame(eroads_seg[3], road_anim_ctr);
                            FLIC_Draw(itr_screen_x, itr_screen_y, eroads_seg[3]);
                        }
                        if((*(terrain_flags_table_sgmt + terrain_flags_table_ofst + 121) & 0x08) != 0)
                        {
                            Set_Animation_Frame(eroads_seg[4], road_anim_ctr);
                            FLIC_Draw(itr_screen_x, itr_screen_y, eroads_seg[4]);
                        }
                        if((*(terrain_flags_table_sgmt + terrain_flags_table_ofst + 120) & 0x08) != 0)
                        {
                            Set_Animation_Frame(eroads_seg[5], road_anim_ctr);
                            FLIC_Draw(itr_screen_x, itr_screen_y, eroads_seg[5]);
                        }
                        if((*(terrain_flags_table_sgmt + terrain_flags_table_ofst +  60) & 0x08) != 0)
                        {
                            Set_Animation_Frame(eroads_seg[6], road_anim_ctr);
                            FLIC_Draw(itr_screen_x, itr_screen_y, eroads_seg[6]);
                        }
                        if((*(terrain_flags_table_sgmt + terrain_flags_table_ofst +   0) & 0x08) != 0)
                        {
                            Set_Animation_Frame(eroads_seg[7], road_anim_ctr);
                            FLIC_Draw(itr_screen_x, itr_screen_y, eroads_seg[7]);
                        }                        
                    }

                }
            }

            itr_screen_x += SQUARE_WIDTH;
            itr_world_x += 1;
        }

        itr_screen_y += SQUARE_HEIGHT;
        itr_world_y += 1;
    }

}

// WZD o150p15
/*
includes specific handling for 3x3 maps - 'City Screen', etc.

*/
void Draw_Map_Units(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y)
{
    int16_t map_start_x;
    int16_t map_start_y;
    int16_t itr_screen_x;
    int16_t itr_screen_y;
    int16_t itr_map_x;
    int16_t itr_map_y;
    int16_t unit_idx;
    int16_t entities_index;

    if(map_grid_width == 3)
    {
        map_start_x = world_grid_x - _map_x;
        if(map_start_x < 0)
        {
            map_start_x = map_start_x + WORLD_WIDTH;
        }
        if(map_start_x >= WORLD_WIDTH)
        {
            map_start_x = map_start_x - WORLD_WIDTH;
        }
        map_start_y = world_grid_y - _map_y;
    }
    else
    {
        map_start_x = 0;
        map_start_y = 0;
    }

    itr_screen_y = screen_y;
    itr_map_y = 0;
    while(itr_map_y < map_grid_height)
    {
        itr_screen_x = screen_x;
        itr_map_x = 0;
        while(itr_map_x < map_grid_width)
        {
            // unit_idx = entities_on_movement_map[ ((itr_map_y + map_start_y) * MAP_WIDTH) + (itr_map_x + map_start_x) ];
            entities_index = (((itr_map_y + map_start_y) * MAP_WIDTH) + (itr_map_x + map_start_x));
            unit_idx = entities_on_movement_map[entities_index];

            if((unit_idx != ST_UNDEFINED) && (unit_idx < MAX_UNIT_COUNT))
            {
                Draw_Unit_Picture(itr_screen_x, itr_screen_y, unit_idx, 2);
            }

            itr_screen_x += SQUARE_WIDTH;
            itr_map_x += 1;
        }

        itr_screen_y += SQUARE_HEIGHT;
        itr_map_y += 1;
    }

// TODO      for(itr_screen_y = screen_y, itr_map_y = 0; itr_map_y < map_grid_height; itr_screen_y += SQUARE_HEIGHT, itr_map_y++)
// TODO      {
// TODO          
// TODO          for(itr_screen_x = screen_x, itr_map_x = 0; itr_map_x < map_grid_width; itr_screen_x += SQUARE_WIDTH, itr_map_x++)
// TODO          {
// TODO  
// TODO              entities_index = (((itr_map_y + map_start_y) * MAP_WIDTH) + (itr_map_x + map_start_x));
// TODO  
// TODO              unit_idx = entities_on_movement_map[entities_index];
// TODO  
// TODO              if(
// TODO                  (unit_idx != ST_UNDEFINED)
// TODO                  &&
// TODO                  (unit_idx < MAX_UNIT_COUNT)
// TODO              )
// TODO              {
// TODO  
// TODO                  Draw_Unit_Picture(itr_screen_x, itr_screen_y, unit_idx, 2);
// TODO  
// TODO              }
// TODO  
// TODO          }
// TODO  
// TODO      }

}


// WZD o150p16
// "Reduced Map"/"World Window"
// AKA Draw_Minimap()
// AKA Draw_Reduced_Map
// AKA Draw_Minimap()
// void Draw_Minimap(int16_t minimap_start_x, int16_t minimap_start_y, int16_t wp, byte_ptr minimap_pict_seg, int16_t minimap_width, int16_t minimap_height, int16_t Mark_X, int16_t Mark_Y, int16_t Mark);
// TODO  rename to ~create/build
/*
    TODO(JimBalcomb,20240914):  MainScr_Maps.C  Create_Reduced_Map_Picture()  needs a full review

Elsewhere, Draw_World_Window() is called to call Draw_Picture() on _reduced_map_seg

*/
void Create_Reduced_Map_Picture(int16_t minimap_start_x, int16_t minimap_start_y, int16_t wp, byte_ptr minimap_pict_seg, int16_t minimap_width, int16_t minimap_height, int16_t mark_x, int16_t mark_y, int16_t mark_flag)
{
// minimap_pict_data_ptr = dword ptr - 1Ch
// world_data_ptr = dword ptr - 18h
// explore_data_ptr = dword ptr - 14h
// terrain_type_idx_base = word ptr - 10h
// city_owner_idx = word ptr - 0Eh
// itr_cities = word ptr - 0Ch
// minimap_square_y = word ptr - 0Ah
// terrain_type_idx = word ptr - 8
// minimap_square_x = word ptr - 6
// itr_minimap_height = word ptr - 4
// itr_minimap_width = word ptr - 2
// 
// minimap_start_x = word ptr  6
// minimap_start_y = word ptr  8
// wp = word ptr  0Ah
// minimap_pict_seg = word ptr  0Ch
// minimap_width = word ptr  0Eh
// minimap_height = word ptr  10h
// Mark_X = word ptr  12h
// Mark_Y = word ptr  14h
// mark_flag = word ptr  16h
// 
// _SI_city_world_x = si
// _DI_city_world_y = di

    int16_t terrain_type_idx_base = 0;
    int16_t terrain_type_idx = 0;
    uint8_t * explore_data_ptr = 0;
    uint8_t * world_data_ptr = 0;
    uint8_t * minimap_pict_data_ptr = 0;
    int16_t itr_minimap_height = 0;
    int16_t itr_minimap_width = 0;

    int16_t minimap_square_y = 0;
    int16_t minimap_square_x = 0;
    int8_t explored_flag = 0;
    uint8_t minimap_terrain_color = 0;
    int16_t itr_cities = 0;
    int16_t city_world_x = 0;
    int16_t city_world_y = 0;
    int16_t city_minimap_x = 0;
    int16_t city_minimap_y = 0;
    int16_t city_owner_idx = 0;
    int16_t mark_minimap_x = 0;
    int16_t mark_minimap_y = 0;

    uint8_t square_explored_flag = 0;  // DNE in Dasm
    int16_t minimap_square_y_gte_zero = 0;  // DNE in Dasm
    int16_t minimap_square_y_lt_world_height = 0;  // DNE in Dasm

    terrain_type_idx_base = wp * NUM_TERRAIN_TYPES;

    explore_data_ptr = (_square_explored + (wp * WORLD_SIZE));
    world_data_ptr = (_world_maps + (wp * WORLD_SIZE_DW));
    minimap_pict_data_ptr = minimap_pict_seg + SZ_PARAGRAPH_B;  // +1 segment (paragraph), for the animation/picture/bitmap header

    // WZD s30p06
    // void Create_Blank_Picture(int16_t width, int16_t height, byte_ptr pict_seg, int16_t color);
    Create_Blank_Picture(minimap_width, minimap_height, minimap_pict_seg, ST_BLACK);
    // Create_Blank_Picture(minimap_width, minimap_height, minimap_pict_seg, 171);
    // 217);  // ~ bright green
    // banner colors: {171, 216, 205, 201, 210, 50}
    // 171 ~ blue
    // 216 ~ green
    // 205 ~ purple
    // 201 ~ red
    // 210 ~ yellow
    //  50 ~ brown
    // WZD dseg:700A AB D8 CD C9 D2 32                               COL_MinimapBanners db 171, 216, 205, 201, 210, 50
    // WZD dseg:7010 32                                              COL_MinimapNeutral db 50


    
    for(itr_minimap_height = 0; itr_minimap_height < minimap_height; itr_minimap_height++)
    {
        for(itr_minimap_width = 0; itr_minimap_width < minimap_width; itr_minimap_width++)
        {

            minimap_square_x = minimap_start_x + itr_minimap_width;
            minimap_square_y = minimap_start_y + itr_minimap_height;
            if(minimap_square_x < 0) { minimap_square_x += WORLD_WIDTH; }
            if(minimap_square_x > WORLD_WIDTH) { minimap_square_x -= WORLD_WIDTH; }

            if(
                (
                    minimap_square_y >= 0
                    &&
                    minimap_square_y < WORLD_HEIGHT
                )
                &&
                *(explore_data_ptr + (minimap_square_y * WORLD_WIDTH) + minimap_square_x) != ST_FALSE
            )
            {

            }
            square_explored_flag = *(explore_data_ptr + (minimap_square_y * WORLD_WIDTH) + minimap_square_x);
            square_explored_flag = GET_SQUARE_EXPLORED(minimap_square_x, minimap_square_y, wp);
            // // if ((minimap_square_y >= 0 && minimap_square_y < WORLD_HEIGHT) && (square_explored != UNEXPLORED));
            // if(
            //     (
            //         (minimap_square_y >= 0)
            //         &&
            //         (minimap_square_y < WORLD_HEIGHT)
            //     )
            //     &&
            //     (square_explored != 0)
            // )
            minimap_square_y_gte_zero = (minimap_square_y >= 0);
            minimap_square_y_lt_world_height = (minimap_square_y < WORLD_HEIGHT);
            // WTF  IDGI
            if(
                (
                    minimap_square_y_gte_zero
                    &&
                    minimap_square_y_lt_world_height
                )
                &&
                square_explored_flag
            )
            {
                assert(square_explored_flag != ST_FALSE);
                terrain_type_idx = GET_2B_OFS(world_data_ptr, ((minimap_square_y * 120) + (minimap_square_x * 2)));
                terrain_type_idx += terrain_type_idx_base;
                minimap_terrain_color = m_terrain_lbx_002[terrain_type_idx];
                *(minimap_pict_data_ptr + ((itr_minimap_width * minimap_height) + itr_minimap_height)) = minimap_terrain_color;
            }
        }
    }

    /*
        BEGIN: Loop Cities
    */
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].wp == wp)
        {
            city_world_x = _CITIES[itr_cities].wx;
            city_world_y = _CITIES[itr_cities].wy;
            // DELETEME  if(*(explore_data_ptr + (city_world_y * WORLD_WIDTH) + city_world_x) != ST_FALSE)
            if(GET_SQUARE_EXPLORED(minimap_square_x, minimap_square_y, wp) != ST_FALSE)
            {
                city_minimap_x = city_world_x - minimap_start_x;
                if(city_minimap_x < 0)
                {
                    city_minimap_x += WORLD_WIDTH;
                }
                city_minimap_y = city_world_y - minimap_start_y;
                if(city_minimap_y >= 0)
                {
                    if(city_minimap_y < minimap_height)
                    {
                        if(city_minimap_x >= 0)
                        {
                            if(city_minimap_x < minimap_width)
                            {
                                city_owner_idx = _CITIES[itr_cities].owner_idx;
                                if(city_owner_idx == NEUTRAL_PLAYER_IDX)
                                {
                                    *(minimap_pict_data_ptr + ((city_minimap_x * minimap_height) + city_minimap_y)) = COL_MinimapNeutral;
                                }
                                else
                                {
                                    *(minimap_pict_data_ptr + ((city_minimap_x * minimap_height) + city_minimap_y)) = COL_MinimapBanners[_players[city_owner_idx].banner_id];
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    /*
        END: Loop Cities
    */


    if(mark_flag != ST_FALSE)
    {
        mark_minimap_x = mark_x - minimap_start_x;
        mark_minimap_y = mark_y - minimap_start_y;
        if(mark_minimap_x < 0) { mark_minimap_x += WORLD_WIDTH; }
        if(mark_minimap_x > WORLD_WIDTH) { mark_minimap_x -= WORLD_WIDTH; }
        if(mark_minimap_y < 0) { mark_minimap_y += WORLD_HEIGHT; }
        if(mark_minimap_y > WORLD_HEIGHT) { mark_minimap_y -= WORLD_HEIGHT; }
        *(minimap_pict_data_ptr + ((mark_minimap_x * minimap_height) + mark_minimap_y)) = MINIMAP_MARK_COLOR;
    }

}


// WZD o150p17
// Cartograph_Screen_Draw_Map_Terrain__NOWORKIE
/*


Cartograph_Screen_Draw_Map__WIP()
    |-> Cartograph_Screen_Draw_Map_Terrain__NOWORKIE(flag, cartograph_seg);

~ Draw_Map_Terrain()

*/
void Cartograph_Screen_Draw_Map_Terrain__NOWORKIE(int16_t cartograph_plane, SAMB_ptr cartograph_seg)
{
    byte_ptr ptr_cartograph_pict_palette = 0;
    byte_ptr ptr_current_palette = 0;
    SAMB_ptr cartograph_pict = 0;
    int16_t itr_colors = 0;
    int16_t square_explored_flag = 0;
    int16_t Color_Intensity = 0;
    byte_ptr terrain_pict_seg = 0;
    int16_t itr_world_height = 0;
    int16_t Map_Start_Offset = 0;
    int16_t itr_palette = 0;
    int16_t terrain_000_elpn = 0;
    int16_t terrain_type = 0;
    uint8_t l_terrain_001_0 = 0;
    uint8_t l_terrain_001_1 = 0;  // DNE in Dasm
    uint16_t l_terrain_001 = 0;  // DNE in Dasm
    int16_t itr_world_width = 0;  // _SI_

    byte_ptr src_sgmt = 0;  // _DS_
    int16_t src_ofst = 0;  // _SI_
    byte_ptr dst_sgmt = 0;  // _ES_
    int16_t dst_ofst = 0;  // _DI_

    uint32_t terrain_lbx_000_offset = 0;  // DNE in Dasm
    uint16_t terrain_001_index = 0;  // DNE inm Dasm
    uint8_t baito = 0;  // DNE in Dasm
    uint8_t animated_terrain_flag = 0;  // DNE in Dasm
    uint8_t emm_logical_page_index = 0;  // DNE in Dasm
    uint8_t emm_terain_picture_index = 0;  // DNE in Dasm


    cartograph_pict = (SAMB_ptr)&cartograph_seg[16];  // ¿ sizeof(struct s_FLIC_HEADER) or SZ_PARAGRAPH or color block ?

    Create_Picture(240, 160, cartograph_pict);  // (WORLD_WIDTH * 4) x (WORLD_HEIGHT * 4)

    // ptr_current_palette = SA_MK_FP0(current_palette);
    ptr_current_palette = current_palette;

    // ptr_cartograph_pict_palette = SA_MK_FP0(cartograph_seg);
    // ptr_cartograph_pict_palette = (SAMB_ptr)cartograph_seg;
    ptr_cartograph_pict_palette = (SAMB_ptr)&cartograph_seg[32];

    // drake178: fill the first 256 bytes (the color data) with intensity values ((R+G+B/2)/5+E0h) of the current palette colors, reserving 102 and 103 as zeroes
    // for(itr = 0; itr < 768; itr++)
    // {
    //     *(p_Palette + itr) = 0;
    // }
    /*
    itr_colors = 0;
    itr_palette = 0;
    while(itr_palette < 768)
    {

        if(itr_colors == 102)
        {
            ptr_cartograph_pict_palette[itr_colors] = ST_TRANSPARENT;
        }
        else if(itr_colors == 103)
        {
            ptr_cartograph_pict_palette[itr_colors] = ST_TRANSPARENT;
        }
        else
        {

            Color_Intensity = ((ptr_current_palette[itr_palette] + ptr_current_palette[(itr_palette + 1)]) + (ptr_current_palette[(itr_palette + 2)] / 2));

            ptr_cartograph_pict_palette[itr_colors] = (224 + (Color_Intensity / 5));  // ¿ RLE op repeat ? ¿ manual Replace_Color() ?
            
        }

        itr_colors += 1;
        
        itr_palette += 3;
    }
    */

    terrain_000_elpn = ST_UNDEFINED;

    Map_Start_Offset = 272;  // ¿ color-map index ?

    for(itr_world_width = 0; itr_world_width < WORLD_WIDTH; itr_world_width++)
    {

        for(itr_world_height = 0; itr_world_height < WORLD_HEIGHT; itr_world_height++)
        {

            if(GET_SQUARE_EXPLORED(itr_world_width, itr_world_height, cartograph_plane) != UNEXPLORED)
            {

                terrain_type = _world_maps[((cartograph_plane * WORLD_SIZE) + (itr_world_height * WORLD_WIDTH) + itr_world_width)];

                l_terrain_001 = m_terrain_lbx_001[terrain_type];
                animated_terrain_flag    = (uint8_t)((l_terrain_001 & 0x8000) >> 15);
                emm_logical_page_index   = (uint8_t)((l_terrain_001 & 0x7F00) >>  8); 
                emm_terain_picture_index = (uint8_t)( l_terrain_001 & 0x00FF       ); 

                /* HACK */  terrain_001_index = (terrain_type * 2);

                l_terrain_001_0 = 0;
                l_terrain_001_0 = GET_1B_OFS(m_terrain_lbx_001, terrain_001_index + 0);
                l_terrain_001_1 = GET_1B_OFS(m_terrain_lbx_001, terrain_001_index + 1);

                assert( (l_terrain_001_0 == 0 && l_terrain_001_1 >= 2) || (l_terrain_001_0 != 0) );
                assert((l_terrain_001_0 & 0x7F) <= 39);
                assert(l_terrain_001_1 <= 127);

                if((l_terrain_001_0 & 0x80) != 0) /* drake178: animated tile */
                {
                    m_terrain_lbx_001 += terrain_anim_ctr;
                    l_terrain_001_0 = (l_terrain_001_0 & 0x7F);  // clear the sign bit
                }

                // drake178: ; * 180h, from FILE start, not entry start!  ; contains the segment address of the EMS page frame
                // TODO  terrain_pict_seg = (EMS_PFBA + (m_terrain_lbx_001 * 2 * 2 * 2));

                // if(terrain_000_elpn != m_terrain_lbx_001)
                // {
                //     // ; maps in four consecutive logical pages from the
                //     // ; passed handle, starting with the one specified
                //     // ; uses a different EMM function than seg012:0255
                //     // ; preserves all register values
                //     // TODO  EMM_Map4Pages(m_terrain_lbx_001, ehn_terrain_lbx);
                // }

                if(l_terrain_001_0 == 0)
                {
                    assert(l_terrain_001_1 >= 2);
                    terrain_lbx_000_offset = 0xC0 + ((l_terrain_001_1 - 2) * 384);
                    // terrain_lbx_000_offset = (terrain_001_0 * 16384) + ((m_terrain_lbx_001 - 2) * 384) + 0xC0;
                }
                else
                {
                    assert(l_terrain_001_1 >= 0);
                    terrain_lbx_000_offset = (l_terrain_001_0 * 16384) + (l_terrain_001_1 * 384) - 0xC0 - 384;
                    // terrain_lbx_000_offset = (terrain_001_0 * 16384) + (m_terrain_lbx_001 * 384) - 0xC0 - 384;
                }

                assert(terrain_lbx_000_offset <= (676416 - 384));

                terrain_pict_seg = (m_terrain_lbx_000 + terrain_lbx_000_offset);

                /*
                    ¿ remapping the terrain color-map indices to the cartograph_pic palette ?

                */
                /* DS */  src_sgmt = (byte_ptr)(terrain_pict_seg + (1 * SZ_SEGMENT));  // pointer to the picture data (color-map indices)
                /* SI */  src_ofst = itr_world_width;
                // /* ES */  dst_sgmt = (byte_ptr)cartograph_seg;
                /* ES */  dst_sgmt = (byte_ptr)&cartograph_seg[32];
                /* DI */  dst_ofst = Map_Start_Offset;  // starts at 272 ... 

                /* ~ mov bl, ds:0;  mov al, [es:bx];  stosb */
                /* ES:DI = AL */
                // baito = src_sgmt[0];
                // dst_sgmt[dst_ofst] = dst_sgmt[src_sgmt[0]];
                // baito = dst_sgmt[dst_ofst];
                // dst_ofst++;

                /*
                dst_sgmt[dst_ofst] = dst_sgmt[src_sgmt[0]];
                dst_ofst++;

                dst_sgmt[dst_ofst] = dst_sgmt[src_sgmt[5]];
                dst_ofst++;

                dst_sgmt[dst_ofst] = dst_sgmt[src_sgmt[10]];
                dst_ofst++;

                dst_sgmt[dst_ofst] = dst_sgmt[src_sgmt[15]];
                dst_ofst++;

                dst_ofst =+ 156;

                dst_sgmt[dst_ofst] = dst_sgmt[src_sgmt[90]];
                dst_ofst++;

                dst_sgmt[dst_ofst] = dst_sgmt[src_sgmt[95]];
                dst_ofst++;

                dst_sgmt[dst_ofst] = dst_sgmt[src_sgmt[100]];
                dst_ofst++;

                dst_sgmt[dst_ofst] = dst_sgmt[src_sgmt[105]];
                dst_ofst++;

                dst_ofst =+ 156;

                dst_sgmt[dst_ofst] = dst_sgmt[src_sgmt[180]];
                dst_ofst++;

                dst_sgmt[dst_ofst] = dst_sgmt[src_sgmt[185]];
                dst_ofst++;

                dst_sgmt[dst_ofst] = dst_sgmt[src_sgmt[190]];
                dst_ofst++;

                dst_sgmt[dst_ofst] = dst_sgmt[src_sgmt[195]];
                dst_ofst++;

                dst_ofst =+ 156;

                dst_sgmt[dst_ofst] = dst_sgmt[src_sgmt[270]];
                dst_ofst++;

                dst_sgmt[dst_ofst] = dst_sgmt[src_sgmt[275]];
                dst_ofst++;

                dst_sgmt[dst_ofst] = dst_sgmt[src_sgmt[280]];
                dst_ofst++;

                dst_sgmt[dst_ofst] = dst_sgmt[src_sgmt[285]];
                dst_ofst++;
                */

            }  /* END:  if(GET_SQUARE_EXPLORED(itr_world_width, itr_world_height, cartograph_plane) != UNEXPLORED) */

            // // Map_Start_Offset += 2;  // 4 / sizeof()
            // Map_Start_Offset += 4;
            Map_Start_Offset += 1;

        }  /* END:  for(itr_world_height = 0; itr_world_height < WORLD_HEIGHT; itr_world_height++) */

        // // Map_Start_Offset += 240;  // 480 / sizeof()
        // Map_Start_Offset += 480;
        Map_Start_Offset += 240;

    }  /* END:  for(itr_world_width = 0; itr_world_width < WORLD_WIDTH; itr_world_width++) */

    Map_Start_Offset = 0;
    for(itr_world_width = 0; itr_world_width < WORLD_WIDTH; itr_world_width++)
    {

        for(itr_world_height = 0; itr_world_height < WORLD_HEIGHT; itr_world_height++)
        {

            square_explored_flag = GET_SQUARE_EXPLORED(itr_world_width, itr_world_height, cartograph_plane);

            if(square_explored_flag != UNEXPLORED)
            {

                if(square_explored_flag != EXPLORED)
                {

                    switch(square_explored_flag)
                    {

                        case 0x0:
                        {
                            dst_ofst = Map_Start_Offset;
                            dst_sgmt = (byte_ptr)&cartograph_pict[32];
                            
                            dst_sgmt[(dst_ofst +  16)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 176)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 336)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 496)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 497)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 498)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 499)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 337)] = ST_TRANSPARENT;

                        } break;

                        case 0x1:
                        {
                            dst_ofst = Map_Start_Offset;
                            dst_sgmt = (byte_ptr)&cartograph_pict[32];
                            
                            dst_sgmt[(dst_ofst +  19)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 179)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 339)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 496)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 497)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 498)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 499)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 338)] = ST_TRANSPARENT;

                        } break;

                        case 0x2:
                        {
                            dst_ofst = Map_Start_Offset;
                            dst_sgmt = (byte_ptr)&cartograph_pict[32];

                            dst_sgmt[(dst_ofst + 496)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 497)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 498)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 499)] = ST_TRANSPARENT;

                        } break;

                        case 0x3:
                        {
                            dst_ofst = Map_Start_Offset;
                            dst_sgmt = (byte_ptr)&cartograph_pict[32];
                            
                            dst_sgmt[(dst_ofst +  16)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst +  17)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst +  18)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst +  19)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 179)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 339)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 499)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 178)] = ST_TRANSPARENT;

                        } break;

                        case 0x4:
                        {
                            dst_ofst = Map_Start_Offset;
                            dst_sgmt = (byte_ptr)&cartograph_pict[32];
                            
                            dst_sgmt[(dst_ofst +  19)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 496)] = ST_TRANSPARENT;

                        } break;

                        case 0x5:
                        {
                            dst_ofst = Map_Start_Offset;
                            dst_sgmt = (byte_ptr)&cartograph_pict[32];
                            
                            dst_sgmt[(dst_ofst +  19)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 179)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 339)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 499)] = ST_TRANSPARENT;

                        } break;

                        case 0x6:
                        {
                            dst_ofst = Map_Start_Offset;
                            dst_sgmt = (byte_ptr)&cartograph_pict[32];
                            
                            dst_sgmt[(dst_ofst + 499)] = ST_TRANSPARENT;

                        } break;

                        case 0x7:
                        {
                            dst_ofst = Map_Start_Offset;
                            dst_sgmt = (byte_ptr)&cartograph_pict[32];
                            
                            dst_sgmt[(dst_ofst +  16)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst +  17)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst +  18)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst +  19)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 176)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 336)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 496)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 177)] = ST_TRANSPARENT;

                        } break;

                        case 0x8:
                        {
                            dst_ofst = Map_Start_Offset;
                            dst_sgmt = (byte_ptr)&cartograph_pict[32];
                            
                            dst_sgmt[(dst_ofst +  16)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 176)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 336)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 496)] = ST_TRANSPARENT;

                        } break;

                        case 0x9:
                        {
                            dst_ofst = Map_Start_Offset;
                            dst_sgmt = (byte_ptr)&cartograph_pict[32];
                            
                            dst_sgmt[(dst_ofst +  16)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst + 499)] = ST_TRANSPARENT;

                        } break;

                        case 0xA:
                        {
                            dst_ofst = Map_Start_Offset;
                            dst_sgmt = (byte_ptr)&cartograph_pict[32];
                            
                            dst_sgmt[(dst_ofst +  19)] = ST_TRANSPARENT;

                        } break;

                        case 0xB:
                        {
                            dst_ofst = Map_Start_Offset;
                            dst_sgmt = (byte_ptr)&cartograph_pict[32];
                            
                            dst_sgmt[(dst_ofst +  16)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst +  17)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst +  18)] = ST_TRANSPARENT;
                            dst_sgmt[(dst_ofst +  19)] = ST_TRANSPARENT;

                        } break;

                        case 0xC:
                        {
                            dst_ofst = Map_Start_Offset;
                            dst_sgmt = (byte_ptr)&cartograph_pict[32];
                            
                            dst_sgmt[(dst_ofst +  16)] = ST_TRANSPARENT;

                        } break;

                        case 0xD:
                        {
                            dst_ofst = Map_Start_Offset;
                            dst_sgmt = (byte_ptr)&cartograph_pict[32];
                            
                            dst_sgmt[(dst_ofst +  496)] = ST_TRANSPARENT;

                        } break;

                    }

                }

            }

            // // Map_Start_Offset += 2;  // 4 / sizeof()
            // Map_Start_Offset += 4;
            Map_Start_Offset += 1;

        }

        // // Map_Start_Offset += 240;  // 480 / sizeof()
        // Map_Start_Offset += 480;
        Map_Start_Offset += 240;

    }  /* END:  for(itr_world_width = 0; itr_world_width < WORLD_WIDTH; itr_world_width++) */

}

// WZD o150p18
// sub_DAFF4()
