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

class Square_Is_Forest_NewGame_test : public ::testing::Test
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

TEST_F(Square_Is_Forest_NewGame_test, Forest1ReturnsTrue)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Forest1);
    EXPECT_EQ(Square_Is_Forest_NewGame(30, 20, ARCANUS_PLANE), ST_TRUE);
}

TEST_F(Square_Is_Forest_NewGame_test, Forest2ReturnsTrue)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Forest2);
    EXPECT_EQ(Square_Is_Forest_NewGame(30, 20, ARCANUS_PLANE), ST_TRUE);
}

TEST_F(Square_Is_Forest_NewGame_test, Forest3ReturnsTrue)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Forest3);
    EXPECT_EQ(Square_Is_Forest_NewGame(30, 20, ARCANUS_PLANE), ST_TRUE);
}

TEST_F(Square_Is_Forest_NewGame_test, GrasslandReturnsFalse)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Grasslands1);
    EXPECT_EQ(Square_Is_Forest_NewGame(30, 20, ARCANUS_PLANE), ST_FALSE);
}

TEST_F(Square_Is_Forest_NewGame_test, OceanReturnsFalse)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Ocean1);
    EXPECT_EQ(Square_Is_Forest_NewGame(30, 20, ARCANUS_PLANE), ST_FALSE);
}

class Square_Is_Mountain_NewGame_test : public ::testing::Test
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

TEST_F(Square_Is_Mountain_NewGame_test, Mountain1ReturnsTrue)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Mountain1);
    EXPECT_EQ(Square_Is_Mountain_NewGame(30, 20, ARCANUS_PLANE), ST_TRUE);
}

TEST_F(Square_Is_Mountain_NewGame_test, GrasslandReturnsFalse)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Grasslands1);
    EXPECT_EQ(Square_Is_Mountain_NewGame(30, 20, ARCANUS_PLANE), ST_FALSE);
}

TEST_F(Square_Is_Mountain_NewGame_test, HillsReturnsFalse)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Hills1);
    EXPECT_EQ(Square_Is_Mountain_NewGame(30, 20, ARCANUS_PLANE), ST_FALSE);
}

class Square_Is_Hills_NewGame_test : public ::testing::Test
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

TEST_F(Square_Is_Hills_NewGame_test, Hills1ReturnsTrue)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Hills1);
    EXPECT_EQ(Square_Is_Hills_NewGame(30, 20, ARCANUS_PLANE), ST_TRUE);
}

TEST_F(Square_Is_Hills_NewGame_test, GrasslandReturnsFalse)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Grasslands1);
    EXPECT_EQ(Square_Is_Hills_NewGame(30, 20, ARCANUS_PLANE), ST_FALSE);
}

TEST_F(Square_Is_Hills_NewGame_test, MountainReturnsFalse)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Mountain1);
    EXPECT_EQ(Square_Is_Hills_NewGame(30, 20, ARCANUS_PLANE), ST_FALSE);
}

class Square_Is_Swamp_NewGame_test : public ::testing::Test
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

TEST_F(Square_Is_Swamp_NewGame_test, Swamp1ReturnsTrue)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Swamp1);
    EXPECT_EQ(Square_Is_Swamp_NewGame(30, 20, ARCANUS_PLANE), ST_TRUE);
}

TEST_F(Square_Is_Swamp_NewGame_test, Swamp2ReturnsTrue)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Swamp2);
    EXPECT_EQ(Square_Is_Swamp_NewGame(30, 20, ARCANUS_PLANE), ST_TRUE);
}

TEST_F(Square_Is_Swamp_NewGame_test, Swamp3ReturnsTrue)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Swamp3);
    EXPECT_EQ(Square_Is_Swamp_NewGame(30, 20, ARCANUS_PLANE), ST_TRUE);
}

TEST_F(Square_Is_Swamp_NewGame_test, GrasslandReturnsFalse)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Grasslands1);
    EXPECT_EQ(Square_Is_Swamp_NewGame(30, 20, ARCANUS_PLANE), ST_FALSE);
}

class Square_Is_Desert_NewGame_test : public ::testing::Test
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

TEST_F(Square_Is_Desert_NewGame_test, Desert1ReturnsTrue)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Desert1);
    EXPECT_EQ(Square_Is_Desert_NewGame(30, 20, ARCANUS_PLANE), ST_TRUE);
}

TEST_F(Square_Is_Desert_NewGame_test, Desert2ReturnsTrue)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Desert2);
    EXPECT_EQ(Square_Is_Desert_NewGame(30, 20, ARCANUS_PLANE), ST_TRUE);
}

TEST_F(Square_Is_Desert_NewGame_test, Desert3ReturnsTrue)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Desert3);
    EXPECT_EQ(Square_Is_Desert_NewGame(30, 20, ARCANUS_PLANE), ST_TRUE);
}

TEST_F(Square_Is_Desert_NewGame_test, Desert4ReturnsTrue)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Desert4);
    EXPECT_EQ(Square_Is_Desert_NewGame(30, 20, ARCANUS_PLANE), ST_TRUE);
}

TEST_F(Square_Is_Desert_NewGame_test, GrasslandReturnsFalse)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Grasslands1);
    EXPECT_EQ(Square_Is_Desert_NewGame(30, 20, ARCANUS_PLANE), ST_FALSE);
}

class Square_Is_Grassland_NewGame_test : public ::testing::Test
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

TEST_F(Square_Is_Grassland_NewGame_test, Grasslands1ReturnsTrue)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Grasslands1);
    EXPECT_EQ(Square_Is_Grassland_NewGame(30, 20, ARCANUS_PLANE), ST_TRUE);
}

TEST_F(Square_Is_Grassland_NewGame_test, Grasslands2ReturnsTrue)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Grasslands2);
    EXPECT_EQ(Square_Is_Grassland_NewGame(30, 20, ARCANUS_PLANE), ST_TRUE);
}

TEST_F(Square_Is_Grassland_NewGame_test, Grasslands3ReturnsTrue)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Grasslands3);
    EXPECT_EQ(Square_Is_Grassland_NewGame(30, 20, ARCANUS_PLANE), ST_TRUE);
}

TEST_F(Square_Is_Grassland_NewGame_test, Grasslands4ReturnsTrue)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Grasslands4);
    EXPECT_EQ(Square_Is_Grassland_NewGame(30, 20, ARCANUS_PLANE), ST_TRUE);
}

TEST_F(Square_Is_Grassland_NewGame_test, BugGrassReturnsTrue)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_BugGrass);
    EXPECT_EQ(Square_Is_Grassland_NewGame(30, 20, ARCANUS_PLANE), ST_TRUE);
}

TEST_F(Square_Is_Grassland_NewGame_test, ForestReturnsFalse)
{
    SET_TERRAIN_TYPE(30, 20, ARCANUS_PLANE, tt_Forest1);
    EXPECT_EQ(Square_Is_Grassland_NewGame(30, 20, ARCANUS_PLANE), ST_FALSE);
}
