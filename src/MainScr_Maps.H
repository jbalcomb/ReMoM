#ifndef MAINSCR_MAPS_H
#define MAINSCR_MAPS_H

#include "MoX_TYPE.H"



#define WORLD_WIDTH 60
#define WORLD_HEIGHT 40

#define MAP_WIDTH 12
#define MAP_HEIGHT 10
#define MAP_WIDTH_HALF 6
#define MAP_HEIGHT_HALF 5

#define TILE_WIDTH 20
#define TILE_HEIGHT 18

#define _MAP_MAX_X 60
#define _MAP_MAX_Y 40

#define TERRAIN_COUNT 762



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
extern char terrain_lbx_file[];
// dseg:29DE terrstat_lbx_file db 'TERRSTAT',0
// WZD dseg:29E7
extern char mapback_lbx_file[];
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



// // BEGIN: Draw_Minimap()
// // WZD dseg:700A
// uint8_t COL_MinimapBanners[6] = {171, 216, 205, 201, 210, 50};  // 0xAB, 0xD8, 0xCD, 0xC9, 0xD2, 0x32
// // WZD dseg:7010
// uint8_t COL_MinimapNeutral = 50;  // 0x32
// // END: Draw_Minimap()



// WZD dseg:974A
extern int16_t _prev_world_y;
// WZD dseg:974C
extern int16_t _prev_world_x;



// WZD dseg:9CA8
extern SAMB_ptr terrain_lbx_002;
// array of color indecies by tile type for minimap / "reduced map"


// WZD dseg:CC22
extern SAMB_ptr terrain_lbx_001;
// WZD dseg:CC26
extern byte_ptr terrain_001_1;
// WZD dseg:CC28
// g_EmmHndl_TERRAIN dw 0                  ; EMM_Load_LBX handle
extern SAMB_ptr terrain_lbx_000;



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

// WZD o67p08
void Minimap_Set_Dims(int16_t width, int16_t height);

// WZD o67p09
void Draw_Minimap_Window(int16_t start_x, int16_t start_y, int16_t width, int16_t height);


/*
    WIZARDS.EXE  ovr068
*/

// WZD o68p01
void OVL_Map_Init(int16_t * map_x, int16_t map_y, int16_t xpos, int16_t ypos, int16_t plane);


// WZD o68p03
void Minimap_Coords(int16_t * minimap_x, int16_t * minimap_y, int16_t mid_x, int16_t mid_y, int16_t minimap_width, int16_t minimap_height);


/*
    WIZARDS.EXE  ovr150
*/

// WZD o150p04
void Draw_Map_Window(int16_t x, int16_t y, int16_t HTiles, int16_t VTiles, int16_t map_xpos, int16_t map_ypos, int16_t map_plane);

// WZD o150p05
void TST_Draw_Map_Terrain(int16_t x, int16_t y, int16_t HTiles, int16_t VTiles, int16_t map_xpos, int16_t map_ypos, int16_t map_plane);
void Draw_Map_Terrain(int16_t x, int16_t y, int16_t HTiles, int16_t VTiles, int16_t map_xpos, int16_t map_ypos, int16_t map_plane);

// WZD o150p16
void Draw_Minimap(int16_t minimap_start_x, int16_t minimap_start_y, int16_t world_plane, byte_ptr minimap_pict_seg, int16_t minimap_width, int16_t minimap_height, int16_t Mark_X, int16_t Mark_Y, int16_t Mark);



#ifdef __cplusplus
}
#endif


#endif  /* MAINSCR_MAPS_H */
