/*
    Cityscape  (City Screen)

    WIZARDS.EXE
        ovr144
*/

#include "CITYSCAP.h"

#include "MOX/Allocate.h"
#include "MOX/FLIC_Draw.h"
#include "MOX/Fonts.h"
#include "MOX/Graphics.h"
#include "MOX/MOM_Data.h"
#include "MOX/MOX_BITS.h"
#include "MOX/MOX_DAT.h"
#include "MOX/MOX_DEF.h"
#include "MOX/MOX_TYPE.h"

#include "MOX/random.h"
#include "RACETYPE.h"

#include "STU/STU_DBG.h"
#include "Terrain.h"

#include <assert.h>



// WZD dseg:6ED0                                                 BEGIN:  ovr144 - Initialized Data  (Cityscape)

// WZD dseg:6ED0
// drake178: CTY_EnchantAnimStage
int16_t cityscape_bldg_anim_ctr = 0;
int16_t cityscape_bldg_anim_itr = 0;  // DNE in Dasm

// WZD dseg:6ED2
int16_t cityscape_water_anim_ctr = 0;
int16_t cityscape_water_anim_itr = 0;  // DNE in Dasm

// WZD dseg:6ED4
int16_t cityscape_wall_anim_ctr = 0;
int16_t cityscape_wall_anim_itr = 0;  // DNE in Dasm

// WZD dseg:6ED6
int16_t cityscape_build_anim_ctr = 100;
int16_t cityscape_build_anim_itr = 100;  // DNE in Dasm

// WZD dseg:6ED8
int16_t cityscape_roads_cr[8] = {
    4,  /* road #A */
    9,  /* road #B */
    13, /* road #C */
    18, /* road #D */
    0,  /* road #1 */
    5,  /* road #2 */
    9,  /* road #3 */
    14  /* road #4 */
};

// WZD dseg:6EE8
struct s_BLDG_CR cityscape_cr[5][3] =
{
    {
        /* 2x2 */
        {  0, -12,  18,   0 },
        {  5, -20,  24, -13 },
        { 19, -13,  24,  -7 },
    },
    {
        /* 2x3 */
        {  0, -14,  29,   0 },
        {  4, -17,  31,  -4 },
        {  7, -21,  35,  -6 },
    },
    {
        /* 3x2 */
        {  0, -15,  19,   0 },
        {  6, -20,  23,  -6 },
        { 10, -25,  30, -11 },
    },
    {
        /* 3x3 */
        {  0, -14,  29,   0 },
        {  6, -19,  34,  -5 },
        { 11, -25,  39, -11 },
    },
    {
        /* Fortress */
        {  6, -41,  30,  -3 },
        { 11, -47,  30, -42 },
        { 25, -41,  30,  -9 },
    }

};

// WZD dseg:6EE8                                                 END:  ovr144 - Initialized Data  (Cityscape)



// WZD dseg:CA78                                                 BEGIN:  ovr144 - Uninitialized Data  (Cityscape)

// WZD dseg:CA78
int16_t new_bldg_idx;

// WZD dseg:CA7A
/*
~ int16_t cityscape_bldg_array->RC[NUM_CITYSCAPE_ROWS][NUM_CITYSCAPE_COLS];
allocated in Cityscape_Draw()
 2: house
 1: building
 0: open
-1: unuseable
-2: road
-3: tree
-4: rock
*/
struct s_CITYSCAPE_RC * cityscape_bldg_array;

// WZD dseg:CA7E
// drake178: IMG_CTY_Vtrcl_Mask@
SAMB_ptr cityscape_roads_vertical_mask_seg;

// WZD dseg:CA80
// drake178: IMG_CTY_Horz_Mask@
SAMB_ptr cityscape_roads_horizontal_mask_seg;

// WZD dseg:CA82
int16_t cityscape_background_frame;

// WZD dseg:CA84
int16_t m_cityscape_summon_city;

// WZD dseg:CA86
int16_t m_cityscape_fortress_city;

// WZD dseg:CA88
// MoO2  bldg_bitm
SAMB_ptr cityscape_pict_seg;

// WZD dseg:CA88                                                 END:  ovr144 - Uninitialized Data  (Cityscape)



/*
    WIZARDS.EXE  ovr144
*/


// WZD o144p01
void Cityscape_Build_Anim_Reset(void)
{
    cityscape_build_anim_ctr = 0;
    cityscape_build_anim_itr = 0;  // DNE in Dasm
}


// WZD o144p02
void Cityscape_Window__WIP(int16_t city_idx, int16_t xstart, int16_t ystart, int16_t bldg_idx_1, int16_t bldg_idx_2)
{
    uint32_t temp_random_seed;
    int16_t itr_players;
    int16_t city_wp;
    int16_t city_wy;
    int16_t city_wx;

    Update_Remap_Color_Range(7, 7);

    new_bldg_idx = bldg_idx_2;

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

    // ¿ unset bldg_idx_1 ... if it's invalid or normal ?
    if(bldg_idx_1 < bt_Barracks)
    {
        bldg_idx_1 = bt_NONE;
    }
    if((bldg_idx_1 > bt_MinersGuild) && (bldg_idx_1 < bt_NUM_BUILDINGS))
    {
        bldg_idx_1 = bt_NONE;
    }

    temp_random_seed = Get_Random_Seed();

    Mark_Block(_screen_seg);

    cityscape_pict_seg = Allocate_Next_Block(_screen_seg, 1250);  // 1250 PR, 20000 B

    cityscape_bldg_fields = (struct s_BLDG *)Allocate_Next_Block(_screen_seg, 176);  // 176 PR, 2816 B  ¿ 33 * 3 * 7 * 2 = 2800 ?

    cityscape_bldg_array = (struct s_CITYSCAPE_RC *)Allocate_Next_Block(_screen_seg, 44);  // 44 PR, 704   23 * 15 = 345 * 2 = 690

    city_wx = _CITIES[city_idx].wx;
    city_wy = _CITIES[city_idx].wy;
    city_wp = _CITIES[city_idx].wp;
    
    Set_Random_Seed((0x2895 + (city_wx * city_wy)));  // IDGI:  deterministic per City? Why the addition? Related to (max) wx,wy?

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

    // HERE: bldg_idx_1 is bt_NONE or city walls or a magic building
    Cityscape_Make_Buildings_Array(city_idx, bldg_idx_1);

    Cityscape_Roads_1__WIP(xstart, ystart);

    Cityscape_Draw_Buildings(city_idx, xstart, ystart, bldg_idx_1);

    Cityscape_Draw_Foreground(city_idx, xstart, ystart);


    /*
        How to systematize and make configurable, these animation cycles?
    */
    // cityscape_bldg_anim_ctr  = ((cityscape_bldg_anim_ctr  + 1) %  9);
    // cityscape_water_anim_ctr = ((cityscape_water_anim_ctr + 1) % 12);
    // cityscape_wall_anim_ctr  = ((cityscape_wall_anim_ctr  + 1) %  4);

    cityscape_bldg_anim_itr++;
    if(cityscape_bldg_anim_itr == 60)  // IDK ... (9 * F) + (F - 1) + 1 ... 
    {
        cityscape_bldg_anim_itr = 6;   // IDK ... F/F = 1
    }
    cityscape_bldg_anim_ctr = (cityscape_bldg_anim_itr / 6);  // { 1, 1, 1, 1, ..., 9, 9, 9, 9 }

    cityscape_water_anim_itr++;
    if(cityscape_water_anim_itr == 84)
    {
        cityscape_water_anim_itr = 6;
    }
    cityscape_water_anim_ctr = (cityscape_water_anim_itr / 6);

    cityscape_wall_anim_itr++;
    if(cityscape_wall_anim_itr == 60)
    {
        cityscape_wall_anim_itr = 6;
    }
    cityscape_wall_anim_ctr = (cityscape_wall_anim_itr / 6);



    Line((xstart      ), (ystart     ), (xstart + 204), (ystart     ), ST_TRANSPARENT);
    Line((xstart      ), (ystart + 95), (xstart + 204), (ystart + 95), ST_TRANSPARENT);
    Line((xstart      ), (ystart     ), (xstart      ), (ystart + 95), ST_TRANSPARENT);
    Line((xstart + 204), (ystart     ), (xstart + 204), (ystart + 95), ST_TRANSPARENT);

    if(cityscape_build_anim_ctr < 100)
    {
        cityscape_build_anim_ctr += 10;
        cityscape_build_anim_itr++;

    }

    Release_Block(_screen_seg);

}


