/*
    Cityscape  (City Screen)

    WIZARDS.EXE
        ovr144
*/

#include "MoM.H"
#include "CITYSCAP.H"



// WZD dseg:CA78                                                 ¿ BEGIN: Cityscape - ?

// WZD dseg:CA78
int16_t IDK_animate_new_building_idx;

// WZD dseg:CA7A
/*
allocated in Cityscape_Draw()
44 PR, 704 B
¿ struct sizeof 46h 70d ?
*/
// struct s_CITYSCAPE * cityscape_bldg_array;
struct s_CITYSCAPE_CR * cityscape_bldg_array;

// WZD dseg:CA7E 00 00                                           IDK_BUILDS1_vertical_mask_seg dw 0      ; DATA XREF: GFX_Swap_AppndCtScap+5EBw
// WZD dseg:CA80 00 00                                           IDK_BUILDS1_horizontal_mask_seg dw 0    ; DATA XREF: GFX_Swap_AppndCtScap+5D4w ...

// WZD dseg:CA82
int16_t cityscape_background_frame;

// WZD dseg:CA84
int16_t m_cityscape_summon_city;

// WZD dseg:CA86
int16_t m_cityscape_fortress_city;


// WZD dseg:CA88
SAMB_ptr cityscape_pict_seg;

// WZD dseg:CA88                                                 ¿ END: Cityscape - ?




/*
    WIZARDS.EXE  ovr144
*/


// WZD o144p01
void IDK_Clear_Cityscape_Vanish_Percent(void)
{
    IDK_cityscape_vanish_percent = 0;
}


// WZD o144p02
void Cityscape_Draw__WIP(int16_t city_idx, int16_t xstart, int16_t ystart, int16_t bldg_idx_1, int16_t bldg_idx_2)
{
    uint32_t tmp_random_seed;
    int16_t itr_players;
    int16_t city_wp;
    int16_t city_wy;
    int16_t city_wx;

    Update_Remap_Color_Range(7, 7);

    IDK_animate_new_building_idx = bldg_idx_2;

    if(_CITIES[city_idx].enchantments[FLYING_FORTRESS] != ST_FALSE)
    {
        cityscape_background_frame = 1;
    }
    else if(_CITIES[city_idx].enchantments[CURSED_LANDS] != ST_FALSE)
    {
        cityscape_background_frame = 0;
    }
    else if(_CITIES[city_idx].enchantments[FAMINE] != ST_FALSE)
    {
        cityscape_background_frame = 2;
    }
    else if(_CITIES[city_idx].enchantments[GAIAS_BLESSING] != ST_FALSE)
    {
        cityscape_background_frame = 3;
    }
    else
    {
        cityscape_background_frame = 4;
    }

    if(bldg_idx_1 < 3) /* bt_Barracks */
    {
        bldg_idx_1 = 0;
    }
    if( (bldg_idx_1 > 34) && (bldg_idx_1 < 100))  /* bt_MinersGuild && ¿ ? */
    {
        bldg_idx_1 = 0;
    }

    tmp_random_seed = Get_Random_Seed();

    Mark_Block(_screen_seg);
    cityscape_pict_seg = Allocate_Next_Block(_screen_seg, 1250);  // 20,000 bytes
    cityscape_bldgs = (struct s_BLDG *)Allocate_Next_Block(_screen_seg, 176);  // 2,816 bytes  ¿ 16 + 100 * 14 * 2 ?
    // cityscape_bldg_array = (struct s_CITYSCAPE *)Allocate_Next_Block(_screen_seg, 44);  // 704 bytes ... 15 * sizeof(s_CITYSCAPE) = 690 / 16 = 43 + 1 = 44 ...Or, ((690 + 15) / 16) = 44
    // cityscape_bldg_array = (struct s_CITYSCAPE_CR *)Allocate_Next_Block(_screen_seg, (((15 * sizeof(struct s_BLDG *)) / 16) + 1));  // 704 bytes ... 15 * sizeof(s_CITYSCAPE) = 690 / 16 = 43 + 1 = 44
    cityscape_bldg_array = (struct s_CITYSCAPE_CR *)Allocate_Next_Block(_screen_seg, (((15 * 23 * sizeof(struct s_BLDG)) / 16) + 1));  // 704 bytes ... 15 * sizeof(s_CITYSCAPE) = 690 / 16 = 43 + 1 = 44

    city_wx = _CITIES[city_idx].wx;
    city_wy = _CITIES[city_idx].wy;
    city_wp = _CITIES[city_idx].wp;
    
    Set_Random_Seed(((city_wx * city_wy) + 10389));  // IDGI:  deterministic per City? Why the addition? Related to (max) wx,wy?

    m_cityscape_fortress_city = ST_FALSE;
    m_cityscape_summon_city = ST_FALSE;

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        if(_FORTRESSES[itr_players].active == ST_TRUE)
        {
            if((_FORTRESSES[itr_players].wx == city_wx) && (_FORTRESSES[itr_players].wy == city_wy) && (_FORTRESSES[itr_players].wp == city_wp))
            {
                m_cityscape_fortress_city = ST_TRUE;
            }
            if((_CITIES[city_idx].wx == _players[itr_players].summon_wx) && (_CITIES[city_idx].wy == _players[itr_players].summon_wy) && (_CITIES[city_idx].wp == _players[itr_players].summon_wp))
            {
                m_cityscape_summon_city = ST_TRUE;
            }
        }
    }

    Cityscape_Draw_Background(city_idx, xstart, ystart, city_wx, city_wy, city_wp);

    Cityscape_Set_BldgStruc__WIP(city_idx, bldg_idx_1);

    Cityscape_Roads_1__WIP(xstart, ystart);

    Cityscape_Draw_Buildings_And_Features__WIP(city_idx, xstart, ystart, bldg_idx_1);

    Cityscape_Draw_Wards_And_Walls__STUB(city_idx, xstart, ystart);

    cityscape_bldg_anim_ctr  = ((cityscape_bldg_anim_ctr  + 1) %  9);
    cityscape_water_anim_ctr = ((cityscape_water_anim_ctr + 1) % 12);
    cityscape_wall_anim_ctr  = ((cityscape_wall_anim_ctr  + 1) %  4);

    Line(xstart, ystart, (xstart + 204), ystart, 0);
    Line(xstart, (ystart + 95), (xstart + 204), (ystart + 95), 0);
    Line(xstart, ystart, xstart, (ystart + 95), 0);
    Line((xstart + 204), ystart, (xstart + 204), (ystart + 95), 0);

    if(IDK_cityscape_vanish_percent < 100)
    {
        IDK_cityscape_vanish_percent += 10;
    }

    Release_Block(_screen_seg);
}


// WZD o144p03
// sub_CD8B0()

// WZD o144p04
// IDK_City_Screen_Draw_Cityscape()

// WZD o144p05
void Cityscape_Roads_1__WIP(int16_t xstart, int16_t ystart)
{



    FLIC_Set_CurrentFrame(cityscape_roads_seg, cityscape_background_frame);

    Draw_Picture_To_Bitmap(cityscape_roads_seg, cityscape_pict_seg);

    // BUGBUG  FLIC_Reset_CurrentFrame(IDK_BUILDS1_horizontal_mask_seg, 0);
    FLIC_Reset_CurrentFrame(IDK_BUILDS1_horizontal_mask_seg);

    Draw_Picture_To_Bitmap(IDK_BUILDS1_horizontal_mask_seg, GfxBuf_2400B);

    // TODO  Cityscape_Roads_2__STUB(cityscape_pict_seg);

    Draw_Picture((xstart + 1), (ystart + 17), cityscape_pict_seg);

}

