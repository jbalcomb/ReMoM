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

class Build_Landmass_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _landmasses = (uint8_t *)Allocate_Space(300);
        memset(_landmasses, 0, WORLD_SIZE * NUM_PLANES);
        m_landmasses_ctr = 1;
    }

    void TearDown() override
    {
        free(_landmasses);
        _landmasses = NULL;
    }
};

TEST_F(Build_Landmass_test, AssignsNewLandmassId_WhenNoNeighbors)
{
    // Act
    Build_Landmass(ARCANUS_PLANE, 30, 20);

    // Assert
    EXPECT_EQ(GET_LANDMASS(30, 20, ARCANUS_PLANE), 1) << "Should assign landmass id 1";
    EXPECT_EQ(m_landmasses_ctr, 2) << "Counter should increment to 2";
}

TEST_F(Build_Landmass_test, IncrementsCounter_ForMultipleIsolatedSquares)
{
    // Act
    Build_Landmass(ARCANUS_PLANE, 10, 10);
    Build_Landmass(ARCANUS_PLANE, 30, 30);

    // Assert
    EXPECT_EQ(GET_LANDMASS(10, 10, ARCANUS_PLANE), 1);
    EXPECT_EQ(GET_LANDMASS(30, 30, ARCANUS_PLANE), 2);
    EXPECT_EQ(m_landmasses_ctr, 3);
}

TEST_F(Build_Landmass_test, UsesExistingId_WhenAdjacentToLandmass)
{
    // Arrange - create first landmass
    Build_Landmass(ARCANUS_PLANE, 30, 20);

    // Act - add adjacent square
    Build_Landmass(ARCANUS_PLANE, 31, 20);

    // Assert - should use same landmass id
    EXPECT_EQ(GET_LANDMASS(30, 20, ARCANUS_PLANE), 1);
    EXPECT_EQ(GET_LANDMASS(31, 20, ARCANUS_PLANE), 1);
    EXPECT_EQ(m_landmasses_ctr, 2) << "Counter should not increment for adjacent square";
}

TEST_F(Build_Landmass_test, MergesLandmasses_WhenConnectingTwo)
{
    // Arrange - create two separate landmasses
    Build_Landmass(ARCANUS_PLANE, 30, 20);
    Build_Landmass(ARCANUS_PLANE, 32, 20);
    
    int16_t id1 = GET_LANDMASS(30, 20, ARCANUS_PLANE);
    int16_t id2 = GET_LANDMASS(32, 20, ARCANUS_PLANE);
    
    ASSERT_NE(id1, id2) << "Initial landmasses should be separate";

    // Act - connect them with a square in between
    Build_Landmass(ARCANUS_PLANE, 31, 20);

    // Assert - all three should have the same landmass id (the first one)
    EXPECT_EQ(GET_LANDMASS(30, 20, ARCANUS_PLANE), id1);
    EXPECT_EQ(GET_LANDMASS(31, 20, ARCANUS_PLANE), id1);
    EXPECT_EQ(GET_LANDMASS(32, 20, ARCANUS_PLANE), id1);
}

TEST_F(Build_Landmass_test, ChecksDiagonalNeighbors)
{
    // Arrange
    Build_Landmass(ARCANUS_PLANE, 30, 20);

    // Act - add diagonal neighbor
    Build_Landmass(ARCANUS_PLANE, 31, 21);

    // Assert - should use same landmass id
    EXPECT_EQ(GET_LANDMASS(30, 20, ARCANUS_PLANE), GET_LANDMASS(31, 21, ARCANUS_PLANE));
}

TEST_F(Build_Landmass_test, WorksOnMyrrorPlane)
{
    // Act
    Build_Landmass(MYRROR_PLANE, 30, 20);

    // Assert
    EXPECT_EQ(GET_LANDMASS(30, 20, MYRROR_PLANE), 1);
    EXPECT_EQ(GET_LANDMASS(30, 20, ARCANUS_PLANE), 0) << "Should not affect Arcanus plane";
}

TEST_F(Build_Landmass_test, DoesNotModifyExistingSquare_WithSameId)
{
    // Arrange
    Build_Landmass(ARCANUS_PLANE, 30, 20);
    
    // Act - call again on same square
    Build_Landmass(ARCANUS_PLANE, 30, 20);

    // Assert - should still be id 1, counter should be 2
    EXPECT_EQ(GET_LANDMASS(30, 20, ARCANUS_PLANE), 1);
    EXPECT_EQ(m_landmasses_ctr, 2);
}

