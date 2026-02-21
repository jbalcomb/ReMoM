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

class TILE_IsOcean_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _world_maps = (uint8_t *)Allocate_Space(602);
        memset(_world_maps, 0, WORLD_SIZE * NUM_PLANES * sizeof(uint16_t));
    }

    void TearDown() override
    {
        free(_world_maps);
        _world_maps = NULL;
    }
};

TEST_F(TILE_IsOcean_test, OceanTileReturnsTrue)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Ocean1);
    EXPECT_EQ(TILE_IsOcean(30, 20, ARCANUS_PLANE), ST_TRUE);
}

TEST_F(TILE_IsOcean_test, GrasslandReturnsFalse)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Grasslands1);
    EXPECT_EQ(TILE_IsOcean(30, 20, ARCANUS_PLANE), ST_FALSE);
}

TEST_F(TILE_IsOcean_test, ForestReturnsFalse)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Forest1);
    EXPECT_EQ(TILE_IsOcean(30, 20, ARCANUS_PLANE), ST_FALSE);
}

TEST_F(TILE_IsOcean_test, MountainReturnsFalse)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Mountain1);
    EXPECT_EQ(TILE_IsOcean(30, 20, ARCANUS_PLANE), ST_FALSE);
}

TEST_F(TILE_IsOcean_test, DesertReturnsFalse)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Desert1);
    EXPECT_EQ(TILE_IsOcean(30, 20, ARCANUS_PLANE), ST_FALSE);
}

TEST_F(TILE_IsOcean_test, TundraReturnsFalse)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Tundra1);
    EXPECT_EQ(TILE_IsOcean(30, 20, ARCANUS_PLANE), ST_FALSE);
}

TEST_F(TILE_IsOcean_test, SwampReturnsFalse)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Swamp1);
    EXPECT_EQ(TILE_IsOcean(30, 20, ARCANUS_PLANE), ST_FALSE);
}

TEST_F(TILE_IsOcean_test, HillsReturnsFalse)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Hills1);
    EXPECT_EQ(TILE_IsOcean(30, 20, ARCANUS_PLANE), ST_FALSE);
}

class TILE_HasNode_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        memset(_NODES, 0, sizeof(_NODES));
    }
};

TEST_F(TILE_HasNode_test, TileWithNodeReturnsTrue)
{
    _NODES[0].wx = 30;
    _NODES[0].wy = 20;
    _NODES[0].wp = ARCANUS_PLANE;
    
    EXPECT_EQ(TILE_HasNode(30, 20, ARCANUS_PLANE), ST_TRUE);
}

TEST_F(TILE_HasNode_test, TileWithoutNodeReturnsFalse)
{
    _NODES[0].wx = 10;
    _NODES[0].wy = 10;
    _NODES[0].wp = ARCANUS_PLANE;
    
    EXPECT_EQ(TILE_HasNode(30, 20, ARCANUS_PLANE), ST_FALSE);
}

TEST_F(TILE_HasNode_test, WrongPlaneReturnsFalse)
{
    _NODES[0].wx = 30;
    _NODES[0].wy = 20;
    _NODES[0].wp = MYRROR_PLANE;
    
    EXPECT_EQ(TILE_HasNode(30, 20, ARCANUS_PLANE), ST_FALSE);
}

TEST_F(TILE_HasNode_test, MultipleNodesFindsCorrectOne)
{
    _NODES[0].wx = 10;
    _NODES[0].wy = 10;
    _NODES[0].wp = ARCANUS_PLANE;
    
    _NODES[1].wx = 30;
    _NODES[1].wy = 20;
    _NODES[1].wp = ARCANUS_PLANE;
    
    _NODES[2].wx = 50;
    _NODES[2].wy = 30;
    _NODES[2].wp = ARCANUS_PLANE;
    
    EXPECT_EQ(TILE_HasNode(30, 20, ARCANUS_PLANE), ST_TRUE);
}

class TILE_HasTower_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        memset(_TOWERS, 0, sizeof(_TOWERS));
    }
};

TEST_F(TILE_HasTower_test, TileWithTowerReturnsTrue)
{
    _TOWERS[0].wx = 30;
    _TOWERS[0].wy = 20;
    
    EXPECT_EQ(TILE_HasTower(30, 20), ST_TRUE);
}

TEST_F(TILE_HasTower_test, TileWithoutTowerReturnsFalse)
{
    _TOWERS[0].wx = 10;
    _TOWERS[0].wy = 10;
    
    EXPECT_EQ(TILE_HasTower(30, 20), ST_FALSE);
}

TEST_F(TILE_HasTower_test, MultipleTowersFindsCorrectOne)
{
    _TOWERS[0].wx = 10;
    _TOWERS[0].wy = 10;
    
    _TOWERS[1].wx = 30;
    _TOWERS[1].wy = 20;
    
    _TOWERS[2].wx = 50;
    _TOWERS[2].wy = 30;
    
    EXPECT_EQ(TILE_HasTower(30, 20), ST_TRUE);
}