// WZD o144p06
/*
Cityscape_Roads_1()
    |-> Cityscape_Roads_2(cityscape_pict_seg)
*/
void Cityscape_Roads_2__STUB(SAMB_ptr picture)
{


    
    // Cityscape_Roads_3((29 + (itr * 10)), 4, IDK_cityscape_pict_seg);



    // Cityscape_Roads_4(69, 4, IDK_cityscape_pict_seg);



    // Cityscape_Roads_5((70 - (5 * _DI_itr)), ((5 * _DI_itr) - 1), IDK_cityscape_pict_seg);



}

// WZD o144p07
/*
Cityscape_Roads_2()
    |-> Cityscape_Roads_3((29 + (itr * 10)), 4, IDK_cityscape_pict_seg);
*/
void Cityscape_Roads_3__STUB(int16_t x, int16_t y, SAMB_ptr picture)
{



}


// WZD o144p08
/*
Cityscape_Roads_2()
    |-> Cityscape_Roads_4(69, 4, IDK_cityscape_pict_seg);
*/
void Cityscape_Roads_4__STUB(int16_t x, int16_t y, SAMB_ptr picture)
{



}


// WZD o144p09
/*
Cityscape_Roads_2()
    |-> Cityscape_Roads_5((70 - (5 * _DI_itr)), ((5 * _DI_itr) - 1), IDK_cityscape_pict_seg);
*/
void Cityscape_Roads_5__STUB(int16_t x, int16_t y, SAMB_ptr picture)
{



}


// WZD o144p10
/*

Cityscape_Draw__WIP()
    |-> Cityscape_Draw_Buildings_And_Features__WIP(_SI_city_idx, _DI_xstart, ystart, bldg_idx_1)

*/
void Cityscape_Draw_Buildings_And_Features__WIP(int16_t city_idx, int16_t x_start, int16_t y_start, int16_t bldg_idx)
{
    SAMB_ptr src_pict_seg;
    int16_t cr_bldg_idx;
    int16_t var_8;
    int16_t city_race_house_type;
    int16_t itr_row;
    int16_t itr_col;
    int16_t IDK_X_or_R;  // _SI_
    int16_t IDK_Y_or_C;  // _DI_


    cityscape_bldg_count = 0;

    city_race_house_type = _race_type_table[_CITIES[city_idx].race].house_type;

    for(itr_row = 1; itr_row < NUM_CITYSCAPE_ROWS; itr_row++)
    {
        for(itr_col = 0; itr_col < NUM_CITYSCAPE_COLS; itr_col++)
        {
            IDK_Y_or_C = ((itr_row - 1) * 5);

            if(itr_row > 4)
            {
                IDK_Y_or_C -= 2;
            }

            if(itr_row > 8)
            {
                IDK_Y_or_C -= 2;
            }

            if(itr_row > 13)
            {
                IDK_Y_or_C -= 2;
            }

            IDK_Y_or_C += (y_start + 27);

            IDK_X_or_R = (itr_col * 10);

            if(itr_col > 3)
            {
                IDK_X_or_R -= 2;
            }

            if(itr_col > 8)
            {
                IDK_X_or_R -= 2;
            }

            if(itr_col > 13)
            {
                IDK_X_or_R -= 2;
            }

            if(itr_col > 18)
            {
                IDK_X_or_R -= 2;
            }

            IDK_X_or_R += ((x_start + 27) - (IDK_Y_or_C - y_start - 27));





            // cr_bldg_idx = cityscape_bldg_array[itr1][itr2].bldg_idx;
            // cr_bldg_idx = ( (struct s_BLDG *)( cityscape_bldg_array + (itr1 * sizeof(struct s_CITYSCAPE)) + (itr2 * sizeof(struct s_BLDG *)) ) )->bldg_idx;
            // cr_bldg_idx = cityscape_bldg_array.cr[itr_row][itr_col].bldg_idx;
            cr_bldg_idx = cityscape_bldg_array->cr[itr_row][itr_col].bldg_idx;


            if (cr_bldg_idx == 2)
            {
                if ((x_start + 195) > IDK_X_or_R)
                {
                    // WTF  FLIC_Draw(IDK_X_or_R, (IDK_Y_or_C - 19), cityscape_trees_seg[(2 + ((city_race_house_type * 10) + Random(5)))]);
                }
            }

            if (cr_bldg_idx == -3)
            {
                var_8 = (Random(3) - 1);
                if ((x_start + 195) > (IDK_X_or_R + var_8))
                {
                    // WTF  FLIC_Draw((IDK_X_or_R + var_8), (IDK_Y_or_C - 6 - var_8), cityscape_rocks_seg[(2 + (Random(3)))]);
                }
            }

            if (cr_bldg_idx == -4)
            {
                var_8 = (Random(3) - 1);
                if ((x_start + 195) > (IDK_X_or_R + var_8))
                {
                    // WTF  FLIC_Draw((IDK_X_or_R + var_8), (IDK_Y_or_C - 6 - var_8), cityscape_summon_circle_seg[Random(3)]);
                }
            }

            /*
                BEGIN:  buildings
                    IDA color #11 green,bright,1
            */
            if((cr_bldg_idx >= bt_Barracks) && (cr_bldg_idx <= bt_MinersGuild))
            {
                Cityscape_XY(IDK_X_or_R, IDK_Y_or_C, cr_bldg_idx, bldg_data_table[cr_bldg_idx].G_Animation);

                if (_CITIES[city_idx].bldg_status[cr_bldg_idx] == bs_Removed)
                {
                    // TODO  src_pict_seg = IMG_CTY_Rubble[bldg_data_table[cr_bldg_idx].G_Animation];  // ; GFX_Swap_Seg, 2x2 - 2x3 - 3x2 - 3x3 rubble images
                    // TODO  FLIC_Set_CurrentFrame(src_pict_seg, cityscape_bldg_anim_ctr);
                    // TODO  FLIC_Set_LoopFrame_1(GfxBuf_2400B);
                    // TODO  Draw_Picture_To_Bitmap(src_pict_seg, GfxBuf_2400B);
                }
                else
                {
                    FLIC_Set_CurrentFrame(bldg_picts_seg[cr_bldg_idx], cityscape_bldg_anim_ctr);
                    Draw_Picture_To_Bitmap(bldg_picts_seg[cr_bldg_idx], GfxBuf_2400B);
                    if (cr_bldg_idx == IDK_animate_new_building_idx)
                    {
                        // TODO  Vanish_Bitmap(GfxBuf_2400B, IDK_cityscape_vanish_percent);
                    }
                    FLIC_Set_LoopFrame_1(GfxBuf_2400B);
                }

                Draw_Picture(IDK_X_or_R, (IDK_Y_or_C - 32), GfxBuf_2400B);

                if((cr_bldg_idx >= bt_Shrine) && (cr_bldg_idx <= bt_Parthenon) && (_CITIES[city_idx].enchantments[EVIL_PRESENCE] != ST_FALSE))
                {
                    // TODO  FLIC_Set_CurrentFrame(IMG_CTY_EvilPresnc, cityscape_bldg_anim_ctr);
                    // TODO  Draw_Picture_To_Bitmap(IMG_CTY_EvilPresnc, GfxBuf_2400B);
                    if(cr_bldg_idx == IDK_animate_new_building_idx)
                    {
                        // TODO  Vanish_Bitmap(GfxBuf_2400B, IDK_cityscape_vanish_percent);
                    }
                    // TODO  FLIC_Set_LoopFrame_1(GfxBuf_2400B);
                    // TODO  Draw_Picture(IDK_X_or_R, (IDK_Y_or_C - 32), GfxBuf_2400B);
                }
            }
            /*
                END:  buildings
            */


            /*
                BEGIN:  Dark Rituals  (105)
            */
            /*
                END:  Dark Rituals  (105)
            */

            /*
                BEGIN:  Summon Circle  (100)
            */
            if((cr_bldg_idx == 100) || (bldg_idx == 100))
            {
                FLIC_Set_CurrentFrame(cityscape_summon_circle_seg, cityscape_bldg_anim_ctr);
                Draw_Picture_To_Bitmap(cityscape_summon_circle_seg, GfxBuf_2400B);
                if(cr_bldg_idx == IDK_animate_new_building_idx)
                {
                    // TODO  Vanish_Bitmap(GfxBuf_2400B, IDK_cityscape_vanish_percent);
                }
                FLIC_Set_LoopFrame_1(GfxBuf_2400B);
                Draw_Picture(IDK_X_or_R, (IDK_Y_or_C - 32), GfxBuf_2400B);
                Cityscape_XY(IDK_X_or_R, IDK_Y_or_C, 100, 1);
            }
            /*
                END:  Summon Circle  (100)
            */

            /*
                BEGIN:  Earth Gate  (101)
            */
            /*
                END:  Earth Gate  (101)
            */

            /*
                BEGIN:  Stream Of Life  (102)
            */
            /*
                END:  Stream Of Life  (102)
            */

            /*
                BEGIN:  Astral Gate  (103)
            */
            /*
                END:  Astral Gate  (103)
            */

            /*
                BEGIN:  Fortress  (104)
            */
            if((cr_bldg_idx == 104) || (bldg_idx == 104))
            {
                FLIC_Set_CurrentFrame(cityscape_fortress_seg, cityscape_bldg_anim_ctr);
                Draw_Picture_To_Bitmap(cityscape_fortress_seg, GfxBuf_2400B);
                if(cr_bldg_idx == IDK_animate_new_building_idx)
                {
                    // TODO  Vanish_Bitmap(GfxBuf_2400B, IDK_cityscape_vanish_percent);
                }
                FLIC_Set_LoopFrame_1(GfxBuf_2400B);
                Draw_Picture(IDK_X_or_R, (IDK_Y_or_C - 47), GfxBuf_2400B);
                Cityscape_XY(IDK_X_or_R, IDK_Y_or_C, 104, 4);
            }
            /*
                END:  Fortress  (104)
            */

            /*
                BEGIN:  Altar Of Battle  (106)
            */
            /*
                END:  Altar Of Battle  (106)
            */

            /*
                BEGIN:  
            */
            /*
                END:  
            */

            /*
                BEGIN:  
            */
            /*
                END:  
            */

            /*
                BEGIN:  
            */
            /*
                END:  
            */

            /*
                BEGIN:  
            */
            /*
                END:  
            */

            /*
                BEGIN:  
            */
            /*
                END:  
            */

            /*
                BEGIN:  
            */
            /*
                END:  
            */


        }
    }

    /*
        BEGIN:  City Walls
    */

    if(_CITIES[city_idx].bldg_status[bt_CityWalls] == bs_Built)
    {
        FLIC_Set_CurrentFrame(cityscape_city_walls_seg, 0);
        Draw_Picture_To_Bitmap(cityscape_city_walls_seg, GfxBuf_2400B);
        if(cr_bldg_idx == IDK_animate_new_building_idx)
        {
            // TODO  Vanish_Bitmap(GfxBuf_2400B, IDK_cityscape_vanish_percent);
        }
        FLIC_Set_LoopFrame_1(GfxBuf_2400B);
        Draw_Picture(x_start, (y_start + 82), GfxBuf_2400B);
        Cityscape_XY(IDK_X_or_R, IDK_Y_or_C, 104, 4);

        cityscape_bldgs[cityscape_bldg_count].bldg_idx = bt_CityWalls;
        cityscape_bldgs[cityscape_bldg_count].x1 = x_start;
        cityscape_bldgs[cityscape_bldg_count].y1 = (y_start + 82);
        cityscape_bldgs[cityscape_bldg_count].x2 = (x_start + 204);
        cityscape_bldgs[cityscape_bldg_count].y2 = (y_start + 93);
        cityscape_bldgs[cityscape_bldg_count].IDK_x = (x_start + 102);
        cityscape_bldgs[cityscape_bldg_count].IDK_y = (y_start + 85);
        cityscape_bldg_count++;
    }

    /*
        END:  City Walls
    */

}