TEST_F(Build_Landmass_test, MergesMultipleLandmasses_ComplexCase)
{
    // Arrange - create a cross pattern with 4 landmasses
    Build_Landmass(ARCANUS_PLANE, 30, 19); // North
    Build_Landmass(ARCANUS_PLANE, 29, 20); // West
    Build_Landmass(ARCANUS_PLANE, 31, 20); // East
    Build_Landmass(ARCANUS_PLANE, 30, 21); // South

    // Act - fill in center to merge all four
    Build_Landmass(ARCANUS_PLANE, 30, 20);

    // Assert - all should have the same id
    int16_t center_id = GET_LANDMASS(30, 20, ARCANUS_PLANE);
    EXPECT_EQ(GET_LANDMASS(30, 19, ARCANUS_PLANE), center_id);
    EXPECT_EQ(GET_LANDMASS(29, 20, ARCANUS_PLANE), center_id);
    EXPECT_EQ(GET_LANDMASS(31, 20, ARCANUS_PLANE), center_id);
    EXPECT_EQ(GET_LANDMASS(30, 21, ARCANUS_PLANE), center_id);
}

class NODE_IsAuraUnique_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        memset(_NODES, 0, sizeof(_NODES));
    }
};

TEST_F(NODE_IsAuraUnique_test, FirstNode_ReturnsNotUnique)
{
    // Arrange
    _NODES[0].power = 5;
    _NODES[0].Aura_Xs[0] = 30;
    _NODES[0].Aura_Ys[0] = 20;
    
    // Act
    int16_t result = NODE_IsAuraUnique__WIP(0);

    // Assert
    EXPECT_EQ(result, ST_FALSE) << "First node should always be unique (no previous nodes to overlap)";
}

TEST_F(NODE_IsAuraUnique_test, NoOverlap_ReturnsNotUnique)
{
    // Arrange - two nodes with different auras
    _NODES[0].power = 3;
    _NODES[0].Aura_Xs[0] = 10;
    _NODES[0].Aura_Ys[0] = 10;
    _NODES[0].Aura_Xs[1] = 11;
    _NODES[0].Aura_Ys[1] = 10;
    
    _NODES[1].power = 3;
    _NODES[1].Aura_Xs[0] = 30;
    _NODES[1].Aura_Ys[0] = 20;
    _NODES[1].Aura_Xs[1] = 31;
    _NODES[1].Aura_Ys[1] = 20;

    // Act
    int16_t result = NODE_IsAuraUnique__WIP(1);

    // Assert
    EXPECT_EQ(result, ST_FALSE) << "Non-overlapping auras should be unique";
}

TEST_F(NODE_IsAuraUnique_test, Overlap_ReturnsUnique)
{
    // Arrange - two nodes with overlapping auras
    _NODES[0].power = 3;
    _NODES[0].Aura_Xs[0] = 30;
    _NODES[0].Aura_Ys[0] = 20;
    _NODES[0].Aura_Xs[1] = 31;
    _NODES[0].Aura_Ys[1] = 20;
    
    _NODES[1].power = 3;
    _NODES[1].Aura_Xs[0] = 30;
    _NODES[1].Aura_Ys[0] = 20; // Same as node 0's first aura square
    _NODES[1].Aura_Xs[1] = 32;
    _NODES[1].Aura_Ys[1] = 20;

    // Act
    int16_t result = NODE_IsAuraUnique__WIP(1);

    // Assert
    EXPECT_EQ(result, ST_TRUE) << "Overlapping auras should return TRUE (not unique)";
}

TEST_F(NODE_IsAuraUnique_test, PartialOverlap_ReturnsUnique)
{
    // Arrange - second aura square overlaps
    _NODES[0].power = 3;
    _NODES[0].Aura_Xs[0] = 30;
    _NODES[0].Aura_Ys[0] = 20;
    _NODES[0].Aura_Xs[1] = 31;
    _NODES[0].Aura_Ys[1] = 20;
    
    _NODES[1].power = 3;
    _NODES[1].Aura_Xs[0] = 29;
    _NODES[1].Aura_Ys[0] = 20;
    _NODES[1].Aura_Xs[1] = 31;
    _NODES[1].Aura_Ys[1] = 20; // Overlaps with node 0's second square

    // Act
    int16_t result = NODE_IsAuraUnique__WIP(1);

    // Assert
    EXPECT_EQ(result, ST_TRUE) << "Any overlap should return TRUE";
}

TEST_F(NODE_IsAuraUnique_test, ThirdNode_ChecksAllPrevious)
{
    // Arrange - three nodes, third overlaps with first
    _NODES[0].power = 2;
    _NODES[0].Aura_Xs[0] = 10;
    _NODES[0].Aura_Ys[0] = 10;
    
    _NODES[1].power = 2;
    _NODES[1].Aura_Xs[0] = 20;
    _NODES[1].Aura_Ys[0] = 20;
    
    _NODES[2].power = 2;
    _NODES[2].Aura_Xs[0] = 10;
    _NODES[2].Aura_Ys[0] = 10; // Overlaps with node 0

    // Act
    int16_t result = NODE_IsAuraUnique__WIP(2);

    // Assert
    EXPECT_EQ(result, ST_TRUE) << "Should detect overlap with any previous node";
}
