#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/random.h"
#include "../src/MAPGEN.h"
#include "../src/TerrType.h"
#ifdef __cplusplus
}
#endif

#include <cstdlib>

static uint32_t Find_Seed_For_First_Random5_Value(int16_t target_value)
{
    for(uint32_t seed = 1; seed < 1000000; seed++)
    {
        Set_Random_Seed(seed);
        if(Random(5) == target_value)
        {
            return seed;
        }
    }

    return 0;
}

class Animate_Oceans_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _world_maps = static_cast<uint8_t *>(malloc(WORLD_SIZE * NUM_PLANES * sizeof(int16_t)));
        ASSERT_NE(_world_maps, nullptr);

        p_world_map = (int16_t (*)[WORLD_HEIGHT][WORLD_WIDTH])_world_maps;

        for(int16_t wp = 0; wp < NUM_PLANES; wp++)
        {
            for(int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
            {
                for(int16_t wx = 0; wx < WORLD_WIDTH; wx++)
                {
                    p_world_map[wp][wy][wx] = tt_Grasslands1;
                }
            }
        }
    }

    void TearDown() override
    {
        free(_world_maps);
        _world_maps = nullptr;
        p_world_map = nullptr;
    }
};

TEST_F(Animate_Oceans_test, OceanTile_FirstRandomHit_ConvertsToOceanAnim)
{
    const int16_t wp = ARCANUS_PLANE;
    const int16_t wy = 10;
    const int16_t wx = 10;

    const uint32_t seed = Find_Seed_For_First_Random5_Value(1);
    ASSERT_NE(seed, 0u);

    p_world_map[wp][wy][wx] = tt_Ocean;

    Set_Random_Seed(seed);
    Animate_Oceans();

    EXPECT_EQ(p_world_map[wp][wy][wx], tt_OceanAnim);
}

TEST_F(Animate_Oceans_test, OceanTile_FirstRandomMiss_RemainsOcean)
{
    const int16_t wp = ARCANUS_PLANE;
    const int16_t wy = 10;
    const int16_t wx = 10;

    const uint32_t seed = Find_Seed_For_First_Random5_Value(2);
    ASSERT_NE(seed, 0u);

    p_world_map[wp][wy][wx] = tt_Ocean;

    Set_Random_Seed(seed);
    Animate_Oceans();

    EXPECT_EQ(p_world_map[wp][wy][wx], tt_Ocean);
}

TEST_F(Animate_Oceans_test, LegacyMyrranLiteralTile_FirstRandomHit_ConvertsTo1363)
{
    const int16_t wp = ARCANUS_PLANE;
    const int16_t wy = 10;
    const int16_t wx = 10;

    const uint32_t seed = Find_Seed_For_First_Random5_Value(1);
    ASSERT_NE(seed, 0u);

    p_world_map[wp][wy][wx] = 762;

    Set_Random_Seed(seed);
    Animate_Oceans();

    EXPECT_EQ(p_world_map[wp][wy][wx], 1363);
}