// WZD o144p11
// MoO2  Module: COLDRAW  Make_Bldg_Array_For_Colony_()
/*

*/
void Cityscape_Set_BldgStruc__WIP(int16_t city_idx, int16_t bldg_idx)
{
    int16_t var_BC[8];
    int16_t var_AC[8];
    int16_t var_9C[8];
    int16_t var_8C[8];
    int16_t var_7C[8];
    int16_t var_6C[8];
    int16_t var_5C[14];
    int16_t var_40[14];
    int16_t house_count;
    int16_t IDK_itr_bldg_status;
    int16_t rock_count;
    int16_t tree_count;
    int16_t var_1C;
    int16_t row;
    int16_t col;
    int16_t var_16;
    int16_t var_14;
    int16_t var_12;
    int16_t var_10;
    int16_t var_E;
    int16_t building_shape_3x3_count;
    int16_t building_shape_2x3_count;
    int16_t building_shape_3x2_count;
    int16_t building_shape_2x2_count;
    int16_t IDK_itr_bldg;
    int16_t itr_bldg;  // Dasm uses IDK_itr_bldg
    int16_t itr2;
    int16_t itr1;  // _DI_
    int16_t itr_row;  // Dasm uses itr2
    int16_t itr_col;  // Dasm uses itr1

    for(itr_row = 1; itr_row < NUM_CITYSCAPE_ROWS; itr_row++)
    {
        for(itr_col = 0; itr_col < NUM_CITYSCAPE_COLS; itr_col++)
        {
            cityscape_bldg_array->cr[itr_row][itr_col].bldg_idx = 0;
        }
    }

    for(itr_row = 4; itr_row < NUM_CITYSCAPE_ROWS; itr_row++)
    {
        cityscape_bldg_array->cr[itr_row][0].bldg_idx = -1;
    }

    for(itr_row = 7; itr_row < NUM_CITYSCAPE_ROWS; itr_row++)
    {
        cityscape_bldg_array->cr[itr_row][1].bldg_idx = -1;
    }

    for(itr_row = 10; itr_row < NUM_CITYSCAPE_ROWS; itr_row++)
    {
        cityscape_bldg_array->cr[itr_row][2].bldg_idx = -1;
    }

    for(itr_row = 12; itr_row < NUM_CITYSCAPE_ROWS; itr_row++)
    {
        cityscape_bldg_array->cr[itr_row][3].bldg_idx = -1;
    }

    for(itr_col = 19; itr_col < NUM_CITYSCAPE_COLS; itr_col++)
    {
        for(itr_row = 0; itr_row < 5; itr_row++)
        {
            cityscape_bldg_array->cr[itr_row][itr_col].bldg_idx = -1;
        }
    }

    cityscape_bldg_array->cr[6][21].bldg_idx = -1;
    cityscape_bldg_array->cr[6][22].bldg_idx = -1;
    cityscape_bldg_array->cr[7][22].bldg_idx = -1;
    cityscape_bldg_array->cr[0][18].bldg_idx = -1;

    // fortress or building type 104
    if(
        (m_cityscape_fortress_city == ST_TRUE)
        ||
        (bldg_idx == 104)
    )
    {
        for(itr2 = 10; itr2 < 13; itr2++)
        {
            for(itr1 = 6; itr1 < 9; itr1++)
            {
                cityscape_bldg_array->cr[itr1][itr2].bldg_idx = 1;
            }
        }

        for(itr1 = 1; itr1 <= 2; itr1++)
        {
            cityscape_bldg_array->cr[itr1][7].bldg_idx = -1;
        }

        for(itr1 = 1; itr1 <= 2; itr1++)
        {
            cityscape_bldg_array->cr[itr1][8].bldg_idx = -1;
        }

        for(itr1 = 3; itr1 <= 4; itr1++)
        {
            cityscape_bldg_array->cr[itr1][10].bldg_idx = -1;
        }

    }

    cityscape_bldg_array->cr[7][22].bldg_idx = -1;
    cityscape_bldg_array->cr[8][22].bldg_idx = -1;
    cityscape_bldg_array->cr[7][21].bldg_idx = -1;
    cityscape_bldg_array->cr[8][21].bldg_idx = -1;

    for(itr2 = 0; itr2 < 18; itr2++)
    {
        cityscape_bldg_array->cr[0][itr2].bldg_idx = -2;
    }

    for(itr2 = 1; itr2 < 23; itr2++)
    {
        cityscape_bldg_array->cr[10][itr2].bldg_idx = -2;
    }

    for(itr2 = 3; itr2 < 23; itr2++)
    {
        cityscape_bldg_array->cr[9][itr2].bldg_idx = -2;
    }

    for(itr2 = 4; itr2 < 23; itr2++)
    {
        cityscape_bldg_array->cr[14][itr2].bldg_idx = -2;
    }

    for(itr1 = 0; itr1 < 15; itr1++)
    {
        cityscape_bldg_array->cr[itr1][4].bldg_idx = -2;
    }

    for(itr1 = 0; itr1 < 15; itr1++)
    {
        cityscape_bldg_array->cr[itr1][9].bldg_idx = -2;
    }

    for(itr1 = 0; itr1 < 15; itr1++)
    {
        cityscape_bldg_array->cr[itr1][13].bldg_idx = -2;
    }

    for(itr1 = 1; itr1 < 15; itr1++)
    {
        cityscape_bldg_array->cr[itr1][18].bldg_idx = -2;
    }

    building_shape_2x2_count = 0;
    building_shape_3x2_count = 0;
    building_shape_2x3_count = 0;
    building_shape_3x3_count = 0;

    for(IDK_itr_bldg = bt_Barracks; IDK_itr_bldg <= bt_MinersGuild; IDK_itr_bldg++)
    {
        IDK_itr_bldg_status = _CITIES[city_idx].bldg_status[IDK_itr_bldg];

        if(
            (
                (IDK_itr_bldg_status < bs_Built)
                &&
                (IDK_itr_bldg != bldg_idx)
            )
            ||
            (IDK_itr_bldg == bt_ShipYard)
            ||
            (IDK_itr_bldg == bt_ShipWrightsGuild)
            ||
            (IDK_itr_bldg == bt_MaritimeGuild)
        )
        {
            continue;
        }
    
        switch(bldg_data_table[IDK_itr_bldg].G_Animation)
        {
            case bp_2x2:
            {
                building_shape_2x2_count++;
            } break;
            case bp_2x3:
            {
                building_shape_2x3_count++;
            } break;
            case bp_3x2:
            {
                building_shape_3x2_count++;
            } break;
            case bp_3x3:
            {
                building_shape_3x3_count++;
            } break;
        }
    }

    if(
        (m_cityscape_summon_city != ST_FALSE)
        ||
        (bldg_idx == 100)
    )
    {
        building_shape_2x3_count++;
    }

    if(
        (_CITIES[city_idx].enchantments[EARTH_GATE] != ST_FALSE)
        ||
        (bldg_idx == 101)
    )
    {
        building_shape_2x2_count++;
    }

    if(
        (_CITIES[city_idx].enchantments[STREAM_OF_LIFE] != ST_FALSE)
        ||
        (bldg_idx == 102)
    )
    {
        building_shape_2x2_count++;
    }

    if(
        (_CITIES[city_idx].enchantments[ASTRAL_GATE] != ST_FALSE)
        ||
        (bldg_idx == 103)
    )
    {
        building_shape_2x2_count++;
    }

    if(
        (_CITIES[city_idx].enchantments[DARK_RITUALS] != ST_FALSE)
        ||
        (bldg_idx == 105)
    )
    {
        building_shape_2x3_count++;
    }

    if(
        (_CITIES[city_idx].enchantments[ALTAR_OF_BATTLE] != ST_FALSE)
        ||
        (bldg_idx == 106)
    )
    {
        building_shape_2x3_count++;
    }

    // {..., bt_ShipWrightsGuild, bt_ShipYard, bt_MaritimeGuild, ...}
    for(itr_bldg = 12; itr_bldg <= 14; itr_bldg++)
    {
        if(
            (
                (_CITIES[city_idx].bldg_status[itr_bldg] < bs_Built)
                &&
                (itr_bldg != bldg_idx)
            )
        )
        {
            continue;
        }

        for(itr_row = 6; itr_row < 9; itr_row++)
        {
            for(itr_col = 2; itr_col < 4; itr_col++)
            {
                cityscape_bldg_array->cr[itr_row][itr_col].bldg_idx = 1;
            }
        }

    }


    /*
        BEGIN:  building_shape_3x3_count
    */
   {
        var_1C = 3;
        IDK_itr_bldg = 0;
        var_16 = 0;

        while(IDK_itr_bldg < building_shape_3x3_count)
        {
            col = ((Random((var_1C + 1)) + 10) - (var_1C / 2));
            row = ((Random((var_1C + 1)) +  6) - (var_1C / 2));

            if((col >= 0) && (col < 20) && (row >= 0) && (row < 12))
            {
                if(
                    cityscape_bldg_array->cr[(row + 0)][(col + 0)].bldg_idx == 0 &&
                    cityscape_bldg_array->cr[(row + 0)][(col + 1)].bldg_idx == 0 &&
                    cityscape_bldg_array->cr[(row + 0)][(col + 2)].bldg_idx == 0 &&
                    cityscape_bldg_array->cr[(row + 1)][(col + 0)].bldg_idx == 0 &&
                    cityscape_bldg_array->cr[(row + 1)][(col + 1)].bldg_idx == 0 &&
                    cityscape_bldg_array->cr[(row + 1)][(col + 2)].bldg_idx == 0 &&
                    cityscape_bldg_array->cr[(row + 2)][(col + 0)].bldg_idx == 0 &&
                    cityscape_bldg_array->cr[(row + 2)][(col + 1)].bldg_idx == 0 &&
                    cityscape_bldg_array->cr[(row + 2)][(col + 2)].bldg_idx == 0

                )
                {
                    var_16 = 0;

                    var_AC[IDK_itr_bldg] = col;
                    var_BC[IDK_itr_bldg] = row;

                    IDK_itr_bldg++;

                    for(itr_row = 0; itr_row < 3; itr_row++)
                    {
                        for(itr_col = 0; itr_col < 3; itr_col++)
                        {
                            cityscape_bldg_array->cr[(row + itr_row)][(col + itr_col)].bldg_idx = 1;
                        }
                    }

                }

                var_16++;

                if(var_16 > 500)
                {
                    var_16 = 0;
                    building_shape_3x3_count--;
                }
            }

            if(var_1C < 30)
            {
                var_1C++;
            }
        }
   }
    /*
        END:  building_shape_3x3_count
    */


    /*
        BEGIN:  building_shape_2x3_count
    */
    {

        var_1C = 3;
        IDK_itr_bldg = 0;
        var_16 = 0;

        while(IDK_itr_bldg < building_shape_2x3_count)
        {
            col = ((Random((var_1C + 1)) + 10) - (var_1C / 2));
            row = ((Random((var_1C + 1)) +  6) - (var_1C / 2));

            if((col >= 0) && (col < 20) && (row >= 0) && (row < 13))
            {
                if(
                    cityscape_bldg_array->cr[(row + 0)][(col + 0)].bldg_idx == 0 &&
                    cityscape_bldg_array->cr[(row + 0)][(col + 1)].bldg_idx == 0 &&
                    cityscape_bldg_array->cr[(row + 0)][(col + 2)].bldg_idx == 0 &&
                    cityscape_bldg_array->cr[(row + 1)][(col + 0)].bldg_idx == 0 &&
                    cityscape_bldg_array->cr[(row + 1)][(col + 1)].bldg_idx == 0 &&
                    cityscape_bldg_array->cr[(row + 1)][(col + 2)].bldg_idx == 0

                )
                {
                    var_16 = 0;

                    var_8C[IDK_itr_bldg] = col;
                    var_9C[IDK_itr_bldg] = row;

                    IDK_itr_bldg++;

                    for(itr1 = 0; itr1 < 2; itr1++)
                    {
                        for(itr2 = 0; itr2 < 3; itr2++)
                        {
                            cityscape_bldg_array->cr[(row + itr1)][(col + itr2)].bldg_idx = 1;
                        }
                    }

                }

                var_16++;

                if(var_16 > 500)
                {
                    var_16 = 0;
                    building_shape_2x3_count--;
                }
            }

            if(var_1C < 30)
            {
                if(Random(3) == 1)
                {
                    var_1C++;
                }
            }

        }


    }
    /*
        END:  building_shape_2x3_count
    */



    /*
        BEGIN:  building_shape_3x2_count
    */
    {

        var_1C = 3;
        IDK_itr_bldg = 0;
        var_16 = 0;

        while(IDK_itr_bldg < building_shape_3x2_count)
        {
            col = ((Random((var_1C + 1)) + 10) - (var_1C / 2));
            row = ((Random((var_1C + 1)) +  6) - (var_1C / 2));

            if((col >= 0) && (col < 21) && (row >= 0) && (row < 12))
            {
                if(
                    cityscape_bldg_array->cr[(row + 0)][(col + 0)].bldg_idx == 0 &&
                    cityscape_bldg_array->cr[(row + 0)][(col + 1)].bldg_idx == 0 &&
                    cityscape_bldg_array->cr[(row + 1)][(col + 0)].bldg_idx == 0 &&
                    cityscape_bldg_array->cr[(row + 1)][(col + 1)].bldg_idx == 0 &&
                    cityscape_bldg_array->cr[(row + 2)][(col + 0)].bldg_idx == 0 &&
                    cityscape_bldg_array->cr[(row + 2)][(col + 1)].bldg_idx == 0
                )
                {
                    var_16 = 0;

                    var_6C[IDK_itr_bldg] = col;
                    var_7C[IDK_itr_bldg] = row;

                    IDK_itr_bldg++;

                    for(itr1 = 0; itr1 < 3; itr1++)
                    {
                        for(itr2 = 0; itr2 < 2; itr2++)
                        {
                            cityscape_bldg_array->cr[(row + itr1)][(col + itr2)].bldg_idx = 1;
                        }
                    }

                }

                var_16++;

                if(var_16 > 500)
                {
                    var_16 = 0;
                    building_shape_3x2_count--;
                }
            }

            if(var_1C < 30)
            {
                if(Random(3) == 1)
                {
                    var_1C++;
                }
            }

        }


    }
    /*
        END:  building_shape_3x2_count
    */



    /*
        BEGIN:  building_shape_2x2_count
    */
    {

        var_1C = 3;
        IDK_itr_bldg = 0;
        var_16 = 0;

        while(IDK_itr_bldg < building_shape_2x2_count)
        {
            col = ((Random((var_1C + 1)) + 10) - (var_1C / 2));
            row = ((Random((var_1C + 1)) +  6) - (var_1C / 2));

            if((col >= 0) && (col < 21) && (row >= 0) && (row < 13))
            {
                if(
                    cityscape_bldg_array->cr[(row + 0)][(col + 0)].bldg_idx == 0 &&
                    cityscape_bldg_array->cr[(row + 0)][(col + 1)].bldg_idx == 0 &&
                    cityscape_bldg_array->cr[(row + 1)][(col + 0)].bldg_idx == 0 &&
                    cityscape_bldg_array->cr[(row + 1)][(col + 1)].bldg_idx == 0
                )
                {
                    var_16 = 0;

                    var_40[IDK_itr_bldg] = col;
                    var_5C[IDK_itr_bldg] = row;

                    IDK_itr_bldg++;

                    for(itr1 = 0; itr1 < 2; itr1++)
                    {
                        for(itr2 = 0; itr2 < 2; itr2++)
                        {
                            cityscape_bldg_array->cr[(row + itr1)][(col + itr2)].bldg_idx = 1;
                        }
                    }

                }

                var_16++;

                if(var_16 > 500)
                {
                    var_16 = 0;
                    building_shape_2x2_count--;
                }
            }

            if(var_1C < 30)
            {
                if(Random(3) == 1)
                {
                    var_1C++;
                }
            }

        }


    }
    /*
        END:  building_shape_2x2_count
    */


    /*
        BEGIN:  houses
    */
    {
        house_count = (_CITIES[city_idx].population * 8);

        if(house_count = 0)
        {
            house_count = 1;
        }

        var_1C = 3;

        IDK_itr_bldg = 0;

        while(IDK_itr_bldg < house_count)
        {
            col = ((Random((var_1C + 1)) + 10) - (var_1C / 2));
            row = ((Random((var_1C + 1)) +  6) - (var_1C / 2));

            if((col >= 0) && (col < 23) && (row >= 0) && (row < 15))
            {
                if(
                  cityscape_bldg_array->cr[(row + 0)][(col + 0)].bldg_idx == 0
                )
                {
                    cityscape_bldg_array->cr[(row + itr1)][(col + itr2)].bldg_idx = 2;
                    IDK_itr_bldg++;
                }
            }
            else
            {
                if(Random(4) == 1)
                {
                    if(var_1C < 30)
                    {
                        var_1C++;
                    }
                }

                if(Random(4) != 1)
                {
                    IDK_itr_bldg--;
                }
                
            }
        }
    }
    /*
        END:  houses
    */


    /*
        BEGIN:  trees & rocks
    */
    {
        if(
            (Terrain_Is_Explored_Forest(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp) == ST_TRUE)
            ||
            (_race_type_table[_CITIES[city_idx].race].house_type == 1)
        )
        {
            tree_count = (Random(20) + 80);
        }
        else
        {
            tree_count = (Random(8) + Random(8) + 5);
        }

        if(
            (Terrain_Is_Mountain(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp) == ST_TRUE)
            ||
            (Terrain_Is_Hills(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp) == ST_TRUE)
        )
        {
            rock_count = (Random(10) + 20);
        }
        else
        {
            rock_count = (Random(8) + Random(8));
        }


        var_1C = 6;
        for(IDK_itr_bldg = 0; IDK_itr_bldg < tree_count; IDK_itr_bldg++)
        {
            col = ((Random((var_1C + 1)) + 10) - (var_1C / 2));
            row = ((Random((var_1C + 1)) +  6) - (var_1C / 2));

            if((col >= 0) && (col < 23) && (row >= 0) && (row < 15))
            {
                if(
                   cityscape_bldg_array->cr[(row + 0)][(col + 0)].bldg_idx == 0
                )
                {
                    cityscape_bldg_array->cr[(row + itr1)][(col + itr2)].bldg_idx = -3;
                    IDK_itr_bldg++;
                }
            }
            else
            {
                if(var_1C < 30)
                {
                    if(Random(4) == 1)
                    {
                        var_1C++;
                    }
                }
                if(Random(2) == 1)
                {
                    IDK_itr_bldg--;
                }
            }
        }
        

        var_1C = 6;
        for(IDK_itr_bldg = 0; IDK_itr_bldg < rock_count; IDK_itr_bldg++)
        {
            col = ((Random((var_1C + 1)) + 10) - (var_1C / 2));
            row = ((Random((var_1C + 1)) +  6) - (var_1C / 2));

            if((col >= 0) && (col < 23) && (row >= 0) && (row < 15))
            {
                if(
                    cityscape_bldg_array->cr[(row + 0)][(col + 0)].bldg_idx == 0
                )
                {
                    cityscape_bldg_array->cr[(row + itr1)][(col + itr2)].bldg_idx = -4;
                    IDK_itr_bldg++;
                }
            }
            else
            {
                if(var_1C < 30)
                {
                    if(Random(4) == 1)
                    {
                        var_1C++;
                    }
                }
                if(Random(2) == 1)
                {
                    IDK_itr_bldg--;
                }
            }
        }
        
    }
    /*
        END:  trees & rocks
    */


    cityscape_bldg_count = 0;

    var_E = 0;
    var_10 = 0;
    var_12 = 0;
    var_14 = 0;

    if(
        (m_cityscape_summon_city != ST_FALSE)
        ||
        (bldg_idx == 100)
    )
    {
        if(var_12 < building_shape_2x3_count)
        {
            cityscape_bldg_array->cr[(var_9C[var_12] + 1)][(var_8C[var_12])].bldg_idx = 100;
        }
        var_12++;
    }

    if(
        (_CITIES[city_idx].enchantments[EARTH_GATE] != ST_FALSE)
        ||
        (bldg_idx == 101)
    )
    {
        if(var_E < building_shape_2x2_count)
        {
            cityscape_bldg_array->cr[(var_5C[var_E] + 1)][(var_40[var_E])].bldg_idx = 101;
        }
        var_E++;
    }

    if(
        (_CITIES[city_idx].enchantments[STREAM_OF_LIFE] != ST_FALSE)
        ||
        (bldg_idx == 102)
    )
    {
        if(var_E < building_shape_2x2_count)
        {
            cityscape_bldg_array->cr[(var_5C[var_E] + 1)][(var_40[var_E])].bldg_idx = 102;
        }
        var_E++;
    }

    if(
        (_CITIES[city_idx].enchantments[ASTRAL_GATE] != ST_FALSE)
        ||
        (bldg_idx == 103)
    )
    {
        if(var_E < building_shape_2x2_count)
        {
            cityscape_bldg_array->cr[(var_5C[var_E] + 1)][(var_40[var_E])].bldg_idx = 103;
        }
        var_E++;
    }

    if(
        (_CITIES[city_idx].enchantments[ALTAR_OF_BATTLE] != ST_FALSE)
        ||
        (bldg_idx == 106)
    )
    {
        if(var_12 < building_shape_2x3_count)
        {
            cityscape_bldg_array->cr[(var_9C[var_12] + 1)][(var_8C[var_12])].bldg_idx = 106;
        }
        var_12++;
    }

    if(
        (_CITIES[city_idx].enchantments[DARK_RITUALS] != ST_FALSE)
        ||
        (bldg_idx == 105)
    )
    {
        if(var_12 < building_shape_2x3_count)
        {
            cityscape_bldg_array->cr[(var_9C[var_12] + 1)][(var_8C[var_12])].bldg_idx = 106;
        }
        var_12++;
    }

    for(itr_bldg = bt_Barracks; itr_bldg <= bt_MinersGuild; itr_bldg++)
    {
        if(
            (
                (_CITIES[city_idx].bldg_status[itr_bldg] < bs_Built)
                &&
                (itr_bldg != bldg_idx)
            )
            ||
            (itr_bldg == bt_ShipYard)
            ||
            (itr_bldg == bt_ShipWrightsGuild)
            ||
            (itr_bldg == bt_MaritimeGuild)
        )
        {
            continue;
        }
        
        switch(bldg_data_table[itr_bldg].G_Animation)
        {
            case bp_2x2:
            {
                if(
                    (var_E < building_shape_2x2_count)
                    &&
                    (_CITIES[city_idx].bldg_status[itr_bldg] >= bs_Built)  /* {-1:NotBuilt,0:Replaced,1:Built,2:Removed} */
                )
                {
                    cityscape_bldg_array->cr[(var_5C[var_E] + 1)][(var_40[var_E])].bldg_idx = itr_bldg;
                    var_E++;
                }
            } break;
            case bp_2x3:
            {
                if(
                    (var_12 < building_shape_2x3_count)
                    &&
                    (_CITIES[city_idx].bldg_status[itr_bldg] >= bs_Built)  /* {-1:NotBuilt,0:Replaced,1:Built,2:Removed} */
                )
                {
                    cityscape_bldg_array->cr[(var_9C[var_12] + 1)][(var_8C[var_12])].bldg_idx = itr_bldg;
                    var_12++;
                }

            } break;
            case bp_3x2:
            {
                if(
                    (var_10 < building_shape_3x2_count)
                    &&
                    (_CITIES[city_idx].bldg_status[itr_bldg] >= 1)  /* {-1:NotBuilt,0:Replaced,1:Built,2:Removed} */
                )
                {
                    cityscape_bldg_array->cr[(var_7C[var_10] + 1)][(var_6C[var_10])].bldg_idx = itr_bldg;
                    var_10++;
                }

            } break;
            case bp_3x3:
            {
                if(
                    (var_14 < building_shape_3x3_count)
                    &&
                    (_CITIES[city_idx].bldg_status[itr_bldg] >= bs_Built)  /* {-1:NotBuilt,0:Replaced,1:Built,2:Removed} */
                )
                {
                    cityscape_bldg_array->cr[(var_BC[var_14] + 1)][(var_AC[var_14])].bldg_idx = itr_bldg;
                    var_14++;
                }

            } break;
        }

    }


    if(m_cityscape_fortress_city != ST_FALSE)
    {
        cityscape_bldg_array->cr[8][10].bldg_idx = 104;
    }


    // {..., bt_ShipWrightsGuild, bt_ShipYard, bt_MaritimeGuild, ...}
    for(itr_bldg = bt_ShipWrightsGuild; itr_bldg <= bt_MaritimeGuild; itr_bldg++)
    {
        if(
            (_CITIES[city_idx].bldg_status[itr_bldg] >= bs_Built)
            ||
            (itr_bldg == bldg_idx)
        )
        {
            cityscape_bldg_array->cr[8][2].bldg_idx = itr_bldg;
        }
    }

}


