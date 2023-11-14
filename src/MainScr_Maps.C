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
void Draw_Reduced_Map(int16_t minimap_start_x, int16_t minimap_start_y, int16_t world_plane, byte_ptr minimap_pict_seg, int16_t minimap_width, int16_t minimap_height, int16_t mark_x, int16_t mark_y, int16_t mark_flag)

*/
#include "MoM.H"

#include "MainScr.H"
#include "MainScr_Maps.H"
#include "SCastScr.H"



/*
    Forward Declare Private Functions
*/

// WZD o68p04
void Redraw_Map_Unexplored_Area(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane);




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
uint8_t COL_MinimapBanners[6] = {171, 216, 205, 201, 210, 50};  // 0xAB, 0xD8, 0xCD, 0xC9, 0xD2, 0x32
// WZD dseg:7010
uint8_t COL_MinimapNeutral = 50;  // 0x32
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
 uint8_t COL_City_Banner[] = {
    0x60, 0x61, 0x62, 0x63, 0x64, 
    0x48, 0x49, 0x4A, 0x4B, 0x4C, 
    0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 
    0xC9, 0xA5, 0xCB, 0xA6, 0x2D, 
    0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 
    0x1D, 0x1D, 0x1C, 0x1C, 0x1B, 
    0x1D, 0x1D, 0x1C, 0x1C, 0x1B, 
};


// WZD dseg:7034
int8_t TBL_Warp_GFX_Lines[20] = {0};              // ; 20 bytes randomized between -1, 0, or +1
// WZD dseg:7048  9B 9F        WarpNode_SeedSave_LO dw   9F9Bh
// WZD dseg:704A  0F 00        WarpNode_SeedSave_HO dw     0Fh
// WZD dseg:7048  9B 9F 0F 00  WarpNode_SeedSave    dd 0F9F9Bh
uint32_t WarpNode_SeedSave = 0x000F9F9B;

// WZD dseg:704A                                                 ? END:  ovr150 ?



// WZD dseg:974A
int16_t _prev_world_y;
// WZD dseg:974C
int16_t _prev_world_x;

// NIU?  // WZD dseg:9746
// NIU?  int16_t CRP_OVL_MapWindowY;
// NIU?  // ; would be the top Y pixel position if the map was direct copied from another part of the VGA memory as it stands, it's repurposable
// NIU?  // set to 0 in GAME_Overland_Init()
// NIU?  // WZD dseg:9748
// NIU?  int16_t CRP_OVL_MapWindowX;
// NIU?  // ; would be the left X pixel position if the map was direct copied from another part of the VGA memory as it stands, it's repurposable
// NIU?  // set to 0 in GAME_Overland_Init()


// WZD dseg:9CA8
SAMB_ptr terrain_lbx_002;  // 2 * 762 color map indecies, by terrain type, for minimap



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
SAMB_ptr terrain_lbx_001;                   // load in Terrain_Init() ovr052
// WZD dseg:CC26
byte_ptr terrain_001_1;                     // load in Terrain_Init() ovr052
// WZD dseg:CC28
// g_EmmHndl_TERRAIN dw 0  ; EMM_Load_LBX handle
SAMB_ptr terrain_lbx_000;                   // load in Terrain_Init() ovr052





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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Maps(screen_x = %d, screen_y = %d, map_width = %d, map_height = %d , *map_x = %d, *map_y = %d, map_plane = %d, xpos = %d, ypos = %d, player_idx = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_width, map_height, *map_x, *map_y, map_plane, xpos, ypos, player_idx);
#endif

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

#ifdef STU_DEBUG
    if(DBG_TST_Made_Map_Move == 1 && map_moved_flag != ST_TRUE)
    {
        dbg_prn("DEBUG: [%s, %d]: DBG_TST_FAILURE (DBG_Made_Map_Move == 1 && map_moved_flag != ST_TRUE)\n", __FILE__, __LINE__);
    }
    if(DBG_TST_Made_Map_Move == 1 && map_moved_flag == ST_TRUE)
    {
        dbg_prn("DEBUG: [%s, %d]: DBG_TST_SUCCESS (DBG_Made_Map_Move == 1 && map_moved_flag == ST_TRUE)\n", __FILE__, __LINE__);
        DBG_TST_Validate_Entities = 1;
    }
#endif
    if(map_moved_flag == ST_TRUE)
    {
        DLOG("(map_moved_flag == ST_TRUE)");
        Set_Unit_Draw_Priority();
        Reset_Stack_Draw_Priority();
        Set_Entities_On_Map_Window(l_map_x, l_map_y, map_plane);

        if(player_idx == _human_player_idx)
        {
            _map_x = l_map_x;
            _map_y = l_map_y;
        }
#ifdef STU_DEBUG
    DBG_TST_Made_Map_Move = 0;
    DBG_TST_Validate_Entities = 0;
#endif
    }
    else
    {
        DLOG("(map_moved_flag != ST_TRUE)");
    }
    /*
        END: Map-Moved!!
    */

    Reduced_Map_Coords(&minimap_x, &minimap_y, ((l_map_x + (MAP_WIDTH/2)) % WORLD_WIDTH), (l_map_y + (MAP_HEIGHT/2)), minimap_width, minimap_height);

    // Draw_Minimap(minimap_x, minimap_y, map_plane, _reduced_map_seg, minimap_width, minimap_height, 0, 0, 0);
    Draw_Reduced_Map(minimap_x, minimap_y, map_plane, _reduced_map_seg, minimap_width, minimap_height, 0, 0, 0);  // mark_x = 0, mark_y = 0, mark_flag = 0


    Draw_Map_Window(screen_x, screen_y, map_width, map_height, l_map_x, l_map_y, map_plane);


    if(map_moved_flag == ST_FALSE)
    {
        Draw_Active_Unit_Stack(*map_x, *map_y, map_plane);
    }


    // j_OVL_RedrawScouting(x, y, map_width, map_height, l_map_x, l_map_y, map_plane);
    Redraw_Map_Unexplored_Area(screen_x,screen_y, map_width, map_height, l_map_x, l_map_y, map_plane);

    Reset_Window();

    *map_x = l_map_x;
    *map_y = l_map_y;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_Maps(screen_x = %d, screen_y = %d, map_width = %d, map_height = %d , *map_x = %d, *map_y = %d, map_plane = %d, xpos = %d, ypos = %d, player_idx = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_width, map_height, *map_x, *map_y, map_plane, xpos, ypos, player_idx);
#endif

}

// WZD o67p0
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
    if( (all_units_moved == ST_FALSE) && (_unit_stack_count > 0) )
    {
        DLOG("( (all_units_moved == ST_FALSE) && (_unit_stack_count > 0) )");
        draw_active_stack_flag = 0;
    }
    Reset_Map_Draw();
}

// WZD o67p07
// AKA OVL_DrawActiveStack
void Draw_Active_Unit_Stack(int16_t mmap_xw, int16_t mmap_yw, int16_t world_plane)
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
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Active_Unit_Stack(mmap_xw = %d, mmap_yw = %d, world_plane = %d)\n", __FILE__, __LINE__, mmap_xw, mmap_yw, world_plane);
#endif

    int16_t in_view;
    int16_t first_active_stack_unit_idx;
    int16_t unit_xw;
    int16_t unit_yw;
    int16_t unit_idx;
    int16_t itr_unit_stack_count;


#ifdef STU_DEBUG
    if(DBG_TST_Selected_Stack == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: Draw_Active_Unit_Stack()  (DBG_TST_Selected_Stack == 1)\n", __FILE__, __LINE__);
    }
#endif



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: draw_active_stack_flag: %d\n", __FILE__, __LINE__, draw_active_stack_flag);
    dbg_prn("DEBUG: [%s, %d]: _unit_stack[0].unit_idx: %d\n", __FILE__, __LINE__, _unit_stack[0].unit_idx);
    dbg_prn("DEBUG: [%s, %d]: _UNITS[_unit_stack[0].unit_idx].owner_idx: %d\n", __FILE__, __LINE__, _UNITS[_unit_stack[0].unit_idx].owner_idx);
    dbg_prn("DEBUG: [%s, %d]: _UNITS[_unit_stack[0].unit_idx].world_plane: %d\n", __FILE__, __LINE__, _UNITS[_unit_stack[0].unit_idx].world_plane);
    dbg_prn("DEBUG: [%s, %d]: _UNITS[_unit_stack[0].unit_idx].In_Tower: %d\n", __FILE__, __LINE__, _UNITS[_unit_stack[0].unit_idx].In_Tower);
    dbg_prn("DEBUG: [%s, %d]: world_plane: %d\n", __FILE__, __LINE__, world_plane);
#endif


    if(draw_active_stack_flag != -2)  /* -2: never draw*/
    {
        if((_UNITS[_unit_stack[0].unit_idx].owner_idx != ST_UNDEFINED))
        {
            unit_idx = _unit_stack[0].unit_idx;
            if( (_UNITS[_unit_stack[0].unit_idx].world_plane == world_plane) || (_UNITS[_unit_stack[0].unit_idx].In_Tower == ST_TRUE) )
            {
                unit_xw = _UNITS[unit_idx].world_x;
                unit_yw = _UNITS[unit_idx].world_y;
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
                        // Draw_Minimap(minimap_x, minimap_y, world_plane, _reduced_map_seg, minimap_width, minimap_height, _UNITS[unit_idx].world_x, _UNITS[unit_idx].world_y, ST_TRUE);
                        Draw_Reduced_Map(minimap_x, minimap_y, world_plane, _reduced_map_seg, minimap_width, minimap_height, _UNITS[unit_idx].world_x, _UNITS[unit_idx].world_y, ST_TRUE);

                    }
                }
            }
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_Active_Unit_Stack(mmap_xw = %d, mmap_yw = %d, world_plane = %d)\n", __FILE__, __LINE__, mmap_xw, mmap_yw, world_plane);
#endif
}

