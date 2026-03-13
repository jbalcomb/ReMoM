#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../src/MAPGEN.h"
#include "../src/TerrType.h"
#ifdef __cplusplus
}
#endif

#include <cstdlib>

class Desert_Autotile_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _world_maps = static_cast<uint8_t *>(malloc(WORLD_SIZE * NUM_PLANES * sizeof(int16_t)));
        ASSERT_NE(_world_maps, nullptr);

        p_world_map = (int16_t (*)[WORLD_HEIGHT][WORLD_WIDTH])_world_maps;

        _landmasses = static_cast<uint8_t *>(malloc(WORLD_SIZE * NUM_PLANES));
        ASSERT_NE(_landmasses, nullptr);

        for(int16_t wp = 0; wp < NUM_PLANES; wp++)
        {
            for(int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
            {
                for(int16_t wx = 0; wx < WORLD_WIDTH; wx++)
                {
                    p_world_map[wp][wy][wx] = tt_Desert2;
                    _landmasses[(wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx] = 0;
                }
            }
        }
    }

    void TearDown() override
    {
        free(_landmasses);
        _landmasses = nullptr;

        free(_world_maps);
        _world_maps = nullptr;
        p_world_map = nullptr;
    }
};

TEST_F(Desert_Autotile_test, DesertSquareWithOnlyDesertNeighbors_NormalizesToDesert1)
{
    const int16_t wp = ARCANUS_PLANE;
    const int16_t wx = 20;
    const int16_t wy = 20;

    p_world_map[wp][wy][wx] = tt_Desert4;
    p_world_map[wp][(wy - 1)][(wx - 1)] = tt_Desert3;
    p_world_map[wp][(wy - 1)][(wx    )] = tt_Desert3;
    p_world_map[wp][(wy - 1)][(wx + 1)] = tt_Desert3;
    p_world_map[wp][(wy    )][(wx + 1)] = tt_Desert3;
    p_world_map[wp][(wy + 1)][(wx + 1)] = tt_Desert3;
    p_world_map[wp][(wy + 1)][(wx    )] = tt_Desert3;
    p_world_map[wp][(wy + 1)][(wx - 1)] = tt_Desert3;
    p_world_map[wp][(wy    )][(wx - 1)] = tt_Desert3;

    Desert_Autotile();

    EXPECT_EQ(p_world_map[wp][wy][wx], tt_Desert1);
}

TEST_F(Desert_Autotile_test, NonDesertNeighborOfDesert_RemainsUnchanged)
{
    const int16_t wp = ARCANUS_PLANE;
    const int16_t wx = 20;
    const int16_t wy = 20;

    p_world_map[wp][wy][wx] = tt_Desert4;
    p_world_map[wp][(wy - 1)][(wx - 1)] = tt_Desert3;
    p_world_map[wp][(wy - 1)][(wx    )] = tt_Desert3;
    p_world_map[wp][(wy - 1)][(wx + 1)] = tt_Desert3;
    p_world_map[wp][(wy    )][(wx + 1)] = tt_Desert3;
    p_world_map[wp][(wy + 1)][(wx + 1)] = tt_Desert3;
    p_world_map[wp][(wy + 1)][(wx    )] = tt_Desert3;
    p_world_map[wp][(wy + 1)][(wx - 1)] = tt_Desert3;
    p_world_map[wp][(wy    )][(wx - 1)] = tt_Grasslands1;

    Desert_Autotile();

    EXPECT_EQ(p_world_map[wp][wy][(wx - 1)], tt_Grasslands1);
}