// WZD o144p12
/*
    Death Ward
    Chaos Ward
    Nature Ward
    Life Ward
    Sorcery Ward
    Nature's Eye
    Inspirations
    Prosperity
    Consecration
    Wall of Darkness
    Wall of Fire
NOTE: Cloud Of Shadow, Heavenly Light, and Chaos Rift are handled in Cityscape_Draw_Background()
¿ FLYING_FORTRESS ?

*/
void Cityscape_Draw_Wards_And_Walls__STUB(int16_t city_idx, int16_t xstart, int16_t ystart)
{

}


// WZD o144p13
void Cityscape_Draw_Background(int16_t city_idx, int16_t xstart, int16_t ystart, int16_t city_wx, int16_t city_wy, int16_t city_wp)
{
    int16_t itr;

    if(_CITIES[city_idx].wp == ARCANUS_PLANE)
    {
        FLIC_Set_CurrentFrame(cityscape_background_arcanus_ground_seg, cityscape_background_frame);
        FLIC_Draw(xstart, ystart, cityscape_background_arcanus_ground_seg);
    }
    else
    {
        FLIC_Set_CurrentFrame(cityscape_background_myrror_ground_seg, cityscape_background_frame);
        FLIC_Draw(xstart, ystart, cityscape_background_myrror_ground_seg);
    }

    /*
        BEGIN: overlay animated water - River or Ocean
    */

    /*
        River Background
        OR
        Ocean Background

        TL, L, BL, T, C, B, TR, R, BR
    */
    if(
        (Terrain_Is_River((city_wx - 1), (city_wy - 1), city_wp) == ST_TRUE) ||
        (Terrain_Is_River((city_wx - 1), city_wy, city_wp) == ST_TRUE) ||
        (Terrain_Is_River((city_wx - 1), (city_wy + 1), city_wp) == ST_TRUE) ||
        (Terrain_Is_River(city_wx, (city_wy - 1), city_wp) == ST_TRUE) ||
        (Terrain_Is_River(city_wx, city_wy, city_wp) == ST_TRUE) ||
        (Terrain_Is_River(city_wx, (city_wy + 1), city_wp) == ST_TRUE) ||
        (Terrain_Is_River((city_wx + 1), (city_wy - 1), city_wp) == ST_TRUE) ||
        (Terrain_Is_River((city_wx + 1), city_wy, city_wp) == ST_TRUE) ||
        (Terrain_Is_River((city_wx + 1), (city_wy + 1), city_wp) == ST_TRUE)
    )
    {
        if(city_wp == ARCANUS_PLANE)
        {
            FLIC_Set_CurrentFrame(cityscape_background_arcanus_river_seg, 0);
            for(itr = 0; (cityscape_water_anim_ctr / 2) >= itr; itr++)
            {
                FLIC_Draw(xstart, ystart, cityscape_background_arcanus_river_seg);
            }
        }
        else
        {
            FLIC_Set_CurrentFrame(cityscape_background_myrror_river_seg, 0);
            for(itr = 0; (cityscape_water_anim_ctr / 2) >= itr; itr++)
            {
                FLIC_Draw(xstart, ystart, cityscape_background_myrror_river_seg);
            }
        }
    }
    else if(
        (Terrain_Is_Ocean((city_wx - 1), (city_wy - 1), city_wp) == ST_TRUE) ||
        (Terrain_Is_Ocean((city_wx - 1), city_wy, city_wp) == ST_TRUE) ||
        (Terrain_Is_Ocean((city_wx - 1), (city_wy + 1), city_wp) == ST_TRUE) ||
        (Terrain_Is_Ocean(city_wx, (city_wy - 1), city_wp) == ST_TRUE) ||
        (Terrain_Is_Ocean(city_wx, city_wy, city_wp) == ST_TRUE) ||
        (Terrain_Is_Ocean(city_wx, (city_wy + 1), city_wp) == ST_TRUE) ||
        (Terrain_Is_Ocean((city_wx + 1), (city_wy - 1), city_wp) == ST_TRUE) ||
        (Terrain_Is_Ocean((city_wx + 1), city_wy, city_wp) == ST_TRUE) ||
        (Terrain_Is_Ocean((city_wx + 1), (city_wy + 1), city_wp) == ST_TRUE)
    )
    {
        if(city_wp == ARCANUS_PLANE)
        {
            FLIC_Set_CurrentFrame(cityscape_background_arcanus_ocean_seg, 0);
            for(itr = 0; (cityscape_water_anim_ctr / 2) >= itr; itr++)
            {
                FLIC_Draw(xstart, ystart, cityscape_background_arcanus_ocean_seg);
            }
        }
        else
        {
            FLIC_Set_CurrentFrame(cityscape_background_myrror_ocean_seg, 0);
            for(itr = 0; (cityscape_water_anim_ctr / 2) >= itr; itr++)
            {
                FLIC_Draw(xstart, ystart, cityscape_background_myrror_ocean_seg);
            }
        }
    }

    /*
        END: overlay animated water - River or Ocean
    */



    if(_CITIES[city_idx].enchantments[FLYING_FORTRESS] != ST_TRUE)
    {
        if(_CITIES[city_idx].enchantments[CLOUD_OF_SHADOW] == ST_TRUE)
        {
            if(city_wp == ARCANUS_PLANE)
            {
                FLIC_Draw(xstart, ystart, cityscape_background_arcanus_darkcloud_seg);
            }
            else
            {
                FLIC_Draw(xstart, ystart, cityscape_background_myrror_darkcloud_seg);
            }
        }
        else if(_CITIES[city_idx].enchantments[HEAVENLY_LIGHT] == ST_TRUE)
        {
            if(city_wp == ARCANUS_PLANE)
            {
                FLIC_Draw(xstart, ystart, cityscape_background_arcanus_alkar_seg);
            }
            else
            {
                FLIC_Draw(xstart, ystart, cityscape_background_myrror_alkar_seg);
            }
        }
        else if(_CITIES[city_idx].enchantments[CHAOS_RIFT] == ST_TRUE)
        {
            if(city_wp == ARCANUS_PLANE)
            {
                FLIC_Draw(xstart, ystart, cityscape_background_arcanus_chaosrift_seg);
            }
            else
            {
                FLIC_Draw(xstart, ystart, cityscape_background_myrror_chaosrift_seg);
            }
        }
        else
        {
            /*
                Mountain Background
                OR
                Hills Background

                TL, T, TR, C
            */
            if(
                (Terrain_Is_Mountain((city_wx - 1), (city_wy - 1), city_wp) == ST_TRUE) ||
                (Terrain_Is_Mountain(city_wx, (city_wy - 1), city_wp) == ST_TRUE) ||
                (Terrain_Is_Mountain((city_wx + 1), (city_wy - 1), city_wp) == ST_TRUE) ||
                (Terrain_Is_Mountain(city_wx, city_wy, city_wp) == ST_TRUE)
            )
            {
                if(city_wp == ARCANUS_PLANE)
                {
                    FLIC_Reset_CurrentFrame(cityscape_background_arcanus_mountain_seg);
                    FLIC_Draw(xstart, ystart, cityscape_background_arcanus_mountain_seg);
                }
                else
                {
                    FLIC_Reset_CurrentFrame(cityscape_background_myrror_mountain_seg);
                    FLIC_Draw(xstart, ystart, cityscape_background_myrror_mountain_seg);
                }
            }
            else if(
                (Terrain_Is_Hills((city_wx - 1), (city_wy - 1), city_wp) == ST_TRUE) ||
                (Terrain_Is_Hills(city_wx, (city_wy - 1), city_wp) == ST_TRUE) ||
                (Terrain_Is_Hills((city_wx + 1), (city_wy - 1), city_wp) == ST_TRUE) ||
                (Terrain_Is_Hills(city_wx, city_wy, city_wp) == ST_TRUE)
            )
            {
                if(city_wp == ARCANUS_PLANE)
                {
                    FLIC_Reset_CurrentFrame(cityscape_background_arcanus_hills_seg);
                    FLIC_Draw(xstart, ystart, cityscape_background_arcanus_hills_seg);
                }
                else
                {
                    FLIC_Reset_CurrentFrame(cityscape_background_myrror_hills_seg);
                    FLIC_Draw(xstart, ystart, cityscape_background_myrror_hills_seg);
                }
            }
        }
    }

}