// WZD o144p03
// drake178: sub_CD8B0()
/*

*/
void Cityscape_Roads_1__WIP(int16_t xstart, int16_t ystart)
{
    int16_t var_E = 0;
    int16_t var_C = 0;
    int16_t var_A = 0;
    int16_t var_8 = 0;
    int16_t var_6 = 0;
    int16_t itr2 = 0;
    int16_t var_2 = 0;
    int16_t itr1 = 0;  // _DI_
    int16_t n_roads = 0;  // _SI_



    /* IDK  #1 */  for(itr1 =  0; itr1 <  4; itr1++) { if((cityscape_bldg_array->RC[ 1][itr1] > 0                                            )) { for(itr2 =  0; itr2 <  4; itr2++) { cityscape_bldg_array->RC[ 0][itr2] = -5; } itr1 = 1000; } }
    /* IDK  #2 */  for(itr1 =  5; itr1 <  9; itr1++) { if((cityscape_bldg_array->RC[ 1][itr1] > 0                                            )) { for(itr2 =  5; itr2 <  9; itr2++) { cityscape_bldg_array->RC[ 0][itr2] = -5; } itr1 = 1000; } }
    /* IDK  #3 */  for(itr1 = 10; itr1 < 13; itr1++) { if((cityscape_bldg_array->RC[ 1][itr1] > 0                                            )) { for(itr2 = 10; itr2 < 13; itr2++) { cityscape_bldg_array->RC[ 0][itr2] = -5; } itr1 = 1000; } }
    /* IDK  #4 */  for(itr1 = 14; itr1 < 18; itr1++) { if((cityscape_bldg_array->RC[ 1][itr1] > 0                                            )) { for(itr2 = 14; itr2 < 18; itr2++) { cityscape_bldg_array->RC[ 0][itr2] = -5; } itr1 = 1000; } }

    /* IDK  #5 */  for(itr1 =  1; itr1 <  4; itr1++) { if((cityscape_bldg_array->RC[ 4][itr1] > 0) || (cityscape_bldg_array->RC[ 6][itr1] > 0)) { for(itr2 =  1; itr2 <  4; itr2++) { cityscape_bldg_array->RC[ 5][itr2] = -5; } itr1 = 1000; } }
    /* IDK  #6 */  for(itr1 =  5; itr1 <  9; itr1++) { if((cityscape_bldg_array->RC[ 4][itr1] > 0) || (cityscape_bldg_array->RC[ 6][itr1] > 0)) { for(itr2 =  5; itr2 <  9; itr2++) { cityscape_bldg_array->RC[ 5][itr2] = -5; } itr1 = 1000; } }
    /* IDK  #7 */  for(itr1 = 10; itr1 < 13; itr1++) { if((cityscape_bldg_array->RC[ 4][itr1] > 0) || (cityscape_bldg_array->RC[ 6][itr1] > 0)) { for(itr2 = 10; itr2 < 13; itr2++) { cityscape_bldg_array->RC[ 5][itr2] = -5; } itr1 = 1000; } }
    /* IDK  #8 */  for(itr1 = 19; itr1 < 23; itr1++) { if((cityscape_bldg_array->RC[ 4][itr1] > 0) || (cityscape_bldg_array->RC[ 6][itr1] > 0)) { for(itr2 = 19; itr2 < 23; itr2++) { cityscape_bldg_array->RC[ 5][itr2] = -5; } itr1 = 1000; } }

    /* IDK  #9 */  for(itr1 =  2; itr1 <  4; itr1++) { if((cityscape_bldg_array->RC[ 8][itr1] > 0) || (cityscape_bldg_array->RC[10][itr1] > 0)) { for(itr2 =  2; itr2 <  4; itr2++) { cityscape_bldg_array->RC[ 9][itr2] = -5; } itr1 = 1000; } }
    /* IDK #10 */  for(itr1 =  5; itr1 <  9; itr1++) { if((cityscape_bldg_array->RC[ 8][itr1] > 0) || (cityscape_bldg_array->RC[10][itr1] > 0)) { for(itr2 =  5; itr2 <  9; itr2++) { cityscape_bldg_array->RC[ 9][itr2] = -5; } itr1 = 1000; } }
    /* IDK #11 */  for(itr1 = 10; itr1 < 13; itr1++) { if((cityscape_bldg_array->RC[ 8][itr1] > 0) || (cityscape_bldg_array->RC[10][itr1] > 0)) { for(itr2 = 10; itr2 < 13; itr2++) { cityscape_bldg_array->RC[ 9][itr2] = -5; } itr1 = 1000; } }
    /* IDK #12 */  for(itr1 = 14; itr1 < 18; itr1++) { if((cityscape_bldg_array->RC[ 8][itr1] > 0) || (cityscape_bldg_array->RC[10][itr1] > 0)) { for(itr2 = 14; itr2 < 18; itr2++) { cityscape_bldg_array->RC[ 9][itr2] = -5; } itr1 = 1000; } }
    /* IDK #12 */  for(itr1 = 19; itr1 < 23; itr1++) { if((cityscape_bldg_array->RC[ 8][itr1] > 0) || (cityscape_bldg_array->RC[10][itr1] > 0)) { for(itr2 = 19; itr2 < 23; itr2++) { cityscape_bldg_array->RC[ 9][itr2] = -5; } itr1 = 1000; } }

    /* IDK #13 */  for(itr1 =  5; itr1 <  9; itr1++) { if((cityscape_bldg_array->RC[13][itr1] > 0)                                            ) { for(itr2 =  5; itr2 <  9; itr2++) { cityscape_bldg_array->RC[14][itr2] = -5; } itr1 = 1000; } }
    /* IDK #14 */  for(itr1 = 10; itr1 < 13; itr1++) { if((cityscape_bldg_array->RC[13][itr1] > 0)                                            ) { for(itr2 = 10; itr2 < 13; itr2++) { cityscape_bldg_array->RC[14][itr2] = -5; } itr1 = 1000; } }
    /* IDK #15 */  for(itr1 = 14; itr1 < 18; itr1++) { if((cityscape_bldg_array->RC[13][itr1] > 0)                                            ) { for(itr2 = 14; itr2 < 18; itr2++) { cityscape_bldg_array->RC[14][itr2] = -5; } itr1 = 1000; } }
    /* IDK #16 */  for(itr1 = 19; itr1 < 23; itr1++) { if((cityscape_bldg_array->RC[13][itr1] > 0)                                            ) { for(itr2 = 19; itr2 < 23; itr2++) { cityscape_bldg_array->RC[14][itr2] = -5; } itr1 = 1000; } }



    // IDA yellow #2 ... switch roads A,B,C,D #1,#2,#3
    for(itr2 = 0; itr2 < 4; itr2++)
    {
        switch(itr2)
        {
            case 0: { itr1 =  4; } break; /* road #A */
            case 1: { itr1 =  9; } break; /* road #B */
            case 2: { itr1 = 13; } break; /* road #C */
            case 3: { itr1 = 18; } break; /* road #D */
        }

        // rows 1 to 5, col 3||5, 8||10, 12||14, 17||19
        // ...for rows 1 to 5, if there is a building on either side of the column, mask off the column
        // ...but, only down to the first match
        for(var_2 =  1; var_2 <  5; var_2++) {if( (cityscape_bldg_array->RC[var_2][(itr1 - 1)] > 0) || (cityscape_bldg_array->RC[var_2][(itr1 + 1)] > 0)) { for(var_6 =  1; var_6 <  5; var_6++) { cityscape_bldg_array->RC[var_6][itr1] = -5; } var_2 = 1000; } }
        for(var_2 =  6; var_2 <  9; var_2++) {if( (cityscape_bldg_array->RC[var_2][(itr1 - 1)] > 0) || (cityscape_bldg_array->RC[var_2][(itr1 + 1)] > 0)) { for(var_6 =  6; var_6 <  9; var_6++) { cityscape_bldg_array->RC[var_6][itr1] = -5; } var_2 = 1000; } }
        for(var_2 = 10; var_2 < 14; var_2++) {if( (cityscape_bldg_array->RC[var_2][(itr1 - 1)] > 0) || (cityscape_bldg_array->RC[var_2][(itr1 + 1)] > 0)) { for(var_6 = 10; var_6 < 14; var_6++) { cityscape_bldg_array->RC[var_6][itr1] = -5; } var_2 = 1000; } }
    }



    // IDA green #2  cityroads_cr[]
    for(var_6 = 0; var_6 < 4; var_6++)
    {
        for(itr2 = 0; itr2 < 4; itr2++)
        {
            var_8 = cityscape_roads_cr[itr2];
            var_A = cityscape_roads_cr[(4 + var_6)];
            n_roads = 0;
            if( (cityscape_bldg_array->RC[(var_A - 1)][var_8] == -5) && (var_A > 0)  ) { n_roads++; }
            if( (cityscape_bldg_array->RC[(var_A + 1)][var_8] == -5) && (var_A < 14) ) { n_roads++; }
            if( (cityscape_bldg_array->RC[var_A][(var_8 - 1)] == -5) && (var_8 > 0)  ) { n_roads++; }
            if( (cityscape_bldg_array->RC[var_A][(var_8 + 1)] == -5) && (var_8 < 22) ) { n_roads++; }

            if(n_roads > 0)
            {
                if(itr2 > 0)
                {
                    if(cityscape_bldg_array->RC[var_A][(var_8 - 1)] != -5)
                    {
                        var_C = cityscape_roads_cr[(itr2 - 1)];
                        var_E = var_A;
                        n_roads = 0;
                        if(
                            (cityscape_bldg_array->RC[(var_E - 1)][var_C] == -5)
                            &&
                            (var_E > 0)
                        )
                        {
                            n_roads++;
                        }
                        if(
                            (cityscape_bldg_array->RC[(var_E + 1)][var_C] == -5)
                            &&
                            (var_E < 14)
                        )
                        {
                            n_roads++;
                        }
                        if(
                            (cityscape_bldg_array->RC[var_E][(var_C - 1)] == -5)
                            &&
                            (var_C > 0)
                        )
                        {
                            n_roads++;
                        }
                        if(
                            (cityscape_bldg_array->RC[var_E][(var_C + 1)] == -5)
                            &&
                            (var_C < 22)
                        )
                        {
                            n_roads++;
                        }
                        if(n_roads > 0)
                        {
                            for(itr1 = (var_C + 1); itr1 < var_8; itr1++)
                            {
                                cityscape_bldg_array->RC[var_A][itr1] = -5;
                            }
                        }
                    }
                }  /* if(itr2 > 0) */
                if(itr2 < 3)
                {
                    if(cityscape_bldg_array->RC[(var_A - 1)][var_8] != -5)
                    {
                        var_C = cityscape_roads_cr[(itr2 + 1)];
                        var_E = var_A;
                        n_roads = 0;
                        if(
                            (cityscape_bldg_array->RC[(var_E - 1)][var_C] == -5)
                            &&
                            (var_E > 0)
                        )
                        {
                            n_roads++;
                        }
                        if(
                            (cityscape_bldg_array->RC[(var_E + 1)][var_C] == -5)
                            &&
                            (var_E < 14)
                        )
                        {
                            n_roads++;
                        }
                        if(
                            (cityscape_bldg_array->RC[var_E][(var_C - 1)] == -5)
                            &&
                            (var_C > 0)
                        )
                        {
                            n_roads++;
                        }
                        if(
                            (cityscape_bldg_array->RC[var_E][(var_C + 1)] == -5)
                            &&
                            (var_C < 22)
                        )
                        {
                            n_roads++;
                        }
                        if(n_roads > 0)
                        {
                            for(var_2 = (var_E + 1); var_2 < var_A; var_2++)
                            {
                                cityscape_bldg_array->RC[var_2][var_8] = -5;
/* DEBUG */  if(cityscape_bldg_array->RC[ 4][18] != -2) { STU_DEBUG_BREAK(); }
/* DEBUG */  if(cityscape_bldg_array->RC[ 8][18] != -2) { STU_DEBUG_BREAK(); }
/* DEBUG */  if(cityscape_bldg_array->RC[13][18] != -2) { STU_DEBUG_BREAK(); }
                            }
                        }
                    }
                }  /* if(itr2 < 3) */
                if(var_6 > 0)
                {
                    if(cityscape_bldg_array->RC[(var_A - 1)][var_8] != -5)
                    {
                        var_C = var_8;
                        var_E = cityscape_roads_cr[(var_6 + 3)];
                        n_roads = 0;
                        if(
                            (cityscape_bldg_array->RC[(var_E - 1)][var_C] == -5)
                            &&
                            (var_E > 0)
                        )
                        {
                            n_roads++;
                        }
                        if(
                            (cityscape_bldg_array->RC[(var_E + 1)][var_C] == -5)
                            &&
                            (var_E < 14)
                        )
                        {
                            n_roads++;
                        }
                        if(
                            (cityscape_bldg_array->RC[var_E][(var_C - 1)] == -5)
                            &&
                            (var_C > 0)
                        )
                        {
                            n_roads++;
                        }
                        if(
                            (cityscape_bldg_array->RC[var_E][(var_C + 1)] == -5)
                            &&
                            (var_C < 22)
                        )
                        {
                            n_roads++;
                        }
                        if(n_roads > 0)
                        {
                            for(itr1 = (var_C + 1); itr1 < var_8; itr1++)
                            {
                                cityscape_bldg_array->RC[var_A][itr1] = -5;
/* DEBUG */  if(cityscape_bldg_array->RC[ 4][18] != -2) { STU_DEBUG_BREAK(); }
/* DEBUG */  if(cityscape_bldg_array->RC[ 8][18] != -2) { STU_DEBUG_BREAK(); }
/* DEBUG */  if(cityscape_bldg_array->RC[13][18] != -2) { STU_DEBUG_BREAK(); }
                            }
                        }
                    }
                }  /* if(var_6 > 0) */
                if(var_6 < 3)
                {
                    if(cityscape_bldg_array->RC[(var_A + 1)][var_8] != -5)
                    {
                        var_C = var_8;
                        var_E = cityscape_roads_cr[(var_6 + 5)];
                        n_roads = 0;
                        if(
                            (cityscape_bldg_array->RC[(var_E - 1)][var_C] == -5)
                            &&
                            (var_E > 0)
                        )
                        {
                            n_roads++;
                        }
                        if(
                            (cityscape_bldg_array->RC[(var_E + 1)][var_C] == -5)
                            &&
                            (var_E < 14)
                        )
                        {
                            n_roads++;
                        }
                        if(
                            (cityscape_bldg_array->RC[var_E][(var_C - 1)] == -5)
                            &&
                            (var_C > 0)
                        )
                        {
                            n_roads++;
                        }
                        if(
                            (cityscape_bldg_array->RC[var_E][(var_C + 1)] == -5)
                            &&
                            (var_C < 22)
                        )
                        {
                            n_roads++;
                        }
                        if(n_roads > 0)
                        {
                            for(var_2 = (var_A + 1); var_2 < var_E; var_2++)
                            {
                                cityscape_bldg_array->RC[var_2][var_8] = -5;
                            }
                        }
                    }
                }  /* if(var_6 < 3) */
            }  /* if(n_roads > 0) */
        }  /* for(itr2 = 0; itr2 < 4; itr2++) */
    }  /* for(var_6 = 0; var_6 < 4; var_6++) */



    /*
        BEGIN:  Intersections
    */
    {
        n_roads = 0;
        if(cityscape_bldg_array->RC[ 0][ 3] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 0][ 5] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 1][ 4] == -5) { n_roads++; }
        if(n_roads > 1) { cityscape_bldg_array->RC[ 0][ 4] = -5; }  // [ 0][ 4] intersection A1
        n_roads = 0;
        if(cityscape_bldg_array->RC[ 0][ 8] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 0][10] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 1][ 9] == -5) { n_roads++; }
        if(n_roads > 1) { cityscape_bldg_array->RC[ 0][ 9] = -5; }  // [ 0][ 9] intersection A2
        n_roads = 0;
        if(cityscape_bldg_array->RC[ 0][12] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 0][14] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 1][13] == -5) { n_roads++; }
        if(n_roads > 1) { cityscape_bldg_array->RC[ 0][13] = -5; }  // [ 0][13] intersection A3
        // NOTE:  DNE  [ 0][18] intersection A4

        n_roads = 0;
        if(cityscape_bldg_array->RC[ 5][ 3] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 5][ 5] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 6][ 4] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 4][ 4] == -5) { n_roads++; }
        if(n_roads > 1) { cityscape_bldg_array->RC[ 5][ 4] = -5; }  // [ 5][ 4] intersection B1
        n_roads = 0;
        if(cityscape_bldg_array->RC[ 5][ 8] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 5][10] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 6][ 9] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 4][ 9] == -5) { n_roads++; }
        if(n_roads > 1) { cityscape_bldg_array->RC[ 5][ 9] = -5; }  // [ 5][ 9] intersection B2
        n_roads = 0;
        if(cityscape_bldg_array->RC[ 5][12] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 5][14] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 6][13] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 4][13] == -5) { n_roads++; }
        if(n_roads > 1) { cityscape_bldg_array->RC[ 5][13] = -5; }  // [ 5][13] intersection B3
        n_roads = 0;
        if(cityscape_bldg_array->RC[ 5][17] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 5][19] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 6][18] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 4][18] == -5) { n_roads++; }  //  4,18
        if(n_roads > 1) { cityscape_bldg_array->RC[ 5][18] = -5; }  // [ 5][18] intersection B4

        n_roads = 0;
        if(cityscape_bldg_array->RC[ 9][ 3] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 9][ 5] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[10][ 4] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 8][ 4] == -5) { n_roads++; }
        if(n_roads > 1) { cityscape_bldg_array->RC[ 9][ 4] = -5; }  // [ 9][ 4] intersection C1
        n_roads = 0;
        if(cityscape_bldg_array->RC[ 9][ 8] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 9][ 10] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 10][ 9] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 8][ 9] == -5) { n_roads++; }
        if(n_roads > 1) { cityscape_bldg_array->RC[ 9][ 9] = -5; }  // [ 9][ 9] intersection C2
        n_roads = 0;
        if(cityscape_bldg_array->RC[ 9][12] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 9][14] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[10][13] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 8][13] == -5) { n_roads++; }
        if(n_roads > 1) { cityscape_bldg_array->RC[ 9][13] = -5; }  // [ 9][13] intersection C3
        n_roads = 0;
        if(cityscape_bldg_array->RC[ 9][17] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[ 9][19] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[10][18] == -5) { n_roads++; }  // [10][18]
        if(cityscape_bldg_array->RC[ 8][18] == -5) { n_roads++; }
        if(n_roads > 1) { cityscape_bldg_array->RC[ 9][18] = -5; }  // [ 9][18] intersection C4

        n_roads = 0;
        if(cityscape_bldg_array->RC[14][ 3] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[14][ 5] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[13][ 4] == -5) { n_roads++; }
        if(n_roads > 1) { cityscape_bldg_array->RC[14][4] = -5; }  // [14][ 4] intersection D1
        n_roads = 0;
        if(cityscape_bldg_array->RC[14][ 8] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[14][10] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[13][ 9] == -5) { n_roads++; }
        if(n_roads > 1) { cityscape_bldg_array->RC[14][9] = -5; }  // [14][ 9] intersection D2
        n_roads = 0;
        if(cityscape_bldg_array->RC[14][12] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[14][14] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[13][13] == -5) { n_roads++; }
        if(n_roads > 1) { cityscape_bldg_array->RC[14][13] = -5; }  // [14][13] intersection D3
        n_roads = 0;
        if(cityscape_bldg_array->RC[14][17] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[14][19] == -5) { n_roads++; }
        if(cityscape_bldg_array->RC[13][18] == -5) { n_roads++; }
        if(n_roads > 1) { cityscape_bldg_array->RC[14][18] = -5; }  // [14][18] intersection D4
    }
    /*
        END:  Intersections
    */    



    Set_Animation_Frame(cityscape_roads_seg, cityscape_background_frame);

    Draw_Picture_To_Bitmap(cityscape_roads_seg, cityscape_pict_seg);

    // BUGBUG  Reset_Animation_Frame(cityscape_roads_horizontal_mask_seg, 0);
    Reset_Animation_Frame(cityscape_roads_horizontal_mask_seg);

    Draw_Picture_To_Bitmap(cityscape_roads_horizontal_mask_seg, GfxBuf_2400B);

    Cityscape_Roads_2__WIP(cityscape_pict_seg);

    Draw_Picture((xstart + 1), (ystart + 17), cityscape_pict_seg);

}

