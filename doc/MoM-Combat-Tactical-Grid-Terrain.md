



BaseTile_X  ==> IDK_base_cgc2
BaseTile_Y  ==> IDK_base_cgc1
Tile_X      ==> cgc2
Tile_Y      ==> cgc1

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





¿ scanned combat grid cell outline ?
¿ combat grid cell scanned ?


scanned_battle_unit_idx = CMB_TargetRows[Tile_Y][Tile_X];






## CMB_DrawMap__WIP()


for(itr_y = 0; itr_y < 22; itr_y++)
    for(itr_x = 0; itr_x < 11; itr_x++)


// IDK_base_cgc2 = (itr_y / 2);
// IDK_base_cgc1 = (9 + (itr_y + 1) / 2);
// cgc2 = (IDK_base_cgc2 + itr_x);
// cgc1 = (IDK_base_cgc1 - itr_x);
cgc2 = ((itr_y / 2) + itr_x);
cgc1 = ((9 + (itr_y + 1) / 2) - itr_x);

battlefield_terrain = battlefield->Tile_Terrain[((cgc1 * 21) + cgc2)];


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



 


CMB_DrawMap__WIP() has a check for `if( (cgc2 >= 0) && (cgc2 < 21) && (cgc1 >= 0) && (cgc1 < 22) )`.




## CMB_ComposeBackgrnd__WIP()

for(itr_y = 0; itr_y < 22; itr_y++)
    for(itr_x = 0; itr_x < 11; itr_x++)

IDK_base_cgc2 = (itr_y / 2);
IDK_base_cgc1 = (((itr_y + 1) / 2) + 9);

cgc2 = (IDK_base_cgc2 + itr_x);
cgc2 = ((itr_y / 2) + itr_x);

cgc1 = (IDK_base_cgc1 - itr_x);
cgc1 = ((((itr_y + 1) / 2) + 9) - itr_x);

screen_x = ((itr_x * 32) - ((itr_y & 1) * 16));
screen_y = ((itr_y * 8) - 8);

combat_terrain_type = battlefield->Tile_Terrain[((cgc1 * 21) + cgc2)];






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
            cgc2 = (IDK_base_cgc2 + itr_x);
            cgc1 = ((itr_y / 2) + itr_x);
            CALC_CGC2()
            CALC_CGC1();

            // { 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22 }
            // { 0,  1,  1,  2,  2,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8,  9,  9, 10, 10, 11 }
            // { 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20 }
            // { {  9,  8,  7,  6,  5,  4,  3,  2,  1,  0, -1 } }
            // ...
            // { { 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10 } }
            cgc1 = (IDK_base_cgc1 - itr_x);
            cgc1 = ((((itr_y + 1) / 2) + 9) - itr_x);


            screen_x = ((itr_x * 32) - ((itr_y & 1) * 16));
            screen_y = ((itr_y * 8) - 8);  // {-8, 0, -8, ...}, {0, 8, 0, ...}, ... {8, 16, 8, ...}, ...
