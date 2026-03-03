#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/MOM_Data.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_BITS.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../src/MAPGEN.h"
#include "../src/TerrType.h"
#ifdef __cplusplus
}
#endif

#include <cstring>
#include <cstdlib>

class NEWG_SetBaseLands__WIP_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _world_maps = static_cast<uint8_t *>(malloc(WORLD_SIZE * NUM_PLANES * sizeof(uint16_t)));
        ASSERT_NE(_world_maps, nullptr);
        p_world_map = (int16_t (*)[WORLD_HEIGHT][WORLD_WIDTH])_world_maps;
        memset(_world_maps, tt_Ocean, WORLD_SIZE * NUM_PLANES * sizeof(uint16_t));
    }

    void TearDown() override
    {
        free(_world_maps);
        _world_maps = nullptr;
        p_world_map = nullptr;
    }
};

TEST_F(NEWG_SetBaseLands__WIP_test, ConvertsInteriorTilesToExpectedBaseTerrains)
{
    const int16_t wp = ARCANUS_PLANE;
    const int16_t wy = 10;

    SET_TERRAIN_TYPE(10, wy, wp, 2);
    SET_TERRAIN_TYPE(11, wy, wp, 3);
    SET_TERRAIN_TYPE(12, wy, wp, 4);
    SET_TERRAIN_TYPE(13, wy, wp, 5);
    SET_TERRAIN_TYPE(14, wy, wp, 6);
    SET_TERRAIN_TYPE(15, wy, wp, tt_Ocean);

    Translate_Heightmap_To_Base_Terrain_Types(wp);

    EXPECT_EQ(GET_TERRAIN_TYPE(10, wy, wp), tt_Forest1);
    EXPECT_EQ(GET_TERRAIN_TYPE(11, wy, wp), tt_Forest1);
    EXPECT_EQ(GET_TERRAIN_TYPE(12, wy, wp), tt_Hills1);
    EXPECT_EQ(GET_TERRAIN_TYPE(13, wy, wp), tt_Hills1);
    EXPECT_EQ(GET_TERRAIN_TYPE(14, wy, wp), tt_Mountain1);
    EXPECT_EQ(GET_TERRAIN_TYPE(15, wy, wp), tt_Ocean);
}

TEST_F(NEWG_SetBaseLands__WIP_test, TopAndBottomRowsAreAlwaysTundra)
{
    const int16_t wp = MYRROR_PLANE;

    for(int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(int16_t wx = 0; wx < WORLD_WIDTH; wx++)
        {
            SET_TERRAIN_TYPE(wx, wy, wp, tt_Mountain1);
        }
    }

    Translate_Heightmap_To_Base_Terrain_Types(wp);

    for(int16_t wx = 0; wx < WORLD_WIDTH; wx++)
    {
        EXPECT_EQ(GET_TERRAIN_TYPE(wx, WORLD_YMIN, wp), tt_Tundra1);
        EXPECT_EQ(GET_TERRAIN_TYPE(wx, WORLD_YMAX, wp), tt_Tundra1);
    }
}

TEST_F(NEWG_SetBaseLands__WIP_test, PolarAdjacentRowsStayWithinAllowedRandomizedOutcomes)
{
    const int16_t wp = ARCANUS_PLANE;

    for(int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(int16_t wx = 0; wx < WORLD_WIDTH; wx++)
        {
            SET_TERRAIN_TYPE(wx, wy, wp, tt_Ocean);
        }
    }

    for(int16_t wx = 0; wx < WORLD_WIDTH; wx++)
    {
        SET_TERRAIN_TYPE(wx, WORLD_YMIN + 1, wp, 6);
        SET_TERRAIN_TYPE(wx, WORLD_YMAX - 1, wp, 6);
    }

    Translate_Heightmap_To_Base_Terrain_Types(wp);

    for(int16_t wx = 0; wx < WORLD_WIDTH; wx++)
    {
        const int16_t top_inner = GET_TERRAIN_TYPE(wx, WORLD_YMIN + 1, wp);
        const int16_t bottom_inner = GET_TERRAIN_TYPE(wx, WORLD_YMAX - 1, wp);

        EXPECT_TRUE((top_inner == tt_Mountain1) || (top_inner == tt_Tundra1));
        EXPECT_TRUE((bottom_inner == tt_Mountain1) || (bottom_inner == tt_Tundra1));
    }
}