// WZD o67p08
// AKA Minimap_Set_Dims()
void Reduced_Map_Set_Dims(int16_t width, int16_t height)
{
    minimap_width = width;
    minimap_height = height;
}


// WZD o67p09
// void Draw_Minimap_Window(int16_t start_x, int16_t start_y, int16_t width, int16_t height)
void Draw_World_Window(int16_t start_x, int16_t start_y, int16_t width, int16_t height)
{
    int16_t color;
    int16_t line_x;
    int16_t line_y;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_World_Window()\n", __FILE__, __LINE__);
#endif

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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_World_Window()\n", __FILE__, __LINE__);
#endif
}

// WZD o67p10
void Set_Entities_On_Map_Window(int16_t world_x, int16_t world_y, int16_t world_plane)
{
    int16_t city_in_view;
    int16_t entity_owner_idx;
    int16_t entity_world_y;
    int16_t entity_world_x;
    int16_t entity_world_p;
    int16_t square_is_explored;
    int16_t square_is_scouted;
    int16_t itr_map_x;
    int16_t itr_map_y;
    int16_t itr_units;
    int16_t itr_cities;
    int16_t entity_map_x;
    int16_t entity_map_y;
    int16_t entity_table_idx;
    int16_t prior_entity_idx;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Set_Entities_On_Map_Window(world_x = %d, world_y = %d, world_plane = %d)\n", __FILE__, __LINE__, world_x, world_y, world_plane);
#endif

#ifdef STU_DEBUG
    if(DBG_TST_Selected_Stack == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: Set_Entities_On_Map_Window()  (DBG_TST_Selected_Stack == 1)\n", __FILE__, __LINE__);
    }
#endif

    city_in_view = ST_FALSE;
    entity_world_y = 0;
    entity_world_x = 0;

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
        entity_world_x = _UNITS[itr_units].world_x;
        entity_world_y = _UNITS[itr_units].world_y;
        entity_world_p = _UNITS[itr_units].world_plane;
        square_is_scouted = Check_Square_Scouted(entity_world_x, entity_world_y, world_plane);
#ifdef STU_DEBUG
    if(DBG_TST_Selected_Stack == 1)
    {
        if(entity_owner_idx != _human_player_idx)
        {
            dbg_prn("DEBUG: [%s, %d]: DBG_TST_FAILURE: entity_owner_idx: %d\n", __FILE__, __LINE__, entity_owner_idx);
        }
        if(entity_world_x != 19)
        {
            dbg_prn("DEBUG: [%s, %d]: DBG_TST_FAILURE: entity_world_x: %d\n", __FILE__, __LINE__, entity_world_x);
        }
        if(entity_world_y != 10)
        {
            dbg_prn("DEBUG: [%s, %d]: DBG_TST_FAILURE: entity_world_y: %d\n", __FILE__, __LINE__, entity_world_y);
        }
        if(entity_world_p != 0)
        {
            dbg_prn("DEBUG: [%s, %d]: DBG_TST_FAILURE: entity_world_p: %d\n", __FILE__, __LINE__, entity_world_p);
        }

        if(square_is_scouted != ST_TRUE)
        {
            dbg_prn("DEBUG: [%s, %d]: DBG_TST_FAILURE: square_is_scouted: %d\n", __FILE__, __LINE__, square_is_scouted);
        }
    }
#endif

        if(entity_owner_idx != ST_UNDEFINED)
        {
            if(square_is_scouted == ST_TRUE)
            {
                if( (_UNITS[itr_units].world_plane == world_plane) || (_UNITS[itr_units].In_Tower == ST_TRUE) )
                {
                    // entity_world_y = _UNITS[itr_units].world_y;
                    if( (entity_world_y >= world_y) && (entity_world_y < world_y + MAP_HEIGHT) )
                    {
                        // entity_world_x = _UNITS[itr_units].world_x;
                        if(
                             ((entity_world_x >= world_x) && (entity_world_x < world_x + MAP_WIDTH)) ||
                             ((entity_world_x + WORLD_WIDTH >= world_x) && (entity_world_x + WORLD_WIDTH < world_x + MAP_WIDTH))
                        )
                        {
                            if((entity_world_x + WORLD_WIDTH >= world_x) && (entity_world_x + WORLD_WIDTH < world_x + MAP_WIDTH))
                            {
                                entity_world_x = entity_world_x + WORLD_WIDTH;
                            }

                            entity_map_x = entity_world_x - world_x;
                            entity_map_y = entity_world_y - world_y;
                            entity_table_idx = (entity_map_y * MAP_WIDTH) + entity_map_x;

                            prior_entity_idx = entities_on_movement_map[entity_table_idx];

                            /*
                                Add UNIT to entities_on_movement_map[]
                            */
                            if(prior_entity_idx != ST_UNDEFINED)
                            {
                                if(_UNITS[itr_units].Draw_Priority > _UNITS[prior_entity_idx].Draw_Priority)
                                {
                                    // Add UNIT to entities_on_movement_map[]
#ifdef STU_DEBUG
    if(DBG_TST_Selected_Stack == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: DBG_TST_FAILURE: \n", __FILE__, __LINE__);
    }
#endif
                                    entities_on_movement_map[entity_table_idx] = itr_units;
                                }
                            }
                            else
                            {
                                if(_UNITS[itr_units].Draw_Priority > 0)
                                {
                                    // Add UNIT to entities_on_movement_map[]
#ifdef STU_DEBUG
    if(DBG_TST_Selected_Stack == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: DBG_TST_FAILURE: \n", __FILE__, __LINE__);
    }
#endif
                                    entities_on_movement_map[entity_table_idx] = itr_units;
                                }
                                else
                                {
                                    if( (_UNITS[itr_units].Draw_Priority == 0) && (draw_active_stack_flag == -1) )  /* ALWAYS draw active stack */
                                    {
                                        // Add UNIT to entities_on_movement_map[]
#ifdef STU_DEBUG
    if(DBG_TST_Selected_Stack == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: DBG_TST_FAILURE: \n", __FILE__, __LINE__);
    }
#endif
                                        entities_on_movement_map[entity_table_idx] = itr_units;
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
        entity_world_x = _CITIES[itr_cities].world_x;
        entity_world_y = _CITIES[itr_cities].world_y;
        entity_world_p = _CITIES[itr_cities].world_plane;
        square_is_explored = TBL_Scouting[((entity_world_p * WORLD_SIZE) + (entity_world_y * WORLD_WIDTH) + entity_world_x)];
        if( ((entity_world_p == world_plane) || (entity_world_p == 2)) && (square_is_explored!= ST_FALSE) )
        {
            entity_world_y = _CITIES[itr_cities].world_y;
            if( (entity_world_y >= world_y) && (entity_world_y < world_y + MAP_HEIGHT) )
            {
                city_in_view = ST_FALSE;
                entity_map_y = entity_world_y - world_y;
                entity_world_x = _CITIES[itr_cities].world_x;
                if(
                    ( (entity_world_x >= world_x) && (entity_world_x < world_x + MAP_WIDTH) ) ||
                    ( (entity_world_x + WORLD_WIDTH >= world_x) && (entity_world_x + WORLD_WIDTH < world_x + MAP_WIDTH) )
                )
                {
                    if( (entity_world_x + WORLD_WIDTH >= world_x) && (entity_world_x + WORLD_WIDTH < world_x + MAP_WIDTH) )
                    {
                        entity_world_x = entity_world_x + WORLD_WIDTH;
                    }
                    entity_map_x = entity_world_x - world_x;
                    city_in_view = ST_TRUE;
                }
            }
        }
        if(city_in_view == ST_TRUE)
        {
            entity_table_idx = (entity_map_y * MAP_WIDTH) + entity_map_x;
            entities_on_movement_map[entity_table_idx] = (itr_cities + 1000);
        }

    }

#ifdef STU_DEBUG
    if(DBG_TST_Validate_Entities == 1)
    {
        DLOG("(DBG_TST_Validate_Entities == 1)");
        for(itr_map_y = 0; itr_map_y < MAP_HEIGHT; itr_map_y++)
        {
            for(itr_map_x = 0; itr_map_x < MAP_WIDTH; itr_map_x++)
            {
                // if(entities_on_movement_map[(itr_map_y * MAP_WIDTH) + itr_map_x] != DBG_TST_entities_on_movement_map[(itr_map_y * MAP_WIDTH) + itr_map_x])
                // {
                //     dbg_prn("DEBUG: [%s, %d]: DBG_TST_FAILURE: entities_on_movement_map[] != DBG_TST_entities_on_movement_map[]\n", __FILE__, __LINE__);
                // }
            }
        }
    }
    DBG_TST_Validate_Entities = 0;
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Set_Entities_On_Map_Window(world_x = %d, world_y = %d, world_plane = %d)\n", __FILE__, __LINE__, world_x, world_y, world_plane);
#endif
}

// WZD o67p13
void Set_Unit_Draw_Priority(void)
{
    int16_t draw_priority;
    int16_t itr_units;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Set_Unit_Draw_Priority()\n", __FILE__, __LINE__);
#endif

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        draw_priority = _unit_type_table[_UNITS[itr_units].type].Melee + _unit_type_table[_UNITS[itr_units].type].Ranged;

        if(draw_priority == 0)
        {
            draw_priority = 1;
        }

        if(_unit_type_table[_UNITS[itr_units].type].Transport > 0)
        {
            draw_priority = 50;
        }

        if(_UNITS[itr_units].owner_idx == -1)
        {
            draw_priority = ST_UNDEFINED;
        }

        if(_UNITS[itr_units].owner_idx != _human_player_idx && Unit_Has_Invisibility(itr_units) == ST_TRUE)
        {
            draw_priority = ST_UNDEFINED;
        }

        _UNITS[itr_units].Draw_Priority = draw_priority;

    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Set_Unit_Draw_Priority()\n", __FILE__, __LINE__);
#endif
}

// WZD o67p14
// drake178: STK_NoUnitDraw
void Reset_Stack_Draw_Priority(void)
{
    int16_t itr_unit_stack_count;
    int16_t unit_idx;

    for(itr_unit_stack_count = 0; itr_unit_stack_count < _unit_stack_count; itr_unit_stack_count++)
    {
        unit_idx = _unit_stack[itr_unit_stack_count].unit_idx;
        _UNITS[unit_idx].Draw_Priority = 0;
    }
}


/*
    WIZARDS.EXE  ovr068
*/

// WZD o68p01
// ? MoO2  Module: MAINSCR  Center_Map_() ?
/*
    in: xpos,ypos are new map center
    in/out: map_x,map_y are cur/new map start
*/
void Center_Map(int16_t * map_x, int16_t * map_y, int16_t world_gird_x, int16_t world_grid_y, int16_t world_plane)
{
    int16_t tmp_map_x;
    int16_t tmp_map_y;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Center_Map(*map_x, *map_y, world_gird_x, world_grid_y, world_plane = %d)\n", __FILE__, __LINE__, *map_x, *map_y, world_gird_x, world_grid_y, world_plane);
#endif

    if(world_gird_x >= WORLD_WIDTH)
    {
        world_gird_x -= WORLD_WIDTH;
    }
    if(world_gird_x < 0)
    {
        world_gird_x += WORLD_WIDTH;
    }

    *map_x = world_gird_x - MAP_WIDTH_HALF;
    if(*map_x <= 0)
    {
        *map_x = ((*map_x + WORLD_WIDTH) / WORLD_WIDTH);
    }

    *map_y = world_grid_y - MAP_HEIGHT_HALF;
    if(*map_y <= 0)
    {
        *map_y = 0;
    }

    if(*map_y + MAP_HEIGHT >= WORLD_HEIGHT)
    {
        *map_y = (WORLD_HEIGHT - MAP_HEIGHT);  // 40 - 10  World Height - Map Height
    }

    _prev_world_x = *map_x;
    _prev_world_y = *map_y;

    // NIU CRP_OVL_MapWindowX = G_OVL_MapDisplay_X * TILE_WIDTH;
    // NIU CRP_OVL_MapWindowY = G_OVL_MapDisplay_Y * TILE_HEIGHT;
    
    Set_Unit_Draw_Priority();
    Reset_Stack_Draw_Priority();
    Set_Entities_On_Map_Window(*map_x, *map_y, world_plane);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Center_Map(*map_x, *map_y, world_gird_x, world_grid_y, world_plane = %d)\n", __FILE__, __LINE__, *map_x, *map_y, world_gird_x, world_grid_y, world_plane);
#endif
}

// WZD o68p03
// drake178: OVL_GetMinimapStart()
// AKA Minimap_Coords()
/*
push    [minimap_height]
push    [minimap_width]
mov     ax, _DI_cur_map_ypos
add     ax, 5                           ; 10 / 2
push    ax                              ; Center_Y
mov     ax, _SI_cur_map_xpos
add     ax, 6                           ; 12 / 2
mov     bx, 60
cwd
idiv    bx
push    dx                              ; Center_X
mov     ax, offset minimap_y
push    ax                              ; TopY@
mov     ax, offset minimap_x
push    ax                              ; LeftX@
j_OVL_GetMinimapStart
; sets the return values to the top left map tile
; coordinates that should be displayed at the top left
; corner of the minimap
*/
void Reduced_Map_Coords(int16_t * minimap_x, int16_t * minimap_y, int16_t mid_x, int16_t mid_y, int16_t minimap_width, int16_t minimap_height)
{
    int16_t tmp_minimap_x;
    int16_t tmp_minimap_y;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Reduced_Map_Coords(*minimap_x = %d, *minimap_y = %d, mid_x = %d, mid_y = %d, minimap_width = %d, minimap_height = %d)\n", __FILE__, __LINE__, *minimap_x, *minimap_y, mid_x, mid_y, minimap_width, minimap_height);
#endif

    tmp_minimap_x = mid_x - (minimap_width / 2);
    if(tmp_minimap_x > 0)
    {
        *minimap_x = tmp_minimap_x;
    }
    else
    {
        *minimap_x = (tmp_minimap_x + WORLD_WIDTH) % WORLD_WIDTH;
    }

    tmp_minimap_y = mid_y - (minimap_height / 2);

    *minimap_y = tmp_minimap_y;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Reduced_Map_Coords(*minimap_x = %d, *minimap_y = %d, mid_x = %d, mid_y = %d, minimap_width = %d, minimap_height = %d)\n", __FILE__, __LINE__, *minimap_x, *minimap_y, mid_x, mid_y, minimap_width, minimap_height);
#endif

}


// WZD o68p04
// drake178: OVL_RedrawScouting()
void Redraw_Map_Unexplored_Area(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane)
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Redraw_Map_Unexplored_Area(screen_x = %d, screen_y = %d, map_grid_width = %d, map_grid_height = %d, world_grid_x = %d, world_grid_y = %d, world_plane = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_grid_width, map_grid_height, world_grid_x, world_grid_y, world_plane);
#endif

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

            terrain_001_index = GET_2B_OFS(_world_maps, ((world_plane * WORLD_SIZE * 2) + (itr_world_y * WORLD_WIDTH * 2) + (curr_world_x * 2)));
            terrain_001_0 = GET_1B_OFS(terrain_lbx_001, (terrain_001_index * 2) + 0);

            animated_terrain_flag = ((terrain_001_0 & 0x80) != 0);  // ¿ prefer ((bitfield >> 7) & 0x01) - saves the CPU doing a full register-sized compare ?

            // if( (map_draw_full == ST_TRUE) || (animated_terrain_flag != ST_TRUE) )
            // {
                square_explored_flag = TBL_Scouting[(world_plane * WORLD_SIZE) + (itr_world_y * WORLD_WIDTH) + (curr_world_x)];

                if( (square_explored_flag != 0) && (square_explored_flag != 15) )
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


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Redraw_Map_Unexplored_Area(screen_x = %d, screen_y = %d, map_grid_width = %d, map_grid_height = %d, world_grid_x = %d, world_grid_y = %d, world_plane = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_grid_width, map_grid_height, world_grid_x, world_grid_y, world_plane);
#endif

}


// WZD o68p05
// RP_OVL_DrawCities2()


// WZD o68p06
/*
    called from Units_In_Tower()
        only for human_player_idx
    called from TILE_ExploreRadius()

*/
void TILE_Explore(int16_t wx, int16_t wy, int16_t wp)
{

    uint8_t * ptr_square_explored;
    int16_t X_LoopVar;
    int16_t Y_Up_2;
    int16_t Y_Down_2;
    int16_t X_Left_2;
    int16_t X_Right_2;
    int16_t Y_Up_1;
    int16_t Y_Down_1;
    int16_t X_Left_1;
    int16_t X_Right_1;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: TILE_Explore(wx = %d, wy = %d, wp = %d)\n", __FILE__, __LINE__, wx, wy, wp);
#endif

    // ptr_square_explored = (square_explored + (2400 * wp));
    ptr_square_explored = (uint8_t *)(TBL_Scouting + (2400 * wp));

    if(wy == 0)
    {
        // ; BX = (wy * WORLD_WIDTH) + wx
        // mov     [byte ptr es:bx], SCT_BottomLeft or SCT_TopLeft or SCT_TopRight or SCT_BottomRight
        // *(ptr_square_explored + ((wy * WORLD_WIDTH) + wx)) = (SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight);
// enum SCOUT_BITS (bitfield)
// 01 SCT_BottomLeft  = 1
// 02 SCT_TopLeft  = 2
// 04 SCT_TopRight  = 4
// 08 SCT_BottomRight  = 8
        *(ptr_square_explored + ((wy * WORLD_WIDTH) + wx)) = (0x01 | 0x02 | 0x04 | 0x08);  // ... sets all four corners ...
    }
    else
    {

        /*
            BEGIN: sanitize wy & wx
        */
        if(wy == 0)
        {
            Y_Up_1 = 0;
            Y_Up_2 = 0;
        }
        else
        {
            Y_Up_1 = wy - 1;
            Y_Up_2 = wy - 2;
            if(Y_Up_2 < 0)
            {
                Y_Up_2 = 0;
            }
        }

        if(wy == WORLD_Y_MAX)
        {
            Y_Down_1 = WORLD_WIDTH - 1;  // 40 - 1 = 39
            Y_Down_2 = WORLD_WIDTH - 2;  // 40 - 2 = 38
        }
        else
        {
            Y_Down_1 = wy + 1;
            Y_Down_2 = wy + 2;
            if(Y_Down_2 > WORLD_WIDTH)
            {
                Y_Down_2 = WORLD_Y_MAX;
            }
        }

        if(wx == 0)
        {
            X_Left_1 = WORLD_WIDTH - 1;  // 60 - 1 = 59
            X_Left_2 = WORLD_WIDTH - 2;  // 60 - 2 = 58
        }
        else
        {
            X_Left_1 = wx - 1;
            X_Left_2 = wx - 2;
            if(X_Left_2 < 0)
            {
                X_Left_2 = WORLD_X_MAX;
            }
        }

        if(wx == WORLD_X_MAX)
        {
            X_Right_1 = 0;
            X_Right_2 = 1;
        }
        else
        {
            X_Right_1 = wx + 1;
            X_Right_2 = wx + 2;
        }
        /*
            END: sanitize wy & wx
        */

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: X_Left_1 = %d, wx = %d, X_Right_1 = %d)\n", __FILE__, __LINE__, X_Left_1, wx, X_Right_1);
    dbg_prn("DEBUG: [%s, %d]: Y_Up_1 = %d, wy = %d, X_Right_1 = %d)\n", __FILE__, __LINE__, Y_Up_1, wy, Y_Down_1);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: *(ptr_square_explored + ((wy * WORLD_WIDTH) + wx)): %d)\n", __FILE__, __LINE__, *(ptr_square_explored + ((wy * WORLD_WIDTH) + wx)));
#endif

    *(ptr_square_explored + ((Y_Up_1   * WORLD_WIDTH) + X_Left_1 )) = *(ptr_square_explored + ((Y_Up_1   * WORLD_WIDTH) + X_Left_1 ))                      | 0x08;  //                                               SCT_BottomRight
    *(ptr_square_explored + ((Y_Up_1   * WORLD_WIDTH) + wx       )) = *(ptr_square_explored + ((Y_Up_1   * WORLD_WIDTH) + wx       )) | 0x01               | 0x08;  // SCT_BottomLeft                              | SCT_BottomRight
    *(ptr_square_explored + ((Y_Up_1   * WORLD_WIDTH) + X_Right_1)) = *(ptr_square_explored + ((Y_Up_1   * WORLD_WIDTH) + X_Right_1)) | 0x01                     ;  // SCT_BottomLeft
    *(ptr_square_explored + ((wy       * WORLD_WIDTH) + X_Left_1 )) = *(ptr_square_explored + ((wy       * WORLD_WIDTH) + X_Left_1 ))               | 0x04 | 0x08;  //                                SCT_TopRight | SCT_BottomRight
    *(ptr_square_explored + ((wy       * WORLD_WIDTH) + wx       )) = *(ptr_square_explored + ((wy       * WORLD_WIDTH) + wx       )) | 0x01 | 0x02 | 0x04 | 0x08;  // SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight
    *(ptr_square_explored + ((wy       * WORLD_WIDTH) + X_Right_1)) = *(ptr_square_explored + ((wy       * WORLD_WIDTH) + X_Right_1)) | 0x01 | 0x02              ;  // SCT_BottomLeft | SCT_TopLeft
    *(ptr_square_explored + ((Y_Down_1 * WORLD_WIDTH) + X_Left_1 )) = *(ptr_square_explored + ((Y_Down_1 * WORLD_WIDTH) + X_Left_1 ))               | 0x04       ;  //                                SCT_TopRight
    *(ptr_square_explored + ((Y_Down_1 * WORLD_WIDTH) + wx       )) = *(ptr_square_explored + ((Y_Down_1 * WORLD_WIDTH) + wx       ))        | 0x02 | 0x04       ;  //                  SCT_TopLeft | SCT_TopRight
    *(ptr_square_explored + ((Y_Down_1 * WORLD_WIDTH) + X_Right_1)) = *(ptr_square_explored + ((Y_Down_1 * WORLD_WIDTH) + X_Right_1))        | 0x02              ;  //                  SCT_TopLeft

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: *(ptr_square_explored + ((wy * WORLD_WIDTH) + wx)): %d)\n", __FILE__, __LINE__, *(ptr_square_explored + ((wy * WORLD_WIDTH) + wx)));
#endif



            // drake178:  ; entirely redundant, TILE_ExploreCorners, if it was
            // drake178:  ; implemented correctly, would deal with each and every
            // drake178:  ; one of these cases, and then some
            // drake178:  ;
            // drake178:  ; performs exploration matching of neighboring tiles
            // drake178:  ; in a 5-by-5 rectangle around the selected tile in an
            // drake178:  ; attempt to remove fog of war from between tiles that
            // drake178:  ; are already explored
            // drake178:  ;
            // drake178:  ; BUG: the array contains many elements that are only
            // drake178:  ; intended to bugfix artifacts that the developers
            // drake178:  ; could not track down, but is missing 10 valid and
            // drake178:  ; occuring configurations, and has wrong values for 4
            // drake178:  ; of the ones it does include
        // TODO  RP_TILE_ExploreMatch(wx, wy, wp);



        for(X_LoopVar = X_Left_2; X_LoopVar < X_Right_2; X_LoopVar++)
        {
                // drake178:  ; compares the tile and it's bottom, right, and
                // drake178:  ; diagonal bottom-right neighbors to NoSide_Explores,
                // drake178:  ; and if all match one of them, removes the corner
                // drake178:  ; corresponding to the original tile's bottom right one
                // drake178:  ;
                // drake178:  ; conceptually flawed, it should simply check whether
                // drake178:  ; there is any exploration on all 4 tiles, rather than
                // drake178:  ; comparing them against the corner only masks
            // TODO  TILE_ExploreCorners(X_LoopVar, Y_Up_2, wp);
        }
        // TODO  TILE_ExploreCorners(X_Right_1, Y_Up_1, wp);
        // TODO  TILE_ExploreCorners(X_Right_1, wy, wp);
        for(X_LoopVar = X_Left_2; X_LoopVar < X_Right_2; X_LoopVar++)
        {
            // TODO  TILE_ExploreCorners(X_LoopVar, Y_Down_1, wp);
        }



    }


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: TILE_Explore(wx = %d, wy = %d, wp = %d)\n", __FILE__, __LINE__, wx, wy, wp);
#endif

}


// WZD o68p07
// TILE_ExploreCorners()


// WZD o68p08
// RP_TILE_ExploreMatch()


// WZD o68p09
// AKA IDK_Draw_MiniMap_s5B828()
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: List_Screen_Draw_Reduced_Map(x = %d, y = %d, w = %d, h = %d, wp = %d, wx = %d, wy = %d)\n", __FILE__, __LINE__, x, y, w, h, wp, wx, wy);
#endif

    reduced_map_mark_cycle = ((reduced_map_mark_cycle + 1) % 8);

    mark_flag  = ST_FALSE;

    // NOTE: same logic as cycler on 'Main Screen' for 'Active Stack'
    if(reduced_map_mark_cycle < 4)
    {
        mark_flag = ST_TRUE;
    }

    Reduced_Map_Coords(&reduced_map_x, &reduced_map_y, wx, wy, w, h);
    Draw_Reduced_Map(reduced_map_x, reduced_map_y, wp, _reduced_map_seg, w, h, wx, wy, mark_flag);

    Reset_Window();
    Draw_Picture(x, y, _reduced_map_seg);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: List_Screen_Draw_Reduced_Map(x = %d, y = %d, w = %d, h = %d, wp = %d, wx = %d, wy = %d)\n", __FILE__, __LINE__, x, y, w, h, wp, wx, wy);
#endif

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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Map_Window(screen_x = %d, screen_y = %d, map_w = %d, map_h = %d, map_x = %d, map_y = %d, map_p = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_w, map_h, map_x, map_y, map_p);
#endif

    /*
        x,y for the last time it thinks we drew the movement map
        x,y for the target of this request to draw the movement
        ~== curr. vs. prev.
    */
    map_draw_curr_x = map_x;
    map_draw_curr_y = map_y;


    if(map_draw_curr_x != map_draw_prev_x || map_draw_prev_y != map_draw_prev_y)
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

#ifdef STU_DEBUG
dbg_prn("DEBUG: [%s, %d]: END: Draw_Map_Window(screen_x = %d, screen_y = %d, map_w = %d, map_h = %d, map_x = %d, map_y = %d, map_p = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_w, map_h, map_x, map_y, map_p);
#endif

}


// WZD o150p05
void Draw_Map_Terrain(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane)
{
    int16_t terrain_tile_base;
    int16_t itr_world_x;
    int16_t itr_world_y;
    int16_t itr_screen_y;
    int16_t itr_screen_x;
    int16_t curr_world_x;
    uint8_t unexplored_area_flag;
    int16_t world_maps_offset;
    uint32_t terrain_lbx_000_offset;
    uint16_t terrain_001_index;
    uint8_t terrain_001_0;
    uint8_t terrain_001_1;
    byte_ptr terrain_pict_seg;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Map_Terrain(screen_x = %d, screen_y = %d, map_grid_width = %d, map_grid_width = %d, world_grid_x = %d, world_grid_x = %d, world_plane = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_grid_width, map_grid_height, world_grid_x, world_grid_y, world_plane);
#endif

    if(world_plane == 0)
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


            unexplored_area_flag = TBL_Scouting[(world_plane * WORLD_SIZE) + (itr_world_y * WORLD_WIDTH) + (curr_world_x)];
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: (%d, %d) unexplored: 0x%02X\n", __FILE__, __LINE__, curr_world_x, itr_world_y, unexplored_area_flag);
// #endif
            if(unexplored_area_flag == ST_FALSE)
            {
                Fill(itr_screen_x, itr_screen_y, (itr_screen_x + 19), (itr_screen_y + 17), 0);
            }
            else
            {
                world_maps_offset = ((world_plane * WORLD_SIZE * 2) + (itr_world_y * WORLD_WIDTH * 2) + (curr_world_x * 2));
                terrain_001_index = GET_2B_OFS(_world_maps,world_maps_offset);
                terrain_001_index += terrain_tile_base;
                terrain_001_index *= 2;  // because, sizeof(int16_t)
                terrain_001_0 = 0;
                terrain_001_0 = GET_1B_OFS(terrain_lbx_001, terrain_001_index + 0);
                terrain_001_1 = GET_1B_OFS(terrain_lbx_001, terrain_001_index + 1);

                assert( (terrain_001_0 == 0 && terrain_001_1 >= 2) || (terrain_001_0 != 0) );

                map_draw_full = ST_TRUE;
                if( ((terrain_001_0 & 0x80) == 0) || (map_draw_full == ST_TRUE) )
                {
                    if((terrain_001_0 & 0x80) != 0)
                    {
                        terrain_001_0 = terrain_001_0 & 0x7F;  // ~== -128
                        terrain_001_1 += terrain_anim_ctr;
                    }

                    // MS-DOS  // Dasm  terrain_pict_seg = ( (((((terrain_001_1 * 2) + terrain_001_1) * 2) * 2) *2) ) + EMM_PageFrame;
                    // MS-DOS  terrain_pict_seg = EMM_PageFrame + (terrain_001_1 * 24);  // segments; 24 * 16 = 384, sizeof pict; 180h offset from File, not Entry
                    // MS-DOS  if(terrain_000_elpn != terrain_001_0)
                    // MS-DOS  {
                    // MS-DOS      EMM_Map4Pages(terrain_001_0, g_EmmHndl_TERRAIN); // First Logical Page, EMM Handle Name
                    // MS-DOS  }

                    // map index in terrain_001_1 to TERRAIN.LBX Entry 0, instead of TERRAIN.LBX entire file in EMM
                    // loading entry 0, instead of whole file
                    // ( EMM Page * 16K ) + (terrain pict index * 384) - (difference in header size)
                    if(terrain_001_0 == 0)
                    {
                        terrain_lbx_000_offset = 0xC0 + ((terrain_001_1 - 2) * 384);
                        // terrain_lbx_000_offset = (terrain_001_0 * 16384) + ((terrain_001_1 - 2) * 384) + 0xC0;
                    }
                    else
                    {
                        terrain_lbx_000_offset = (terrain_001_0 * 16384) + (terrain_001_1 * 384) - 0xC0 - 384;
                        // terrain_lbx_000_offset = (terrain_001_0 * 16384) + (terrain_001_1 * 384) - 0xC0 - 384;
                    }

                    terrain_pict_seg = terrain_lbx_000 + terrain_lbx_000_offset;
                    Draw_Picture(itr_screen_x, itr_screen_y, terrain_pict_seg);
                }

            } /* if(unexplored_area_flag == ST_FALSE) */

            if(DBG_ShowTileInfo == ST_TRUE)
            {
                // TODO  j_EMM_Map_CONTXXX();  // ; maps in the EMM_ContXXX_H handle (all 4 pages), and resets its corresponding global pointers
                // TODO  Set_Font(0,0);
                // Print_Integer(itr_screen_x, itr_screen_y + 12, TBL_Landmasses[(_map_plane * WORLD_SIZE) + (itr_world_y * WORLD_WIDTH) + curr_world_x]);
                // TODO  Set_Font(0, 2);
                // Print_Integer(itr_screen_x, itr_screen_y, curr_world_x);
            }

            itr_screen_x += SQUARE_WIDTH;
            itr_world_x++;
        }

        itr_screen_y += SQUARE_HEIGHT;
        itr_world_y++;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_Map_Terrain(screen_x = %d, screen_y = %d, map_grid_width = %d, map_grid_width = %d, world_grid_x = %d, world_grid_x = %d, world_plane = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_grid_width, map_grid_height, world_grid_x, world_grid_y, world_plane);
#endif

}


// WZD o150p06
void Cycle_Map_Animations(void)
{
    map_anim_cycle = (1 - map_anim_cycle);  // toggle {1-0=1,1-1=0}
    if(map_anim_cycle == 0)
    {
        terrain_anim_ctr = (terrain_anim_ctr + 1) % 4;
        node_anim_ctr = (node_anim_ctr + 1) % 6;
        road_anim_ctr = (road_anim_ctr + 1) % 6;
    }
}

// WZD o150p07
void Draw_Map_Unexplored_Area(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane)
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Map_Unexplored_Area(screen_x = %d, screen_y = %d, map_grid_width = %d, map_grid_height = %d, world_grid_x = %d, world_grid_y = %d, world_plane = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_grid_width, map_grid_height, world_grid_x, world_grid_y, world_plane);
#endif

#ifdef STU_DEBUG
    if(DBG_TST_Selected_Stack == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Map_Unexplored_Area (DBG_TST_Selected_Stack == 1)\n", __FILE__, __LINE__);
    }
#endif

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

            terrain_001_index = GET_2B_OFS(_world_maps, ((world_plane * WORLD_SIZE * 2) + (itr_world_y * WORLD_WIDTH * 2) + (curr_world_x * 2)));
            terrain_001_0 = GET_1B_OFS(terrain_lbx_001, (terrain_001_index * 2) + 0);
            animated_terrain_flag = ((terrain_001_0 & 0x80) != 0);  // ¿ prefer ((bitfield >> 7) & 0x01) - saves the CPU doing a full register-sized compare ?


            // HERE: Redraw_Map_Unexplored_Area() does not test m_draw_full or animated terrain type bit

            if( (map_draw_full == ST_TRUE) || (animated_terrain_flag != ST_TRUE) )
            {
                square_explored_flag = TBL_Scouting[(world_plane * WORLD_SIZE) + (itr_world_y * WORLD_WIDTH) + (curr_world_x)];

                if( (square_explored_flag != 0) && (square_explored_flag != 15) )
                {
                    unexplored_mask_pict_seg = unexplored_mask_seg[square_explored_flag - 1];
                    FLIC_Draw(itr_screen_x, itr_screen_y, unexplored_mask_pict_seg);
                }
            }

            itr_screen_x += SQUARE_WIDTH;
            itr_world_x += 1;
        }

        itr_screen_y += SQUARE_HEIGHT;
        itr_world_y += 1;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_Map_Unexplored_Area(screen_x = %d, screen_y = %d, map_grid_width = %d, map_grid_height = %d, world_grid_x = %d, world_grid_y = %d, world_plane = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_grid_width, map_grid_height, world_grid_x, world_grid_y, world_plane);
#endif
}