// WZD o144p14
/*
    populates cityscape_bldgs[], indexed by cityscape_bldg_count

IDK_X_or_R, IDK_Y_or_C, bldg_idx, bldg_data_table[bldg_idx].G_Animation)
*type*, as in 'size & position'

*/
void Cityscape_XY(int16_t x, int16_t y, int16_t bldg_idx, int16_t type)
{
    int16_t x_max;
    int16_t x_min;
    int16_t itr;  // _SI_

    x_max = 0;
    x_min = 1000;

    if(bldg_idx == 104)
    {
        MOX_DBG_BREAK;
    }

    for(itr = 0; itr < 3; itr++)
    {
        cityscape_bldgs[cityscape_bldg_count].bldg_idx = bldg_idx;

        cityscape_bldgs[cityscape_bldg_count].x1 = (x + cityscape_cr[type][itr].x1);
        cityscape_bldgs[cityscape_bldg_count].y1 = (y + cityscape_cr[type][itr].y1);
        cityscape_bldgs[cityscape_bldg_count].x2 = (x + cityscape_cr[type][itr].x2);
        cityscape_bldgs[cityscape_bldg_count].y2 = (y + cityscape_cr[type][itr].y2);

        cityscape_bldgs[cityscape_bldg_count].IDK_y = (y + 1);

        if(cityscape_bldgs[cityscape_bldg_count].x1 < x_min)
        {
            x_min = cityscape_bldgs[cityscape_bldg_count].x1;
        }

        if(cityscape_bldgs[cityscape_bldg_count].x2 > x_max)
        {
            x_max = cityscape_bldgs[cityscape_bldg_count].x2;
        }

        cityscape_bldg_count++;
    }

    // DEDU  ¿ -3, because 1st *real* building is 3 ?
    for(itr = (cityscape_bldg_count - 3); itr < cityscape_bldg_count; itr++)
    {
        cityscape_bldgs[itr].IDK_x = (((x_max + x_min) / 2) - 3);
    }

}


