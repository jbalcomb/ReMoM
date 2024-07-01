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
SAMB_ptr IDK_Cityscape_BldgStruc;

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

    if(bldg_idx_1 < 3)
    {
        bldg_idx_1 = 0;
    }
    if( (bldg_idx_1 > 34) && (bldg_idx_1 < 100))
    {
        bldg_idx_1 = 0;
    }

    tmp_random_seed = Get_Random_Seed();

    Mark_Block(_screen_seg);
    cityscape_pict_seg = Allocate_Next_Block(_screen_seg, 1250);  // 20,000 bytes
    cityscape_bldgs = (struct s_BLDG *)Allocate_Next_Block(_screen_seg, 176);  // 2,816 bytes  ¿ 16 + 100 * 14 * 2 ?
    IDK_Cityscape_BldgStruc = Allocate_Next_Block(_screen_seg, 44);  // 704 bytes ¿ sizeof(header) + 26 x 26 ?

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

    IDK_Cityscape_Set_BldgStruc__STUB(city_idx, bldg_idx_1);

    // TODO  IDK_CityScreen_Cityscape_sCDC32(xstart, ystart);

    // TODO  IDK_CityScreen_Cityscape_sCF7F1(city_idx, xstart, ystart, bldg_idx_1);

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
// IDK_CityScreen_Cityscape_sCDC32()

// WZD o144p06
// IDK_CityScreen_Cityscape_sCEB93()

// WZD o144p07
// sub_CF555()

// WZD o144p08
// sub_CF5FA()

// WZD o144p09
// sub_CF69F()

// WZD o144p10
// IDK_CityScreen_Cityscape_sCF7F1()

// WZD o144p11
void IDK_Cityscape_Set_BldgStruc__STUB(int16_t city_idx, int16_t bldg_idx)
{

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

    if(_CITIES[city_idx].wp != ARCANUS_PLANE)
    {
        FLIC_Set_CurrentFrame(cityscape_background_myrror_ground_seg, cityscape_background_frame);
        FLIC_Draw(xstart, ystart, cityscape_background_myrror_ground_seg);
    }
    else
    {
        FLIC_Set_CurrentFrame(cityscape_background_arcanus_ground_seg, cityscape_background_frame);
        FLIC_Draw(xstart, ystart, cityscape_background_arcanus_ground_seg);
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
        (TILE_IsAISailable2((city_wx - 1), (city_wy - 1), city_wp) == ST_TRUE) ||
        (TILE_IsAISailable2((city_wx - 1), city_wy, city_wp) == ST_TRUE) ||
        (TILE_IsAISailable2((city_wx - 1), (city_wy + 1), city_wp) == ST_TRUE) ||
        (TILE_IsAISailable2(city_wx, (city_wy - 1), city_wp) == ST_TRUE) ||
        (TILE_IsAISailable2(city_wx, city_wy, city_wp) == ST_TRUE) ||
        (TILE_IsAISailable2(city_wx, (city_wy + 1), city_wp) == ST_TRUE) ||
        (TILE_IsAISailable2((city_wx + 1), (city_wy - 1), city_wp) == ST_TRUE) ||
        (TILE_IsAISailable2((city_wx + 1), city_wy, city_wp) == ST_TRUE) ||
        (TILE_IsAISailable2((city_wx + 1), (city_wy + 1), city_wp) == ST_TRUE)
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
// IDK_CityScreen_Cityscape_sD1A17()

// WZD o144p15
// CTY_DrawOutpost()

// WZD o144p16
// CTY_DrawOutpostScape()
