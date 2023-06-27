
#include "MoX_TYPE.H"
#include "MoX_DEF.H"
#include "MoM_DEF.H"

#include "MoX_Data.H"
#include "UnitType.H"
#include "UnitMove.H"

#include "Explore.H"
#include "FLIC_Draw.H"
#include "Graphics.H"
#include "MainScr.H"
#include "MainScr_Maps.H"
#include "SCastScr.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif



// dseg:291E     BEGIN: ovr052
// dseg:291E
// dseg:291E Load/Init Main Game
// dseg:291E
// dseg:291E rsc01_MAIN_LBX db 'MAIN.LBX',0          ; should use dseg:29ef (reused in cud thrown/breath,
// dseg:291E                                         ; with the first two bytes still free)
// dseg:2927 rsc02_BACKGRND_LBX db 'BACKGRND.LBX',0  ; should use dseg:29f4 (reused in cud thrown/breath)
// dseg:2934 rsc03_UNITS1_LBX db 'UNITS1.LBX',0      ; should use dseg:2a1f (reused in cud thrown/breath)
// dseg:293F rsc04_UNITS2_LBX db 'UNITS2.LBX',0      ; should use dseg:2a26 (reused in cud thrown/breath)
// dseg:294A rsc05_UNITVIEW_LBX db 'UNITVIEW.LBX',0  ; should use dseg:2a59 (reused in cud thrown/breath)
// dseg:2957 rsc06_SPECIAL_LBX db 'SPECIAL.LBX',0    ; should use dseg:2a62 (reused in cud thrown/breath)
// dseg:2963 rsc07_SPECIAL2_LBX db 'SPECIAL2.LBX',0  ; should use dseg:2a6a (reused in cud thrown/breath)
// dseg:2970 rsc08_ITEMS_LBX db 'ITEMS.LBX',0        ; should use dseg:2a73 (reused in cud thrown/breath)
// dseg:297A rsc09_CHRIVER_LBX db 'CHRIVER.LBX',0    ; should use dseg:2a3f (reused in cud thrown/breath)
// dseg:2986 rsc0A_ITEMISC_LBX db 'ITEMISC.LBX',0    ; should use dseg:2a79 (first 9 bytes reused in cud
// dseg:2986                                         ; thrown/breath, last 3 in word of recall at circle)
// dseg:2992 rsc0B_CITYSCAP_LBX db 'CITYSCAP.LBX',0  ; should use dseg:2a50 (reused in recall at circle)
// dseg:299F rsc0C_MAPBACK_LBX db 'MAPBACK.LBX',0    ; should use dseg:29e7 (reused in recall at circle)
// dseg:29AB rsc0D_CMBMAGIC_LBX db 'CMBMAGIC.LBX',0  ; should use dseg:2a2d
// dseg:29B8 rsc0E_CMBTCITY_LBX db 'CMBTCITY.LBX',0  ; should use dseg:2a36
// dseg:29C5 rsc0F_CITYWALL_LBX db 'CITYWALL.LBX',0  ; should use dseg:2a47
// dseg:29D2 Init_Terrain()
// WZD dseg:29D2
char terrain_lbx_file[] = "TERRAIN.LBX";
// dseg:29DE terrstat_lbx_file db 'TERRSTAT',0
// WZD dseg:29E7
char mapback_lbx_file[] = "MAPBACK";
// dseg:29EF Menu_Screen_Load_Pictures()
// dseg:29EF main_lbx_file db 'MAIN',0
// dseg:29F4 backgrnd_lbx_file db 'BACKGRND',0
// dseg:29FD cnst_SOUNDFX_File db 'SOUNDFX',0
// dseg:2A05 spelldat_lbx_file db 'SPELLDAT.LBX',0
// dseg:2A12 cnst_BUILDDAT_File db 'BUILDDAT.LBX',0
// dseg:2A1F cnst_Units1_File db 'UNITS1',0
// dseg:2A26 cnst_Units2_File db 'UNITS2',0
// dseg:2A2D cnst_CMBMAGIC_File db 'CMBMAGIC',0
// dseg:2A36 cnst_CMBTCITY_File db 'CMBTCITY',0
// dseg:2A3F cnst_CHRIVER_File db 'CHRIVER',0
// dseg:2A47 cnst_CITYWALL_File db 'CITYWALL',0
// dseg:2A50 cnst_CITYSCAP_File db 'CITYSCAP',0
// dseg:2A59 cnst_UNITVIEW_File db 'UNITVIEW',0
// dseg:2A62 cnst_SPECIAL1_File db 'SPECIAL',0
// dseg:2A6A cnst_SPECIAL2_File db 'SPECIAL2',0
// dseg:2A73 cnst_ITEMS_File db 'ITEMS',0
// dseg:2A79 cnst_ITEMISC_File db 'ITEMISC',0
// dseg:2A81 special2_lbx_file db 'special2',0       ; should use dseg:2a6a
// dseg:2A81
// dseg:2A81     END: ovr052


// WZD dseg:6FF4                                                 BEGIN: ovr150

// WZD dseg:6FF4
// int16_t OVL_TileAnim_Stage = 0;
// WZD dseg:6FF6
// int16_t OVL_EnchRoad_Stage = 0;
// WZD dseg:6FF8
// int16_t OVL_NodeSprkl_Stage = 0;
// WZD dseg:6FFA db    0
// WZD dseg:6FFB db    0
// WZD dseg:6FFC
// int16_t OVL_Anim_Stepper = 0;
// WZD dseg:6FFE
int16_t map_draw_prev_x = ST_UNDEFINED;
// WZD dseg:7000
int16_t map_draw_prev_y = ST_UNDEFINED;
// WZD dseg:7002
// AKA OVL_NewMapSustain
int16_t map_draw_sustain = 2;

// WZD dseg:7002                                                 END: ovr150




// BEGIN: Draw_Minimap()
// WZD dseg:700A
uint8_t COL_MinimapBanners[6] = {171, 216, 205, 201, 210, 50};  // 0xAB, 0xD8, 0xCD, 0xC9, 0xD2, 0x32
// WZD dseg:7010
uint8_t COL_MinimapNeutral = 50;  // 0x32
// END: Draw_Minimap()



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


// WZD dseg:C11C
int16_t minimap_height;
// WZD dseg:C11E
int16_t minimap_width;
// WZD dseg:C120
int16_t minimap_y;
// WZD dseg:C122
int16_t minimap_x;