// WZD o144p15
/*
; a wrapper for Outpost_Cityscape_Draw that sets an unused random seed and draws a border around the final image
; BUG: fails to draw the altered backgrounds of the Gaia's Blessing, Flying Fortress, Famine, and Cursed Lands enchantments
*/
void Outpost_Cityscape(int16_t city_idx, int16_t x_start, int16_t y_start)
{
    uint32_t LFSR;
    int16_t city_wp;
    int16_t city_wy;
    int16_t city_wx;

    // TODO  LFSR = Get_Random_Seed();

    city_wx = _CITIES[city_idx].wx;
    city_wy = _CITIES[city_idx].wy;
    city_wp = _CITIES[city_idx].wp;

// TODO  mov     ax, [bp+city_wx]                ; would fit into 16 bits, but unnecessary anyway
// TODO  cwd
// TODO  push    ax
// TODO  mov     ax, [bp+city_wy]
// TODO  push    dx
// TODO  cwd
// TODO  pop     cx
// TODO  pop     bx
// TODO  call    LXMUL@
// TODO  add     ax, 10100010010101b
// TODO  adc     dx, 0
// TODO  push    dx                              ; LFSR_HI
// TODO  push    ax                              ; LFSR_LO
// TODO  call    Set_Random_Seed
// TODO  pop     cx
// TODO  pop     cx                              ; no randomness involved here


    Outpost_Cityscape_Draw(city_idx, x_start, y_start);


    cityscape_bldg_anim_ctr = ((cityscape_bldg_anim_ctr + 1) / 10);


    // TODO  Set_Random_Seed(LFSR);  // BUGBUG  ; no randomness involved here


    Line(x_start, y_start, (x_start + 71), y_start, 0);
    Line(x_start, (y_start + 65), (x_start + 71), (y_start + 65), 0);
    Line(x_start, y_start, x_start, (y_start + 65), 0);
    Line((x_start + 71), y_start, (x_start + 71), (y_start + 65), 0);

}


