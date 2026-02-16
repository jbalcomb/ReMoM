#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/MOM_Data.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../src/MAPGEN.h"
#include "../src/TerrType.h"
#ifdef __cplusplus
}
#endif

#include <cstring>



/*
    Square_Food2_NewGame(int16_t wx, int16_t wy, int16_t wp)

    Returns the food2 value (in half-food units) for the terrain type
    at map square (wx, wy, wp), based on the TERRAIN_TYPE macro applied
    to _world_maps.

    The function uses a cascaded if/else-if structure and a switch
    statement over base terrain types.  Extended terrain types
    (shores, rivers, mountains range, hills range, desert range, etc.)
    are handled by range comparisons.

    Base terrain type food2 values (via the inner switch):
        Grasslands1..4:   3
        Forest1..3:       1
        Mountain1:        0
        Desert1..4:       0
        Swamp1..3:        0  (BUG: manual says 1/2)
        Tundra1..3:       0
        SorceryNode:      4
        NatureNode:       4  (BUG: is 5 in WZD Square_Food2)
        ChaosNode:        0
        Hills1:           1
        Volcano:          0

    Extended terrain type food2 values (via range comparisons):
        Ocean1:           0  (early return)
        Ocean2+:          0  (early return)
        BugGrass:         3  (early return)
        Shore1 range:     1
        Shore2 range:     1
        Shore3 range:     1
        RiverM/Lake:      4
        Rivers range:     4
        4WRiver range:    4
        Mountain range:   0
        Hills range:      1
        Desert range:     0
*/



class Square_Food2_NewGame_test : public ::testing::Test
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

    // Helper: set a terrain type at (wx, wy, wp) in _world_maps using 2-byte SET_TERRAIN_TYPE
    void Set_Terrain(int16_t wx, int16_t wy, int16_t wp, int16_t terrain_type)
    {
        SET_TERRAIN_TYPE(wx, wy, wp, terrain_type);
    }
};



// ============================================================================
// Base terrain types (inner switch)
// ============================================================================


// Grasslands1: food2 = 3
TEST_F(Square_Food2_NewGame_test, Grasslands1_Returns3)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_Grasslands1);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 3);
}


// Grasslands2: food2 = 3
TEST_F(Square_Food2_NewGame_test, Grasslands2_Returns3)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_Grasslands2);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 3);
}


// Grasslands3: food2 = 3
TEST_F(Square_Food2_NewGame_test, Grasslands3_Returns3)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_Grasslands3);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 3);
}


// Grasslands4: food2 = 3
TEST_F(Square_Food2_NewGame_test, Grasslands4_Returns3)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_Grasslands4);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 3);
}


// Forest1: food2 = 1
TEST_F(Square_Food2_NewGame_test, Forest1_Returns1)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_Forest1);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 1);
}


// Forest2: food2 = 1
TEST_F(Square_Food2_NewGame_test, Forest2_Returns1)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_Forest2);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 1);
}


// Forest3: food2 = 1
TEST_F(Square_Food2_NewGame_test, Forest3_Returns1)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_Forest3);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 1);
}


// Mountain1: food2 = 0
TEST_F(Square_Food2_NewGame_test, Mountain1_Returns0)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_Mountain1);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 0);
}


// Desert1: food2 = 0
TEST_F(Square_Food2_NewGame_test, Desert1_Returns0)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_Desert1);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 0);
}


// Desert2: food2 = 0
TEST_F(Square_Food2_NewGame_test, Desert2_Returns0)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_Desert2);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 0);
}


// Swamp1: food2 = 0 (BUG: manual says 1/2)
TEST_F(Square_Food2_NewGame_test, Swamp1_Returns0)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_Swamp1);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 0);
}


// Swamp2: food2 = 0 (BUG: manual says 1/2)
TEST_F(Square_Food2_NewGame_test, Swamp2_Returns0)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_Swamp2);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 0);
}


