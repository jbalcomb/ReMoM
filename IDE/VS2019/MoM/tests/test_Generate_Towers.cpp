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

class Generate_Towers_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _world_maps = (uint8_t *)Allocate_Space(602);
        memset(_world_maps, tt_Ocean1, WORLD_SIZE * NUM_PLANES * sizeof(uint16_t));
        
        _landmasses = (uint8_t *)Allocate_Space(300);
        memset(_landmasses, 0, WORLD_SIZE * NUM_PLANES);
        
        memset(_TOWERS, 0, sizeof(_TOWERS));
        memset(_NODES, 0, sizeof(_NODES));
        
        Seed_Random(12345);
    }

    void TearDown() override
    {
        free(_world_maps);
        _world_maps = NULL;
        free(_landmasses);
        _landmasses = NULL;
    }
};

TEST_F(Generate_Towers_test, CreatesSixTowers)
{
    // Act
    Generate_Towers();

    // Assert - verify 6 towers were created
    for (int i = 0; i < NUM_TOWERS; i++)
    {
        EXPECT_NE(_TOWERS[i].wx, 0) << "Tower " << i << " should have non-zero x coordinate";
        EXPECT_NE(_TOWERS[i].wy, 0) << "Tower " << i << " should have non-zero y coordinate";
        EXPECT_EQ(_TOWERS[i].owner_idx, ST_UNDEFINED) << "Tower " << i << " should be unowned";
    }
}

TEST_F(Generate_Towers_test, TowersAreWithinMapBounds)
{
    // Act
    Generate_Towers();

    // Assert - all towers within valid coordinates
    for (int i = 0; i < NUM_TOWERS; i++)
    {
        EXPECT_GE(_TOWERS[i].wx, 2) << "Tower " << i << " x too small";
        EXPECT_LT(_TOWERS[i].wx, 57) << "Tower " << i << " x too large";
        EXPECT_GE(_TOWERS[i].wy, 2) << "Tower " << i << " y too small";
        EXPECT_LT(_TOWERS[i].wy, 37) << "Tower " << i << " y too large";
    }
}

TEST_F(Generate_Towers_test, TowersSetsGrasslandsOnBothPlanes)
{
    // Arrange - set some land tiles
    for (int y = 0; y < WORLD_HEIGHT; y++)
    {
        for (int x = 0; x < WORLD_WIDTH; x++)
        {
            SET_TERRAIN_TYPE(x, y, ARCANUS_PLANE, tt_Grasslands1);
            SET_TERRAIN_TYPE(x, y, MYRROR_PLANE, tt_Grasslands1);
        }
    }

    // Act
    Generate_Towers();

    // Assert - tower locations have grasslands on both planes
    for (int i = 0; i < NUM_TOWERS; i++)
    {
        int16_t wx = _TOWERS[i].wx;
        int16_t wy = _TOWERS[i].wy;
        
        EXPECT_EQ(GET_TERRAIN_TYPE(wx, wy, ARCANUS_PLANE), tt_Grasslands1) 
            << "Tower " << i << " should have grasslands on Arcanus";
        EXPECT_EQ(GET_TERRAIN_TYPE(wx, wy, MYRROR_PLANE), tt_Grasslands1) 
            << "Tower " << i << " should have grasslands on Myrror";
    }
}