// WZD o144p04
/*
Cityscape_Roads_1()
    |-> Cityscape_Roads_2(cityscape_pict_seg)
*/
void Cityscape_Roads_2__WIP(SAMB_ptr cityscape_pict_seg)
{
    int16_t itr1 = 0;  // _SI_
    int16_t itr2 = 0;  // _DI_

    // #1 CR3
    for(itr1 = 0; itr1 < 4; itr1++)
    {
        if(cityscape_bldg_array->RC[0][itr1] == -2)
        {
            Cityscape_Roads_3__WIP((29 + (itr1 * 10)), 4, cityscape_pict_seg);
        }
    }

    // #2 CR4
    if(cityscape_bldg_array->RC[0][4] == -2)  // [ 0][ 4] intersection A1
    {
        Cityscape_Roads_4__WIP(69, 4, cityscape_pict_seg);
    }

    // #3 CR3
    for(itr1 = 5; itr1 < 9; itr1++)
    {
        if(cityscape_bldg_array->RC[0][itr1] == -2)
        {
            Cityscape_Roads_3__WIP((27 + (itr1 * 10)), 4, cityscape_pict_seg);
        }
    }

    // #4 CR4
    if(cityscape_bldg_array->RC[0][9] == -2)  // [ 0][ 9] intersection A2
    {
        Cityscape_Roads_4__WIP(117, 4, cityscape_pict_seg);
        if(
            (cityscape_bldg_array->RC[0][8] == -2)
            ||
            (cityscape_bldg_array->RC[0][10] == -2)
        )
        {
            Cityscape_Roads_4__WIP(120, 1, cityscape_pict_seg);
        }
    }

    // #5 CR3
    for(itr1 = 10; itr1 < 13; itr1++)
    {
        if(cityscape_bldg_array->RC[0][itr1] == -2)
        {
            Cityscape_Roads_3__WIP((25 + (itr1 * 10)), 4, cityscape_pict_seg);
        }
    }

    // #6 CR4
    if(cityscape_bldg_array->RC[0][13] == -2)
    {
        Cityscape_Roads_4__WIP(154, 4, cityscape_pict_seg);
        if(
            (cityscape_bldg_array->RC[0][12] == -2)
            ||
            (cityscape_bldg_array->RC[0][14] == -2)
        )
        {
            Cityscape_Roads_4__WIP(157, 1, cityscape_pict_seg);
        }
    }

    // #7 CR3
    for(itr1 = 14; itr1 < 18; itr1++)
    {
        if(cityscape_bldg_array->RC[0][itr1] == -2)
        {
            Cityscape_Roads_3__WIP((22 + (itr1 * 10)), 4, cityscape_pict_seg);
        }
    }

    
    // Fuck it.


    // #8 CR3
    for(itr1 = 1; itr1 < 4; itr1++)
    {
        if(cityscape_bldg_array->RC[5][itr1] == -2)
        {
            Cityscape_Roads_3__WIP((6 + (itr1 * 10)), 27, cityscape_pict_seg);
        }
    }

    // #9 CR4
    if(cityscape_bldg_array->RC[5][4] == -2)  // road #1,B
    {
        Cityscape_Roads_4__WIP(46, 27, cityscape_pict_seg);
    }

    // #10 CR3
    for(itr1 = 5; itr1 < 9; itr1++)
    {
        if(cityscape_bldg_array->RC[5][itr1] == -2)
        {
            Cityscape_Roads_3__WIP((4 + (itr1 * 10)), 27, cityscape_pict_seg);
        }
    }

    // #11 CR4
    if(cityscape_bldg_array->RC[5][9] == -2)  // road #2,B
    {
        Cityscape_Roads_4__WIP(94, 27, cityscape_pict_seg);
    }

    // #12 CR3
    for(itr1 = 10; itr1 < 13; itr1++)
    {
        if(cityscape_bldg_array->RC[5][itr1] == -2)
        {
            Cityscape_Roads_3__WIP((2 + (itr1 * 10)), 27, cityscape_pict_seg);
        }
    }

    // #13 CR4
    if(cityscape_bldg_array->RC[5][13] == -2)  // road #3,B
    {
        Cityscape_Roads_4__WIP(131, 27, cityscape_pict_seg);
    }

    // #14 CR3
    for(itr1 = 14; itr1 < 18; itr1++)
    {
        if(cityscape_bldg_array->RC[5][itr1] == -2)
        {
            Cityscape_Roads_3__WIP((10 + (itr1 * 10)), 27, cityscape_pict_seg);
        }
    }

    // #15 CR4
    if(cityscape_bldg_array->RC[5][18] == -2)  // [ 5][18] intersection B4
    {
        Cityscape_Roads_4__WIP(180, 27, cityscape_pict_seg);
    }

    // #16 CR3
    if(cityscape_bldg_array->RC[5][19] == -2)  // [ 5][19] road #B nub right
    {
        Cityscape_Roads_3__WIP(188, 27, cityscape_pict_seg);  // BUGBUG
    }
// BUGBUGFIX    for(itr1 = 19; itr1 < 22; itr1++)
// BUGBUGFIX    {
// BUGBUGFIX        if(cityscape_bldg_array->RC[5][itr1] == -2)
// BUGBUGFIX        {
// BUGBUGFIX            Cityscape_Roads_3__WIP((-3 + (itr1 * 10)), 27, cityscape_pict_seg);
// BUGBUGFIX        }
// BUGBUGFIX    }


    // #17 CR3
    for(itr1 = 3; itr1 < 4; itr1++)
    {
        if(cityscape_bldg_array->RC[9][itr1] == -2)
        {
            Cityscape_Roads_3__WIP((-12 + (itr1 * 10)), 45, cityscape_pict_seg);
        }
    }

    // #18 CR4
    if(cityscape_bldg_array->RC[9][4] == -2)
    {
        Cityscape_Roads_4__WIP(28, 45, cityscape_pict_seg);
    }

    // #19 CR3
    for(itr1 = 5; itr1 < 9; itr1++)
    {
        if(cityscape_bldg_array->RC[9][itr1] == -2)
        {
            Cityscape_Roads_3__WIP((-14 + (itr1 * 10)), 45, cityscape_pict_seg);
        }
    }

    // #20 CR4
    if(cityscape_bldg_array->RC[9][9] == -2)  // [ 9][ 9] intersection C2
    {
        Cityscape_Roads_4__WIP(76, 45, cityscape_pict_seg);
    }

    // #21 CR3
    for(itr1 = 10; itr1 < 13; itr1++)
    {
        if(cityscape_bldg_array->RC[9][itr1] == -2)
        {
            Cityscape_Roads_3__WIP((-16 + (itr1 * 10)), 45, cityscape_pict_seg);
        }
    }

    // #22 CR4
    if(cityscape_bldg_array->RC[9][13] == -2)  // [ 9][13] intersection C3
    {
        Cityscape_Roads_4__WIP(113, 45, cityscape_pict_seg);
    }

    // #23 CR3
    for(itr1 = 14; itr1 < 18; itr1++)
    {
        if(cityscape_bldg_array->RC[9][itr1] == -2)
        {
            Cityscape_Roads_3__WIP((-19 + (itr1 * 10)), 45, cityscape_pict_seg);
        }
    }

    // #24 CR4
    if(cityscape_bldg_array->RC[9][18] == -2)  // [ 9][18] intersection C4
    {
        Cityscape_Roads_4__WIP(161, 45, cityscape_pict_seg);
    }

    // #25 CR3
    for(itr1 = 19; itr1 < 22; itr1++)  // [ 9][19] road #C nub right
    {
        if(cityscape_bldg_array->RC[9][itr1] == -2)
        {
            Cityscape_Roads_3__WIP((-21 + (itr1 * 10)), 45, cityscape_pict_seg);
        }
    }

    // #26 CR4
    if(cityscape_bldg_array->RC[14][4] == -2)  // [14][ 4] intersection D1
    {
        Cityscape_Roads_4__WIP(5, 68, cityscape_pict_seg);
        Cityscape_Roads_4__WIP(0, 45, cityscape_pict_seg);
        Cityscape_Roads_4__WIP(2, 71, cityscape_pict_seg);
    }

    // #27 CR3
    for(itr1 = 5; itr1 < 9; itr1++)
    {
        if(cityscape_bldg_array->RC[14][itr1] == -2)
        {
            Cityscape_Roads_3__WIP((-37 + (itr1 * 10)), 68, cityscape_pict_seg);
        }
    }

    // #28 CR4
    if(cityscape_bldg_array->RC[14][9] == -2)  // [14][ 9] intersection D2
    {
        Cityscape_Roads_4__WIP(68, 53, cityscape_pict_seg);
        if(
            (cityscape_bldg_array->RC[14][8] == -2)
            ||
            (cityscape_bldg_array->RC[14][10] == -2)
        )
        {
            Cityscape_Roads_4__WIP(52, 71, cityscape_pict_seg);
        }
    }

    // #29 CR3
    for(itr1 = 10; itr1 < 13; itr1++)
    {
        if(cityscape_bldg_array->RC[14][itr1] == -2)
        {
            Cityscape_Roads_3__WIP((-39 + (itr1 * 10)), 68, cityscape_pict_seg);
        }
    }

    // #30 CR4
    if(cityscape_bldg_array->RC[14][13] == -2)  // [14][13] intersection D3
    {
        Cityscape_Roads_4__WIP(91, 68, cityscape_pict_seg);
        if(
            (cityscape_bldg_array->RC[14][12] == -2)
            ||
            (cityscape_bldg_array->RC[14][14] == -2)
        )
        {
            Cityscape_Roads_4__WIP(89, 71, cityscape_pict_seg);
        }
    }

    // #31 CR3
    for(itr1 = 14; itr1 < 18; itr1++)
    {
        if(cityscape_bldg_array->RC[14][itr1] == -2)
        {
            Cityscape_Roads_3__WIP((-41 + (itr1 * 10)), 68, cityscape_pict_seg);
        }
    }

    // #32 CR4
    if(cityscape_bldg_array->RC[14][18] == -2)  // [14][18] intersection D4
    {
        Cityscape_Roads_4__WIP(139, 68, cityscape_pict_seg);
        if(
            (cityscape_bldg_array->RC[14][17] == -2)
            ||
            (cityscape_bldg_array->RC[14][19] == -2)
        )
        {
            Cityscape_Roads_4__WIP(137, 71, cityscape_pict_seg);  // ~[15][18]
        }
    }

    // #33 CR3
    for(itr1 = 19; itr1 < 23; itr1++)  // [14][19] road #D nub right
    {
        if(cityscape_bldg_array->RC[14][itr1] == -2)
        {
            Cityscape_Roads_3__WIP((-44 + (itr1 * 10)), 68, cityscape_pict_seg);
        }
    }



    // #34 CR5
    for(itr2 = 1; itr2 < 5; itr2++)
    {
        if(cityscape_bldg_array->RC[itr2][4] == -2)
        {
            Cityscape_Roads_5__WIP((70 - (itr2 * 5)), (-1 + (itr2 * 5)), cityscape_pict_seg);
        }
    }

    // #35 CR5
    for(itr2 = 6; itr2 < 9; itr2++)
    {
        if(cityscape_bldg_array->RC[itr2][4] == -2)
        {
            Cityscape_Roads_5__WIP((72 - (itr2 * 5)), (-3 + (itr2 * 5)), cityscape_pict_seg);
        }
    }

    // #36 CR5
    for(itr2 = 10; itr2 < 14; itr2++)
    {
        if(cityscape_bldg_array->RC[itr2][4] == -2)
        {
            Cityscape_Roads_5__WIP((74 - (itr2 * 5)), (-5 + (itr2 * 5)), cityscape_pict_seg);
        }
    }

    // #37 CR5
    for(itr2 = 1; itr2 < 5; itr2++)
    {
        if(cityscape_bldg_array->RC[itr2][9] == -2)
        {
            Cityscape_Roads_5__WIP((118 - (itr2 * 5)), (-1 + (itr2 * 5)), cityscape_pict_seg);
        }
    }

    // #38 CR5
    for(itr2 = 6; itr2 < 9; itr2++)
    {
        if(cityscape_bldg_array->RC[itr2][9] == -2)
        {
            Cityscape_Roads_5__WIP((120 - (itr2 * 5)), (-3 + (itr2 * 5)), cityscape_pict_seg);
        }
    }

    // #39 CR5
    for(itr2 = 10; itr2 < 14; itr2++)
    {
        if(cityscape_bldg_array->RC[itr2][9] == -2)
        {
            Cityscape_Roads_5__WIP((122 - (itr2 * 5)), (-5 + (itr2 * 5)), cityscape_pict_seg);
        }
    }

    // #40 CR5
    for(itr2 = 1; itr2 < 5; itr2++)
    {
        if(cityscape_bldg_array->RC[itr2][13] == -2)
        {
            Cityscape_Roads_5__WIP((156 - (itr2 * 5)), (-1 + (itr2 * 5)), cityscape_pict_seg);
        }
    }

    // #41 CR5
    for(itr2 = 6; itr2 < 9; itr2++)
    {
        if(cityscape_bldg_array->RC[itr2][13] == -2)
        {
            Cityscape_Roads_5__WIP((158 - (itr2 * 5)), (-3 + (itr2 * 5)), cityscape_pict_seg);
        }
    }

    // #42 CR5
    for(itr2 = 10; itr2 < 14; itr2++)
    {
        if(cityscape_bldg_array->RC[itr2][13] == -2)
        {
            Cityscape_Roads_5__WIP((160 - (itr2 * 5)), (-5 + (itr2 * 5)), cityscape_pict_seg);
        }
    }

    // #43 CR5
    for(itr2 = 2; itr2 < 5; itr2++)
    {
        if(cityscape_bldg_array->RC[itr2][18] == -2)
        {
            Cityscape_Roads_5__WIP((204 - (itr2 * 5)), (-1 + (itr2 * 5)), cityscape_pict_seg);
        }
    }

    // #44 CR5
    for(itr2 = 6; itr2 < 9; itr2++)
    {
        if(cityscape_bldg_array->RC[itr2][18] == -2)
        {
            Cityscape_Roads_5__WIP((206 - (itr2 * 5)), (-3 + (itr2 * 5)), cityscape_pict_seg);
        }
    }

    // #45 CR5
    for(itr2 = 10; itr2 < 14; itr2++)
    {
        if(cityscape_bldg_array->RC[itr2][18] == -2)
        {
            Cityscape_Roads_5__WIP((208 - (itr2 * 5)), (-5 + (itr2 * 5)), cityscape_pict_seg);
        }
    }

}

