/*
    WIZARDS.EXE
        ovr067
        ovr068
        ovr150
*/

#ifndef MAINSCR_MAPS_H
#define MAINSCR_MAPS_H

#include "MoX.H"



// WZD dseg:7002                                                 END: ovr150
// WZD dseg:7002
// WZD dseg:7004                                                 ovr057
// WZD dseg:7004                                                 ovr150
// WZD dseg:7004
extern int16_t DBG_ShowTileInfo;

// WZD dseg:7006 00                                              db    0
// WZD dseg:7007 00                                              db    0
// WZD dseg:7008 00                                              db    0
// WZD dseg:7009 00                                              db    0


// WZD dseg:700A                                                 BEGIN: Draw_Minimap()

// // WZD dseg:700A
// uint8_t COL_MinimapBanners[6] = {171, 216, 205, 201, 210, 50};  // 0xAB, 0xD8, 0xCD, 0xC9, 0xD2, 0x32
// // WZD dseg:7010
// uint8_t COL_MinimapNeutral = 50;  // 0x32
// // END: Draw_Minimap()


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
 // PRIVATE uint8_t COL_City_Banner[];
 
// WZD dseg:7034 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+TBL_Warp_GFX_Lines db 14h dup(0)        ; DATA XREF: Draw_Map_Nodes+27Er ...
// WZD dseg:7034 00 00 00 00                                                                             ; 20 bytes randomized between -1, 0, or +1

// WZD dseg:7048  9B 9F        WarpNode_SeedSave_LO dw 9F9Bh
// WZD dseg:704A  0F 00        WarpNode_SeedSave_HO dw 0Fh
// WZD dseg:7048  9B 9F 0F 00
extern uint32_t WarpNode_SeedSave;

// WZD dseg:704A                                                 ? END:  ovr150 ?




// WZD dseg:974A
extern int16_t _prev_world_y;
// WZD dseg:974C
extern int16_t _prev_world_x;



// WZD dseg:9CA8
extern SAMB_ptr terrain_lbx_002;
// array of color indecies by tile type for minimap / "reduced map"


// WZD dseg:CC22
extern SAMB_ptr terrain_lbx_001;            // load in Terrain_Init() ovr052
// WZD dseg:CC26
extern byte_ptr terrain_001_1;              // load in Terrain_Init() ovr052
// WZD dseg:CC28
// g_EmmHndl_TERRAIN dw 0  ; EMM_Load_LBX handle
extern SAMB_ptr terrain_lbx_000;            // load in Terrain_Init() ovr052



//                                          ¿ BEGIN: ~ Reduced/World Map  - Uninitialized Data ?

// WZD dseg:C11C
extern int16_t minimap_height;
extern int16_t reduced_map_height;

// WZD dseg:C11E
extern int16_t minimap_width;
extern int16_t reduced_map_width;

// WZD dseg:C120
extern int16_t minimap_y;
extern int16_t reduced_map_y;

// WZD dseg:C122
extern int16_t minimap_x;
extern int16_t reduced_map_x;

// WZD dseg:C124 00 00                                           dw 0

// WZD dseg:C126
extern int16_t reduced_map_mark_cycle;

//                                          ¿ END: ~ Reduced/World Map  - Uninitialized Data ?



//                                          ovr150
// WZD dseg : CB56 00 00                                           OVL_MapDrawY_Save dw 0; DATA XREF : OVL_MapStateSave + Cw ...
// WZD dseg:CB58 00 00                                           OVL_MapDrawX_Save dw 0; DATA XREF : OVL_MapStateSave + 6w ...
// WZD dseg:CB5A 00 00                                           OVL_NewMapSust_Save dw 0; DATA XREF : OVL_MapStateSave + 12w ...
// WZD dseg:CB5C
int16_t map_draw_full;





#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    WIZARDS.EXE  ovr067
*/

// WZD o67p01
/*
; draws the visible sections of the overland into the
; current draw segment, and the minimap into the
; Minimap_IMG_Seg allocation, according to the passed
; parameters:
;   Top/Left - main display pixel coordinates
;   H/VTiles - horizontal/vertical window size in tiles
;   XPos/YPos/Plane - top left map coordinates
;   MapX@/MapY@ - return values to set
; resets the draw window after finishing
*/
void Draw_Maps(int16_t x, int16_t y, int16_t HTiles, int16_t VTiles, int16_t *MapX, int16_t *MapY, int16_t Plane, int16_t XPos, int16_t YPos, int16_t player_idx);

// WZD o67p0
// AKA IDK_CheckSet_MapDisplay_XY
void IDK_CheckSet_MapDisplay_XY(void);

// WZD o67p04
// AKA OVL_ShowActiveStack
void Set_Draw_Active_Stack_Always(void);

// WZD o67p05
// AKA OVL_HideActiveStack
void Set_Draw_Active_Stack_Never(void);

// WZD o67p06
// Reset_Active_Stack_Draw
void Reset_Draw_Active_Stack(void);

