#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/MOM_Data.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../src/MAPGEN.h"
#include "../src/TerrType.h"
#ifdef __cplusplus
}
#endif

#include <cstring>



/*
    City_Maximum_Size_NewGame(int16_t wx, int16_t wy, int16_t wp)

    Returns the base maximum population of a city built at (wx, wy, wp),
    calculated as the sum of food2 values for all useable city area squares
    divided by 2.

    The city area is a 5x5 diamond (minus corners) centered on the city
    square, yielding up to 21 squares (less at map edges).

    Each terrain type contributes a food2 value via Square_Food2_NewGame():
        Grasslands: 3
        Forest:     1
        Mountain:   0
        Desert:     0
        Swamp:      0  (BUG: manual says 1/2)
        Tundra:     0
        SorceryNode:4
        NatureNode: 4  (BUG: is 5 in WZD Square_Food2)
        ChaosNode:  0
        Hills:      1
        Volcano:    0
        Ocean:      0

    City_Maximum_Size_NewGame = sum(food2) / 2
*/



class City_Maximum_Size_NewGame_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Allocate _world_maps: 2 planes * 2400 tiles * 2 bytes = 9600 bytes = 600 paragraphs
        _world_maps = (uint8_t *)Allocate_Space(602);

        // Clear to Ocean (food2 = 0)
        memset(_world_maps, tt_Ocean1, WORLD_SIZE * NUM_PLANES * sizeof(uint16_t));
    }

    void TearDown() override
    {
        free(_world_maps);
        _world_maps = NULL;
    }

    // Helper: set a terrain type at (wx, wy, wp) in _world_maps
    void Set_Terrain(int16_t wx, int16_t wy, int16_t wp, int16_t terrain_type)
    {
        _world_maps[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] = (uint8_t)terrain_type;
    }
};



// All ocean: every square yields food2 = 0, so max pop = 0
TEST_F(City_Maximum_Size_NewGame_test, AllOcean_ReturnsZero)
{
    // Arrange
    for (int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for (int16_t wx = 0; wx < WORLD_WIDTH; wx++)
        {
            Set_Terrain(wx, wy, ARCANUS_PLANE, tt_Ocean1);
        }
    }

    // Act
    int16_t result = City_Maximum_Size_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 0);
}


// All Grasslands: each city area square yields food2 = 3
// At (30,20) on Arcanus, the full 21-square diamond is in bounds
// max pop = (21 * 3) / 2 = 63 / 2 = 31
/*
All Grasslands
wx              wy
   17,18,19     33
16,17,18,19,20  34
16,17,18,19,20  36
16,17,18,19,20  37
   17,18,19     38
3 + 5 + 5 + 5 + 3 = 21 squares
21 map square at 3 food per tt_Grasslands1 = 63 food total
63 / 2 = 31 max population
*/
TEST_F(City_Maximum_Size_NewGame_test, AllGrasslands_Returns31)
{
    // Arrange
    for (int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for (int16_t wx = 0; wx < WORLD_WIDTH; wx++)
        {
            Set_Terrain(wx, wy, ARCANUS_PLANE, tt_Grasslands1);
        }
    }

    // Act
    int16_t result = City_Maximum_Size_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 31);
}


// All Forest: each city area square yields food2 = 1
// max pop = (21 * 1) / 2 = 21 / 2 = 10
TEST_F(City_Maximum_Size_NewGame_test, AllForest_Returns10)
{
    // Arrange
    for (int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for (int16_t wx = 0; wx < WORLD_WIDTH; wx++)
        {
            Set_Terrain(wx, wy, ARCANUS_PLANE, tt_Forest1);
        }
    }

    // Act
    int16_t result = City_Maximum_Size_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 10);
}


// All Desert: each city area square yields food2 = 0
// max pop = 0
TEST_F(City_Maximum_Size_NewGame_test, AllDesert_ReturnsZero)
{
    // Arrange
    for (int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for (int16_t wx = 0; wx < WORLD_WIDTH; wx++)
        {
            Set_Terrain(wx, wy, ARCANUS_PLANE, tt_Desert1);
        }
    }

    // Act
    int16_t result = City_Maximum_Size_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 0);
}


// Center square is Grasslands, rest is Ocean
// max pop = (1 * 3) / 2 = 1
TEST_F(City_Maximum_Size_NewGame_test, SingleGrasslandCenter_Returns1)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_Grasslands1);

    // Act
    int16_t result = City_Maximum_Size_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 1);
}


// Myrror plane: all Hills yields food2 = 1 per square
// max pop = (21 * 1) / 2 = 10
TEST_F(City_Maximum_Size_NewGame_test, MyrrorAllHills_Returns10)
{
    // Arrange
    for (int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for (int16_t wx = 0; wx < WORLD_WIDTH; wx++)
        {
            Set_Terrain(wx, wy, MYRROR_PLANE, tt_Hills1);
        }
    }

    // Act
    int16_t result = City_Maximum_Size_NewGame(30, 20, MYRROR_PLANE);

    // Assert
    EXPECT_EQ(result, 10);
}


// Top edge of map: city at (30, 0) loses top rows of the diamond
// At y=0:  row y=-2 and y=-1 are out of bounds
// Remaining rows: y=0 (5 squares), y=1 (5 squares), y=2 (3 squares) = 13 squares
// All Grasslands: max pop = (13 * 3) / 2 = 39 / 2 = 19
TEST_F(City_Maximum_Size_NewGame_test, TopEdge_ReducedCityArea)
{
    // Arrange
    for (int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for (int16_t wx = 0; wx < WORLD_WIDTH; wx++)
        {
            Set_Terrain(wx, wy, ARCANUS_PLANE, tt_Grasslands1);
        }
    }

    // Act
    int16_t result = City_Maximum_Size_NewGame(30, 0, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 19);
}


// Mixed terrain: Sorcery Node center (food2=4), surrounded by Grasslands (food2=3)
// 1 Sorcery Node + 20 Grasslands = (4 + 60) / 2 = 32
TEST_F(City_Maximum_Size_NewGame_test, SorceryNodeCenter_MixedTerrain)
{
    // Arrange
    for (int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for (int16_t wx = 0; wx < WORLD_WIDTH; wx++)
        {
            Set_Terrain(wx, wy, ARCANUS_PLANE, tt_Grasslands1);
        }
    }
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_SorceryNode);

    // Act
    int16_t result = City_Maximum_Size_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 32);
}