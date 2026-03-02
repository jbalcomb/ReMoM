#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/MOM_Data.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/random.h"
#include "../src/MAPGEN.h"
#include "../src/TerrType.h"
#ifdef __cplusplus
}
#endif

#include <cstdlib>
#include <cstring>

struct s_river_seed_plan
{
    uint32_t seed;
    int16_t base_wx;
    int16_t base_wy;
    int16_t downstream;
};

static int16_t Predict_Random(uint32_t * seed_state, int16_t max)
{
    uint32_t state = *seed_state;

    state ^= (state << 13);
    state ^= (state >> 17);
    state ^= (state << 5);

    *seed_state = state;

    return (int16_t)(((state >> 16) % max) + 1);
}

static int16_t Count_Accepts_For_Seed(uint32_t seed)
{
    const int16_t upstream_tbl[4] = { 2, 3, 0, 1 };

    uint32_t seed_state = seed;
    int16_t downstream = 0;
    int16_t same_dir = ST_UNDEFINED;
    int16_t accepted_steps = 0;
    int16_t attempts = 0;
    int16_t direction = 0;

    (void)Predict_Random(&seed_state, (WORLD_WIDTH - 8));
    (void)Predict_Random(&seed_state, (WORLD_HEIGHT - 8));
    downstream = (Predict_Random(&seed_state, 4) - 1);

    while(accepted_steps < 3)
    {
        if((Predict_Random(&seed_state, 2) > 1) || (same_dir == ST_UNDEFINED))
        {
            direction = downstream;
        }
        else
        {
            do
            {
                direction = (Predict_Random(&seed_state, 4) - 1);
            } while(upstream_tbl[downstream] == direction);
        }

        same_dir = direction;
        attempts++;

        if(attempts > 30)
        {
            return ST_FALSE;
        }

        if(direction == downstream)
        {
            accepted_steps++;
        }
    }

    return ST_TRUE;
}

static int16_t Find_Usable_Seed(s_river_seed_plan * plan)
{
    const int16_t dir_wx[4] = { 0, -1, 0, 1 };
    const int16_t dir_wy[4] = { 1, 0, -1, 0 };
    const int16_t outflow_wx[4] = { 1, 0, -1, 0 };
    const int16_t outflow_wy[4] = { 0, 1, 0, -1 };

    for(uint32_t seed = 1; seed < 100000; seed++)
    {
        uint32_t seed_state = seed;
        int16_t base_wx = (4 + Predict_Random(&seed_state, (WORLD_WIDTH - 8)));
        int16_t base_wy = (4 + Predict_Random(&seed_state, (WORLD_HEIGHT - 8)));
        int16_t downstream = (Predict_Random(&seed_state, 4) - 1);
        int16_t end_wx = (base_wx + (3 * dir_wx[downstream]));
        int16_t end_wy = (base_wy + (3 * dir_wy[downstream]));
        int16_t ocean_wx = (end_wx + outflow_wx[downstream]);
        int16_t ocean_wy = (end_wy + outflow_wy[downstream]);

        if(
            (end_wx <= WORLD_XMIN) || (end_wx >= WORLD_XMAX)
            ||
            (end_wy <= WORLD_YMIN) || (end_wy >= WORLD_YMAX)
            ||
            (ocean_wx <= WORLD_XMIN) || (ocean_wx >= WORLD_XMAX)
            ||
            (ocean_wy <= WORLD_YMIN) || (ocean_wy >= WORLD_YMAX)
        )
        {
            continue;
        }

        if(Count_Accepts_For_Seed(seed) == ST_TRUE)
        {
            plan->seed = seed;
            plan->base_wx = base_wx;
            plan->base_wy = base_wy;
            plan->downstream = downstream;
            return ST_TRUE;
        }
    }

    return ST_FALSE;
}

