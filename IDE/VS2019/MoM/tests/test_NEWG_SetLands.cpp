#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/MOM_Data.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/random.h"
#include "../src/MAPGEN.h"
#include "../src/TerrType.h"
#ifdef __cplusplus
}
#endif

#include <cstring>

class NEWG_SetBaseLands_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _world_maps = (uint8_t *)Allocate_Space(602);
        memset(_world_maps, 0, WORLD_SIZE * NUM_PLANES * sizeof(uint16_t));
        Seed_Random(12345);
    }

    void TearDown() override
    {
        free(_world_maps);
        _world_maps = NULL;
    }
};

TEST_F(NEWG_SetBaseLands_test, OceanRemainsOcean)
{
    // Arrange
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Ocean1);

    // Act
    NEWG_SetBaseLands__WIP(ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(GET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE), tt_Ocean1);
}

TEST_F(NEWG_SetBaseLands_test, TopRow_BecomesTundra)
{
    // Arrange - set some land
    for (int x = 0; x < WORLD_WIDTH; x++)
    {
        SET_TERRAIN_TYPE(x, WORLD_YMIN, ARCANUS_PLANE, tt_BugGrass);
    }

    // Act
    NEWG_SetBaseLands__WIP(ARCANUS_PLANE);

    // Assert
    for (int x = 0; x < WORLD_WIDTH; x++)
    {
        EXPECT_EQ(GET_TERRAIN_TYPE(x, WORLD_YMIN, ARCANUS_PLANE), tt_Tundra1)
            << "Top row x=" << x << " should be tundra";
    }
}

TEST_F(NEWG_SetBaseLands_test, BottomRow_BecomesTundra)
{
    // Arrange
    for (int x = 0; x < WORLD_WIDTH; x++)
    {
        SET_TERRAIN_TYPE(x, WORLD_YMAX, ARCANUS_PLANE, tt_BugGrass);
    }

    // Act
    NEWG_SetBaseLands__WIP(ARCANUS_PLANE);

    // Assert
    for (int x = 0; x < WORLD_WIDTH; x++)
    {
        EXPECT_EQ(GET_TERRAIN_TYPE(x, WORLD_YMAX, ARCANUS_PLANE), tt_Tundra1)
            << "Bottom row x=" << x << " should be tundra";
    }
}

TEST_F(NEWG_SetBaseLands_test, HitCount1_BecomesGrasslandOrForest)
{
    // Arrange - set some squares to hit count 1
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, 1); // tt_BugGrass

    // Act
    NEWG_SetBaseLands__WIP(ARCANUS_PLANE);

    // Assert
    int16_t terrain = GET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE);
    EXPECT_TRUE(terrain == tt_Grasslands1 || terrain == tt_Forest1)
        << "Hit count 1 should become Grasslands or Forest";
}

TEST_F(NEWG_SetBaseLands_test, HitCount2_BecomesForest)
{
    // Arrange
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, 2);

    // Act
    NEWG_SetBaseLands__WIP(ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(GET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE), tt_Forest1);
}

TEST_F(NEWG_SetBaseLands_test, HitCount3_BecomesForest)
{
    // Arrange
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, 3);

    // Act
    NEWG_SetBaseLands__WIP(ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(GET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE), tt_Forest1);
}

TEST_F(NEWG_SetBaseLands_test, HitCount4_BecomesHills)
{
    // Arrange
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, 4);

    // Act
    NEWG_SetBaseLands__WIP(ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(GET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE), tt_Hills1);
}

TEST_F(NEWG_SetBaseLands_test, HitCount5_BecomesHills)
{
    // Arrange
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, 5);

    // Act
    NEWG_SetBaseLands__WIP(ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(GET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE), tt_Hills1);
}

TEST_F(NEWG_SetBaseLands_test, HitCount6Plus_BecomesMountain)
{
    // Arrange
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, 6);
    SET_TERRAIN_TYPE(31, 20, ARCANUS_PLANE, 10);

    // Act
    NEWG_SetBaseLands__WIP(ARCANUS_PLANE);

    // Assert
    EXPECT_EQ(GET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE), tt_Mountain1);
    EXPECT_EQ(GET_TERRAIN_TYPE(31, 20, ARCANUS_PLANE), tt_Mountain1);
}

TEST_F(NEWG_SetBaseLands_test, WorksOnMyrrorPlane)
{
    // Arrange
    SET_TERRAIN_TYPE(30, 20, MYRROR_PLANE, 6);

    // Act
    NEWG_SetBaseLands__WIP(MYRROR_PLANE);

    // Assert
    EXPECT_EQ(GET_TERRAIN_TYPE(30, 20, MYRROR_PLANE), tt_Mountain1);
    EXPECT_EQ(GET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE), 0) << "Should not affect Arcanus";
}

