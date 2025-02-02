




Terrain Movement Points Per Square  
Cities          1/2  
Hills           2  
Rivers          2  
Roads           1/2  
Rough (Dirt)    2  
Tree            2  





BaseTile_X  ==> IDK_base_cgc2
BaseTile_Y  ==> IDK_base_cgc1
Tile_X      ==> cgc2  ==> cgx
Tile_Y      ==> cgc1  ==> cgy

Origin_X        ==> draw_x
Origin_Y        ==> draw_y
OriginX         ==> draw_x
OriginY         ==> draw_y
OrigX           ==> draw_x
OrigY           ==> draw_y
figure_draw_x   ==> draw_x
figure_draw_y   ==> draw_y

ZDepth                  ==> draw_order_value
CMB_ZSort_Array         ==>  combat_entity_draw_order_array
CMB_ZSort_Insert__WIP() ==> Set_Entity_Draw_Order()

CMB_GetInTileX__WIP()   ==> Screen_To_Combat_Grid_Cell_X_And_Offset()
CMB_GetInTileY__WIP()   ==> Screen_To_Combat_Grid_Cell_Y_And_Offset()
Tile_X                  ==> combat_grid_cell_x
InTile_X                ==> combat_grid_cell_x_offset
Tile_Y                  ==> combat_grid_cell_y
InTile_Y                ==> combat_grid_cell_y_offset

CMB_GetTileX()  ==> Get_Combat_Grid_Cell_X()
CMB_GetTileY()  ==> Get_Combat_Grid_Cell_Y()

CMB_TerrPatchGen() ==> CMB_TerrPatchGen__WIP() ==> Patch_Terrain_Group()
CMB_Terrain_Init() ==> CMB_Terrain_Init__WIP() ==> 

IMG_CMB_TerrTiles  ==> _combat_terrain_pict_segs
IMG_CMB_Trees  ==> 
IMG_CMB_Rocks  ==> 

TerrGroup_UpLeft     ==>  ctg_nw  ctg_7
TerrGroup_Up         ==>  ctg_no  ctg_8
TerrGroup_UpRight    ==>  ctg_ne  ctg_9
TerrGroup_Left       ==>  ctg_we  ctg_4
TerrGroup_Middle     ==>  ctg_??  ctg_5
TerrGroup_Right      ==>  ctg_ea  ctg_6
TerrGroup_DownLeft   ==>  ctg_sw  ctg_1
TerrGroup_Down       ==>  ctg_so  ctg_2
TerrGroup_DownRight  ==>  ctg_se  ctg_3

battlefield->Tile_Terrain   ==> terrain_type
battlefield->Tile_TerrGroup ==> terrain_group





¿ scanned combat grid cell outline ?
¿ combat grid cell scanned ?


scanned_battle_unit_idx = CMB_TargetRows[Tile_Y][Tile_X];



## DEBUG

combat map/grid terrain?
Where?
How?

Load_Combat_Terrain_Pictures()
    loads set of 48 terrain pictures, 5 trees, 5 rocks

Tactical_Combat_Draw()
    |-> CMB_DrawMap__WIP()
        battlefield_terrain = battlefield->Tile_Terrain[((cgy * COMBAT_GRID_WIDTH) + cgx)];
        ONLY handles drawing *special* terrain types  (battlefield_terrain >= 48)

Tactical_Combat__WIP()
    |-> CMB_Terrain_Init__WIP(wx, wy, wp);

CMB_Terrain_Init__WIP()
    ...
    |-> Load_Combat_Terrain_Pictures(Terrain_Type, wp);
    |-> Generate_Combat_Map__WIP(Location_Type, City_House_Type, &Road_Matrix[0], Enchanted_Roads, Terrain_Type, &River_Matrix[0], Flying_Fortress, ATKR_FloatingIsland, DEFR_FloatingIsland, City_Walls, City_Population, Magic_Walls);
    |-> CMB_ComposeBackgrnd__WIP()