class River_Path_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _world_maps = static_cast<uint8_t *>(malloc(WORLD_SIZE * NUM_PLANES * sizeof(int16_t)));
        ASSERT_NE(_world_maps, nullptr);

        p_world_map = (int16_t (*)[WORLD_HEIGHT][WORLD_WIDTH])_world_maps;

        _map_square_terrain_specials = static_cast<uint8_t *>(malloc(WORLD_SIZE * NUM_PLANES));
        ASSERT_NE(_map_square_terrain_specials, nullptr);

        _NODES = static_cast<struct s_NODE *>(malloc(sizeof(struct s_NODE) * NUM_NODES));
        ASSERT_NE(_NODES, nullptr);

        for(int16_t itr = 0; itr < NUM_NODES; itr++)
        {
            _NODES[itr].wx = ST_UNDEFINED;
            _NODES[itr].wy = ST_UNDEFINED;
            _NODES[itr].wp = ST_UNDEFINED;
        }

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

        memset(_map_square_terrain_specials, 1, WORLD_SIZE * NUM_PLANES);
    }

    void TearDown() override
    {
        free(_NODES);
        _NODES = nullptr;

        free(_map_square_terrain_specials);
        _map_square_terrain_specials = nullptr;

        free(_world_maps);
        _world_maps = nullptr;
        p_world_map = nullptr;
    }
};

TEST_F(River_Path_test, TerrainSpecialAtStart_RejectsPathAndLeavesMapUnchanged)
{
    const int16_t result = River_Path(ARCANUS_PLANE);

    EXPECT_EQ(result, ST_FALSE);

    for(int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(int16_t wx = 0; wx < WORLD_WIDTH; wx++)
        {
            EXPECT_NE(p_world_map[ARCANUS_PLANE][wy][wx], 1000);
        }
    }
}

TEST_F(River_Path_test, ManufacturedCandidate_SucceedsAndWritesExpectedPath)
{
    const int16_t dir_wx[4] = { 0, -1, 0, 1 };
    const int16_t dir_wy[4] = { 1, 0, -1, 0 };
    const int16_t outflow_wx[4] = { 1, 0, -1, 0 };
    const int16_t outflow_wy[4] = { 0, 1, 0, -1 };
    const int16_t wp = ARCANUS_PLANE;

    s_river_seed_plan plan = { 0, 0, 0, 0 };
    ASSERT_EQ(Find_Usable_Seed(&plan), ST_TRUE);

    for(int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(int16_t wx = 0; wx < WORLD_WIDTH; wx++)
        {
            p_world_map[wp][wy][wx] = tt_Desert1;
            _map_square_terrain_specials[(wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx] = 0;
        }
    }

    const int16_t step1_wx = (plan.base_wx + dir_wx[plan.downstream]);
    const int16_t step1_wy = (plan.base_wy + dir_wy[plan.downstream]);
    const int16_t step2_wx = (plan.base_wx + (2 * dir_wx[plan.downstream]));
    const int16_t step2_wy = (plan.base_wy + (2 * dir_wy[plan.downstream]));
    const int16_t step3_wx = (plan.base_wx + (3 * dir_wx[plan.downstream]));
    const int16_t step3_wy = (plan.base_wy + (3 * dir_wy[plan.downstream]));
    const int16_t ocean_wx = (step3_wx + outflow_wx[plan.downstream]);
    const int16_t ocean_wy = (step3_wy + outflow_wy[plan.downstream]);

    p_world_map[wp][plan.base_wy][plan.base_wx] = tt_Grasslands1;
    p_world_map[wp][step1_wy][step1_wx] = tt_Grasslands1;
    p_world_map[wp][step2_wy][step2_wx] = tt_Grasslands1;
    p_world_map[wp][step3_wy][step3_wx] = tt_Grasslands1;
    p_world_map[wp][ocean_wy][ocean_wx] = tt_Ocean1;

    Set_Random_Seed(plan.seed);

    const int16_t result = River_Path(wp);
    EXPECT_EQ(result, ST_TRUE);

    int16_t river_placeholder_count = 0;
    for(int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(int16_t wx = 0; wx < WORLD_WIDTH; wx++)
        {
            if(p_world_map[wp][wy][wx] == 1000)
            {
                river_placeholder_count++;
            }
        }
    }

    EXPECT_EQ(river_placeholder_count, 4);
    EXPECT_EQ(p_world_map[wp][plan.base_wy][plan.base_wx], 1000);
    EXPECT_EQ(p_world_map[wp][step1_wy][step1_wx], 1000);
    EXPECT_EQ(p_world_map[wp][step2_wy][step2_wx], 1000);
    EXPECT_EQ(p_world_map[wp][step3_wy][step3_wx], 1000);
}
