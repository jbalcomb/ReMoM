#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_BITS.h"
#include "../../STU/src/STU_VLD.h"
#include "../src/MAPGEN.h"
#include "../src/TerrType.h"
#ifdef __cplusplus
}
#endif

#include <cstdlib>
#include <cstring>

class GenerateLairsTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _LAIRS = static_cast<struct s_LAIR *>(malloc(sizeof(struct s_LAIR) * NUM_LAIRS));
        _TOWERS = static_cast<struct s_TOWER *>(malloc(sizeof(struct s_TOWER) * NUM_TOWERS));
        _NODES = static_cast<struct s_NODE *>(malloc(sizeof(struct s_NODE) * NUM_NODES));
        _world_maps = static_cast<uint8_t *>(malloc(WORLD_SIZE * NUM_PLANES * sizeof(uint16_t)));

        ASSERT_NE(_LAIRS, nullptr);
        ASSERT_NE(_TOWERS, nullptr);
        ASSERT_NE(_NODES, nullptr);
        ASSERT_NE(_world_maps, nullptr);

        memset(_LAIRS, 0xFF, sizeof(struct s_LAIR) * NUM_LAIRS);
        memset(_TOWERS, 0xFF, sizeof(struct s_TOWER) * NUM_TOWERS);
        memset(_NODES, 0xFF, sizeof(struct s_NODE) * NUM_NODES);

        p_world_map = (int16_t (*)[WORLD_HEIGHT][WORLD_WIDTH])_world_maps;

        // Fill both planes with grasslands so the strong/weak lair loops can
        // find valid non-ocean locations and terminate.
        for(int16_t wp = 0; wp < NUM_PLANES; wp++)
        {
            for(int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
            {
                for(int16_t wx = 0; wx < WORLD_WIDTH; wx++)
                {
                    SET_TERRAIN_TYPE(wx, wy, wp, tt_Grasslands1);
                }
            }
        }

        _difficulty = god_Normal;
        _magic = 1;

        for(int16_t itr = 0; itr < NUM_TOWERS; itr++)
        {
            _TOWERS[itr].wx = static_cast<int8_t>(5 + itr);
            _TOWERS[itr].wy = static_cast<int8_t>(7 + itr);
        }

        for(int16_t itr = 0; itr < NUM_NODES; itr++)
        {
            _NODES[itr].wx = static_cast<int8_t>(10 + (itr % 20));
            _NODES[itr].wy = static_cast<int8_t>(12 + (itr % 10));
            _NODES[itr].wp = static_cast<int8_t>((itr % 2) == 0 ? ARCANUS_PLANE : MYRROR_PLANE);
            _NODES[itr].power = static_cast<int8_t>(6 + (itr % 6));

            switch(itr % 3)
            {
                case 0: _NODES[itr].type = nt_Sorcery; break;
                case 1: _NODES[itr].type = nt_Nature; break;
                default: _NODES[itr].type = nt_Chaos; break;
            }
        }
    }

    // The 0xFF poison in SetUp lets us detect slots that Generate_Lairs never wrote.
    // Every slot must have intact set to exactly ST_TRUE or ST_FALSE (not 0xFF poison).
    // Active slots (intact == ST_TRUE) must have coordinates in valid map range.
    void Assert_All_Lairs_Initialized()
    {
        for(int16_t i = 0; i < NUM_LAIRS; i++)
        {
            EXPECT_NE(static_cast<uint8_t>(_LAIRS[i].intact), 0xFFu) << "Lair " << i << " intact field never written by Generate_Lairs (uninitialized 0xFF)";
            EXPECT_TRUE(_LAIRS[i].intact == ST_TRUE || _LAIRS[i].intact == ST_FALSE) << "Lair " << i << " intact has invalid value " << static_cast<int>(_LAIRS[i].intact);

            if(_LAIRS[i].intact == ST_TRUE)
            {
                EXPECT_GE(_LAIRS[i].wp, 0) << "Lair " << i << " (active) has invalid wp";
                EXPECT_LT(_LAIRS[i].wp, NUM_PLANES) << "Lair " << i << " (active) has invalid wp";
                EXPECT_GE(_LAIRS[i].wx, 0) << "Lair " << i << " (active) has invalid wx";
                EXPECT_LT(_LAIRS[i].wx, WORLD_WIDTH) << "Lair " << i << " (active) has invalid wx";
                EXPECT_GE(_LAIRS[i].wy, 0) << "Lair " << i << " (active) has invalid wy";
                EXPECT_LT(_LAIRS[i].wy, WORLD_HEIGHT) << "Lair " << i << " (active) has invalid wy";
            }
        }
    }

    void TearDown() override
    {
        free(_world_maps);
        _world_maps = nullptr;
        p_world_map = nullptr;

        free(_NODES);
        _NODES = nullptr;

        free(_TOWERS);
        _TOWERS = nullptr;

        free(_LAIRS);
        _LAIRS = nullptr;
    }
};

TEST_F(GenerateLairsTest, CreatesTowerMirrorAndNodeEntries)
{
    Generate_Lairs();

    for(int16_t itr = 0; itr < NUM_TOWERS; itr++)
    {
        EXPECT_EQ(_LAIRS[itr].type, lt_Tower);
        EXPECT_EQ(_LAIRS[itr].wp, ARCANUS_PLANE);
        EXPECT_EQ(_LAIRS[itr].wx, _TOWERS[itr].wx);
        EXPECT_EQ(_LAIRS[itr].wy, _TOWERS[itr].wy);
        EXPECT_EQ(_LAIRS[itr].intact, ST_TRUE);

        const int16_t mirror_idx = static_cast<int16_t>(itr + NUM_TOWERS);
        EXPECT_EQ(_LAIRS[mirror_idx].type, _LAIRS[itr].type);
        EXPECT_EQ(_LAIRS[mirror_idx].wx, _LAIRS[itr].wx);
        EXPECT_EQ(_LAIRS[mirror_idx].wy, _LAIRS[itr].wy);
        EXPECT_EQ(_LAIRS[mirror_idx].wp, MYRROR_PLANE);
    }

    const int16_t first_node_lair = static_cast<int16_t>((2 * NUM_TOWERS) + 0);
    const int16_t second_node_lair = static_cast<int16_t>((2 * NUM_TOWERS) + 1);
    const int16_t third_node_lair = static_cast<int16_t>((2 * NUM_TOWERS) + 2);

    EXPECT_EQ(_LAIRS[first_node_lair].type, lt_Sorcery_Node);
    EXPECT_EQ(_LAIRS[second_node_lair].type, lt_Nature_Node);
    EXPECT_EQ(_LAIRS[third_node_lair].type, lt_Chaos_Node);

    EXPECT_EQ(_LAIRS[first_node_lair].wp, _NODES[0].wp);
    EXPECT_EQ(_LAIRS[second_node_lair].wp, _NODES[1].wp);
    EXPECT_EQ(_LAIRS[third_node_lair].wp, _NODES[2].wp);

    Assert_All_Lairs_Initialized();
}

TEST_F(GenerateLairsTest, AllSlotsInitializedAfterGeneration)
{
    Generate_Lairs();

    Assert_All_Lairs_Initialized();
}

TEST_F(GenerateLairsTest, ValidateAllLairsPassesAfterGeneration)
{
    Generate_Lairs();

    EXPECT_EQ(Validate_All_Lairs(), ST_TRUE);

    Assert_All_Lairs_Initialized();
}
