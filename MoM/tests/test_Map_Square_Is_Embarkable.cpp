#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../src/Terrain.h"
#include "../src/TerrType.h"
#ifdef __cplusplus
}
#endif

#include <cstdlib>
#include <cstring>

class Map_Square_Is_Embarkable_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _world_maps = (uint8_t *)Allocate_Space(602);
        ASSERT_NE(_world_maps, nullptr);
        memset(_world_maps, 0, WORLD_SIZE * NUM_PLANES * sizeof(uint16_t));
    }

    void TearDown() override
    {
        free(_world_maps);
        _world_maps = nullptr;
    }

    void Set_Terrain(int16_t wx, int16_t wy, int16_t wp, int16_t terrain_type)
    {
        uint32_t offset = 0;

        offset = ((uint32_t)wp * WORLD_SIZE * 2) + ((uint32_t)wy * WORLD_WIDTH * 2) + ((uint32_t)wx * 2);
        _world_maps[offset + 0] = (uint8_t)(terrain_type & 0x00FF);
        _world_maps[offset + 1] = (uint8_t)((terrain_type >> 8) & 0x00FF);
    }
};

TEST_F(Map_Square_Is_Embarkable_test, BugGrass_IsNotEmbarkable)
{
    Set_Terrain(10, 10, ARCANUS_PLANE, tt_BugGrass);

    EXPECT_EQ(Map_Square_Is_Embarkable(10, 10, ARCANUS_PLANE), ST_FALSE);
}

TEST_F(Map_Square_Is_Embarkable_test, Lake_IsNotEmbarkable)
{
    Set_Terrain(10, 10, ARCANUS_PLANE, tt_Lake);

    EXPECT_EQ(Map_Square_Is_Embarkable(10, 10, ARCANUS_PLANE), ST_FALSE);
}

TEST_F(Map_Square_Is_Embarkable_test, Below_Shore11101110_IsEmbarkable)
{
    Set_Terrain(10, 10, ARCANUS_PLANE, tt_Ocean);

    EXPECT_EQ(Map_Square_Is_Embarkable(10, 10, ARCANUS_PLANE), ST_TRUE);
}

TEST_F(Map_Square_Is_Embarkable_test, Shore11101110_AndBelow_Shore10111000_IsNotEmbarkable)
{
    Set_Terrain(10, 10, ARCANUS_PLANE, _Shore11101110);

    EXPECT_EQ(Map_Square_Is_Embarkable(10, 10, ARCANUS_PLANE), ST_FALSE);
}

TEST_F(Map_Square_Is_Embarkable_test, Shore10111000_IsEmbarkable)
{
    Set_Terrain(10, 10, ARCANUS_PLANE, _Shore10111000);

    EXPECT_EQ(Map_Square_Is_Embarkable(10, 10, ARCANUS_PLANE), ST_TRUE);
}

TEST_F(Map_Square_Is_Embarkable_test, GrasslandsBoundary_IsNotEmbarkable)
{
    Set_Terrain(10, 10, ARCANUS_PLANE, tte_Grasslands);

    EXPECT_EQ(Map_Square_Is_Embarkable(10, 10, ARCANUS_PLANE), ST_FALSE);
}

TEST_F(Map_Square_Is_Embarkable_test, Shore00001R10_IsEmbarkable)
{
    Set_Terrain(10, 10, ARCANUS_PLANE, _Shore00001R10);

    EXPECT_EQ(Map_Square_Is_Embarkable(10, 10, ARCANUS_PLANE), ST_TRUE);
}

TEST_F(Map_Square_Is_Embarkable_test, River1100_3_IsNotEmbarkable)
{
    Set_Terrain(10, 10, ARCANUS_PLANE, _River1100_3);

    EXPECT_EQ(Map_Square_Is_Embarkable(10, 10, ARCANUS_PLANE), ST_FALSE);
}

TEST_F(Map_Square_Is_Embarkable_test, Shore000R0000_IsEmbarkable)
{
    Set_Terrain(10, 10, ARCANUS_PLANE, _Shore000R0000);

    EXPECT_EQ(Map_Square_Is_Embarkable(10, 10, ARCANUS_PLANE), ST_TRUE);
}

TEST_F(Map_Square_Is_Embarkable_test, RiversRange_IsNotEmbarkable)
{
    Set_Terrain(10, 10, ARCANUS_PLANE, tt_Rivers_1st);

    EXPECT_EQ(Map_Square_Is_Embarkable(10, 10, ARCANUS_PLANE), ST_FALSE);
}
