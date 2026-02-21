MoX-Landmasses.md
SEEALSO: MoX-WorldMap.md
SEEALSO: MoX-MapGen.md



MAGIC.EXE  ovr051





_landmasses
_world_maps

m_landmasses_default_race
m_landmasses_ctr



// WZD dseg:9CD0
1-byte, unsigned
Allocate_Data_Space()  _landmasses = (uint8_t *)Allocate_Next_Block(World_Data, 302);  // 302 PR, 4832 B
uint8_t * _landmasses;

// WZD dseg:9CDC
Allocate_Data_Space()  _world_maps = (uint8_t *)Allocate_Next_Block(World_Data, 602);  // 602 PR, 9632 B
uint8_t * _world_maps;

// MGC  dseg:9040
set to 1 in Init_Landmasses(); incremented in TILE_SetLandMass();
int16_t m_landmasses_ctr;




map-generation continent construction



MAPGEN.c

Init_Landmasses()



// MGC o51p10
void NEWG_CreateLands__WIP(int16_t wp)


## NEWG_CreateLands__WIP()
sets _world_map to tt_Ocean1
clears the local scratch array
sets parameters
big loop on generated < desired


TILE_Cardinal_XMod2[]; TILE_Cardinal_YMod2[]; used to add the direction offset
// MGC  dseg:3350
int16_t TILE_Cardinal_XMod2[5] = { 0, -1, 0, 1, 0 };
int16_t TILE_Cardinal_YMod2[5] = { 1, 0, -1, 0, 0 };


### IDK,IDGI
    Origins_Remaining
    Section_Width
    Section_Height
    Used_Map_Sections[]





void TILE_SetLandMass__WIP(int16_t wp, int16_t wx, int16_t wy)
    int16_t Landmass_ID_Array[5] = { 0, 0, 0, 0, 0 };
    int16_t Conflicting_ID = 0;
    int16_t itr_wy = 0;
    int16_t itr_wx = 0;
    int16_t Next_Adjacent_Index = 0;
    int16_t Array_Index = 0;
    int16_t First_Found_Landmass = 0;
    int16_t IDK = 0;  // _CX_

Landmass_ID_Array
    Its intended role is a local scratch list of neighboring landmass IDs.
    In MAPGEN.c:2862, the function scans the center tile plus 8 neighbors and pushes each non-zero landmass ID into Landmass_ID_Array.
    That lets the routine:
        decide whether the current tile should join an existing landmass or start a new one, and
        detect conflicting adjacent IDs and merge them into one (the first found ID).
    So the array is basically the temporary set of adjacent continent labels used for consolidation logic.