// WZD o144p05
/*
Cityscape_Roads_2()
    |-> Cityscape_Roads_3((29 + (itr * 10)), 4, IDK_cityscape_pict_seg);
*/
void Cityscape_Roads_3__WIP(int16_t x, int16_t y, SAMB_ptr cityscape_pict_seg)
{
    uint8_t * fp_cityscape_pict_seg = 0;
    int16_t itr = 0;
    int16_t height = 0;  // _DI_
    int16_t size = 0;  // _SI_

    // DOMSDOS  height = farpeekw(cityscape_pict_seg, s_FLIC_HDR.height);
    height = GET_2B_OFS(cityscape_pict_seg, FLIC_HDR_POS_HEIGHT);

    // DOMSDOS  fp_cityscape_pict_seg = SA_MK_FP0(cityscape_pict_seg);
    fp_cityscape_pict_seg = (uint8_t *)cityscape_pict_seg;

    size = (height * x) + y + 18;

    *(fp_cityscape_pict_seg + size + FLIC_HDR_POS_WIDTH) = 0;

    size += (height - 1);

    *(fp_cityscape_pict_seg + size) = 0;

    *(fp_cityscape_pict_seg + size + 1) = 0;

    size += (height - 1);

    for(itr = 0; itr < 8; itr++)
    {

        *(fp_cityscape_pict_seg + size) = 0;

        *(fp_cityscape_pict_seg + size + 1) = 0;

        *(fp_cityscape_pict_seg + size + 2) = 0;

        size += height;

    }

    *(fp_cityscape_pict_seg + size) = 0;

    *(fp_cityscape_pict_seg + size + 1) = 0;

    size += height;

    *(fp_cityscape_pict_seg + size) = 0;

}


// WZD o144p06
/*
Cityscape_Roads_2()
    |-> Cityscape_Roads_4(69, 4, IDK_cityscape_pict_seg);
*/
void Cityscape_Roads_4__WIP(int16_t x, int16_t y, SAMB_ptr picture)
{
    uint8_t * fp_cityscape_pict_seg = 0;
    int16_t itr = 0;
    int16_t height = 0;  // _DI_
    int16_t size = 0;  // _SI_

    // DOMSDOS  height = farpeekw(cityscape_pict_seg, s_FLIC_HDR.height);
    height = GET_2B_OFS(cityscape_pict_seg, FLIC_HDR_POS_HEIGHT);

    // DOMSDOS  fp_cityscape_pict_seg = SA_MK_FP0(cityscape_pict_seg);
    fp_cityscape_pict_seg = (uint8_t *)cityscape_pict_seg;

    size = (height * x) + y + 18;

    *(fp_cityscape_pict_seg + size + FLIC_HDR_POS_WIDTH) = 0;

    size += (height - 1);

    *(fp_cityscape_pict_seg + size) = 0;

    *(fp_cityscape_pict_seg + size + 1) = 0;

    size += (height - 1);

    for(itr = 0; itr < 6; itr++)
    {

        *(fp_cityscape_pict_seg + size) = 0;

        *(fp_cityscape_pict_seg + size + 1) = 0;

        *(fp_cityscape_pict_seg + size + 2) = 0;

        size += height;

    }

    *(fp_cityscape_pict_seg + size) = 0;

    *(fp_cityscape_pict_seg + size + 1) = 0;

    size += height;

    *(fp_cityscape_pict_seg + size) = 0;

}


// WZD o144p07
/*
Cityscape_Roads_2()
    |-> Cityscape_Roads_5((70 - (5 * _DI_itr)), ((5 * _DI_itr) - 1), IDK_cityscape_pict_seg);
*/
void Cityscape_Roads_5__WIP(int16_t x, int16_t y, SAMB_ptr cityscape_pict_seg)
{
    uint8_t * fp_cityscape_pict_seg = 0;
    int16_t height = 0;  // _DI_
    int16_t size = 0;  // _SI_

    // DOMSDOS  height = farpeekw(cityscape_pict_seg, s_FLIC_HDR.height);
    height = GET_2B_OFS(cityscape_pict_seg, FLIC_HDR_POS_HEIGHT);

    // DOMSDOS  fp_cityscape_pict_seg = SA_MK_FP0(cityscape_pict_seg);
    fp_cityscape_pict_seg = (uint8_t *)cityscape_pict_seg;

    size = (height * x) + y + 23;

    *(fp_cityscape_pict_seg + size + FLIC_HDR_POS_WIDTH) = 0;

    size += (height - 1);

    *(fp_cityscape_pict_seg + size) = 0;

    *(fp_cityscape_pict_seg + size + 1) = 0;

    size += (height - 1);

    *(fp_cityscape_pict_seg + size) = 0;

    *(fp_cityscape_pict_seg + size + 1) = 0;

    *(fp_cityscape_pict_seg + size + 2) = 0;

    size += (height - 1);

    *(fp_cityscape_pict_seg + size) = 0;

    *(fp_cityscape_pict_seg + size + 1) = 0;

    *(fp_cityscape_pict_seg + size + 2) = 0;

    *(fp_cityscape_pict_seg + size + 3) = 0;

    size += (height - 1);

    *(fp_cityscape_pict_seg + size) = 0;

    *(fp_cityscape_pict_seg + size + 1) = 0;

    *(fp_cityscape_pict_seg + size + 2) = 0;

    *(fp_cityscape_pict_seg + size + 3) = 0;

    *(fp_cityscape_pict_seg + size + 4) = 0;

    size += height;

    *(fp_cityscape_pict_seg + size) = 0;

    *(fp_cityscape_pict_seg + size + 1) = 0;

    *(fp_cityscape_pict_seg + size + 2) = 0;

    *(fp_cityscape_pict_seg + size + 3) = 0;

    *(fp_cityscape_pict_seg + size + 4) = 0;

    size += height;

    *(fp_cityscape_pict_seg + size) = 0;

    *(fp_cityscape_pict_seg + size + 1) = 0;

    *(fp_cityscape_pict_seg + size + 2) = 0;

    *(fp_cityscape_pict_seg + size + 3) = 0;

    size += height;

    *(fp_cityscape_pict_seg + size) = 0;

    *(fp_cityscape_pict_seg + size + 1) = 0;

    *(fp_cityscape_pict_seg + size + 2) = 0;

    size += height;

    *(fp_cityscape_pict_seg + size) = 0;

    *(fp_cityscape_pict_seg + size + 1) = 0;

    size += height;

    *(fp_cityscape_pict_seg + size) = 0;

}