// WZD o67p07
// AKA OVL_DrawActiveStack
void Draw_Active_Unit_Stack(int16_t world_x, int16_t world_y, int16_t world_plane);

// WZD o67p08
// AKA Minimap_Set_Dims()
void Reduced_Map_Set_Dims(int16_t width, int16_t height);

// WZD o67p09
// "Reduced Map"/"World Window"
// AKA Draw_Minimap_Window()
void Draw_World_Window(int16_t start_x, int16_t start_y, int16_t width, int16_t height);

// WZD o67p10
void Set_Entities_On_Map_Window(int16_t world_x, int16_t world_y, int16_t world_plane);

// WZD o67p13
/*
    updates draw priority on ALL units in _UNITS
*/
void Set_Unit_Draw_Priority(void);

// WZD o67p14
// drake178: STK_NoUnitDraw
void Reset_Stack_Draw_Priority(void);


/*
    WIZARDS.EXE  ovr068
*/

// WZD o68p01
// void OVL_Map_Init(int16_t * map_x, int16_t map_y, int16_t xpos, int16_t ypos, int16_t plane);
void Center_Map(int16_t * map_x, int16_t * map_y, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane);

// WZD o68p02
void City_Center_Map(int16_t * map_x, int16_t * map_y, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane);

// WZD o68p03
// drake178: OVL_GetMinimapStart()
// AKA Minimap_Coords()
void Reduced_Map_Coords(int16_t * minimap_x, int16_t * minimap_y, int16_t mid_x, int16_t mid_y, int16_t minimap_width, int16_t minimap_height);

// WZD o68p04
// drake178: OVL_RedrawScouting()
void Redraw_Map_Unexplored_Area(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane);

// WZD o68p05
// RP_OVL_DrawCities2()

// WZD o68p06
void TILE_Explore(int16_t wx, int16_t wy, int16_t wp);

// WZD o68p07
// TILE_ExploreCorners()

// WZD o68p08
// RP_TILE_ExploreMatch()

// WZD o68p09
void List_Screen_Draw_Reduced_Map(int16_t x, int16_t y, int16_t w, int16_t h, int16_t wp, int16_t wx, int16_t wy);



/*
    WIZARDS.EXE  ovr150
*/

// WZD o150p01
// AKA Undef_Prev_Map_Draw_XY()
void Reset_Map_Draw(void);

// WZD o150p02 OVL_MapStateSave          
// WZD o150p03 OVL_MapStateRestore       

// WZD o150p04
void Draw_Map_Window(int16_t screen_x, int16_t screen_y, int16_t map_w, int16_t map_h, int16_t map_x, int16_t map_y, int16_t map_p);

// WZD o150p05
void TST_Draw_Map_Terrain(int16_t x, int16_t y, int16_t HTiles, int16_t VTiles, int16_t map_xpos, int16_t map_ypos, int16_t map_plane);
void Draw_Map_Terrain(int16_t x, int16_t y, int16_t HTiles, int16_t VTiles, int16_t map_xpos, int16_t map_ypos, int16_t map_plane);

// WZD o150p06
void Cycle_Map_Animations(void);
// WZD o150p07
void Draw_Map_Unexplored_Area(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane);
// WZD o150p08
void Draw_Map_Cities(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane);
// WZD o150p09
void Draw_Map_Towers(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane);
// WZD o150p10
void Draw_Map_Lairs(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane);
// WZD o150p11
void Draw_Map_Nodes(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane);

// WZD o150p12
/*
    draws Terrain Specials - Biota - Wild Game, Hunters Lodge, Nightshade
*/
void Draw_Map_Biota(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane);

// WZD o150p13
/*
    draws Terrain Specials - Minerals
*/
void Draw_Map_Minerals(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane);

// WZD o150p14
void Draw_Map_Roads(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane);

// WZD o150p15
void Draw_Map_Units(int16_t screen_x, int16_t screen_y, int16_t map_grid_width, int16_t map_grid_height, int16_t world_grid_x, int16_t world_grid_y);

// WZD o150p16
// AKA Draw_Minimap()
// void Draw_Minimap(int16_t minimap_start_x, int16_t minimap_start_y, int16_t world_plane, byte_ptr minimap_pict_seg, int16_t minimap_width, int16_t minimap_height, int16_t Mark_X, int16_t Mark_Y, int16_t Mark);
void Draw_Reduced_Map(int16_t minimap_start_x, int16_t minimap_start_y, int16_t world_plane, byte_ptr minimap_pict_seg, int16_t minimap_width, int16_t minimap_height, int16_t Mark_X, int16_t Mark_Y, int16_t Mark);

// WZD o150p17 IDK_Prepare_TileMap_sDAB1A
// WZD o150p18 sub_DAFF4                 


#ifdef __cplusplus
}
#endif


#endif  /* MAINSCR_MAPS_H */