CMB_ComposeBackgrnd__WIP()
    combat_terrain_type = battlefield->Tile_Terrain[((cgc1 * COMBAT_GRID_WIDTH) + cgc2)];

#define TERRAIN_TYPE(_wx_, _wy_, _wp_)        ( GET_2B_OFS(_world_maps, (((_wp_) * WORLD_SIZE * 2) + ((_wy_) * WORLD_WIDTH * 2) + ((_wx_) * 2))) % NUM_TERRAIN_TYPES )
int16_t Square_Is_Forest(int16_t wx, int16_t wy, int16_t wp)
terrain_type = TERRAIN_TYPE(wx, wy, wp);
            (terrain_type == tt_Forest1)
            ||
            (terrain_type == tt_Forest2)
            ||
            (terrain_type == tt_Forest3)
            ||
            (terrain_type == tt_NatureNode)
if(Square_Is_Forest(wx, wy, wp) == ST_TRUE)
    Terrain_Type = cts_Forest;
Load_Combat_Terrain_Pictures(Terrain_Type, wp);
Generate_Combat_Map__WIP(Location_Type, City_House_Type, &Road_Matrix[0], Enchanted_Roads, Terrain_Type, &River_Matrix[0], Flying_Fortress, ATKR_FloatingIsland, DEFR_FloatingIsland, City_Walls, City_Population, Magic_Walls);

    if(ctt != ctt_Water)
        for(itr_cgy = 0; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
            for(itr_cgx = 0; itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
                battlefield->Tile_TerrGroup[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = CTG_Grass;

    |-> CMB_TileGen__WIP()


CMB_TileGen__WIP()
    for(itr_cgy = 0; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
        for(itr_cgx = 0; itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
            TerrGroup_UpLeft     = battlefield->Tile_TerrGroup[((((itr_cgy - 1) * COMBAT_GRID_WIDTH) + itr_cgx) - 1)];
            TerrGroup_Up         = battlefield->Tile_TerrGroup[((((itr_cgy - 1) * COMBAT_GRID_WIDTH) + itr_cgx)    )];
            TerrGroup_UpRight    = battlefield->Tile_TerrGroup[((((itr_cgy - 1) * COMBAT_GRID_WIDTH) + itr_cgx) + 1)];
            TerrGroup_Left       = battlefield->Tile_TerrGroup[((((itr_cgy    ) * COMBAT_GRID_WIDTH) + itr_cgx) - 1)];
            TerrGroup_Middle     = battlefield->Tile_TerrGroup[((((itr_cgy    ) * COMBAT_GRID_WIDTH) + itr_cgx)    )];
            TerrGroup_Right      = battlefield->Tile_TerrGroup[((((itr_cgy    ) * COMBAT_GRID_WIDTH) + itr_cgx) + 1)];
            TerrGroup_DownLeft   = battlefield->Tile_TerrGroup[((((itr_cgy + 1) * COMBAT_GRID_WIDTH) + itr_cgx) - 1)];
            TerrGroup_Down       = battlefield->Tile_TerrGroup[((((itr_cgy + 1) * COMBAT_GRID_WIDTH) + itr_cgx)    )];
            TerrGroup_DownRight  = battlefield->Tile_TerrGroup[((((itr_cgy + 1) * COMBAT_GRID_WIDTH) + itr_cgx) + 1)];


## battlefield->Tile_TerrGroup

Generate_Combat_Map__WIP()
    if(ctt != ctt_Water)
        for(itr_cgy = 0; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
            for(itr_cgx = 0; itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
                battlefield->Tile_TerrGroup[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = CTG_Grass;
    else
        for(itr_cgy = 0; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
            for(itr_cgx = 0; itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
                battlefield->Tile_TerrGroup[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = CTG_DeepWater;


les     bx, [battlefield]
mov     [byte ptr es:bx+1646], 128      ; tile [8,12]
((12 * 21) + 8) = 260
COMBAT_GRID_CELL_COUNT == 462
1646 / 462 = 3.5627705627705627705627705627706
So, ...MoveCost_Ground[]?


CMB_Terrain_Init__WIP()
    ...
    |-> Load_Combat_Terrain_Pictures(Terrain_Type, wp);
    ...
    |-> Generate_Combat_Map__WIP()
        |-> CMB_TileGen__WIP()
...reduces terrain to a single value in CMB_Terrain_Init__WIP()
...passes that to Generate_Combat_Map__WIP()

`CMB_Terrain_Init__WIP()` passes a single terrain value to `Generate_Combat_Map__WIP()`
¿ "combat terrain [set|core|base|...]" ?

combat terrain set
48 terrain
5 trees
5 rocks
IMG_CMB_TerrTiles[itr] = LBX_Reload_Next(combat_terrain_set_lbx_filename, itr, EMM_PageFrame);
IMG_CMB_Trees[itr] = LBX_Reload_Next(combat_terrain_set_lbx_filename, (48 + itr), EMM_PageFrame);
IMG_CMB_Rocks[itr] = LBX_Reload_Next(combat_terrain_set_lbx_filename, (53 + itr), EMM_PageFrame);


Load_Combat_Terrain_Pictures()
Generate_Combat_Map__WIP()
...both do `if(ctt != ctt_Water)`

Load_Combat_Terrain_Pictures()
...loads the same terrain set for cts_Plains, cts_Hills, cts_Forest

    if(Square_Is_Forest(wx, wy, wp) == ST_TRUE)
        Terrain_Type = cts_Forest;
    if(Square_Is_Hills(wx, wy, wp) == ST_TRUE)
        Terrain_Type = cts_Hills;
    if(Square_Is_Swamp(wx, wy, wp) == ST_TRUE)
        Terrain_Type = cts_Plains;
    if(Square_Is_Grasslands(wx, wy, wp) == ST_TRUE)
        Terrain_Type = cts_Plains;
    if(Square_Is_River(wx, wy, wp) == ST_TRUE)
        Terrain_Type = cts_Plains;
    if(Location_Type == clt_NatureNode)
        Terrain_Type = cts_Forest;

...loads CMBGRASS.LBX/CMBGRASC.LBX
...for cts_Plains, cts_Hills, cts_Forest
...from Square_Is_Forest(), Square_Is_Hills(), Square_Is_Swamp(), Square_Is_Grasslands(), Square_Is_River(), location_type == clt_NatureNode

cts_Water
cts_Desert
cts_Mountains
cts_Tundra
¿ 1:1 with their overland terrain types ?



Doing battlefield->terrain_group[(0 - 1)] lands you at battlefield->terrain_group[65514], which is a little far out on an array of 462 values.


battlefield->Tile_Road[]

Generate_Combat_Map__WIP()
    CMB_Enchanted_Roads = enchanted_roads_flag;
    for(itr_cgy = 0; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
        for(itr_cgx = 0; itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
            battlefield->Tile_Road[((itr2 * COMBAT_GRID_WIDTH) + itr_cgx)] = 0;






## CMB_DrawMap__WIP()


for(itr_y = 0; itr_y < 22; itr_y++)
    for(itr_x = 0; itr_x < 11; itr_x++)


// IDK_base_cgc2 = (itr_y / 2);
// IDK_base_cgc1 = (9 + (itr_y + 1) / 2);
// cgx = (IDK_base_cgc2 + itr_x);
// cgy = (IDK_base_cgc1 - itr_x);
cgx = ((itr_y / 2) + itr_x);
cgy = ((9 + (itr_y + 1) / 2) - itr_x);

battlefield_terrain = battlefield->Tile_Terrain[((cgy * COMBAT_GRID_WIDTH) + cgx)];


screen_x = ((itr_x * 32) - ((itr_y & 0x1) * 16));
screen_y = ((itr_y * 8) - 8);

### screen_x
itr_y                                   {   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10 }
(itr_x * 32)                            {   0,  32,  64,  96, 128, 160, 192, 224, 256, 288, 320 }
((itr_x * 32) - ((itr_y & 0x1) * 16))   {   0,  32,  64,  96, 128, 160, 192, 224, 256, 288, 320 }
                                        { -16,  16,  48,  80, 112, 144, 176, 208, 240, 272, 304 }

### screen_y
itr_y               {   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21 }
(itr_y * 8)         {   0,   8,  16,  24,  32,  40,  48,  56,  64,  72,  80,  88,  96, 104, 112, 120, 128, 136, 144, 152, 160, 168 }
((itr_y * 8) - 8)   {  -8,   0,   8,  16,  24,  32,  40,  48,  56,  64,  72,  80,  88,  96, 104, 112, 120, 128, 136, 144, 152, 160 }



 


CMB_DrawMap__WIP() has a check for `if( (cgx >= 0) && (cgx < 21) && (cgy >= 0) && (cgy < 22) )`.




## CMB_ComposeBackgrnd__WIP()

for(itr_y = 0; itr_y < 22; itr_y++)
    for(itr_x = 0; itr_x < 11; itr_x++)

IDK_base_cgc2 = (itr_y / 2);
IDK_base_cgc1 = (((itr_y + 1) / 2) + 9);

cgx = (IDK_base_cgc2 + itr_x);
cgx = ((itr_y / 2) + itr_x);

cgy = (IDK_base_cgc1 - itr_x);
cgy = ((((itr_y + 1) / 2) + 9) - itr_x);

screen_x = ((itr_x * 32) - ((itr_y & 1) * 16));
screen_y = ((itr_y * 8) - 8);

combat_terrain_type = battlefield->Tile_Terrain[((cgy * 21) + cgx)];






## CMB_ComposeBackgrnd__WIP() vs. CMB_DrawMap__WIP()

Tactical_Combat__WIP()
    |-> CMB_ComposeBackgrnd__WIP()

CMB_Terrain_Init__WIP()
    |-> CMB_ComposeBackgrnd__WIP()

CMB_ComposeBackgrnd__WIP() gets called by CMB_Terrain_Init__WIP() and, whenever there's a change/interaction, it gets called by Tactical_Combat__WIP().
CMB_DrawMap__WIP() only gets called by CMB_DrawFullScreen__WIP(), when it is time to actually (re-)draw the screen.







        // { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 } 
        // { 0,  0,  1,  1,  2,  2,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8,  9,  9, 10, 10 }
        // IDK_base_cgc2 = (itr_y / 2);
        CALC_BASE_CGC2();

        // { 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22 }
        // { 0,  1,  1,  2,  2,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8,  9,  9, 10, 10, 11 }
        // { 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20 }
        // IDK_base_cgc1 = (((itr_y + 1) / 2) + 9);
        CALC_BASE_CGC1();

        for(itr_x = 0; itr_x < 11; itr_x++)
        {

            // { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 } 
            // { 0,  0,  1,  1,  2,  2,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8,  9,  9, 10, 10 }
            // { {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10 } }
            // ...
            // { { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 } }
            cgx = (IDK_base_cgc2 + itr_x);
            cgy = ((itr_y / 2) + itr_x);
            CALC_CGC2()
            CALC_CGC1();

            // { 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22 }
            // { 0,  1,  1,  2,  2,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8,  9,  9, 10, 10, 11 }
            // { 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20 }
            // { {  9,  8,  7,  6,  5,  4,  3,  2,  1,  0, -1 } }
            // ...
            // { { 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10 } }
            cgy = (IDK_base_cgc1 - itr_x);
            cgy = ((((itr_y + 1) / 2) + 9) - itr_x);


            screen_x = ((itr_x * 32) - ((itr_y & 1) * 16));
            screen_y = ((itr_y * 8) - 8);  // {-8, 0, -8, ...}, {0, 8, 0, ...}, ... {8, 16, 8, ...}, ...




Page 93  (PDF Page 98)

Combat Movement and Terrain

All terrain not listed in this chart costs one movement point per square.  

Terrain Movement Points Per Square  
Cities          1/2  
Hills           2  
Rivers          2  
Roads           1/2  
Rough (Dirt)    2  
Tree            2  