// Tundra1: food2 = 0
TEST_F(Square_Food2_NewGame_test, Tundra1_Returns0)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_Tundra1);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 0);
}


// SorceryNode: food2 = 4
TEST_F(Square_Food2_NewGame_test, SorceryNode_Returns4)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_SorceryNode);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 4);
}


// NatureNode: food2 = 4 (BUG: is 5 in WZD Square_Food2)
TEST_F(Square_Food2_NewGame_test, NatureNode_Returns4)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_NatureNode);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 4);
}


// ChaosNode: food2 = 0
TEST_F(Square_Food2_NewGame_test, ChaosNode_Returns0)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_ChaosNode);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 0);
}


// Hills1: food2 = 1
TEST_F(Square_Food2_NewGame_test, Hills1_Returns1)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_Hills1);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 1);
}


// Volcano: food2 = 0
TEST_F(Square_Food2_NewGame_test, Volcano_Returns0)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_Volcano);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 0);
}


// ============================================================================
// Early-return terrain types
// ============================================================================


// Ocean1 (0x0): food2 = 0  (early return)
TEST_F(Square_Food2_NewGame_test, Ocean1_Returns0)
{
    // Arrange
    // _world_maps already zeroed to tt_Ocean1

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 0);
}


// Ocean2 / AnimOcean (0x259): food2 = 0  (early return)
TEST_F(Square_Food2_NewGame_test, Ocean2_Returns0)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_Ocean2);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 0);
}


// BugGrass (0x1): food2 = 3  (early return)
TEST_F(Square_Food2_NewGame_test, BugGrass_Returns3)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_BugGrass);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 3);
}


// ============================================================================
// Extended terrain type ranges (via range comparisons)
// ============================================================================


// Shore1 range (0x02..0xA1): food2 = 1
TEST_F(Square_Food2_NewGame_test, Shore1_Returns1)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_Shore1_Fst);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 1);
}


// RiverM range (0xB9..0xC4): food2 = 4
TEST_F(Square_Food2_NewGame_test, RiverM_Returns4)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, TT_RiverM_1st);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 4);
}


// Mountains range (0x103..0x112): food2 = 0
TEST_F(Square_Food2_NewGame_test, MountainRange_Returns0)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_Mountains_Fst);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 0);
}


// Hills range (0x113..0x123): food2 = 1
TEST_F(Square_Food2_NewGame_test, HillsRange_Returns1)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_Hills_Fst);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 1);
}


// Desert range (0x124..0x1C3): food2 = 0
TEST_F(Square_Food2_NewGame_test, DesertRange_Returns0)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_Desert_Fst);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 0);
}


// Shore2 range (0x1C4..0x1D3): food2 = 1
TEST_F(Square_Food2_NewGame_test, Shore2_Returns1)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_Shore2_1st);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 1);
}


// 4WRiver range (0x1D4..0x1D8): food2 = 4
TEST_F(Square_Food2_NewGame_test, FourWayRiver_Returns4)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, TT_4WRiver1);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 4);
}


// Shore3 range (0x1D9..0x258): food2 = 1
TEST_F(Square_Food2_NewGame_test, Shore3_Returns1)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, TT_Shore3_1st);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 1);
}


// Tundra range (0x25A..0x2F9): food2 = 0  (falls into >= tt_Ocean2 early return)
TEST_F(Square_Food2_NewGame_test, TundraRange_Returns0)
{
    // Arrange
    Set_Terrain(30, 20, ARCANUS_PLANE, tt_Tundra_1st);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(result, 0);
}


// ============================================================================
// Myrror plane
// ============================================================================


// Verify the function works on Myrror plane
TEST_F(Square_Food2_NewGame_test, MyrrorPlane_Grasslands_Returns3)
{
    // Arrange
    Set_Terrain(30, 20, MYRROR_PLANE, tt_Grasslands1);

    // Act
    int16_t result = Square_Food2_NewGame(30, 20, MYRROR_PLANE);

    // Assert
    EXPECT_EQ(result, 3);
}