// WZD o144p16
/*
; draws the reduced cityscape of the outpost screen into the current draw segment
; BUG: fails to draw the altered backgrounds of the Gaia's Blessing, Flying Fortress, Famine, and Cursed Lands enchantments
*/
void Outpost_Cityscape_Draw(int16_t city_idx, int16_t x_start, int16_t y_start)
{
    int16_t race_house_type;
    int16_t itr_anim;
    int16_t city_wp;
    int16_t city_wy;
    int16_t city_wx;

    city_wx = _CITIES[city_idx].wx;
    city_wy = _CITIES[city_idx].wy;
    city_wp = _CITIES[city_idx].wp;

    Set_Window(x_start, y_start, (x_start + 65), (y_start + 71));

    if(_CITIES[city_idx].enchantments[GAIAS_BLESSING] != 0)
    {
        // ; BUG: this is not an actual image
        FLIC_Set_CurrentFrame(cityscape_gaiasblessing_mask_seg, 0);
        Clipped_Draw((x_start - 65), y_start, cityscape_gaiasblessing_mask_seg);
    }
    else if(_CITIES[city_idx].enchantments[FLYING_FORTRESS] != 0)
    {
        // ; BUG: this is not an actual image
        FLIC_Set_CurrentFrame(cityscape_flyingfortress_mask_seg, 0);
        Clipped_Draw((x_start - 65), y_start, cityscape_flyingfortress_mask_seg);
    }
    else
    {
        FLIC_Set_CurrentFrame(cityscape_background_arcanus_ground_seg, 0);
        Clipped_Draw((x_start - 65), y_start, cityscape_background_arcanus_ground_seg);
    }

    if(_CITIES[city_idx].enchantments[FAMINE] != 0)
    {
        FLIC_Set_CurrentFrame(cityscape_famine_mask_seg, 0);
        Clipped_Draw((x_start - 65), y_start, cityscape_famine_mask_seg);
    }

    if(_CITIES[city_idx].enchantments[CURSED_LANDS] != 0)
    {
        FLIC_Set_CurrentFrame(cityscape_cursedland_mask_seg, 0);
        Clipped_Draw((x_start - 65), y_start, cityscape_cursedland_mask_seg);
    }

    if(_CITIES[city_idx].enchantments[FLYING_FORTRESS] == 0)
    {
        if(
            (Terrain_Is_Mountain((city_wx - 1), (city_wy - 1), city_wp) == ST_TRUE) ||
            (Terrain_Is_Mountain( city_wx     , (city_wy - 1), city_wp) == ST_TRUE) ||
            (Terrain_Is_Mountain((city_wx + 1), (city_wy - 1), city_wp) == ST_TRUE) ||
            (Terrain_Is_Mountain( city_wx     ,  city_wy     , city_wp) == ST_TRUE)
        )
        {
            FLIC_Set_CurrentFrame(cityscape_background_arcanus_mountain_seg, 0);
            Clipped_Draw((x_start - 65), y_start, cityscape_background_arcanus_mountain_seg);
        }
        else if(
            (Terrain_Is_Hills((city_wx - 1), (city_wy - 1), city_wp) == ST_TRUE) ||
            (Terrain_Is_Hills( city_wx     , (city_wy - 1), city_wp) == ST_TRUE) ||
            (Terrain_Is_Hills((city_wx + 1), (city_wy - 1), city_wp) == ST_TRUE) ||
            (Terrain_Is_Hills( city_wx     ,  city_wy     , city_wp) == ST_TRUE)
        )
        {
            FLIC_Set_CurrentFrame(cityscape_background_arcanus_mountain_seg, 0);  // BUGBUG  c&p error?
            Clipped_Draw((x_start - 65), y_start, cityscape_background_arcanus_hills_seg);
        }

    }

    if(_CITIES[city_idx].enchantments[CLOUD_OF_SHADOW] != 0)
    {
        FLIC_Set_CurrentFrame(cityscape_background_arcanus_darkcloud_seg, 0);
        for(itr_anim = 0; itr_anim < cityscape_bldg_anim_ctr; itr_anim++)
        {

            Clipped_Draw((x_start - 65), y_start, cityscape_background_arcanus_darkcloud_seg);
        }
    }

    if(_CITIES[city_idx].enchantments[HEAVENLY_LIGHT] != 0)
    {
        FLIC_Set_CurrentFrame(cityscape_background_arcanus_alkar_seg, 0);
        for(itr_anim = 0; itr_anim < cityscape_bldg_anim_ctr; itr_anim++)
        {

            Clipped_Draw((x_start - 65), y_start, cityscape_background_arcanus_alkar_seg);
        }
    }

    if(_CITIES[city_idx].enchantments[CHAOS_RIFT] != 0)
    {
        FLIC_Set_CurrentFrame(cityscape_background_arcanus_chaosrift_seg, 0);
        for(itr_anim = 0; itr_anim < cityscape_bldg_anim_ctr; itr_anim++)
        {

            Clipped_Draw((x_start - 65), y_start, cityscape_background_arcanus_chaosrift_seg);
        }
    }


    race_house_type = _race_type_table[_CITIES[city_idx].race].house_type;

    FLIC_Set_CurrentFrame(cityscape_houses_seg[(race_house_type * 10)], 0);

    Clipped_Draw((x_start + 30), (y_start + 30), cityscape_houses_seg[(race_house_type * 10)]);

    Reset_Window();

}