// WZD dseg:CC22
SAMB_ptr terrain_lbx_001;
// WZD dseg:CC26
byte_ptr terrain_001_1;
// WZD dseg:CC28
// g_EmmHndl_TERRAIN dw 0                  ; EMM_Load_LBX handle
SAMB_ptr terrain_lbx_000;





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

        if(xpos < l_map_x)
        {
            shift_right_flag = ST_FALSE;
        }
        else
        {
            shift_right_flag = ST_TRUE;
        }

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
            if(l_map_x == 60)
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
                l_map_x = 59;  // ? MoO2 _MAP_MAX_X ?
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

    Minimap_Coords(&minimap_x, &minimap_y, ((l_map_x + (MAP_WIDTH/2)) / WORLD_WIDTH), (l_map_y + (MAP_HEIGHT/2)), minimap_width, minimap_height);

    Draw_Minimap(minimap_x, minimap_y, map_plane, _reduced_map_seg, minimap_width, minimap_height, 0, 0, 0);

    Draw_Map_Window(screen_x, screen_y, map_width, map_height, l_map_x, l_map_y, map_plane);


    if(map_moved_flag == ST_FALSE)
    {
        Draw_Active_Unit_Stack(*map_x, *map_y, map_plane);
    }


    // j_OVL_RedrawScouting(x, y, map_width, map_height, l_map_x, l_map_y, map_plane);


    Reset_Window();

    *map_x = l_map_x;
    *map_y = l_map_y;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Maps(screen_x = %d, screen_y = %d, map_width = %d, map_height = %d , *map_x = %d, *map_y = %d, map_plane = %d, xpos = %d, ypos = %d, player_idx = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_width, map_height, *map_x, *map_y, map_plane, xpos, ypos, player_idx);
#endif

}