// WZD o144p08
/*

Cityscape_Window__WIP()
    |-> Cityscape_Draw_Buildings(_SI_city_idx, _DI_xstart, ystart, bldg_idx_1)

*/
void Cityscape_Draw_Buildings(int16_t city_idx, int16_t x_start, int16_t y_start, int16_t bldg_idx)
{
    SAMB_ptr src_pict_seg;
    int16_t cr_bldg_idx;
    int16_t random_shift_1;
    int16_t city_race_house_type;
    int16_t itr_row;
    int16_t itr_col;
    int16_t col_sx;  // _SI_
    int16_t row_sy;  // _DI_
    int16_t random_result = 0;  // DNE in Dasm
    int16_t itr = 0;  // DNE in Dasm

    cityscape_bldg_count = 0;

    city_race_house_type = _race_type_table[_CITIES[city_idx].race].house_type;

    /*
        BEGIN:  Buildings
    */
    for(itr_row = 1; itr_row < NUM_CITYSCAPE_ROWS; itr_row++)
    {

        for(itr_col = 0; itr_col < NUM_CITYSCAPE_COLS; itr_col++)
        {

            row_sy = ((itr_row - 1) * 5);
            if(itr_row >  4) { row_sy -= 2; }  // -2  road #1
            if(itr_row >  8) { row_sy -= 2; }  // -4  road #2
            if(itr_row > 13) { row_sy -= 2; }  // -6  road #3
            row_sy += (y_start + 27);

            col_sx = (itr_col * 10);
            if(itr_col >  3) { col_sx -= 2; }  // -2  road #A
            if(itr_col >  8) { col_sx -= 2; }  // -4  road #B
            if(itr_col > 13) { col_sx -= 2; }  // -6  road #C
            if(itr_col > 18) { col_sx -= 2; }  // -8  road #D
            col_sx += ((x_start + 27) - (row_sy - y_start - 27));


            // cr_bldg_idx = cityscape_bldg_array->RC[itr_row][itr_col];
            cr_bldg_idx = cityscape_bldg_array->RC[itr_row][itr_col];


            // Houses
            if(cr_bldg_idx == 2)
            {
                if((x_start + 195) > col_sx)
                {
                    FLIC_Draw(col_sx, (row_sy - 19), cityscape_houses_seg[((city_race_house_type * 5) + (Random(5) - 1))]);
                }
            }

            // Trees
            if(cr_bldg_idx == -3)
            {
                random_shift_1 = (Random(3) - 1);
                if((x_start + 195) > (col_sx + random_shift_1))
                {
                    FLIC_Draw((col_sx + random_shift_1), (row_sy - 6 - random_shift_1), cityscape_trees_seg[(Random(3) - 1)]);
                }
            }

            // Rocks
            if(cr_bldg_idx == -4)
            {
                random_shift_1 = (Random(3) - 1);
                if((x_start + 195) > (col_sx + random_shift_1))
                {
                    FLIC_Draw((col_sx + random_shift_1), (row_sy - 6), cityscape_rocks_seg[(Random(3) - 1)]);
                }
            }

            
            /*
                BEGIN:  Draw Buildings
                    IDA color #11 green,bright,1
            */
            if((cr_bldg_idx >= bt_Barracks) && (cr_bldg_idx <= bt_MinersGuild))  // first real building, not city walls, not magic buildings
            {

                Cityscape_Add_Bldg_To_Fields_Array(col_sx, row_sy, cr_bldg_idx, bldg_data_table[cr_bldg_idx].shape);

                if (_CITIES[city_idx].bldg_status[cr_bldg_idx] == bs_Removed)
                {
                    src_pict_seg = cityscape_rubble_seg[bldg_data_table[cr_bldg_idx].shape];  // shape: {2x2, 2x3, 3x2, 3x3}
                    Set_Animation_Frame(src_pict_seg, cityscape_bldg_anim_ctr);
                    FLIC_Set_LoopFrame_1(GfxBuf_2400B);
                    Draw_Picture_To_Bitmap(src_pict_seg, GfxBuf_2400B);
                }
                else
                {
                    Set_Animation_Frame(bldg_picts_seg[cr_bldg_idx], cityscape_bldg_anim_ctr);
                    Draw_Picture_To_Bitmap(bldg_picts_seg[cr_bldg_idx], GfxBuf_2400B);
                    if (cr_bldg_idx == new_bldg_idx)
                    {
                        Vanish_Bitmap__WIP(GfxBuf_2400B, cityscape_build_anim_ctr);
                    }
                    FLIC_Set_LoopFrame_1(GfxBuf_2400B);
                }

                Draw_Picture(col_sx, (row_sy - 32), GfxBuf_2400B);

                if((cr_bldg_idx >= bt_Shrine) && (cr_bldg_idx <= bt_Parthenon) && (_CITIES[city_idx].enchantments[EVIL_PRESENCE] != ST_FALSE))
                {
                    Set_Animation_Frame(cityscape_evilpresence_seg, cityscape_bldg_anim_ctr);
                    Draw_Picture_To_Bitmap(cityscape_evilpresence_seg, GfxBuf_2400B);
                    if(cr_bldg_idx == new_bldg_idx)
                    {
                        Vanish_Bitmap__WIP(GfxBuf_2400B, cityscape_build_anim_ctr);
                    }
                    FLIC_Set_LoopFrame_1(GfxBuf_2400B);
                    Draw_Picture(col_sx, (row_sy - 32), GfxBuf_2400B);
                }
            }
            /*
                END:  Draw Buildings
            */


            /*
                BEGIN:  Dark Rituals  (105)
            */
            if((cr_bldg_idx == bt_Dark_Rituals) && (bldg_idx != bt_Dark_Rituals))
            {
                Set_Animation_Frame(cityscape_darkrituals_seg, cityscape_bldg_anim_ctr);
                Draw_Picture_To_Bitmap(cityscape_darkrituals_seg, GfxBuf_2400B);
                if(cr_bldg_idx == new_bldg_idx)
                {
                    Vanish_Bitmap__WIP(GfxBuf_2400B, cityscape_build_anim_ctr);
                }
                FLIC_Set_LoopFrame_1(GfxBuf_2400B);
                Draw_Picture(col_sx, (row_sy - 32), GfxBuf_2400B);
                Cityscape_Add_Bldg_To_Fields_Array(col_sx, row_sy, bt_Dark_Rituals, bp_2x3);
            }
            /*
                END:  Dark Rituals  (105)
            */

            /*
                BEGIN:  Summon Circle  (100)
            */
            if((cr_bldg_idx == bt_Summoning_Circle) && (bldg_idx != bt_Summoning_Circle))
            {
                Set_Animation_Frame(cityscape_summon_circle_seg, cityscape_bldg_anim_ctr);
                Draw_Picture_To_Bitmap(cityscape_summon_circle_seg, GfxBuf_2400B);
                if(cr_bldg_idx == new_bldg_idx)
                {
                    Vanish_Bitmap__WIP(GfxBuf_2400B, cityscape_build_anim_ctr);
                }
                FLIC_Set_LoopFrame_1(GfxBuf_2400B);
                Draw_Picture(col_sx, (row_sy - 32), GfxBuf_2400B);
                Cityscape_Add_Bldg_To_Fields_Array(col_sx, row_sy, bt_Summoning_Circle, bp_2x3);
            }
            /*
                END:  Summon Circle  (100)
            */

            /*
                BEGIN:  Earth Gate  (101)
            */
            if((cr_bldg_idx == bt_Earth_Gate) && (bldg_idx != bt_Earth_Gate))
            {
                Set_Animation_Frame(cityscape_earthgate_seg, cityscape_bldg_anim_ctr);
                Draw_Picture_To_Bitmap(cityscape_earthgate_seg, GfxBuf_2400B);
                if(cr_bldg_idx == new_bldg_idx)
                {
                    Vanish_Bitmap__WIP(GfxBuf_2400B, cityscape_build_anim_ctr);
                }
                FLIC_Set_LoopFrame_1(GfxBuf_2400B);
                Draw_Picture(col_sx, (row_sy - 32), GfxBuf_2400B);
                Cityscape_Add_Bldg_To_Fields_Array(col_sx, row_sy, bt_Earth_Gate, bp_2x2);
            }
            /*
                END:  Earth Gate  (101)
            */

            /*
                BEGIN:  Stream Of Life  (102)
            */
            if((cr_bldg_idx == bt_Stream_Of_Life) && (bldg_idx != bt_Stream_Of_Life))
            {
                Set_Animation_Frame(cityscape_streamoflife_seg, cityscape_bldg_anim_ctr);
                Draw_Picture_To_Bitmap(cityscape_streamoflife_seg, GfxBuf_2400B);
                if(cr_bldg_idx == new_bldg_idx)
                {
                    Vanish_Bitmap__WIP(GfxBuf_2400B, cityscape_build_anim_ctr);
                }
                FLIC_Set_LoopFrame_1(GfxBuf_2400B);
                FLIC_Set_LoopFrame_1(GfxBuf_2400B);  // NOTE: definitely duplicated
                Draw_Picture(col_sx, (row_sy - 32), GfxBuf_2400B);
                Cityscape_Add_Bldg_To_Fields_Array(col_sx, row_sy, bt_Stream_Of_Life, bp_2x2);
            }
            /*
                END:  Stream Of Life  (102)
            */

            /*
                BEGIN:  Astral Gate  (103)
            */
            if((cr_bldg_idx == bt_Astral_Gate) && (bldg_idx != bt_Astral_Gate))
            {
                Set_Animation_Frame(cityscape_astralgate_seg, cityscape_bldg_anim_ctr);
                Draw_Picture_To_Bitmap(cityscape_astralgate_seg, GfxBuf_2400B);
                if(cr_bldg_idx == new_bldg_idx)
                {
                    Vanish_Bitmap__WIP(GfxBuf_2400B, cityscape_build_anim_ctr);
                }
                FLIC_Set_LoopFrame_1(GfxBuf_2400B);
                Draw_Picture(col_sx, (row_sy - 32), GfxBuf_2400B);
                Cityscape_Add_Bldg_To_Fields_Array(col_sx, row_sy, bt_Astral_Gate, bp_2x2);
            }
            /*
                END:  Astral Gate  (103)
            */

            /*
                BEGIN:  Fortress  (104)
            */
            if((cr_bldg_idx == bt_Fortress) && (bldg_idx != bt_Fortress))
            {
                Set_Animation_Frame(cityscape_fortress_seg, cityscape_bldg_anim_ctr);
                Draw_Picture_To_Bitmap(cityscape_fortress_seg, GfxBuf_2400B);
                if(cr_bldg_idx == new_bldg_idx)
                {
                    Vanish_Bitmap__WIP(GfxBuf_2400B, cityscape_build_anim_ctr);
                }
                FLIC_Set_LoopFrame_1(GfxBuf_2400B);
                Draw_Picture(col_sx, (row_sy - 47), GfxBuf_2400B);
                Cityscape_Add_Bldg_To_Fields_Array(col_sx, row_sy, bt_Fortress, bp_Fortress);
            }
            /*
                END:  Fortress  (104)
            */

            /*
                BEGIN:  Altar Of Battle  (106)
            */
            if((cr_bldg_idx == bt_Altar_Of_Battle) && (bldg_idx != bt_Altar_Of_Battle))
            {
                Set_Animation_Frame(cityscape_altarofbattle_seg, cityscape_bldg_anim_ctr);
                Draw_Picture_To_Bitmap(cityscape_altarofbattle_seg, GfxBuf_2400B);
                if(cr_bldg_idx == new_bldg_idx)
                {
                    Vanish_Bitmap__WIP(GfxBuf_2400B, cityscape_build_anim_ctr);
                }
                FLIC_Set_LoopFrame_1(GfxBuf_2400B);
                Draw_Picture(col_sx, (row_sy - 32), GfxBuf_2400B);
                Cityscape_Add_Bldg_To_Fields_Array(col_sx, row_sy, bt_Altar_Of_Battle, bp_2x3);
            }
            /*
                END:  Altar Of Battle  (106)
            */

        }
    }
    /*
        END:  Buildings
    */

    /*
        BEGIN:  City Walls
    */

    if(_CITIES[city_idx].bldg_status[bt_CityWalls] == bs_Built)
    {
        Set_Animation_Frame(cityscape_city_walls_seg, 0);
        Draw_Picture_To_Bitmap(cityscape_city_walls_seg, GfxBuf_2400B);
        if(cr_bldg_idx == new_bldg_idx)
        {
            Vanish_Bitmap__WIP(GfxBuf_2400B, cityscape_build_anim_ctr);
        }
        FLIC_Set_LoopFrame_1(GfxBuf_2400B);
        Draw_Picture(x_start, (y_start + 82), GfxBuf_2400B);

        cityscape_bldg_fields[cityscape_bldg_count].field_bldg_idx = bt_CityWalls;
        cityscape_bldg_fields[cityscape_bldg_count].field_x1 = x_start;
        cityscape_bldg_fields[cityscape_bldg_count].field_y1 = (y_start + 82);
        cityscape_bldg_fields[cityscape_bldg_count].field_x2 = (x_start + 204);
        cityscape_bldg_fields[cityscape_bldg_count].field_y2 = (y_start + 93);
        cityscape_bldg_fields[cityscape_bldg_count].field_print_sx = (x_start + 102);
        cityscape_bldg_fields[cityscape_bldg_count].field_print_sy = (y_start + 85);
        cityscape_bldg_count++;
    }

    /*
        END:  City Walls
    */

    // cityscape_bldg_count
    // reset/cleared in Cityscape_Draw_Buildings()
    // set/incremented in Cityscape_Add_Bldg_To_Fields_Array()
    // cityscape_bldg_fields[]
    // populated in Cityscape_Add_Bldg_To_Fields_Array()
    // allocated in Cityscape_Window__WIP()  100 * 14 * 2
    for(itr = 0; itr < cityscape_bldg_count; itr++)
    {
        bldg_idx = cityscape_bldg_fields[itr].field_bldg_idx;
        // assert((bldg_idx >= MIN_BLDG_IDX) && (bldg_idx <= MAX_BLDG_IDX))
        assert(bldg_idx >= bt_Barracks);
        assert(bldg_idx <= bt_Altar_Of_Battle);

    }

}


