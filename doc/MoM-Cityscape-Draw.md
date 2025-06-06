





cityscape_bldg_array[][]
    stores type, but also bldg_idx
cityscape_bldgs[]
    stores bldg_idx, field_x1, field_y1, field_x2, field_y2, print_sx, print_sy

Cityscape_Draw_Background()
Cityscape_Make_Buildings_Array()
Cityscape_Draw_Buildings()
Cityscape_Draw_Wards_And_Walls()















Cityscape_Add_Bldg_To_Fields_Array()
    cityscape_bldgs[cityscape_bldg_count].bldg_idx
    sets cityscape_bldgs[cityscape_bldg_count].x1,y1,x2,y2
    sets cityscape_bldgs[cityscape_bldg_count].print_sx,print_sy

void Cityscape_Add_Bldg_To_Fields_Array(int16_t x, int16_t y, int16_t bldg_idx, int16_t type)
    cityscape_bldgs[cityscape_bldg_count].print_sy = (y + 1);




## bldg_picts_seg{}

Reload_Cityscape_Graphics_Cache()

    /*
        loads building picts in of bldg idx
            bldg_picts_seg[{3, ..., 30}]
            bldg_picts_seg[31]
            bldg_picts_seg[{32,33}]
        the array skips elements {0,1,2} for None, Housing, and TradeGoods
    */
    // CITYSCAP.LBX,  45  ANIBUILD    barracks
    // ...
    // CITYSCAP.LBX,  71  BUILDS3     granary
    // CITYSCAP.LBX,  72  BUILDS3     farmer's market
    for(itr1 = 3; itr1 <= 30; itr1++)
    {
        bldg_picts_seg[itr1] = LBX_Reload_Next(cityscap_lbx_file, (42 + itr1), GFX_Swap_Seg);
    }

    // CITYSCAP.LBX,  78  BUILDS2     forester's guild
    bldg_picts_seg[31] = LBX_Reload_Next(cityscap_lbx_file, 78, GFX_Swap_Seg);
    // 34th element  bt_ForestersGuild     = 31

    // CITYSCAP.LBX,  73  BUILDS2     builder's hall
    // CITYSCAP.LBX,  74  BUILDS2     mechanician's guil
    // CITYSCAP.LBX,  75  BUILDS2     miner's guild
    for(itr1 = 32; itr1 <= 34; itr1++)
    {
        bldg_picts_seg[itr1] = LBX_Reload_Next(cityscap_lbx_file, (41 + itr1), GFX_Swap_Seg);
    }




## Cityscape_Draw_Buildings()

iter rows
iter cols
calc some x,y
cr_bldg_idx = cityscape_bldg_array->cr[itr_row][itr_col].bldg_idx;
...
Cityscape_Add_Bldg_To_Fields_Array(col_sx, row_sy, cr_bldg_idx, bldg_data_table[cr_bldg_idx].G_Animation);
FLIC_Set_CurrentFrame(bldg_picts_seg[cr_bldg_idx], cityscape_bldg_anim_ctr);
Draw_Picture_To_Bitmap(bldg_picts_seg[cr_bldg_idx], GfxBuf_2400B);
FLIC_Set_LoopFrame_1(GfxBuf_2400B);
Draw_Picture(col_sx, (row_sy - 32), GfxBuf_2400B);

    for(itr_row = 1; itr_row < NUM_CITYSCAPE_ROWS; itr_row++)
        for(itr_col = 0; itr_col < NUM_CITYSCAPE_COLS; itr_col++)
            row_sy = ((itr_row - 1) * 5);
            // these match with the -2 setting in Cityscape_Draw__WIP()?
            if(itr_row >  4) { row_sy -= 2; }  // -2
            if(itr_row >  8) { row_sy -= 2; }  // -4
            if(itr_row > 13) { row_sy -= 2; }  // -6
            row_sy += (y_start + 27);

            col_sx = (itr_col * 10);
            // these match with the -2 setting in Cityscape_Draw__WIP()?
            if(itr_col >  3) { col_sx -= 2; }  // -2
            if(itr_col >  8) { col_sx -= 2; }  // -4
            if(itr_col > 13) { col_sx -= 2; }  // -6
            if(itr_col > 18) { col_sx -= 2; }  // -8
            col_sx += ((x_start + 27) - (row_sy - y_start - 27));