class NEWG_SetSpecLands_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _world_maps = (uint8_t *)Allocate_Space(602);
        memset(_world_maps, 0, WORLD_SIZE * NUM_PLANES * sizeof(uint16_t));
        
        // Set up all land as grasslands
        for (int y = 0; y < WORLD_HEIGHT; y++)
        {
            for (int x = 0; x < WORLD_WIDTH; x++)
            {
                SET_TERRAIN_TYPE(x, y, ARCANUS_PLANE, tt_Grasslands1);
            }
        }
        
        Seed_Random(12345);
    }

    void TearDown() override
    {
        free(_world_maps);
        _world_maps = NULL;
    }
};

TEST_F(NEWG_SetSpecLands_test, TopRows_ConvertToTundra)
{
    // Act
    NEWG_SetSpecLands__WIP(ARCANUS_PLANE);

    // Assert - check that some squares in top rows became tundra
    int tundra_count = 0;
    for (int y = 2; y < 8; y++)
    {
        for (int x = 0; x < WORLD_WIDTH; x++)
        {
            if (GET_TERRAIN_TYPE(x, y, ARCANUS_PLANE) == tt_Tundra1)
            {
                tundra_count++;
            }
        }
    }
    EXPECT_GT(tundra_count, 0) << "Should create some tundra in top rows";
}

TEST_F(NEWG_SetSpecLands_test, BottomRows_ConvertToTundra)
{
    // Act
    NEWG_SetSpecLands__WIP(ARCANUS_PLANE);

    // Assert
    int tundra_count = 0;
    for (int y = 32; y < 38; y++)
    {
        for (int x = 0; x < WORLD_WIDTH; x++)
        {
            if (GET_TERRAIN_TYPE(x, y, ARCANUS_PLANE) == tt_Tundra1)
            {
                tundra_count++;
            }
        }
    }
    EXPECT_GT(tundra_count, 0) << "Should create some tundra in bottom rows";
}

TEST_F(NEWG_SetSpecLands_test, CreatesDesertPatches)
{
    // Arrange - set some forest for desert conversion
    for (int y = 8; y < 32; y++)
    {
        for (int x = 0; x < WORLD_WIDTH; x++)
        {
            SET_TERRAIN_TYPE(x, y, ARCANUS_PLANE, tt_Forest1);
        }
    }

    // Act
    NEWG_SetSpecLands__WIP(ARCANUS_PLANE);

    // Assert
    int desert_count = 0;
    for (int y = 0; y < WORLD_HEIGHT; y++)
    {
        for (int x = 0; x < WORLD_WIDTH; x++)
        {
            if (GET_TERRAIN_TYPE(x, y, ARCANUS_PLANE) == tt_Desert1)
            {
                desert_count++;
            }
        }
    }
    EXPECT_GT(desert_count, 0) << "Should create some desert patches";
}

TEST_F(NEWG_SetSpecLands_test, CreatesSwampPatches)
{
    // Arrange
    for (int y = 10; y < 30; y++)
    {
        for (int x = 0; x < WORLD_WIDTH; x++)
        {
            SET_TERRAIN_TYPE(x, y, ARCANUS_PLANE, tt_Forest1);
        }
    }

    // Act
    NEWG_SetSpecLands__WIP(ARCANUS_PLANE);

    // Assert
    int swamp_count = 0;
    for (int y = 0; y < WORLD_HEIGHT; y++)
    {
        for (int x = 0; x < WORLD_WIDTH; x++)
        {
            if (GET_TERRAIN_TYPE(x, y, ARCANUS_PLANE) == tt_Swamp1)
            {
                swamp_count++;
            }
        }
    }
    EXPECT_GT(swamp_count, 0) << "Should create some swamp patches";
}

TEST_F(NEWG_SetSpecLands_test, WorksOnMyrrorPlane)
{
    // Arrange
    for (int y = 0; y < WORLD_HEIGHT; y++)
    {
        for (int x = 0; x < WORLD_WIDTH; x++)
        {
            SET_TERRAIN_TYPE(x, y, MYRROR_PLANE, tt_Grasslands1);
        }
    }

    // Act
    NEWG_SetSpecLands__WIP(MYRROR_PLANE);

    // Assert - check that tundra was created
    int tundra_count = 0;
    for (int y = 0; y < WORLD_HEIGHT; y++)
    {
        for (int x = 0; x < WORLD_WIDTH; x++)
        {
            if (GET_TERRAIN_TYPE(x, y, MYRROR_PLANE) == tt_Tundra1)
            {
                tundra_count++;
            }
        }
    }
    EXPECT_GT(tundra_count, 0) << "Should create tundra on Myrror plane";
}