// WZD o150p08
void Draw_Map_Cities(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane)
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Map_Cities(screen_x = %d, screen_y = %d, map_grid_width = %d, map_grid_height = %d, world_grid_x = %d, world_grid_y = %d, world_plane = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_grid_width, map_grid_height, world_grid_x, world_grid_y, world_plane);
#endif

    if(current_screen == scr_City_Screen)
    {
        Set_Window(215, 4, 454, 183);
    }
    else
    {
        Set_Window(0, 20, 239, 199);
    }

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].world_plane == world_plane)
        {
            city_x = _CITIES[itr_cities].world_x;
            city_y = _CITIES[itr_cities].world_y;
            if(TBL_Scouting[((world_plane * WORLD_SIZE) + (city_y * WORLD_WIDTH) + city_x)] != ST_FALSE)
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

                                if(city_owner != 5)  /* Neutral Player */
                                {
                                    if(_CITIES[itr_cities].buildings.City_Walls == 0)  /* ¿ "B_Replaced" or just ST_FALSE ? */
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
                                    FLIC_Set_CurrentFrame(city_pict_seg, city_size);
                                    Draw_Picture_To_Bitmap(city_pict_seg, Map_Square_WorkArea);
                                    for(itr_color_remap = 0; itr_color_remap < 5; itr_color_remap++)
                                    {
                                        Replace_Color(Map_Square_WorkArea, 214 + itr_color_remap, (COL_City_Banner[((_players[city_owner].banner_id * 5) + itr_color_remap)] - 1));
                                    }
                                }
                                else
                                {
                                    if(_CITIES[itr_cities].buildings.City_Walls == 0)  /* ¿ "B_Replaced" or just ST_FALSE ? */
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
                                    FLIC_Set_CurrentFrame(city_pict_seg, city_size);
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_Map_Cities(screen_x = %d, screen_y = %d, map_grid_width = %d, map_grid_height = %d, world_grid_x = %d, world_grid_y = %d, world_plane = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_grid_width, map_grid_height, world_grid_x, world_grid_y, world_plane);
#endif
}

// WZD o150p09
void Draw_Map_Towers(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane)
{
    int16_t itr_towers;
    int16_t tower_x;
    int16_t tower_y;
    uint8_t unexplored_flag;
    int16_t tower_owner_idx;
    int16_t start_x;
    int16_t start_y;
    int16_t itr_color_remap;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Map_Towers(screen_x = %d, screen_y = %d, map_grid_width = %d, map_grid_height = %d, world_grid_x = %d, world_grid_y = %d, world_plane = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_grid_width, map_grid_height, world_grid_x, world_grid_y, world_plane);
#endif

    for(itr_towers = 0; itr_towers < NUM_TOWERS; itr_towers++)
    {
        tower_x = _TOWERS[itr_towers].world_x;
        tower_y = _TOWERS[itr_towers].world_y;

        unexplored_flag = TBL_Scouting[(world_plane * WORLD_SIZE) + (tower_y * WORLD_WIDTH) + tower_x];
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_Map_Towers(screen_x = %d, screen_y = %d, map_grid_width = %d, map_grid_height = %d, world_grid_x = %d, world_grid_y = %d, world_plane = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_grid_width, map_grid_height, world_grid_x, world_grid_y, world_plane);
#endif
}

// WZD o150p10
void Draw_Map_Lairs(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane)
{
    int16_t itr_lairs;
    int16_t lair_x;
    int16_t lair_y;
    uint8_t unexplored_flag;
    int16_t start_x;
    int16_t start_y;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Map_Lairs(screen_x = %d, screen_y = %d, map_grid_width = %d, map_grid_height = %d, world_grid_x = %d, world_grid_y = %d, world_plane = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_grid_width, map_grid_height, world_grid_x, world_grid_y, world_plane);
#endif

    for(itr_lairs = 0; itr_lairs < NUM_LAIRS; itr_lairs++)
    {
        if(_LAIRS[itr_lairs].world_plane == world_plane)
        {
            if(_LAIRS[itr_lairs].Intact == 1)
            {
                if(_LAIRS[itr_lairs].Type > EZ_Sorcery_Node)
                {
                    lair_x = _LAIRS[itr_lairs].world_x;
                    lair_y = _LAIRS[itr_lairs].world_y;
                    unexplored_flag = TBL_Scouting[(world_plane * WORLD_SIZE) + (lair_y * WORLD_WIDTH) + lair_x];
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
                                        push    (IMG_OVL_EZ_Cave@ - (2*EZ_Cave))[bx]
                                        push    (IMG_OVL_EZ_Cave@ - (2*EZ_Cave))[bx] ; sa_FLIC_Header
                                        */
                                        // TODO(JimBalcomb,20230701): figure out what is going on with this approach to the pict seg
                                        // FWIW, they are in the same order in the Data Segment, Terrain_Init(), and enum (though I assume the enum is derived)
                                        // No-Workie  FLIC_Draw(start_x, start_y, (SAMB_ptr)(IMG_OVL_EZ_Cave + ((_LAIRS[itr_lairs].Type - EZ_Cave) * sizeof(SAMB_ptr))));
                                        // No-Workie  FLIC_Draw(start_x, start_y, IMG_OVL_EZ_Cave + 1);
                                        // Workie  FLIC_Draw(start_x, start_y, IMG_OVL_EZ_Cave);
                                        switch(_LAIRS[itr_lairs].Type)
                                        {
                                            case EZ_Cave: {           FLIC_Draw(start_x, start_y, IMG_OVL_EZ_Cave);  } break;
                                            case EZ_Dungeon: {        FLIC_Draw(start_x, start_y, IMG_OVL_EZ_Dung);  } break;
                                            case EZ_Ancient_Temple: { FLIC_Draw(start_x, start_y, IMG_OVL_EZ_ATmpl); } break;
                                            case EZ_Keep: {           FLIC_Draw(start_x, start_y, IMG_OVL_EZ_AKeep); } break;
                                            case EZ_Monster_Lair: {   FLIC_Draw(start_x, start_y, IMG_OVL_EZ_MLair); } break;
                                            case EZ_Ruins: {          FLIC_Draw(start_x, start_y, IMG_OVL_EZ_Ruins); } break;
                                            case EZ_Fallen_Temple: {  FLIC_Draw(start_x, start_y, IMG_OVL_EZ_FTmpl); } break;
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_Map_Lairs(screen_x = %d, screen_y = %d, map_grid_width = %d, map_grid_height = %d, world_grid_x = %d, world_grid_y = %d, world_plane = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_grid_width, map_grid_height, world_grid_x, world_grid_y, world_plane);
#endif
}

// WZD o150p11
void Draw_Map_Nodes(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane)
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Map_Nodes(screen_x = %d, screen_y = %d, map_grid_width = %d, map_grid_height = %d, world_grid_x = %d, world_grid_y = %d, world_plane = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_grid_width, map_grid_height, world_grid_x, world_grid_y, world_plane);
#endif

    for(itr_nodes = 0; itr_nodes < NUM_NODES; itr_nodes++)
    {
        if(world_plane == TBL_Nodes[itr_nodes].world_plane)
        {
            node_owner_idx = TBL_Nodes[itr_nodes].Owner;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: node_owner_idx: %d\n", __FILE__, __LINE__, node_owner_idx);
// #endif
            if(node_owner_idx != ST_UNDEFINED)
            {
                if(magic_set.Show_Node_Owners == ST_TRUE)
                {
                    node_owner_banner_idx = _players[node_owner_idx].banner_id;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: node_owner_banner_idx: %d\n", __FILE__, __LINE__, node_owner_banner_idx);
// #endif
                    assert(node_owner_banner_idx <= 5);
                    node_anim_seg = node_auras_seg[node_owner_banner_idx];
                    node_power = TBL_Nodes[itr_nodes].Power;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: node_power: %d\n", __FILE__, __LINE__, node_power);
// #endif
                    Tile_Index = 0;
                    while(Tile_Index < node_power)
                    {
                        node_aura_world_x = TBL_Nodes[itr_nodes].Aura_Xs[Tile_Index];
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: node_aura_world_x: %d\n", __FILE__, __LINE__, node_aura_world_x);
// #endif
                        node_aura_world_y = TBL_Nodes[itr_nodes].Aura_Ys[Tile_Index];
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: node_aura_world_y: %d\n", __FILE__, __LINE__, node_aura_world_y);
// #endif
                        unexplored_area = TBL_Scouting[(world_plane * WORLD_SIZE) + (node_aura_world_y * WORLD_WIDTH) + (node_aura_world_x)];
                        if(unexplored_area != ST_FALSE)
                        {
                            node_aura_map_x = node_aura_world_x - world_grid_x;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: node_aura_map_x: %d\n", __FILE__, __LINE__, node_aura_map_x);
// #endif
                            if(node_aura_map_x < 0)
                            {
                                node_aura_map_x = node_aura_map_x + WORLD_WIDTH;
                            }
                            node_aura_map_y = node_aura_world_y - world_grid_y;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: node_aura_map_y: %d\n", __FILE__, __LINE__, node_aura_map_y);
// #endif
                            if( (node_aura_map_y >= 0) && (node_aura_map_y < map_grid_height) && (node_aura_map_x >= 0) && (node_aura_map_x < map_grid_width) )
                            {
                                start_x = screen_x + (node_aura_map_x * SQUARE_WIDTH);
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: start_x: %d\n", __FILE__, __LINE__, start_x);
// #endif
                                start_y = screen_y + (node_aura_map_y * SQUARE_HEIGHT);
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: start_y: %d\n", __FILE__, __LINE__, start_y);
// #endif
                                node_anim_frame_idx = ((node_anim_ctr + Tile_Index) % 6);
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: node_anim_frame_idx: %d\n", __FILE__, __LINE__, node_anim_frame_idx);
// #endif
                                FLIC_Set_CurrentFrame(node_anim_seg, node_anim_frame_idx);
                                FLIC_Draw(start_x, start_y, node_anim_seg);
                            }
                        }
                        Tile_Index++;
                    }

// TODO                      if( (TBL_Nodes[itr_nodes].Meld_Flags & 0x01) != 0 )/* M_Warped */
// TODO                      {
// TODO                          node_aura_world_x = TBL_Nodes[itr_nodes].Aura_Xs[Tile_Index];
// TODO                          node_aura_world_y = TBL_Nodes[itr_nodes].Aura_Ys[Tile_Index];
// TODO                          unexplored_area = TBL_Scouting[(world_plane * WORLD_SIZE_DB) + (node_aura_world_y) + (node_aura_world_x)];
// TODO                          if(unexplored_area != ST_FALSE)
// TODO                          {
// TODO                              node_aura_map_x = node_aura_world_x - world_grid_x;
// TODO                              if(node_aura_map_x < 0)
// TODO                              {
// TODO                                  node_aura_map_x = node_aura_map_x + WORLD_WIDTH;
// TODO                              }
// TODO                              node_aura_map_y = node_aura_world_y - world_grid_y;
// TODO                              if( (node_aura_map_y >= 0) && (node_aura_map_y < map_grid_height) && (node_aura_map_x >= 0) && (node_aura_map_x < map_grid_width) )
// TODO                              {
// TODO                                  start_x = screen_x + (node_aura_map_y * SQUARE_WIDTH) + node_aura_map_x;
// TODO                                  start_y = screen_y + (node_aura_map_y * SQUARE_HEIGHT) + node_aura_map_x;
// TODO                                  FLIC_Set_CurrentFrame(node_warped_seg, 0);
// TODO                                  Draw_Picture_To_Bitmap(node_warped_seg, Map_Square_WorkArea);
// TODO                                  FLIC_Set_CurrentFrame(node_warped_seg, 0);
// TODO                                  Screen_Picture_Capture(start_x, start_y, start_x + 19, start_y + 17, Warp_Node_WorkArea);
// TODO                                  if(terrain_anim_ctr >= 0)
// TODO                                  {
// TODO                                      WarpNode_SeedSave = Get_Random_Seed();
// TODO                                      for(Warp_Byte_Index = 0; Warp_Byte_Index < 20; Warp_Byte_Index++)
// TODO                                      {
// TODO                                          Warp_Line_Value = (int16_t)TBL_Warp_GFX_Lines[Warp_Byte_Index];
// TODO                                          if(Random(4) == 1)
// TODO                                          {
// TODO                                              Warp_Line_Value++;
// TODO                                          }
// TODO                                          if(Random(4) == 1)
// TODO                                          {
// TODO                                              Warp_Line_Value--;
// TODO                                          }
// TODO                                          if(Warp_Line_Value < -1)
// TODO                                          {
// TODO                                              Warp_Line_Value = -1;
// TODO                                          }
// TODO                                          if(Warp_Line_Value > 1)
// TODO                                          {
// TODO                                              Warp_Line_Value = 1;
// TODO                                          }
// TODO                                          TBL_Warp_GFX_Lines[Warp_Byte_Index] = Warp_Line_Value;
// TODO                                      }
// TODO                                  }
// TODO                                  tmp_random_seed = Get_Random_Seed();
// TODO                                  Set_Random_Seed(WarpNode_SeedSave);
// TODO                                  // TODO  LBX_IMG_RandomDelete(Warp_Node_WorkArea, 50);
// TODO                                  Set_Random_Seed(tmp_random_seed);
// TODO                                  // TODO  LBX_IMG_HorzWarp(&TBL_Warp_GFX_Lines, Warp_Node_WorkArea);
// TODO                                  // TODO  LBX_IMG_VertWarp(&TBL_Warp_GFX_Lines, Warp_Node_WorkArea);
// TODO                                  // TODO  LBX_IMG_Overlay(0, 0, Warp_Node_WorkArea, Map_Square_WorkArea);
// TODO                                  Draw_Picture(start_x, start_y, Warp_Node_WorkArea);
// TODO                              }
// TODO                          }
// TODO                      }
                }
            }
        }
    }


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_Map_Nodes(screen_x = %d, screen_y = %d, map_grid_width = %d, map_grid_height = %d, world_grid_x = %d, world_grid_y = %d, world_plane = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_grid_width, map_grid_height, world_grid_x, world_grid_y, world_plane);
#endif
}

// WZD o150p12
void Draw_Map_Biota(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane)
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Map_Biota(screen_x = %d, screen_y = %d, map_grid_width = %d, map_grid_height = %d, world_grid_x = %d, world_grid_y = %d, world_plane = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_grid_width, map_grid_height, world_grid_x, world_grid_y, world_plane);
#endif

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
            assert(TBL_Scouting = DBG_ORIG_TBL_Scouting);
            unexplored_area = TBL_Scouting[(world_plane * WORLD_SIZE_DB) + (itr_world_y * WORLD_WIDTH) + (curr_world_x)];
            if(unexplored_area != ST_FALSE)
            {
                assert(TBL_Terr_Specials == DBG_ORIG_TBL_Terr_Specials);
                terrain_special = TBL_Terr_Specials[(world_plane * WORLD_SIZE_DB) + (itr_world_y * WORLD_WIDTH) + (curr_world_x)];
// #ifdef STU_DEBUG
//     // dbg_prn("DEBUG: [%s, %d]: terrain_special: 0x%04X\n", __FILE__, __LINE__, terrain_special);
//     if(terrain_special != 0)
//     {
//         dbg_prn("DEBUG: [%s, %d]: terrain_special: 0x%04X\n", __FILE__, __LINE__, terrain_special);
//         dbg_prn("DEBUG: [%s, %d]: Corruption: %d\n", __FILE__, __LINE__, ((terrain_special & 0x20) != 0));
//         dbg_prn("DEBUG: [%s, %d]: Wild Game: %d\n", __FILE__, __LINE__, ((terrain_special & 0x40) != 0));
//         dbg_prn("DEBUG: [%s, %d]: Hunters Lodge: %d\n", __FILE__, __LINE__, ((terrain_special & 0x10) != 0));
//         dbg_prn("DEBUG: [%s, %d]: Nightshade: %d\n", __FILE__, __LINE__, ((terrain_special & 0x80) != 0));
//     }
// #endif
                assert(TBL_Terrain_Flags == DBG_ORIG_TBL_Terrain_Flags);
                terrain_flag = TBL_Terrain_Flags[(world_plane * WORLD_SIZE_DB) + (itr_world_y * WORLD_WIDTH) + (curr_world_x)];

                if((terrain_flag & 0x20) != 0)  /* Corruption */
                {
                    site_pict_seg = IMG_OVL_Corruption;
                    FLIC_Draw(itr_screen_x, itr_screen_y, site_pict_seg);
                }

                if((terrain_special & 0x40) != 0)  /* Wild Game */
                {
                    has_city = ST_FALSE;
                    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
                    {
                        if((_CITIES[itr_cities].world_x == curr_world_x) && (_CITIES[itr_cities].world_y == itr_world_y) && (_CITIES[itr_cities].world_plane == world_plane))
                        {
                            has_city = ST_TRUE;
                        }
                    }

                    if(has_city == ST_FALSE)
                    {
                        site_pict_seg = IMG_OVL_WildGame;
                        FLIC_Draw(itr_screen_x, itr_screen_y, site_pict_seg);

                        if((terrain_special & 0x10) != 0)  /* Hunter's Lodge */
                        {
                            site_pict_seg = UU_hunters_lodge_seg;
                            FLIC_Draw(itr_screen_x, itr_screen_y, site_pict_seg);
                        }
                    }
                }

                if((terrain_special & 0x80) != 0)  /* Nightshade */
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_Map_Biota(screen_x = %d, screen_y = %d, map_grid_width = %d, map_grid_height = %d, world_grid_x = %d, world_grid_y = %d, world_plane = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_grid_width, map_grid_height, world_grid_x, world_grid_y, world_plane);
#endif

}

// WZD o150p13
void Draw_Map_Minerals(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane)
{
    uint8_t * ptr_TBL_Scouting;
    uint8_t * ptr_TBL_Terr_Specials;
    int16_t Terrain_Special;
    int16_t City_Cover;
    int16_t curr_world_x;
    int16_t itr_screen_x;
    int16_t itr_screen_y;
    int16_t itr_world_x;
    int16_t itr_world_y;
    int16_t itr_cities;
    uint8_t unexplored_area;
    SAMB_ptr site_pict_seg;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Map_Minerals(screen_x = %d, screen_y = %d, map_grid_width = %d, map_grid_height = %d, world_grid_x = %d, world_grid_y = %d, world_plane = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_grid_width, map_grid_height, world_grid_x, world_grid_y, world_plane);
#endif

    itr_screen_y = screen_y;
    itr_world_y = world_grid_y;

    while(world_grid_y + map_grid_height > itr_world_y)
    {
        itr_screen_x = screen_x;

        assert(TBL_Terr_Specials == DBG_ORIG_TBL_Terr_Specials);
        ptr_TBL_Terr_Specials = (uint8_t *)(TBL_Terr_Specials + (world_plane * WORLD_SIZE) + (itr_world_y * WORLD_WIDTH));

        ptr_TBL_Scouting = (uint8_t *)(TBL_Scouting + (world_plane * WORLD_SIZE) + (itr_world_y * WORLD_WIDTH));

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

            unexplored_area = *(ptr_TBL_Scouting + curr_world_x);
            if(unexplored_area != ST_FALSE)
            {
                // mov     al, [es:bx]
                // mov     ah, 0
                // and     ax, 0Fh                         ; ¿ clearing sign-extended ?
                // TODO: What's with the 0x40 and 0x80?
                Terrain_Special = (((int16_t)*(ptr_TBL_Terr_Specials + curr_world_x)) & 0x0F);
                if(Terrain_Special != 0)
                {
                    City_Cover = 0;
                    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
                    {
                        if((_CITIES[itr_cities].world_x == curr_world_x) && (_CITIES[itr_cities].world_y == itr_world_y) && (_CITIES[itr_cities].world_plane == world_plane))
                        {
                            City_Cover = 1;
                        }
                    }

                    if(City_Cover == 0)
                    {
                        site_pict_seg = mineral_site_segs[Terrain_Special];
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_Map_Minerals(screen_x = %d, screen_y = %d, map_grid_width = %d, map_grid_height = %d, world_grid_x = %d, world_grid_y = %d, world_plane = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_grid_width, map_grid_height, world_grid_x, world_grid_y, world_plane);
#endif

}

// WZD o150p14
void Draw_Map_Roads(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane)
{
    int16_t itr_screen_x;
    int16_t itr_screen_y;
    int16_t itr_world_x;
    int16_t itr_world_y;
    int16_t curr_world_x;
    uint8_t unexplored_area;
    uint8_t * terrain_flags_table_sgmt;
    int16_t terrain_flags_table_ofst;
    // DEBUG  uint8_t terrain_flag;
    uint8_t has_road;
    uint8_t has_eroad;
    int16_t itr_cities;
    int16_t has_city;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Map_Roads(screen_x = %d, screen_y = %d, map_grid_width = %d, map_grid_height = %d, world_grid_x = %d, world_grid_y = %d, world_plane = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_grid_width, map_grid_height, world_grid_x, world_grid_y, world_plane);
#endif

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

            unexplored_area = TBL_Scouting[(world_plane * WORLD_SIZE) + (itr_world_y * WORLD_WIDTH) + (curr_world_x)];
            if(unexplored_area != ST_FALSE)
            {
                terrain_flags_table_sgmt = (uint8_t *)TBL_Terrain_Flags;
                terrain_flags_table_ofst = (world_plane * WORLD_SIZE) + ((itr_world_y - 1) * WORLD_WIDTH) + (curr_world_x - 1);
                // -------------
                // |  0|  1|  2|
                // -------------
                // | 60| 61| 62|
                // -------------
                // |120|121|122|
                // -------------
                // test order: N, NE, E, SE, S, SW, W, NW  (1, 2, 62, 122, 121, 120, 60, 0)
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: terrain_flags_table_ofst: %d\n", __FILE__, __LINE__, terrain_flags_table_ofst);
// #endif
//                 terrain_flag = *(terrain_flags_table_sgmt + terrain_flags_table_ofst + 61);
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: terrain_flag: %d\n", __FILE__, __LINE__, terrain_flag);
// #endif

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
                            FLIC_Set_CurrentFrame(eroads_seg[0], road_anim_ctr);
                            FLIC_Draw(itr_screen_x, itr_screen_y, eroads_seg[0]);
                        }
                        if((*(terrain_flags_table_sgmt + terrain_flags_table_ofst +   2) & 0x08) != 0)
                        {
                            FLIC_Set_CurrentFrame(eroads_seg[1], road_anim_ctr);
                            FLIC_Draw(itr_screen_x, itr_screen_y, eroads_seg[1]);
                        }
                        if((*(terrain_flags_table_sgmt + terrain_flags_table_ofst +  62) & 0x08) != 0)
                        {
                            FLIC_Set_CurrentFrame(eroads_seg[2], road_anim_ctr);
                            FLIC_Draw(itr_screen_x, itr_screen_y, eroads_seg[2]);
                        }
                        if((*(terrain_flags_table_sgmt + terrain_flags_table_ofst + 122) & 0x08) != 0)
                        {
                            FLIC_Set_CurrentFrame(eroads_seg[3], road_anim_ctr);
                            FLIC_Draw(itr_screen_x, itr_screen_y, eroads_seg[3]);
                        }
                        if((*(terrain_flags_table_sgmt + terrain_flags_table_ofst + 121) & 0x08) != 0)
                        {
                            FLIC_Set_CurrentFrame(eroads_seg[4], road_anim_ctr);
                            FLIC_Draw(itr_screen_x, itr_screen_y, eroads_seg[4]);
                        }
                        if((*(terrain_flags_table_sgmt + terrain_flags_table_ofst + 120) & 0x08) != 0)
                        {
                            FLIC_Set_CurrentFrame(eroads_seg[5], road_anim_ctr);
                            FLIC_Draw(itr_screen_x, itr_screen_y, eroads_seg[5]);
                        }
                        if((*(terrain_flags_table_sgmt + terrain_flags_table_ofst +  60) & 0x08) != 0)
                        {
                            FLIC_Set_CurrentFrame(eroads_seg[6], road_anim_ctr);
                            FLIC_Draw(itr_screen_x, itr_screen_y, eroads_seg[6]);
                        }
                        if((*(terrain_flags_table_sgmt + terrain_flags_table_ofst +   0) & 0x08) != 0)
                        {
                            FLIC_Set_CurrentFrame(eroads_seg[7], road_anim_ctr);
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_Map_Roads(screen_x = %d, screen_y = %d, map_grid_width = %d, map_grid_height = %d, world_grid_x = %d, world_grid_y = %d, world_plane = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_grid_width, map_grid_height, world_grid_x, world_grid_y, world_plane);
#endif
}

// WZD o150p15
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Map_Units(screen_x = %d, screen_y = %d, map_grid_width = %d, map_grid_height = %d, world_grid_x = %d, world_grid_y = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_grid_width, map_grid_height, world_grid_x, world_grid_y);
#endif

#ifdef STU_DEBUG
    if(DBG_TST_Draw_Map_Units == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Map_Units()  (DBG_TST_Draw_Map_Units == 1)\n", __FILE__, __LINE__);

    }
#endif



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
            entities_index = ( ((itr_map_y + map_start_y) * MAP_WIDTH) + (itr_map_x + map_start_x) );
            unit_idx = entities_on_movement_map[entities_index];
#ifdef STU_DEBUG
    if(DBG_TST_Selected_Stack == 1)
    {
        if(unit_idx == 55 || unit_idx == 536 || unit_idx == 828)
        dbg_prn("DEBUG: [%s, %d]: DBG_TST_FAILURE: unit_idx: %d\n", __FILE__, __LINE__, unit_idx);
    }
#endif

            if( (unit_idx != ST_UNDEFINED) && (unit_idx < 1000) )
            {
                Draw_Unit_Picture(itr_screen_x, itr_screen_y, unit_idx, 2);
            }

            itr_screen_x += SQUARE_WIDTH;
            itr_map_x += 1;
        }

        itr_screen_y += SQUARE_HEIGHT;
        itr_map_y += 1;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_Map_Units(screen_x = %d, screen_y = %d, map_grid_width = %d, map_grid_height = %d, world_grid_x = %d, world_grid_y = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_grid_width, map_grid_height, world_grid_x, world_grid_y);
#endif
}


// WZD o150p16
// "Reduced Map"/"World Window"
// AKA Draw_Minimap()
// TODO  rename to ~create/build
void Draw_Reduced_Map(int16_t minimap_start_x, int16_t minimap_start_y, int16_t world_plane, byte_ptr minimap_pict_seg, int16_t minimap_width, int16_t minimap_height, int16_t mark_x, int16_t mark_y, int16_t mark_flag)
{
    int16_t terrain_type_idx_base;
    int16_t terrain_type_idx;
    uint8_t * explore_data_ptr;
    uint8_t * world_data_ptr;
    uint8_t * minimap_pict_data_ptr;
    int16_t itr_minimap_height;
    int16_t itr_minimap_width;
    int16_t minimap_square_y;
    int16_t minimap_square_x;
    int8_t explored_flag;
    uint8_t minimap_terrain_color;
    int16_t itr_cities;
    int16_t city_world_x;
    int16_t city_world_y;
    int16_t city_minimap_x;
    int16_t city_minimap_y;
    int16_t city_owner_idx;
    int16_t mark_minimap_x;
    int16_t mark_minimap_y;
    uint8_t city_owner_player_banner;
    uint8_t banner_color;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Reduced_Map(minimap_start_x = %d, minimap_start_y = %d, world_plane = %d, minimap_pict_seg = %p, minimap_width = %d, minimap_height = %d, mark_x = %d, mark_y = %d, mark_flag = %d)\n", __FILE__, __LINE__, minimap_start_x, minimap_start_y, world_plane, minimap_pict_seg, minimap_width, minimap_height, mark_x, mark_y, mark_flag);
#endif


    terrain_type_idx_base = world_plane * TERRAIN_COUNT;

    explore_data_ptr = (TBL_Scouting + (world_plane * WORLD_SIZE));
    world_data_ptr = (_world_maps + (world_plane * 4800));
    minimap_pict_data_ptr = minimap_pict_seg + 16;  // +1 segment (paragraph), for the animation/picture/bitmap header

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

            if( (minimap_square_y >= 0 && minimap_square_y < WORLD_HEIGHT) && *(explore_data_ptr + (minimap_square_y * WORLD_WIDTH) + minimap_square_x) != ST_FALSE )
            {
                terrain_type_idx = GET_2B_OFS(world_data_ptr, ((minimap_square_y * 120) + (minimap_square_x * 2)));
                terrain_type_idx += terrain_type_idx_base;
                minimap_terrain_color = terrain_lbx_002[terrain_type_idx];
                *(minimap_pict_data_ptr + ((itr_minimap_width * minimap_height) + itr_minimap_height)) = minimap_terrain_color;
            }
        }
    }

    /*
        BEGIN: Loop Cities
    */
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].world_plane == world_plane)
        {
            city_world_x = _CITIES[itr_cities].world_x;
            city_world_y = _CITIES[itr_cities].world_y;
            if(*(explore_data_ptr + (city_world_y * WORLD_WIDTH) + city_world_x) != ST_FALSE)
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
// DELETE  #ifdef STU_DEBUG
// DELETE      dbg_prn("DEBUG: [%s, %d]: NEUTRAL_PLAYER: %d, %d\n", __FILE__, __LINE__, city_world_x, city_world_y);
// DELETE      dbg_prn("DEBUG: [%s, %d]: NEUTRAL_PLAYER: %d, %d\n", __FILE__, __LINE__, city_minimap_x, city_minimap_y);
// DELETE  #endif
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_Reduced_Map(minimap_start_x = %d, minimap_start_y = %d, world_plane = %d, minimap_pict_seg = %p, minimap_width = %d, minimap_height = %d, mark_x = %d, mark_y = %d, mark_flag = %d)\n", __FILE__, __LINE__, minimap_start_x, minimap_start_y, world_plane, minimap_pict_seg, minimap_width, minimap_height, mark_x, mark_y, mark_flag);
#endif

}


// WZD o150p17

// WZD o150p18