( 1 - 1) * 5 =  0 * 5 =  0     =  0 + 27 = 27 + 101 = 128 ... bottom edge of highest house  (there's a road above this)
( 2 - 1) * 5 =  1 * 5 =  5     =  5 + 27 = 32 + 101 = 133 ... bottom edge of highest builing
( 3 - 1) * 5 =  2 * 5 = 10     = 10 + 27 = 37 + 101 = 138
( 4 - 1) * 5 =  3 * 5 = 15     = 15 + 27 = 42 + 101 = 143
( 5 - 1) * 5 =  4 * 5 = 20 - 2 = 18 + 27 = 45 + 101 = 146
( 6 - 1) * 5 =  5 * 5 = 25 - 2 = 23 + 27 = 50 + 101 = 151
( 7 - 1) * 5 =  6 * 5 = 30 - 2 = 28 + 27 = 55 + 101 = 156
( 8 - 1) * 5 =  7 * 5 = 35 - 2 = 33 + 27 = 60 + 101 = 161
( 9 - 1) * 5 =  8 * 5 = 40 - 4 = 36 + 27 = 63 + 101 = 164
(10 - 1) * 5 =  9 * 5 = 45 - 4 = 41 + 27 = 68 + 101 = 169
(11 - 1) * 5 = 10 * 5 = 50 - 4 = 46 + 27 = 73 + 101 = 174
(12 - 1) * 5 = 11 * 5 = 55 - 4 = 51 + 27 = 78 + 101 = 179
(13 - 1) * 5 = 12 * 5 = 60 - 4 = 56 + 27 = 83 + 101 = 184 ... bottom edge of all lowest buildings and houses  (there's a road below this)
(14 - 1) * 5 = 13 * 5 = 65 - 6 = 59 + 27 = 86 + 101 = 187 ... 

col_sx = (itr_col * 10);
 0 * 10              =   0    =    0 + 4 =   4 + 27 =  31 ... - (128 - 101 - 27) =  31 -  0 =  31
 1 * 10              =  10    =   10 + 4 =  14 + 27 =  41 ... - (133 - 101 - 27) =  41 -  5 =  36
 2 * 10              =  20    =   20 + 4 =  24 + 27 = 
 3 * 10              =  30    =   30 + 4 =  34 + 27 = 
 4 * 10              =  40 - 2 =  38 + 4 =  42 + 27 = 
 5 * 10              =  50 - 2 =  48 + 4 =  52 + 27 = 
 6 * 10              =  60 - 2 =  58 + 4 =  62 + 27 = 
 7 * 10              =  70 - 2 =  68 + 4 =  72 + 27 = 
 8 * 10              =  80 - 2 =  78 + 4 =  82 + 27 = 
 9 * 10              =  90 - 4 =  86 + 4 =  90 + 27 = 
10 * 10              = 100 - 4 =  96 + 4 = 100 + 27 = 
11 * 10              = 110 - 4 = 106 + 4 = 110 + 27 = 
12 * 10              = 120 - 4 = 116 + 4 = 120 + 27 = 
13 * 10              = 130 - 4 = 126 + 4 = 130 + 27 = 
14 * 10              = 140 - 6 = 134 + 4 = 138 + 27 = 
15 * 10              = 150 - 6 = 144 + 4 = 148 + 27 = 
16 * 10              = 160 - 6 = 154 + 4 = 158 + 27 = 
17 * 10              = 170 - 6 = 164 + 4 = 168 + 27 = 
18 * 10              = 180 - 6 = 174 + 4 = 178 + 27 = 
19 * 10              = 190 - 8 = 182 + 4 = 186 + 27 = 
20 * 10              = 200 - 8 = 192 + 4 = 196 + 27 = 
21 * 10              = 210 - 8 = 202 + 4 = 206 + 27 = 
22 * 10              = 220 - 8 = 212 + 4 = 216 + 27 = 243 ... - (187 - 101 - 27) = 243 - 59 = 184

{ 128, 133, 138, 143, 146, 151, 156, 161, 164, 169, 174, 179, 184, 187 }

{  31,                                                                                                              }
{    ,  36,                                                                                                         }
...
{    ,    ,    ,    ,    ,    ,    ,    ,    ,    ,    ,    ,    ,    ,    ,    ,    ,    ,    ,    ,    ,    , 184 }