// WZD o67p0
// AKA IDK_CheckSet_MapDisplay_XY
void IDK_CheckSet_MapDisplay_XY(void)
{
    if(_prev_world_y + MAP_HEIGHT >= WORLD_HEIGHT)
    {
        _prev_world_y = WORLD_HEIGHT - MAP_HEIGHT;
    }

    if(_prev_world_x >= WORLD_WIDTH)
    {
        _prev_world_x = _prev_world_x - WORLD_WIDTH;
    }

    if(_prev_world_x < 0)
    {
        _prev_world_x = _prev_world_x + 60;
    }

    if(_prev_world_y < 0)
    {
        _prev_world_y = 0;
    }

    if(_map_x == 0 && _prev_world_x > 49)
    {
        _map_x = 60;
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
    if( (all_units_moved == 0) && (_unit_stack_count == 0) )
    {
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
    int16_t unit_idx;  // ? DNE ?
    int16_t itr_unit_stack_count;

    unit_idx = _unit_stack[0].unit_idx;

    if(
        (draw_active_stack_flag != -2) &&
        (_UNITS[unit_idx].owner_idx != ST_UNDEFINED) &&
        ( (_UNITS[unit_idx].world_plane == world_plane) || (_UNITS[unit_idx].In_Tower == ST_TRUE) )
    )
    {
        unit_xw = _UNITS[unit_idx].world_x;
        unit_yw = _UNITS[unit_idx].world_y;

        in_view = World_To_Screen(mmap_xw, mmap_yw, &unit_xw, &unit_yw);

        // not never, not always, so must be set for draw-cycling, so increment the cycle
        if(draw_active_stack_flag != -1)  /* always draw */
        {
            draw_active_stack_flag = ((draw_active_stack_flag + 1) % 8);
        }

        /*
            WTF, Mate?!?

                ...test success is the jump branch...

                draw_active_stack_flag == -1
                draw_active_stack_flag > 3
                    one must pass, both can't fail ... OR
                
                draw_active_stack_flag != -1
                entities_on_movement_map[] == -1
                draw_active_stack_flag != -1
                    one must pass, all can't fail ... OR

                if( ((...)||(...) && ((...)||(...)||(...)) )
                So, ...
                    if 1of2 & 1of3 ...
            ...
            WRONG!!!!!
        */
        // Test 1: not set to always draw, but is in a draw phase
        // Test 2: is set to always, but there is no entity at our calulated coordinates
        // Test 3: is within the boundaries of the movement map
        if( 
            ( (draw_active_stack_flag != -1) && (draw_active_stack_flag > 3) ) ||
            ( (draw_active_stack_flag == -1) && (entities_on_movement_map[( ((unit_xw - mmap_yw) * 24) + (unit_yw * 2) )] == -1) )
        )
        {
                /*
                    //drake178
                    BUG: variable type mismatch - these coordinates
                    have already been converted into pixel draw
                    coordinates and can't be used to index into the array
                */
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
                Unit_Window_Draw_Unit_Picture(unit_xw, unit_yw, unit_idx, 2);

            }

            unit_idx = _unit_stack[0].unit_idx;
            Draw_Minimap(minimap_x, minimap_y, world_plane, _reduced_map_seg, minimap_width, minimap_height, _UNITS[unit_idx].world_x, _UNITS[unit_idx].world_y, ST_TRUE);
        // Draw_Minimap(int16_t minimap_start_x, int16_t minimap_start_y, int16_t world_plane, byte_ptr minimap_pict_seg, int16_t minimap_width, int16_t minimap_height, int16_t Mark_X, int16_t Mark_Y, int16_t Mark)
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_Active_Unit_Stack(mmap_xw = %d, mmap_yw = %d, world_plane = %d)\n", __FILE__, __LINE__, mmap_xw, mmap_yw, world_plane);
#endif
}

// WZD o67p08
void Minimap_Set_Dims(int16_t width, int16_t height)
{
    minimap_width = width;
    minimap_height = height;
}


// WZD o67p09
void Draw_Minimap_Window(int16_t start_x, int16_t start_y, int16_t width, int16_t height)
{
    int16_t reduced_map_box_color;
    int16_t line_x;
    int16_t line_y;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Minimap_Window()\n", __FILE__, __LINE__);
#endif

    Reset_Window();

    Draw_Picture(start_x, start_y, _reduced_map_seg);


    reduced_map_box_color = 13;
    line_x = start_x + ( width / 2) + ( MOVEMENT_MAP_WIDTH / 2);
    line_y = start_y + (height / 2) + (MOVEMENT_MAP_HEIGHT / 2);

    // TODO  // VGA_Draw_Line(line_x, line_y, line_x, line_y);
    // TODO  VGA_Draw_Line(line_x-1, line_y-1, line_x+1, line_y-1);
    // TODO  VGA_Draw_Line(line_x-1, line_y-1, line_x-1, line_y+1);
    // TODO  VGA_Draw_Line(line_x+10, line_y-1, line_x+12, line_y-1);
    // TODO  VGA_Draw_Line(line_x+12, line_y-1, line_x+12, line_y+1);
    // TODO  VGA_Draw_Line(line_x-1, line_y+10, line_x-1, line_y+8);
    // TODO  VGA_Draw_Line(line_x-1, line_y+10, line_x+1, line_y+10);
    // TODO  VGA_Draw_Line(line_x+12, line_y+10, line_x+12, line_y+8);
    // TODO  VGA_Draw_Line(line_x+12, line_y+10, line_x+10, line_y+10);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_Minimap_Window()\n", __FILE__, __LINE__);
#endif
}

// WZD o67p10
void Set_Entities_On_Map_Window(int16_t world_x, int16_t world_y, int16_t world_plane)
{
    int16_t Unit_On_Tile;
    int16_t City_Visible;
    int16_t entity_world_y;
    int16_t entity_world_x;
    int16_t itr_map_width;
    int16_t itr_map_height;
    int16_t itr_units;
    int16_t itr_cities;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Set_Entities_On_Map_Window()\n", __FILE__, __LINE__);
#endif

    City_Visible = ST_FALSE;


    for(itr_map_height = 0; itr_map_height < MAP_HEIGHT; itr_map_height++)
    {
        for(itr_map_width = 0; itr_map_width < MAP_WIDTH; itr_map_width++)
        {
            entities_on_movement_map[itr_map_height * MAP_WIDTH + itr_map_width] = ST_UNDEFINED;
        }
    }

    for(itr_units =0; itr_units < _units; itr_units++)
    {
        if(_UNITS[itr_units].owner_idx != ST_UNDEFINED)
        {
            if(TILE_IsVisible(world_x, world_y, world_plane) == ST_TRUE)
            {
                if( (_UNITS[itr_units].world_plane == world_plane) || (_UNITS[itr_units].In_Tower == ST_TRUE) )
                {

                    entity_world_y = _UNITS[itr_units].world_y;

                    if( (entity_world_y >= world_y) && (world_y + MAP_HEIGHT > entity_world_y) )
                    {
                        
                        entity_world_x = _UNITS[itr_units].world_x;

                        if(
                             ( (world_x < entity_world_x) && (world_x + MAP_WIDTH > entity_world_x) ) ||
                             ( (world_x < entity_world_x + WORLD_WIDTH) && (world_x + MAP_WIDTH > entity_world_x + WORLD_WIDTH) )
                        )
                        {
                            // HACK: because the Dasm reassigns the value in the middle of the comparisson branches - not sure if/what this could be like in C code
                            if(( (world_x < entity_world_x + WORLD_WIDTH) && (world_x + MAP_WIDTH > entity_world_x + WORLD_WIDTH) ))
                            {
                                entity_world_x = entity_world_x + WORLD_WIDTH;
                            }


                            Unit_On_Tile = entities_on_movement_map[( ((entity_world_y - world_y) * MAP_WIDTH) + (entity_world_x - world_x) )];

                            if(Unit_On_Tile != ST_UNDEFINED)
                            {
                                if(_UNITS[itr_units].Draw_Priority > _UNITS[Unit_On_Tile].Draw_Priority)
                                {
                                    entities_on_movement_map[( ((entity_world_y - world_y) * MAP_WIDTH) + (entity_world_x - world_x) )] = itr_units;
                                }
                            }
                            else
                            {
                                if(_UNITS[itr_units].Draw_Priority > 0)
                                {
                                    entities_on_movement_map[( ((entity_world_y - world_y) * MAP_WIDTH) + (entity_world_x - world_x) )] = itr_units;
                                }
                                else
                                {
                                    if( (_UNITS[itr_units].Draw_Priority == 0) && (draw_active_stack_flag != ST_UNDEFINED) )
                                    {
                                        entities_on_movement_map[( ((entity_world_y - world_y) * MAP_WIDTH) + (entity_world_x - world_x) )] = itr_units;
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
        if(
            ( (_CITIES[itr_cities].world_plane == world_plane) || (_CITIES[itr_cities].world_plane = 2) ) &&
            (TBL_Scouting[( (_CITIES[itr_cities].world_plane * 2400) + (_CITIES[itr_cities].world_y * 60) + _CITIES[itr_cities].world_x )] != ST_FALSE)
        )
        {
                entity_world_y = _CITIES[itr_cities].world_y;

                if( (entity_world_y >= world_y) && (world_y + MAP_HEIGHT > entity_world_y) )
                {
                    entity_world_x = _CITIES[itr_cities].world_x;

                    if(
                        ( (world_x < entity_world_x) && (world_x + MAP_WIDTH > entity_world_x) ) ||
                            ( (world_x < entity_world_x + WORLD_WIDTH) && (world_x + MAP_WIDTH > entity_world_x + WORLD_WIDTH) )
                    )
                    {
                        // HACK: because the Dasm reassigns the value in the middle of the comparisson branches - not sure if/what this could be like in C code
                        if(( (world_x < entity_world_x + WORLD_WIDTH) && (world_x + MAP_WIDTH > entity_world_x + WORLD_WIDTH) ))
                        {
                            entity_world_x = entity_world_x + WORLD_WIDTH;
                        }

                        entity_world_x = entity_world_x - world_x;
                        City_Visible = ST_TRUE;
                    }
            }
        }

        if(City_Visible == ST_TRUE)
        {
            entities_on_movement_map[( ((entity_world_y - world_y) * MAP_WIDTH) + (entity_world_x - world_x) )] = (itr_cities + 1000);
        }
    }


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Set_Entities_On_Map_Window()\n", __FILE__, __LINE__);
#endif
}

// WZD o67p13
void Set_Unit_Draw_Priority(void)
{
    int16_t draw_priority;
    int16_t itr_units;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: UNIT_DrawPriorities()\n", __FILE__, __LINE__);
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
            draw_priority = -1;
        }

        if(_UNITS[itr_units].owner_idx == _human_player_idx && UNIT_HasInvisibility(itr_units) == ST_TRUE)
        {
            draw_priority = -1;
        }

        _UNITS[itr_units].Draw_Priority = draw_priority;


    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: UNIT_DrawPriorities()\n", __FILE__, __LINE__);
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

    if(world_gird_x >= 60)
    {
        world_gird_x -= 60;
    }
    if(world_gird_x < 0)
    {
        world_gird_x += 60;
    }

    *map_x = world_gird_x - MAP_WIDTH_HALF;
    if(*map_x <= 0)
    {
        *map_x = ((*map_x + 60) / 60);
    }

    *map_y = world_grid_y - MAP_HEIGHT_HALF;
    if(*map_y <= 0)
    {
        *map_y = 0;
    }

    if(*map_y + 10 >= 40)
    {
        *map_y = 30;  // 40 - 10  World Width - Map Width
    }

    _prev_world_x = *map_x;
    _prev_world_y = *map_y;

    // NIU CRP_OVL_MapWindowX = G_OVL_MapDisplay_X * TILE_WIDTH;
    // NIU CRP_OVL_MapWindowY = G_OVL_MapDisplay_Y * TILE_HEIGHT;
    
// j_UNIT_DrawPriorities
// ; sets the draw priority field of each unit record
// ; based on attack strength, transport capability, and
// ; visibility (in the case of AI units)

// j_STK_NoUnitDraw
// ; set the draw priority of all units in the active
// ; unit stack to zero

// j_OVL_SetUnitsOnMap(*map_x, *map_y, plane_);
// ; fills out the OVL_UnitsOnMap array with the unit or

}

// WZD o68p03
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
void Minimap_Coords(int16_t * minimap_x, int16_t * minimap_y, int16_t mid_x, int16_t mid_y, int16_t minimap_width, int16_t minimap_height)
{
    int16_t tmp_minimap_x;
    int16_t tmp_minimap_y;


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Minimap_Coords(*minimap_x = %d, *minimap_y = %d, mid_x = %d, mid_y = %d, minimap_width = %d, minimap_height = %d)\n", __FILE__, __LINE__, *minimap_x, *minimap_y, mid_x, mid_y, minimap_width, minimap_height);
#endif

    tmp_minimap_x = mid_x - (minimap_width / 2);
    if(tmp_minimap_x > 0)
    {
        *minimap_x = tmp_minimap_x;
    }
    else
    {
        *minimap_x = (tmp_minimap_x + 60) / 60;
    }

    tmp_minimap_y = mid_y - (minimap_height / 2);

    *minimap_y = tmp_minimap_y;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Minimap_Coords(*minimap_x = %d, *minimap_y = %d, mid_x = %d, mid_y = %d, minimap_width = %d, minimap_height = %d)\n", __FILE__, __LINE__, *minimap_x, *minimap_y, mid_x, mid_y, minimap_width, minimap_height);
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


    // TST_Draw_Map_Terrain(        0, 20 + (18 * 0), map_width, map_height, cur_map_xpos, cur_map_ypos, map_plane);
    // TST_Draw_Map_Terrain((20 * 11), 20 + (18 * 0), map_width, map_height, cur_map_xpos, cur_map_ypos, map_plane);
    // TST_Draw_Map_Terrain(        0, 20 + (18 * 9), map_width, map_height, cur_map_xpos, cur_map_ypos, map_plane);
    // TST_Draw_Map_Terrain((20 * 11), 20 + (18 * 9), map_width, map_height, cur_map_xpos, cur_map_ypos, map_plane);
    Draw_Map_Terrain(screen_x, screen_y, map_w, map_h, map_draw_curr_x, map_draw_curr_y, map_p);
    // ; draws map terrain tiles into the current draw segment
    // ; according to the passed parameters:
    // ;   Top/Left - display pixel coordinates
    // ;   H/map_height - horizontal/vertical window size in tiles
    // ;   XPos/YPos/Plane - top left map coordinates
    // ; tiles that are not scouted are painted with color $00

// push    [bp+map_plane]                  ; Plane
// push    _DI_cur_map_ypos                ; YPos
// push    _SI_cur_map_xpos                ; XPos
// push    [bp+map_height]
// push    [bp+map_width]                     ; map_width
// push    [bp+y]                          ; Top
// push    [bp+x]                          ; Left
// OVL_DrawMinerals()
// // ; draws mineral type terrain specials into the current
// // ; draw segment based on the specified map parameters,
// // ; unless the tile is unscouted or also has a city

// push    [bp+map_plane]                  ; Plane
// push    _DI_cur_map_ypos                ; YPos
// push    _SI_cur_map_xpos                ; XPos
// push    [bp+map_height]                     ; map_height
// push    [bp+map_width]                     ; map_width
// push    [bp+y]                          ; Top
// push    [bp+x]                          ; Left
// OVL_DrawTerrSpecials()
// // ; draws corruption, wild games, and nightshades into
// // ; the current draw segment based on the specified map
// // ; parameters, unless the tile is unscouted (would also
// // ; draw the hunters' lodge if it still existed)
// // ; unlike other specials, nightshades and corruption get
// // ; drawn under a city, but wild games do not

// push    [bp+map_plane]                  ; Plane
// push    _DI_cur_map_ypos                ; YPos
// push    _SI_cur_map_xpos                ; XPos
// push    [bp+map_height]                     ; map_height
// push    [bp+map_width]                     ; map_width
// push    [bp+y]                          ; Top
// push    [bp+x]                          ; Left
// OVL_DrawRoads()
// // ; draws roads and enchanted roads into the current draw
// // ; segment based on the specified map parameters, unless
// // ; the tile is unscouted
// // ; BUG: this code will read out of bounds if roads are
// // ; built on the top row of the arcanus map, or the
// // ; bottom row of the myrror map

// push    [bp+map_plane]                  ; Plane
// push    _DI_cur_map_ypos                ; YPos
// push    _SI_cur_map_xpos                ; XPos
// push    [bp+map_height]                     ; map_height
// push    [bp+map_width]                     ; map_width
// push    [bp+y]                          ; Top
// push    [bp+x]                          ; Left
// OVL_DrawCities()
// // ; draws cities into the current draw segment based on
// // ; the specified map parameters, unless the tile they
// // ; are on is unscouted
// // ; contains 2 BUGs: non-walled cities use the wrong
// // ; graphics due to an outdated value check, and cities
// // ; at a relative X coordinate of -1 don't get their
// // ; image flowing into the screen as they should

// push    [bp+map_plane]                  ; Plane
// push    _DI_cur_map_ypos                ; YPos
// push    _SI_cur_map_xpos                ; XPos
// push    [bp+map_height]                     ; map_height
// push    [bp+map_width]                     ; map_width
// push    [bp+y]                          ; Top
// push    [bp+x]                          ; Left
// OVL_DrawTowers()
// // ; draws towers of wizardry into the current draw
// // ; segment based on the specified map parameters, unless
// // ; the tile they are on is unscouted
// // ; BUG: fails to color the towers according to their
// // ; last controller as originally intended

// push    [bp+map_plane]                  ; Plane
// push    _DI_cur_map_ypos                ; YPos
// push    _SI_cur_map_xpos                ; XPos
// push    [bp+map_height]                     ; map_height
// push    [bp+map_width]                     ; map_width
// push    [bp+y]                          ; Top
// push    [bp+x]                          ; Left
// OVL_DrawEncounters
// // ; draws encounter zones into the current draw segment
// // ; based on the specified map parameters, unless the
// // ; tile they are on is unscouted; skips all nodes and
// // ; towers of wizardry (which are drawn separately)

// push    _DI_cur_map_ypos                ; YPos
// push    _SI_cur_map_xpos                ; XPos
// push    [bp+map_height]                     ; map_height
// push    [bp+map_width]                     ; map_width
// push    [bp+y]                          ; Top
// push    [bp+x]                          ; Left
// OVL_DrawUnits()
// // ; draws unit cards into the current draw segment from
// // ; OVL_UnitsOnMap based on the specified map parameters
// ? MoO2 Draw_Ship_Icons_ ?

// push    [bp+map_plane]                  ; Plane
// push    _DI_cur_map_ypos                ; YPos
// push    _SI_cur_map_xpos                ; XPos
// push    [bp+map_height]                     ; map_height
// push    [bp+map_width]                     ; map_width
// push    [bp+y]                          ; Top
// push    [bp+x]                          ; Left
// OVL_DrawNodeFX()
// // ; draws node effects into the current draw segment
// // ; based on the specified map parameters, unless the
// // ; tile they are on is unscouted

// push    [bp+map_plane]                  ; Plane
// push    _DI_cur_map_ypos                ; YPos
// push    _SI_cur_map_xpos                ; XPos
// push    [bp+map_height]                     ; map_height
// push    [bp+map_width]                     ; map_width
// push    [bp+y]                          ; Top
// push    [bp+x]                          ; Left
// OVL_DrawScouting()
// // ; draws the black edges of partially-scouted tiles into
// // ; the current draw segment based on the specified map
// // ; parameters

// OVL_StepMapAnims()
// // ; on every second call, will advance the animation
// // ; stages of the overland map display (tiles, node
// // ; sparkles, and enchanted roads)

#ifdef STU_DEBUG
dbg_prn("DEBUG: [%s, %d]: END: Draw_Map_Window(screen_x = %d, screen_y = %d, map_w = %d, map_h = %d, map_x = %d, map_y = %d, map_p = %d)\n", __FILE__, __LINE__, screen_x, screen_y, map_w, map_h, map_x, map_y, map_p);
#endif

}



// WZD o150p05
void TST_Draw_Map_Terrain(int16_t x, int16_t y, int16_t map_width, int16_t map_height, int16_t map_xpos, int16_t map_ypos, int16_t map_plane)
{
    byte_ptr terrain_pict_seg;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: TST_Draw_Map_Terrain(x = %d, y = %d, h = %d, w = %d, map_x = %d, map_y = %d, map_plane = %d)\n", __FILE__, __LINE__, x, y, map_width, map_height, map_xpos, map_ypos, map_plane);
#endif
    /*
        Image, if you will...
        terrain_type_idx = _world_maps[0] == 0x00
        terrain_001_0 = terrain_lbx_001[terrain_type_idx] == 0x00
        terrain_001_1 = terrain_lbx_001[terrain_type_idx] == 0x02

    */
    // terrrain_type_idx = 0; // ocean
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] terrain_lbx_000: %p\n", __FILE__, __LINE__, terrain_lbx_000);
#endif
    // terrain_pict_seg = terrain_lbx_000 + (2 * 384);
    // // TERRAIN.LBX, offset 0x0300  768  (2 * 0x0180  384)  sizeof terrain image
    terrain_pict_seg = terrain_lbx_000 + 0xC0;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] terrain_pict_seg: %p\n", __FILE__, __LINE__, terrain_pict_seg);
#endif
    Draw_Picture(x, y, terrain_pict_seg);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: TST_Draw_Map_Terrain(x = %d, y = %d, h = %d, w = %d, map_x = %d, map_y = %d, map_plane = %d)\n", __FILE__, __LINE__, x, y, map_width, map_height, map_xpos, map_ypos, map_plane);
#endif
}


// WZD o150p05
void Draw_Map_Terrain(int16_t x, int16_t y, int16_t map_width, int16_t map_height, int16_t map_xpos, int16_t map_ypos, int16_t map_plane)
{
    int16_t terrain_tile_base;
    int16_t itr_map_ypos;
    int16_t itr_map_xpos;
    int16_t _SI_y;
    int16_t _DI_x;
    int16_t DrawTile_X;
    // int16_t terrain_001_index;
    uint16_t terrain_001_index;
    uint16_t world_map_value;
    uint8_t terrain_001_0;
    uint8_t terrain_001_1;
    byte_ptr terrain_pict_seg;
    uint16_t * world_maps_ptr;
    int16_t world_maps_offset;
    // uint16_t terrain_lbx_000_offset;
    uint32_t terrain_lbx_000_offset;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Map_Terrain(x = %d, y = %d, h = %d, w = %d, map_x = %d, map_y = %d, map_plane = %d)\n", __FILE__, __LINE__, x, y, map_width, map_height, map_xpos, map_ypos, map_plane);
#endif

    // DLOG("DRAW_THE_ONE");
    // terrain_pict_seg = terrain_lbx_000 + 0xC0;
    // Draw_Picture(x, y, terrain_pict_seg);



    if(map_plane == 0)
    {
//         DLOG("(map_plane == 0)");
        terrain_tile_base = 0;
    }
    else
    {
//         DLOG("(map_plane != 0)");
        terrain_tile_base = 762; // 0x2FA
    }

// mov     [bp+Current_Logical_Page], -1

    _SI_y = y;
    itr_map_ypos = map_ypos;
    while(itr_map_ypos < (map_ypos + map_height))
    {
        _DI_x = x;
        itr_map_xpos = map_xpos;
        while(itr_map_xpos < (map_xpos + map_width))
        {
            if(itr_map_xpos >= 60)
            {
                DrawTile_X = itr_map_xpos - 60;
            }
            else
            {
                DrawTile_X = itr_map_xpos;
            }

            /*
                if NOT explored, fill black
            */
            // if(TBL_Scouting[(map_plane * 2400) + (itr_map_ypos * 60) + (DrawTile_X)] == ST_FALSE)
            // {
            //     Fill(_DI_x, _SI_y, (_DI_x + 19), (_SI_y + 17), 0);
            //     continue;
            // }

            // // world_map_base = map_plane * SZ_WORLD_MAP;  // {0 * 2400, 1 * 2400}
            // terrain_001_index = _world_maps[(map_plane * 4800) + (itr_map_ypos * 120) + (DrawTile_X * 2)];
            // terrain_001_index += terrain_tile_base;

            // world_maps_offset = 0;
            // world_maps_offset += (world_z * SA_WORLD_PLANE);
            // world_maps_offset += (world_y * SZ_WORLD_WIDTH);
            // world_maps_offset += world_x

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: _world_maps: %p\n", __FILE__, __LINE__, _world_maps);
// #endif
            // world_maps_ptr = (int16_t *)_world_maps;
            world_maps_ptr = (uint16_t *)_world_maps;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: world_maps_ptr: %p\n", __FILE__, __LINE__, world_maps_ptr);
// #endif
            // terrain_001_index = *(world_map_ptr + (map_plane * 2400) + (itr_map_ypos * 60) + (DrawTile_X));
            // world_maps_offset = ((map_plane * 2400) + (itr_map_ypos * 60) + (DrawTile_X));
            world_maps_offset = ((map_plane * 4800) + (itr_map_ypos * 120) + (DrawTile_X * 2));

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: world_maps_offset: %04X  %d\n", __FILE__, __LINE__, world_maps_offset, world_maps_offset);
// #endif
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(_world_maps,world_maps_offset): %04X\n", __FILE__, __LINE__, GET_2B_OFS(_world_maps,world_maps_offset));
// #endif
            world_map_value = GET_2B_OFS(_world_maps,world_maps_offset);
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: world_map_value: %04X\n", __FILE__, __LINE__, world_map_value);
// #endif
            // // terrain_001_index = *(world_maps_ptr + world_maps_offset);
            // // terrain_001_index = world_maps_ptr[world_maps_offset];
            // terrain_001_index = (uint16_t)*(world_maps_ptr + world_maps_offset);
            terrain_001_index = GET_2B_OFS(_world_maps,world_maps_offset);
// #ifdef STU_DEBUG
//     // dbg_prn("DEBUG: [%s, %d]: terrain_001_index: %d\n", __FILE__, __LINE__, terrain_001_index);
//     dbg_prn("DEBUG: [%s, %d]: terrain_001_index: %u\n", __FILE__, __LINE__, terrain_001_index);
//     dbg_prn("DEBUG: [%s, %d]: terrain_001_index: %04X\n", __FILE__, __LINE__, terrain_001_index);
// #endif
            terrain_001_index += terrain_tile_base;
            terrain_001_index *= 2;  // because, sizeof(int16_t)
// #ifdef STU_DEBUG
//     // dbg_prn("DEBUG: [%s, %d]: terrain_001_index: %d\n", __FILE__, __LINE__, terrain_001_index);
//     dbg_prn("DEBUG: [%s, %d]: terrain_001_index: %u\n", __FILE__, __LINE__, terrain_001_index);
//     dbg_prn("DEBUG: [%s, %d]: terrain_001_index: %04X\n", __FILE__, __LINE__, terrain_001_index);
// #endif

// WZD dseg:CC22 gfp_TER_TileTypeOffsets@ dd 0           ; single-loaded full entry
// LBX_Terrain_Init()
//     gfp_TER_TileTypeOffsets = LBX_Load("TERRAIN.LBX", 1)
            terrain_001_0 = 0;
            // // terrain_001_0 = terrain_lbx_001[terrain_001_index + 0];
            // // terrain_001_1 = terrain_lbx_001[terrain_001_index + 1];
            // terrain_001_0 = terrain_lbx_001[world_map_value + 0];
            // terrain_001_1 = terrain_lbx_001[world_map_value + 1];
            terrain_001_0 = GET_1B_OFS(terrain_lbx_001, terrain_001_index + 0);
            terrain_001_1 = GET_1B_OFS(terrain_lbx_001, terrain_001_index + 1);

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: terrain_001_0: %02X\n", __FILE__, __LINE__, terrain_001_0);
//     dbg_prn("DEBUG: [%s, %d]: terrain_001_1: %02X\n", __FILE__, __LINE__, terrain_001_1);
// #endif





            if(terrain_001_0 & 0x80)
            {
//                 DLOG("(terrain_001_0 & 0x80)");
                terrain_001_0 = terrain_001_0 & 0x7F;
            }
            if(map_draw_full != ST_FALSE)
            {
//                 DLOG("map_draw_full != ST_FALSE)");
            }
// //             // if animated and not new, skip draw
// //             if(terrain_001_0 & 0x80 == 0 || OVL_NewMapDrawing != ST_FALSE)
// //             {
// //                 // TODO  if(terrain_001_0 & 0x80 == 0)
// //                 // TODO  {
// //                 // TODO      // WZD dseg:6FF4 OVL_TileAnim_Stage dw 0
// //                 // TODO      terrain_001_1 += map_animation_frame;  // AKA OVL_TileAnim_Stage
// //                 // TODO      terrain_001_0 = (terrain_001_0 & 0x7F);
// //                 // TODO  }
// // 
// // //     // mov     ax, [terrain_001_1]
// // //     // mov     bx, ax
// // //     // shl     ax, 1
// // //     // add     ax, bx
// // //     // shl     ax, 1
// // //     // shl     ax, 1
// // //     // shl     ax, 1                           ; * 180h, from FILE start, not entry start!
// // //     // add     ax, [EMM_PageFrame]             ; contains the segment address of the EMS page frame
// // //     // mov     [bp+terrain_pict_seg], ax
// // // 
// // //     if(terrain_001_0 != terrain_000_elpn)
// // //     {
// // //         EMM_Map4Pages(Image_Page, g_EmmHndl_TERRAIN);
// // //     }
// // 
// //                 // terrain_pict_seg = terrain_lbx_000 + (terrain_001_1 * 384);
// //                 terrain_pict_seg = terrain_lbx_000 + 0xC0;
// // #ifdef STU_DEBUG
// //     dbg_prn("DEBUG: [%s, %d] terrain_pict_seg: %p\n", __FILE__, __LINE__, terrain_pict_seg);
// // #endif
// //                 Draw_Picture(_DI_x, _SI_y, terrain_pict_seg);
// // 
// //                 Draw_Picture(0, (7 * 18), terrain_pict_seg);
// // 
// // 
// //             }
                // // // // terrain_pict_seg = terrain_lbx_000 + 0xC0 + (terrain_001_1 * 384);
                // // // // // terrain_pict_seg = terrain_lbx_000 + 0xC0 + ((terrain_001_1 - 2) * 384);
                // // // terrain_pict_seg = terrain_lbx_000 + 0xC0 + (terrain_001_0 * 16384) + terrain_001_1;
                // // if(terrain_001_0 == 0)
                // // {
                // //     // map index in terrain_001_1 to TERRAIN.LBX Entry 0, instead of TERRAIN.LBX entire file in EMM
                // //     // 0xC0 + ((terrain_001_1 - 2) * 384)
                // //     terrain_pict_seg = terrain_lbx_000 + 0xC0 + ((terrain_001_1 - 2) * 384);
                // // }
                // // loading entry 0, instead of whole file
                // // so
                // //     EMM Page * 16K
                // //     plus terrain index * 384
                // //     minus difference in header size
                // // terrain_pict_seg = terrain_lbx_000 + ( (terrain_001_0 * 16384) + (terrain_001_1 * 384) - 0xC0);
                // // terrain_lbx_000_offset = (terrain_001_0 * 16384) + (terrain_001_1 * 384) - 0xC0;
                if(terrain_001_0 == 0)
                {
                    terrain_lbx_000_offset = 0xC0 + ((terrain_001_1 - 2) * 384);
                }
                else
                {
                    terrain_lbx_000_offset = (terrain_001_0 * 16384) + (terrain_001_1 * 384) - 0xC0 - 384;
                }

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: terrain_lbx_000_offset: %u\n", __FILE__, __LINE__, terrain_lbx_000_offset);
//     dbg_prn("DEBUG: [%s, %d]: terrain_lbx_000_offset: %04X\n", __FILE__, __LINE__, terrain_lbx_000_offset);
// #endif

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: terrain_id: %u\n", __FILE__, __LINE__, (((terrain_001_0 / 3) * 127) + terrain_001_1));
//     dbg_prn("DEBUG: [%s, %d]: terrain_id: %04X\n", __FILE__, __LINE__, (((terrain_001_0 / 3) * 127) + terrain_001_1));
// #endif


                terrain_pict_seg = terrain_lbx_000 + terrain_lbx_000_offset;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: terrain_pict_seg: %p\n", __FILE__, __LINE__, terrain_pict_seg);
// #endif
                Draw_Picture(_DI_x, _SI_y, terrain_pict_seg);





// //         if(DBG_ShowTileInfo != ST_FALSE)
// //         {
// //             j_EMM_Map_CONTXXX()
// //             // ; maps in the EMM_ContXXX_H handle (all 4 pages), and
// //             // ; resets its corresponding global pointers
// //             Set_Font(0,0);
// //             VGA_DrawNumber(_DI_x, _SI_y + 12, TBL_Landmasses[(_map_plane * 2400) + (itr_map_ypos * 60) + DrawTile_X])
// //             Set_Font(0, 2);
// //             VGA_DrawNumber(_DI_x, _SI_y, DrawTile_X)
// //         }

            _DI_x += 20;  // terrain image width
            itr_map_xpos++;
        }  /* while(itr_map_xpos < (map_xpos + map_width)) */

        _SI_y += 18;  // terrain image height
        itr_map_ypos++;
    }  /* while(itr_map_ypos < (map_ypos + map_height)) */

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_Map_Terrain(x = %d, y = %d, h = %d, w = %d, map_x = %d, map_y = %d, map_plane = %d)\n", __FILE__, __LINE__, x, y, map_width, map_height, map_xpos, map_ypos, map_plane);
#endif

}

// WZD o150p06
// Map Anims

// WZD o150p07
// Draw Unexplored

// WZD o150p08
// Draw Cities

// WZD o150p09
// Draw Towers

// WZD o150p10
// Draw Encounters

// WZD o150p11
// Draw Node FX

// WZD o150p12
// Draw Terrain Specials

// WZD o150p13
// Draw Minerls

// WZD o150p14
// Draw Roads

// WZD o150p15
// Draw Units/Stacks

// WZD o150p16
/*
Draw_Maps()

xor     ax, ax
push    ax                              ; Mark
xor     ax, ax
push    ax                              ; Mark_Y
xor     ax, ax
push    ax                              ; Mark_X
push    [minimap_height]                ; Height
push    [minimap_width]                 ; Width
push    [_reduced_map_seg]              ; IMG_Seg
push    [bp+world_plane]                  ; Plane
push    [minimap_y]                     ; Map_Y
push    [minimap_x]                     ; Map_X
Draw_Minimap
; draws a mini map (1 pixel per tile) into the passed
; image segment, matching its top left corner to the
; map coordinates specified, and displaying tiles that
; are out of bounds or unexplored with color index $01;
; if a mark is specified that will use color index $0F

minimap_x & y are the calculated coordinates of the top-left corner
world_plane is [0,1] [Arcanus,Myrror], as passed in from Main_Screen_Draw_Do_Draw()
*/
void Draw_Minimap(int16_t minimap_start_x, int16_t minimap_start_y, int16_t world_plane, byte_ptr minimap_pict_seg, int16_t minimap_width, int16_t minimap_height, int16_t Mark_X, int16_t Mark_Y, int16_t Mark)
{
    int16_t terrain_type_idx_base;
    int16_t terrain_type_idx;
    uint8_t * explore_data_ptr;
    // uint16_t * world_data_ptr;
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
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Minimap(minimap_start_x = %d, minimap_start_y = %d, world_plane = %d, minimap_pict_seg = %p, minimap_width = %d, minimap_height = %d, Mark_X = %d, Mark_Y = %d, Mark = %d)\n", __FILE__, __LINE__, minimap_start_x, minimap_start_y, world_plane, minimap_pict_seg, minimap_width, minimap_height, Mark_X, Mark_Y, Mark);
#endif


    terrain_type_idx_base = world_plane * TERRAIN_COUNT;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: TBL_Scouting: %p\n", __FILE__, __LINE__, TBL_Scouting);
// #endif
    // explore_data_ptr = (uint8_t *)TBL_Scouting[(world_plane * 2400)];
    explore_data_ptr = (TBL_Scouting + (world_plane * 2400));
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: explore_data_ptr: %p\n", __FILE__, __LINE__, explore_data_ptr);
// #endif

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: _world_maps: %p\n", __FILE__, __LINE__, _world_maps);
// #endif
    // world_data_ptr = (uint16_t *)_world_maps[(world_plane * 4800)];
    world_data_ptr = (_world_maps + (world_plane * 4800));
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: world_data_ptr: %p\n", __FILE__, __LINE__, world_data_ptr);
// #endif

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: minimap_pict_seg: %p\n", __FILE__, __LINE__, minimap_pict_seg);
// #endif
    minimap_pict_data_ptr = minimap_pict_seg + 16;  // +1 paragraph
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: minimap_pict_data_ptr: %p\n", __FILE__, __LINE__, minimap_pict_data_ptr);
// #endif

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

    
    for(itr_minimap_height = 0; itr_minimap_height < minimap_height; itr_minimap_height++)
    {
        for(itr_minimap_width = 0; itr_minimap_width < minimap_width; itr_minimap_width++)
        {

            minimap_square_x = minimap_start_x + itr_minimap_width;
            minimap_square_y = minimap_start_y + itr_minimap_height;
            if(minimap_square_x < 0) { minimap_square_x += WORLD_WIDTH; }
            if(minimap_square_x > WORLD_WIDTH) { minimap_square_x -= WORLD_WIDTH; }

            // the reduced map is centered relative to the movement map, so skip until we get to the start of the world
            if(minimap_square_y < 0 || minimap_square_y >= WORLD_HEIGHT)
            {
                continue;
            }

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: ((minimap_square_y * WORLD_WIDTH) + minimap_square_x): %d\n", __FILE__, __LINE__, ((minimap_square_y * WORLD_WIDTH) + minimap_square_x));
// #endif

//             explored_flag = explore_data_ptr[((minimap_square_y * WORLD_WIDTH) + minimap_square_x)];
// 
//             // map square is explored
//             if(explored_flag == ST_FALSE)
//             {
//                 continue;
//             }

            // terrain_type_idx = *(world_data_ptr + (minimap_square_y * 120) + (minimap_square_x * 2));
            terrain_type_idx = GET_2B_OFS(world_data_ptr, ((minimap_square_y * 120) + (minimap_square_x * 2)));
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: terrain_type_idx: %d\n", __FILE__, __LINE__, terrain_type_idx);
// #endif
            terrain_type_idx += terrain_type_idx_base;
            minimap_terrain_color = terrain_lbx_002[terrain_type_idx];

            *(minimap_pict_data_ptr + ((itr_minimap_width * minimap_height) + itr_minimap_height)) = minimap_terrain_color;

        }
    }

    /*
        BEGIN: Loop Cities
    */
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {

//         // city plane vs. world plane
//         if(_CITIES[itr_cities].world_plane != world_plane)
//         {
//             continue;
//         }
// 
//         city_world_x = _CITIES[itr_cities].world_x;
//         city_world_y = _CITIES[itr_cities].world_y;
// 
// //         explored_flag = explore_data_ptr[(city_world_y * WORLD_WIDTH + city_world_x)];
// // 
// //         // map square is explored / city is visible
// //         if(explored_flag == ST_FALSE)
// //         {
// //             continue;
// //         }
// 
//         // city coords in minimap dims
//         city_minimap_x = city_world_x - minimap_start_x;
//         if(city_minimap_x < 0) { city_minimap_x += WORLD_WIDTH; }
//         city_minimap_y = city_world_y - minimap_start_y;
//         if(city_minimap_x >= minimap_width || city_minimap_x < 0 || city_minimap_y >= minimap_height || city_minimap_y < 0)
//         {
//             continue;
//         }
// 
//         city_owner_idx = _CITIES[itr_cities].owner_idx;
// 
//         if(city_owner_idx != NEUTRAL_PLAYER_IDX)
//         {
// 
// //             city_owner_player_banner = _players[city_owner_idx].Banner;
// // #ifdef STU_DEBUG
// //     dbg_prn("DEBUG: [%s, %d]: city_owner_player_banner: %02X\n", __FILE__, __LINE__, city_owner_player_banner);
// // #endif
// //             banner_color = COL_MinimapBanners[_players[city_owner_idx].Banner];
// // #ifdef STU_DEBUG
// //     dbg_prn("DEBUG: [%s, %d]: banner_color: %02X\n", __FILE__, __LINE__, banner_color);
// // #endif
// 
//             *(minimap_pict_data_ptr + ((city_minimap_x * minimap_height) + city_minimap_y)) = COL_MinimapBanners[_players[city_owner_idx].Banner];
//         }
//         else
//         {
//             *(minimap_pict_data_ptr + ((city_minimap_x * minimap_height) + city_minimap_y)) = COL_MinimapNeutral;
//         }

    }
    /*
        END: Loop Cities
    */


//     if(Mark != 0)
//     {
//         mark_minimap_x = Mark_X - minimap_start_x;
//         mark_minimap_y = Mark_Y - minimap_start_y;
//         if(mark_minimap_x < 0) { mark_minimap_x += 60; }
//         if(mark_minimap_x > 60) { mark_minimap_x -= 60; }
//         if(mark_minimap_y < 0) { mark_minimap_y += 40; }
//         if(mark_minimap_y > 40) { mark_minimap_y -= 40; }
// 
//         *(minimap_pict_data_ptr + ((city_minimap_x * minimap_height) + city_minimap_y)) = MINIMAP_MARK_COLOR;
//     }

}


// WZD o150p17

// WZD o150p18