// WZD o144p09
// MoO2  Module: COLDRAW  Make_Bldg_Array_For_Colony_()
/*

*/
void Cityscape_Make_Buildings_Array(int16_t city_idx, int16_t bldg_idx)
{
    int16_t rows_3x3[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t cols_3x3[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t rows_2x3[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t cols_2x3[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t rows_3x2[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t cols_3x2[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t rows_2x2[14] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t cols_2x2[14] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t house_count = 0;
    int16_t bldg_status = 0;
    int16_t rock_count = 0;
    int16_t tree_count = 0;
    int16_t rnd_loc = 0;
    int16_t row = 0;
    int16_t col = 0;
    int16_t tries = 0;
    int16_t ctr_3x3 = 0;
    int16_t ctr_2x3 = 0;
    int16_t ctr_3x2 = 0;
    int16_t ctr_2x2 = 0;
    int16_t building_shape_3x3_count = 0;
    int16_t building_shape_2x3_count = 0;
    int16_t building_shape_3x2_count = 0;
    int16_t building_shape_2x2_count = 0;
    int16_t itr_bldg = 0;
    int16_t itr2 = 0;
    int16_t itr1 = 0;  // _DI_
    int16_t itr_row = 0;  // Dasm uses itr2
    int16_t itr_col = 0;  // Dasm uses itr1
    int16_t DBG_ever_is = 0;

    /*
        BEGIN:  Clear buildings array  (MoO2  Module: COLONY  Clear_Bldgs_Array_())
    */
    {
        for(itr_row = 1; itr_row < NUM_CITYSCAPE_ROWS; itr_row++)
        {
            for(itr_col = 0; itr_col < NUM_CITYSCAPE_COLS; itr_col++)
            {
                cityscape_bldg_array->RC[itr_row][itr_col] = 0;
            }
        }
    }
    /*
        BEGIN:  Clear buildings array  (MoO2  Module: COLONY  Clear_Bldgs_Array_())
    */

    /*
        BEGIN:  Grid Cell Reservations
    */
    {

        /*
            BEGIN:  Reserve Corners
        */
        {
            for(itr_row = 4; itr_row < NUM_CITYSCAPE_ROWS; itr_row++)
            {
                cityscape_bldg_array->RC[itr_row][0] = -1;
            }
            for(itr_row = 7; itr_row < NUM_CITYSCAPE_ROWS; itr_row++)
            {
                cityscape_bldg_array->RC[itr_row][1] = -1;
            }
            for(itr_row = 10; itr_row < NUM_CITYSCAPE_ROWS; itr_row++)
            {
                cityscape_bldg_array->RC[itr_row][2] = -1;
            }
            for(itr_row = 12; itr_row < NUM_CITYSCAPE_ROWS; itr_row++)
            {
                cityscape_bldg_array->RC[itr_row][3] = -1;
            }

            for(itr_col = 19; itr_col < NUM_CITYSCAPE_COLS; itr_col++)
            {
                for(itr_row = 0; itr_row < 5; itr_row++)
                {
                    cityscape_bldg_array->RC[itr_row][itr_col] = -1;
                }
            }
        }
        /*
            END:  Reserve Corners
        */

        /*
            BEGIN:  Reserve IDK#1
        */
        {
            cityscape_bldg_array->RC[6][21] = -1;
            cityscape_bldg_array->RC[6][22] = -1;
            cityscape_bldg_array->RC[7][22] = -1;
            cityscape_bldg_array->RC[0][18] = -1;
        }
        /*
            END:  Reserve IDK#1
        */

        /*
            BEGIN:  Reserve Fortress
        */
        {
            if(
                (m_cityscape_fortress_city == ST_TRUE)
                ||
                (bldg_idx == bt_Fortress)
            )
            {
                for(itr2 = 10; itr2 < 13; itr2++)
                {
                    for(itr1 = 6; itr1 < 9; itr1++)
                    {
                        cityscape_bldg_array->RC[itr1][itr2] = 1;
                    }
                }

                for(itr1 = 1; itr1 <= 2; itr1++)
                {
                    cityscape_bldg_array->RC[itr1][7] = -1;
                }

                for(itr1 = 1; itr1 <= 2; itr1++)
                {
                    cityscape_bldg_array->RC[itr1][8] = -1;
                }

                for(itr1 = 3; itr1 <= 4; itr1++)
                {
                    cityscape_bldg_array->RC[itr1][10] = -1;
                }

            }
        }
        /*
            END:  Reserve Fortress
        */

        /*
            BEGIN:  Reserve IDK#2
        */
        {
            cityscape_bldg_array->RC[7][22] = -1;
            cityscape_bldg_array->RC[8][22] = -1;
            cityscape_bldg_array->RC[7][21] = -1;
            cityscape_bldg_array->RC[8][21] = -1;
        }
        /*
            END:  Reserve IDK#2
        */

        /*
            BEGIN: Reserve Roads
        */
        {
            for(itr2 = 0; itr2 < 18; itr2++)
            {
                cityscape_bldg_array->RC[0][itr2] = -2;    // Road #1
            }

            for(itr2 = 1; itr2 < 23; itr2++)
            {
                cityscape_bldg_array->RC[5][itr2] = -2;   // Road #2
            }

            for(itr2 = 3; itr2 < 23; itr2++)
            {
                cityscape_bldg_array->RC[9][itr2] = -2;    // Road #3
            }

            for(itr2 = 4; itr2 < 23; itr2++)
            {
                cityscape_bldg_array->RC[14][itr2] = -2;   // Road #4
            }

            for(itr1 = 0; itr1 < 15; itr1++)
            {
                cityscape_bldg_array->RC[itr1][4] = -2;    // Road #A
            }

            for(itr1 = 0; itr1 < 15; itr1++)
            {
                cityscape_bldg_array->RC[itr1][9] = -2;    // Road #B
            }

            for(itr1 = 0; itr1 < 15; itr1++)
            {
                cityscape_bldg_array->RC[itr1][13] = -2;   // Road #C
            }

            for(itr1 = 1; itr1 < 15; itr1++)
            {
                cityscape_bldg_array->RC[itr1][18] = -2;   // Road #D
            }
        }
        /*
            END: Reserve Roads
        */
        
    }
    /*
        END:  Grid Cell Reservations
    */



    /*
        BEGIN:  Counts Per Building Shape
    */
    {
        building_shape_2x2_count = 0;
        building_shape_3x2_count = 0;
        building_shape_2x3_count = 0;
        building_shape_3x3_count = 0;

        for(itr_bldg = bt_Barracks; itr_bldg <= bt_MinersGuild; itr_bldg++)  /* first through last normal building */
        {

            bldg_status = _CITIES[city_idx].bldg_status[itr_bldg];

            if(
                (
                    (bldg_status < bs_Built)
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
        
            switch(bldg_data_table[itr_bldg].shape)
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
            (bldg_idx == bt_Summoning_Circle)
        )
        {
            building_shape_2x3_count++;
        }

        if(
            (_CITIES[city_idx].enchantments[EARTH_GATE] != ST_FALSE)
            ||
            (bldg_idx == bt_Earth_Gate)
        )
        {
            building_shape_2x2_count++;
        }

        if(
            (_CITIES[city_idx].enchantments[STREAM_OF_LIFE] != ST_FALSE)
            ||
            (bldg_idx == bt_Stream_Of_Life)
        )
        {
            building_shape_2x2_count++;
        }

        if(
            (_CITIES[city_idx].enchantments[ASTRAL_GATE] != ST_FALSE)
            ||
            (bldg_idx == bt_Astral_Gate)
        )
        {
            building_shape_2x2_count++;
        }

        if(
            (_CITIES[city_idx].enchantments[DARK_RITUALS] != ST_FALSE)
            ||
            (bldg_idx == bt_Dark_Rituals)
        )
        {
            building_shape_2x3_count++;
        }

        if(
            (_CITIES[city_idx].enchantments[ALTAR_OF_BATTLE] != ST_FALSE)
            ||
            (bldg_idx == bt_Altar_Of_Battle)
        )
        {
            building_shape_2x3_count++;
        }
    }
    /*
        END:  Counts Per Building Shape
    */



    /*
        BEGIN:  Reserve Port
    */
    {
        // {..., bt_ShipWrightsGuild, bt_ShipYard, bt_MaritimeGuild, ...}
        // sets *occuppied* for...
        // {{6,2},{6,3},{7,2},{7,3},{8,2},{8,3}}
        for(itr_bldg = bt_ShipWrightsGuild; itr_bldg <= bt_MaritimeGuild; itr_bldg++)
        {
            if(((_CITIES[city_idx].bldg_status[itr_bldg] >= bs_Built) || (itr_bldg == bldg_idx)))
            {
                for(itr_row = 6; itr_row < 9; itr_row++)
                {
                    for(itr_col = 2; itr_col < 4; itr_col++)
                    {
                        cityscape_bldg_array->RC[itr_row][itr_col] = 1;
                    }
                }
            }
        }
    }
    /*
        END:  Reserve Port
    */

    

    /*
        BEGIN: Building Placements
    */
    
    /*
        BEGIN:  building_shape_3x3_count
    */
    {
        rnd_loc = 3;
        itr_bldg = 0;
        tries = 0;

        while(itr_bldg < building_shape_3x3_count)
        {
            col = ((10 + Random((rnd_loc + 1))) - (rnd_loc / 2));
            row = (( 6 + Random((rnd_loc + 1))) - (rnd_loc / 2));

            if((col >= 0) && (col < (NUM_CITYSCAPE_COLS - 3)) && (row >= 0) && (row < (NUM_CITYSCAPE_ROWS - 3)))
            {
                if(
                    cityscape_bldg_array->RC[(row + 0)][(col + 0)] == 0 &&
                    cityscape_bldg_array->RC[(row + 0)][(col + 1)] == 0 &&
                    cityscape_bldg_array->RC[(row + 0)][(col + 2)] == 0 &&
                    cityscape_bldg_array->RC[(row + 1)][(col + 0)] == 0 &&
                    cityscape_bldg_array->RC[(row + 1)][(col + 1)] == 0 &&
                    cityscape_bldg_array->RC[(row + 1)][(col + 2)] == 0 &&
                    cityscape_bldg_array->RC[(row + 2)][(col + 0)] == 0 &&
                    cityscape_bldg_array->RC[(row + 2)][(col + 1)] == 0 &&
                    cityscape_bldg_array->RC[(row + 2)][(col + 2)] == 0

                )
                {
                    tries = 0;

                    cols_3x3[itr_bldg] = col;
                    rows_3x3[itr_bldg] = row;

                    itr_bldg++;

                    for(itr_row = 0; itr_row < 3; itr_row++)
                    {
                        for(itr_col = 0; itr_col < 3; itr_col++)
                        {
                            cityscape_bldg_array->RC[(row + itr_row)][(col + itr_col)] = 1;
                        }
                    }

                }

                tries++;

                if(tries > 500)
                {
                    tries = 0;
                    building_shape_3x3_count--;
                }
            }

            if(rnd_loc < 30)
            {
                rnd_loc++;
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

        rnd_loc = 3;
        itr_bldg = 0;
        tries = 0;

        while(itr_bldg < building_shape_2x3_count)
        {
            col = ((10 + Random((rnd_loc + 1))) - (rnd_loc / 2));
            row = (( 6 + Random((rnd_loc + 1))) - (rnd_loc / 2));

            if((col >= 0) && (col < (NUM_CITYSCAPE_COLS - 3)) && (row >= 0) && (row < (NUM_CITYSCAPE_ROWS - 2)))
            {
                if(
                    cityscape_bldg_array->RC[(row + 0)][(col + 0)] == 0 &&
                    cityscape_bldg_array->RC[(row + 0)][(col + 1)] == 0 &&
                    cityscape_bldg_array->RC[(row + 0)][(col + 2)] == 0 &&
                    cityscape_bldg_array->RC[(row + 1)][(col + 0)] == 0 &&
                    cityscape_bldg_array->RC[(row + 1)][(col + 1)] == 0 &&
                    cityscape_bldg_array->RC[(row + 1)][(col + 2)] == 0

                )
                {
                    tries = 0;

                    cols_2x3[itr_bldg] = col;
                    rows_2x3[itr_bldg] = row;

                    itr_bldg++;

                    for(itr1 = 0; itr1 < 2; itr1++)
                    {
                        for(itr2 = 0; itr2 < 3; itr2++)
                        {
                            cityscape_bldg_array->RC[(row + itr1)][(col + itr2)] = 1;
                        }
                    }

                }

                tries++;

                if(tries > 500)
                {
                    tries = 0;
                    building_shape_2x3_count--;
                }
            }

            if(rnd_loc < 30)
            {
                if(Random(3) == 1)
                {
                    rnd_loc++;
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

        rnd_loc = 3;
        itr_bldg = 0;
        tries = 0;

        while(itr_bldg < building_shape_3x2_count)
        {
            col = ((10 + Random((rnd_loc + 1))) - (rnd_loc / 2));
            row = (( 6 + Random((rnd_loc + 1))) - (rnd_loc / 2));

            if((col >= 0) && (col < (NUM_CITYSCAPE_COLS - 2)) && (row >= 0) && (row < (NUM_CITYSCAPE_ROWS - 3)))
            {
                if(
                    cityscape_bldg_array->RC[(row + 0)][(col + 0)] == 0 &&
                    cityscape_bldg_array->RC[(row + 0)][(col + 1)] == 0 &&
                    cityscape_bldg_array->RC[(row + 1)][(col + 0)] == 0 &&
                    cityscape_bldg_array->RC[(row + 1)][(col + 1)] == 0 &&
                    cityscape_bldg_array->RC[(row + 2)][(col + 0)] == 0 &&
                    cityscape_bldg_array->RC[(row + 2)][(col + 1)] == 0
                )
                {
                    tries = 0;

                    cols_3x2[itr_bldg] = col;
                    rows_3x2[itr_bldg] = row;

                    itr_bldg++;

                    for(itr1 = 0; itr1 < 3; itr1++)
                    {
                        for(itr2 = 0; itr2 < 2; itr2++)
                        {
                            cityscape_bldg_array->RC[(row + itr1)][(col + itr2)] = 1;
                        }
                    }

                }

                tries++;

                if(tries > 500)
                {
                    tries = 0;
                    building_shape_3x2_count--;
                }
            }

            if(rnd_loc < 30)
            {
                if(Random(3) == 1)
                {
                    rnd_loc++;
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
        rnd_loc = 3;
        itr_bldg = 0;
        tries = 0;
        while(itr_bldg < building_shape_2x2_count)
        {
            col = ((10 + Random((rnd_loc + 1))) - (rnd_loc / 2));
            row = (( 6 + Random((rnd_loc + 1))) - (rnd_loc / 2));
            if((col >= 0) && (col < (NUM_CITYSCAPE_COLS - 2)) && (row >= 0) && (row < (NUM_CITYSCAPE_ROWS - 2)))
            {
                if(
                    cityscape_bldg_array->RC[(row + 0)][(col + 0)] == 0 &&
                    cityscape_bldg_array->RC[(row + 0)][(col + 1)] == 0 &&
                    cityscape_bldg_array->RC[(row + 1)][(col + 0)] == 0 &&
                    cityscape_bldg_array->RC[(row + 1)][(col + 1)] == 0
                )
                {
                    tries = 0;
                    cols_2x2[itr_bldg] = col;
                    rows_2x2[itr_bldg] = row;
                    itr_bldg++;
                    for(itr1 = 0; itr1 < 2; itr1++)
                    {
                        for(itr2 = 0; itr2 < 2; itr2++)
                        {
                            cityscape_bldg_array->RC[(row + itr1)][(col + itr2)] = 1;
                        }
                    }
                }
                tries++;
                if(tries > 500)
                {
                    tries = 0;
                    building_shape_2x2_count--;
                }
            }
            if(rnd_loc < 30)
            {
                if(Random(3) == 1)
                {
                    rnd_loc++;
                }
            }
        }
    }
    /*
        END:  building_shape_2x2_count
    */

    /*
        BEGIN:  Houses Placement
    */
    {
        house_count = (_CITIES[city_idx].population * 8);
        if(house_count == 0)
        {
            house_count = 1;
        }
        rnd_loc = 3;
        for(itr_bldg = 0; itr_bldg < house_count; itr_bldg++)
        {
            col = ((10 + Random((rnd_loc + 1))) - (rnd_loc / 2));
            row = (( 6 + Random((rnd_loc + 1))) - (rnd_loc / 2));
            if(
                ((col >= 0) && (col < NUM_CITYSCAPE_COLS) && (row >= 0) && (row < NUM_CITYSCAPE_ROWS))
                &&
                (cityscape_bldg_array->RC[row][col] == 0)
            )
            {
                    cityscape_bldg_array->RC[row][col] = 2;
            }
            else
            {
                if(rnd_loc < 30)
                {
                    if(Random(4) == 1)
                    {
                        rnd_loc++;
                    }
                }
                if(Random(4) != 1)
                {
                    itr_bldg--;
                }
            }
        }
    }
    /*
        END:  Houses Placement
    */

    /*
        BEGIN:  Trees Placement
        BEGIN:  Rocks Placement
    */
    {
        if(
            (Square_Is_Forest(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp) == ST_TRUE)
            ||
            (_race_type_table[_CITIES[city_idx].race].house_type == 1)  /* tree house */
        )
        {
            tree_count = (80 + Random(20));
        }
        else
        {
            tree_count = (5 + Random(8) + Random(8));
        }

        if(
            (Square_Is_Mountain(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp) == ST_TRUE)
            ||
            (Square_Is_Hills(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp) == ST_TRUE)
        )
        {
            rock_count = (20 + Random(10));
        }
        else
        {
            rock_count = (Random(8) + Random(8));
        }

        rnd_loc = 6;
        for(itr_bldg = 0; itr_bldg < tree_count; itr_bldg++)
        {
            col = ((10 + Random((rnd_loc + 1))) - (rnd_loc / 2));
            row = (( 6 + Random((rnd_loc + 1))) - (rnd_loc / 2));

            if(
                ((col >= 0) && (col < NUM_CITYSCAPE_COLS) && (row >= 0) && (row < NUM_CITYSCAPE_ROWS))
                &&
                (cityscape_bldg_array->RC[row][col] == 0)
            )
            {
                cityscape_bldg_array->RC[row][col] = -3;
            }
            else
            {
                if(rnd_loc < 30)
                {
                    if(Random(4) == 1)
                    {
                        rnd_loc++;
                    }
                }
                if(Random(2) == 1)
                {
                    itr_bldg--;
                }
            }
        }
        
        rnd_loc = 6;
        for(itr_bldg = 0; itr_bldg < rock_count; itr_bldg++)
        {
            col = ((10 + Random((rnd_loc + 1))) - (rnd_loc / 2));
            row = (( 6 + Random((rnd_loc + 1))) - (rnd_loc / 2));

            if(
                ((col >= 0) && (col < NUM_CITYSCAPE_COLS) && (row >= 0) && (row < NUM_CITYSCAPE_ROWS))
                &&
                (cityscape_bldg_array->RC[row][col] == 0)
            )
            {
                cityscape_bldg_array->RC[row][col] = -4;
            }
            else
            {
                if(rnd_loc < 30)
                {
                    if(Random(4) == 1)
                    {
                        rnd_loc++;
                    }
                }
                if(Random(2) == 1)
                {
                    itr_bldg--;
                }
            }
        }
        
    }
    /*
        END:  Trees Placement
        END:  Rocks Placement
    */


    cityscape_bldg_count = 0;

    ctr_2x2 = 0;
    ctr_3x2 = 0;
    ctr_2x3 = 0;
    ctr_3x3 = 0;

    if(
        (m_cityscape_summon_city != ST_FALSE)
        ||
        (bldg_idx == bt_Summoning_Circle)
    )
    {
        if(ctr_2x3 < building_shape_2x3_count)
        {
            cityscape_bldg_array->RC[(rows_2x3[ctr_2x3] + 1)][(cols_2x3[ctr_2x3])] = bt_Summoning_Circle;
        }
        ctr_2x3++;
    }

    if(
        (_CITIES[city_idx].enchantments[EARTH_GATE] != ST_FALSE)
        ||
        (bldg_idx == bt_Earth_Gate)
    )
    {
        if(ctr_2x2 < building_shape_2x2_count)
        {
            cityscape_bldg_array->RC[(rows_2x2[ctr_2x2] + 1)][(cols_2x2[ctr_2x2])] = bt_Earth_Gate;
        }
        ctr_2x2++;
    }

    if(
        (_CITIES[city_idx].enchantments[STREAM_OF_LIFE] != ST_FALSE)
        ||
        (bldg_idx == bt_Stream_Of_Life)
    )
    {
        if(ctr_2x2 < building_shape_2x2_count)
        {
            cityscape_bldg_array->RC[(rows_2x2[ctr_2x2] + 1)][(cols_2x2[ctr_2x2])] = bt_Stream_Of_Life;
        }
        ctr_2x2++;
    }

    if(
        (_CITIES[city_idx].enchantments[ASTRAL_GATE] != ST_FALSE)
        ||
        (bldg_idx == bt_Astral_Gate)
    )
    {
        if(ctr_2x2 < building_shape_2x2_count)
        {
            cityscape_bldg_array->RC[(rows_2x2[ctr_2x2] + 1)][(cols_2x2[ctr_2x2])] = bt_Astral_Gate;
        }
        ctr_2x2++;
    }

    if(
        (_CITIES[city_idx].enchantments[ALTAR_OF_BATTLE] != ST_FALSE)
        ||
        (bldg_idx == bt_Altar_Of_Battle)
    )
    {
        if(ctr_2x3 < building_shape_2x3_count)
        {
            cityscape_bldg_array->RC[(rows_2x3[ctr_2x3] + 1)][(cols_2x3[ctr_2x3])] = bt_Altar_Of_Battle;
        }
        ctr_2x3++;
    }

    if(
        (_CITIES[city_idx].enchantments[DARK_RITUALS] != ST_FALSE)
        ||
        (bldg_idx == bt_Dark_Rituals)
    )
    {
        if(ctr_2x3 < building_shape_2x3_count)
        {
            cityscape_bldg_array->RC[(rows_2x3[ctr_2x3] + 1)][(cols_2x3[ctr_2x3])] = bt_Dark_Rituals;
        }
        ctr_2x3++;
    }


    for(itr_bldg = bt_Barracks; itr_bldg <= bt_MinersGuild; itr_bldg++)
    {

        // skip, manually handled, bt_ShipWrightsGuild, bt_ShipYard, bt_MaritimeGuild
        if(((_CITIES[city_idx].bldg_status[itr_bldg] < bs_Built)&&(itr_bldg != bldg_idx))||(itr_bldg == bt_ShipYard)||(itr_bldg == bt_ShipWrightsGuild)||(itr_bldg == bt_MaritimeGuild))
            continue;


        switch(bldg_data_table[itr_bldg].shape)
        {

            case bp_2x2:
            {
                if(
                    (ctr_2x2 < building_shape_2x2_count)
                    &&
                    (_CITIES[city_idx].bldg_status[itr_bldg] >= bs_Built)
                )
                {
                    cityscape_bldg_array->RC[(rows_2x2[ctr_2x2] + 1)][(cols_2x2[ctr_2x2])] = itr_bldg;
                    ctr_2x2++;
                }
            } break;

            case bp_2x3:
            {
                if(
                    (ctr_2x3 < building_shape_2x3_count)
                    &&
                    (_CITIES[city_idx].bldg_status[itr_bldg] >= bs_Built)
                )
                {
                    cityscape_bldg_array->RC[(rows_2x3[ctr_2x3] + 1)][(cols_2x3[ctr_2x3])] = itr_bldg;
                    ctr_2x3++;
                }

            } break;

            case bp_3x2:
            {
                if(
                    (ctr_3x2 < building_shape_3x2_count)
                    &&
                    (_CITIES[city_idx].bldg_status[itr_bldg] >= bs_Built)
                )
                {
                    cityscape_bldg_array->RC[(rows_3x2[ctr_3x2] + 2)][(cols_3x2[ctr_3x2])] = itr_bldg;
                    ctr_3x2++;
                }

            } break;

            case bp_3x3:
            {
                if(
                    (ctr_3x3 < building_shape_3x3_count)
                    &&
                    (_CITIES[city_idx].bldg_status[itr_bldg] >= bs_Built)
                )
                {
                    cityscape_bldg_array->RC[(rows_3x3[ctr_3x3] + 2)][(cols_3x3[ctr_3x3])] = itr_bldg;
                    ctr_3x3++;
                }

            } break;

        }

    }


    if(m_cityscape_fortress_city != ST_FALSE)
    {
        cityscape_bldg_array->RC[8][10] = bt_Fortress;
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
            cityscape_bldg_array->RC[8][2] = itr_bldg;
        }
    }

}


// WZD o144p10
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
void Cityscape_Draw_Foreground(int16_t city_idx, int16_t xstart, int16_t ystart)
{

    if(_CITIES[city_idx].enchantments[DEATH_WARD] != ST_FALSE)
    {
        FLIC_Draw((xstart + 8), (ystart + 83), cityscape_black_ward_seg);
    }

    if(_CITIES[city_idx].enchantments[CHAOS_WARD] != ST_FALSE)
    {
        FLIC_Draw((xstart + 29), (ystart + 83), cityscape_red_ward_seg);
    }

    if(_CITIES[city_idx].enchantments[NATURE_WARD] != ST_FALSE)
    {
        FLIC_Draw((xstart + 50), (ystart + 83), cityscape_green_ward_seg);
    }

    if(_CITIES[city_idx].enchantments[LIFE_WARD] != ST_FALSE)
    {
        FLIC_Draw((xstart + 71), (ystart + 83), cityscape_white_ward_seg);
    }

    if(_CITIES[city_idx].enchantments[SORCERY_WARD] != ST_FALSE)
    {
        FLIC_Draw((xstart + 92), (ystart + 83), cityscape_blue_ward_seg);
    }


    if(_CITIES[city_idx].enchantments[NATURES_EYE] != ST_FALSE)
    {
        FLIC_Draw((xstart + 113), (ystart + 83), cityscape_natureseye_seg);
    }

    if(_CITIES[city_idx].enchantments[INSPIRATIONS] != ST_FALSE)
    {
        FLIC_Draw((xstart + 134), (ystart + 83), cityscape_inspirations_seg);
    }

    if(_CITIES[city_idx].enchantments[PROSPERITY] != ST_FALSE)
    {
        FLIC_Draw((xstart + 155), (ystart + 83), cityscape_prosperity_seg);
    }

    if(_CITIES[city_idx].enchantments[CONSECRATION] != ST_FALSE)
    {
        FLIC_Draw((xstart + 176), (ystart + 83), cityscape_consecration_seg);
    }

    if(_CITIES[city_idx].enchantments[WALL_OF_DARKNESS] != ST_FALSE)
    {
        Set_Animation_Frame(cityscape_wall_of_air_seg, cityscape_wall_anim_ctr);
        Draw_Picture_To_Bitmap(cityscape_wall_of_air_seg, cityscape_pict_seg);
        Draw_Picture(xstart, (ystart + 84), cityscape_pict_seg);
    }

    if(_CITIES[city_idx].enchantments[WALL_OF_FIRE] != ST_FALSE)
    {
        Set_Animation_Frame(cityscape_wall_of_fire_seg, cityscape_wall_anim_ctr);
        Draw_Picture_To_Bitmap(cityscape_wall_of_fire_seg, cityscape_pict_seg);
        Draw_Picture(xstart, (ystart + 84), cityscape_pict_seg);
    }


    // 'Wall of Stone'
    // if(_CITIES[city_idx].enchantments[WALL_OF_FIRE] != ST_FALSE)
    // {
    //     Set_Animation_Frame(cityscape_wall_of_stine_seg, cityscape_wall_anim_ctr);
    //     Draw_Picture_To_Bitmap(cityscape_wall_of_stine_seg, cityscape_pict_seg);
    //     Draw_Picture(xstart, (ystart + 84), cityscape_pict_seg);
    // }

}


// WZD o144p11
void Cityscape_Draw_Background(int16_t city_idx, int16_t xstart, int16_t ystart, int16_t city_wx, int16_t city_wy, int16_t city_wp)
{
    int16_t itr;

    if(_CITIES[city_idx].wp == ARCANUS_PLANE)
    {
        Set_Animation_Frame(cityscape_background_arcanus_ground_seg, cityscape_background_frame);
        FLIC_Draw(xstart, ystart, cityscape_background_arcanus_ground_seg);
    }
    else  /* (_CITIES[city_idx].wp == MYRROR_PLANE)*/
    {
        Set_Animation_Frame(cityscape_background_myrror_ground_seg, cityscape_background_frame);
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
        (Square_Is_River((city_wx - 1), (city_wy - 1), city_wp) == ST_TRUE) ||
        (Square_Is_River((city_wx - 1), city_wy, city_wp) == ST_TRUE) ||
        (Square_Is_River((city_wx - 1), (city_wy + 1), city_wp) == ST_TRUE) ||
        (Square_Is_River(city_wx, (city_wy - 1), city_wp) == ST_TRUE) ||
        (Square_Is_River(city_wx, city_wy, city_wp) == ST_TRUE) ||
        (Square_Is_River(city_wx, (city_wy + 1), city_wp) == ST_TRUE) ||
        (Square_Is_River((city_wx + 1), (city_wy - 1), city_wp) == ST_TRUE) ||
        (Square_Is_River((city_wx + 1), city_wy, city_wp) == ST_TRUE) ||
        (Square_Is_River((city_wx + 1), (city_wy + 1), city_wp) == ST_TRUE)
    )
    {
        if(city_wp == ARCANUS_PLANE)
        {
            Set_Animation_Frame(cityscape_background_arcanus_river_seg, 0);
            for(itr = 0; (cityscape_water_anim_ctr / 2) >= itr; itr++)
            {
                FLIC_Draw(xstart, ystart, cityscape_background_arcanus_river_seg);
            }
        }
        else
        {
            Set_Animation_Frame(cityscape_background_myrror_river_seg, 0);
            for(itr = 0; (cityscape_water_anim_ctr / 2) >= itr; itr++)
            {
                FLIC_Draw(xstart, ystart, cityscape_background_myrror_river_seg);
            }
        }
    }
    else if(
        (Square_Is_OceanLike((city_wx - 1), (city_wy - 1), city_wp) == ST_TRUE) ||
        (Square_Is_OceanLike((city_wx - 1), city_wy, city_wp) == ST_TRUE) ||
        (Square_Is_OceanLike((city_wx - 1), (city_wy + 1), city_wp) == ST_TRUE) ||
        (Square_Is_OceanLike(city_wx, (city_wy - 1), city_wp) == ST_TRUE) ||
        (Square_Is_OceanLike(city_wx, city_wy, city_wp) == ST_TRUE) ||
        (Square_Is_OceanLike(city_wx, (city_wy + 1), city_wp) == ST_TRUE) ||
        (Square_Is_OceanLike((city_wx + 1), (city_wy - 1), city_wp) == ST_TRUE) ||
        (Square_Is_OceanLike((city_wx + 1), city_wy, city_wp) == ST_TRUE) ||
        (Square_Is_OceanLike((city_wx + 1), (city_wy + 1), city_wp) == ST_TRUE)
    )
    {
        if(city_wp == ARCANUS_PLANE)
        {
            Set_Animation_Frame(cityscape_background_arcanus_ocean_seg, 0);
            for(itr = 0; (cityscape_water_anim_ctr / 2) >= itr; itr++)
            {
                FLIC_Draw(xstart, ystart, cityscape_background_arcanus_ocean_seg);
            }
        }
        else
        {
            Set_Animation_Frame(cityscape_background_myrror_ocean_seg, 0);
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
                (Square_Is_Mountain((city_wx - 1), (city_wy - 1), city_wp) == ST_TRUE) ||
                (Square_Is_Mountain(city_wx, (city_wy - 1), city_wp) == ST_TRUE) ||
                (Square_Is_Mountain((city_wx + 1), (city_wy - 1), city_wp) == ST_TRUE) ||
                (Square_Is_Mountain(city_wx, city_wy, city_wp) == ST_TRUE)
            )
            {
                if(city_wp == ARCANUS_PLANE)
                {
                    Reset_Animation_Frame(cityscape_background_arcanus_mountain_seg);
                    FLIC_Draw(xstart, ystart, cityscape_background_arcanus_mountain_seg);
                }
                else
                {
                    Reset_Animation_Frame(cityscape_background_myrror_mountain_seg);
                    FLIC_Draw(xstart, ystart, cityscape_background_myrror_mountain_seg);
                }
            }
            else if(
                (Square_Is_Hills((city_wx - 1), (city_wy - 1), city_wp) == ST_TRUE) ||
                (Square_Is_Hills(city_wx, (city_wy - 1), city_wp) == ST_TRUE) ||
                (Square_Is_Hills((city_wx + 1), (city_wy - 1), city_wp) == ST_TRUE) ||
                (Square_Is_Hills(city_wx, city_wy, city_wp) == ST_TRUE)
            )
            {
                if(city_wp == ARCANUS_PLANE)
                {
                    Reset_Animation_Frame(cityscape_background_arcanus_hills_seg);
                    FLIC_Draw(xstart, ystart, cityscape_background_arcanus_hills_seg);
                }
                else
                {
                    Reset_Animation_Frame(cityscape_background_myrror_hills_seg);
                    FLIC_Draw(xstart, ystart, cityscape_background_myrror_hills_seg);
                }
            }
        }
    }

}


// WZD o144p12
/*
    populates cityscape_bldg_fields[], indexed by cityscape_bldg_count

Cityscape_Add_Bldg_To_Fields_Array(col_sx, row_sy, cr_bldg_idx, bldg_data_table[cr_bldg_idx].shape);

*type*, as in 'size & position'

*/
void Cityscape_Add_Bldg_To_Fields_Array(int16_t x, int16_t y, int16_t bldg_idx, int16_t type)
{
    int16_t x2_max;
    int16_t x1_min;
    int16_t itr;  // _SI_

    assert(bldg_idx >= bt_Barracks);
    assert(bldg_idx <= bt_Altar_Of_Battle);

    x2_max = 0;
    x1_min = 1000;

    for(itr = 0; itr < 3; itr++)
    {

        cityscape_bldg_fields[cityscape_bldg_count].field_bldg_idx = bldg_idx;

        cityscape_bldg_fields[cityscape_bldg_count].field_x1 = (x + cityscape_cr[type][itr].x1);
        cityscape_bldg_fields[cityscape_bldg_count].field_y1 = (y + cityscape_cr[type][itr].y1);
        cityscape_bldg_fields[cityscape_bldg_count].field_x2 = (x + cityscape_cr[type][itr].x2);
        cityscape_bldg_fields[cityscape_bldg_count].field_y2 = (y + cityscape_cr[type][itr].y2);

        cityscape_bldg_fields[cityscape_bldg_count].field_print_sy = (y + 1);  // (row_sy + 1) ... gets used as?

        if(cityscape_bldg_fields[cityscape_bldg_count].field_x1 < x1_min)
        {
            x1_min = cityscape_bldg_fields[cityscape_bldg_count].field_x1;
        }

        if(cityscape_bldg_fields[cityscape_bldg_count].field_x2 > x2_max)
        {
            x2_max = cityscape_bldg_fields[cityscape_bldg_count].field_x2;
        }

        cityscape_bldg_count++;
    }

    // DEDU  ¿ (cityscape_bldg_count - 3), because 1st *real* building is 3 ?
    // ((x2_max + x1_min) / 2) is centering
    for(itr = (cityscape_bldg_count - 3); itr < cityscape_bldg_count; itr++)
    {
        cityscape_bldg_fields[itr].field_print_sx = (((x2_max + x1_min) / 2) - 3);
    }

}


// WZD o144p13
/*
; a wrapper for Outpost_Cityscape_Draw that sets an unused random seed and draws a border around the final image
; BUG: fails to draw the altered backgrounds of the Gaia's Blessing, Flying Fortress, Famine, and Cursed Lands enchantments
*/
void Outpost_Cityscape(int16_t city_idx, int16_t x_start, int16_t y_start)
{
    uint32_t temp_random_seed;
    int16_t city_wp;
    int16_t city_wy;
    int16_t city_wx;

    temp_random_seed = Get_Random_Seed();

    city_wx = _CITIES[city_idx].wx;
    city_wy = _CITIES[city_idx].wy;
    city_wp = _CITIES[city_idx].wp;

    Set_Random_Seed((0x2895 + (city_wx * city_wy)));  // IDGI:  deterministic per City? Why the addition? Related to (max) wx,wy?

    Outpost_Cityscape_Draw(city_idx, x_start, y_start);

    cityscape_bldg_anim_ctr = ((cityscape_bldg_anim_ctr + 1) % 10);

    Set_Random_Seed(temp_random_seed);  // BUGBUG  ; no randomness involved here

    Line(x_start, y_start, (x_start + 71), y_start, 0);
    Line(x_start, (y_start + 65), (x_start + 71), (y_start + 65), 0);
    Line(x_start, y_start, x_start, (y_start + 65), 0);
    Line((x_start + 71), y_start, (x_start + 71), (y_start + 65), 0);

}


// WZD o144p14
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
        Set_Animation_Frame(cityscape_gaiasblessing_mask_seg, 0);
        Clipped_Draw((x_start - 65), y_start, cityscape_gaiasblessing_mask_seg);
    }
    else if(_CITIES[city_idx].enchantments[FLYING_FORTRESS] != 0)
    {
        // ; BUG: this is not an actual image
        Set_Animation_Frame(cityscape_flyingfortress_mask_seg, 0);
        Clipped_Draw((x_start - 65), y_start, cityscape_flyingfortress_mask_seg);
    }
    else
    {
        Set_Animation_Frame(cityscape_background_arcanus_ground_seg, 0);
        Clipped_Draw((x_start - 65), y_start, cityscape_background_arcanus_ground_seg);
    }

    if(_CITIES[city_idx].enchantments[FAMINE] != 0)
    {
        Set_Animation_Frame(cityscape_famine_mask_seg, 0);
        Clipped_Draw((x_start - 65), y_start, cityscape_famine_mask_seg);
    }

    if(_CITIES[city_idx].enchantments[CURSED_LANDS] != 0)
    {
        Set_Animation_Frame(cityscape_cursedland_mask_seg, 0);
        Clipped_Draw((x_start - 65), y_start, cityscape_cursedland_mask_seg);
    }

    if(_CITIES[city_idx].enchantments[FLYING_FORTRESS] == 0)
    {
        if(
            (Square_Is_Mountain((city_wx - 1), (city_wy - 1), city_wp) == ST_TRUE) ||
            (Square_Is_Mountain( city_wx     , (city_wy - 1), city_wp) == ST_TRUE) ||
            (Square_Is_Mountain((city_wx + 1), (city_wy - 1), city_wp) == ST_TRUE) ||
            (Square_Is_Mountain( city_wx     ,  city_wy     , city_wp) == ST_TRUE)
        )
        {
            Set_Animation_Frame(cityscape_background_arcanus_mountain_seg, 0);
            Clipped_Draw((x_start - 65), y_start, cityscape_background_arcanus_mountain_seg);
        }
        else if(
            (Square_Is_Hills((city_wx - 1), (city_wy - 1), city_wp) == ST_TRUE) ||
            (Square_Is_Hills( city_wx     , (city_wy - 1), city_wp) == ST_TRUE) ||
            (Square_Is_Hills((city_wx + 1), (city_wy - 1), city_wp) == ST_TRUE) ||
            (Square_Is_Hills( city_wx     ,  city_wy     , city_wp) == ST_TRUE)
        )
        {
            Set_Animation_Frame(cityscape_background_arcanus_mountain_seg, 0);  // BUGBUG  c&p error?
            Clipped_Draw((x_start - 65), y_start, cityscape_background_arcanus_hills_seg);
        }

    }

    if(_CITIES[city_idx].enchantments[CLOUD_OF_SHADOW] != 0)
    {
        Set_Animation_Frame(cityscape_background_arcanus_darkcloud_seg, 0);
        for(itr_anim = 0; itr_anim < cityscape_bldg_anim_ctr; itr_anim++)
        {

            Clipped_Draw((x_start - 65), y_start, cityscape_background_arcanus_darkcloud_seg);
        }
    }

    if(_CITIES[city_idx].enchantments[HEAVENLY_LIGHT] != 0)
    {
        Set_Animation_Frame(cityscape_background_arcanus_alkar_seg, 0);
        for(itr_anim = 0; itr_anim < cityscape_bldg_anim_ctr; itr_anim++)
        {

            Clipped_Draw((x_start - 65), y_start, cityscape_background_arcanus_alkar_seg);
        }
    }

    if(_CITIES[city_idx].enchantments[CHAOS_RIFT] != 0)
    {
        Set_Animation_Frame(cityscape_background_arcanus_chaosrift_seg, 0);
        for(itr_anim = 0; itr_anim < cityscape_bldg_anim_ctr; itr_anim++)
        {

            Clipped_Draw((x_start - 65), y_start, cityscape_background_arcanus_chaosrift_seg);
        }
    }

    race_house_type = _race_type_table[_CITIES[city_idx].race].house_type;

    Set_Animation_Frame(cityscape_houses_seg[(race_house_type * 10)], 0);

    Clipped_Draw((x_start + 30), (y_start + 30), cityscape_houses_seg[(race_house_type * 10)]);

    Reset_Window();

}