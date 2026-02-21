#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/MOM_Data.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../src/MAPGEN.h"
#include "../src/TerrType.h"
#ifdef __cplusplus
}
#endif

#include <cstring>
#include <vector>

class NEWG_TileIsleExtend__WIP_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _world_maps = (uint8_t *)Allocate_Space(602);
        _TOWERS = (struct s_TOWER *)Allocate_Space((sizeof(struct s_TOWER) * NUM_TOWERS / SZ_PARAGRAPH_B) + 2);

        ASSERT_NE(_world_maps, nullptr);
        ASSERT_NE(_TOWERS, nullptr);

        memset(_world_maps, tt_Ocean1, WORLD_SIZE * NUM_PLANES * sizeof(uint16_t));
        memset(_TOWERS, 0, sizeof(struct s_TOWER) * NUM_TOWERS);
    }

    void TearDown() override
    {
        free(_TOWERS);
        _TOWERS = nullptr;
        free(_world_maps);
        _world_maps = nullptr;
    }
};

TEST_F(NEWG_TileIsleExtend__WIP_test, AllOcean_NoCandidates_LeavesMapUnchanged)
{
    std::vector<uint8_t> before(WORLD_SIZE * NUM_PLANES * sizeof(uint16_t));
    memcpy(before.data(), _world_maps, before.size());

    NEWG_TileIsleExtend__WIP(ARCANUS_PLANE);

    EXPECT_EQ(memcmp(before.data(), _world_maps, before.size()), 0);
}

TEST_F(NEWG_TileIsleExtend__WIP_test, NodeWithNonOceanCardinalNeighbor_DoesNotAlterMap)
{
    const int16_t wx = 20;
    const int16_t wy = 20;
    const int16_t wp = ARCANUS_PLANE;

    SET_TERRAIN_TYPE(wx, wy, wp, tt_SorceryNode);
    SET_TERRAIN_TYPE(wx, wy - 1, wp, tt_Grasslands1);

    std::vector<uint8_t> before(WORLD_SIZE * NUM_PLANES * sizeof(uint16_t));
    memcpy(before.data(), _world_maps, before.size());

    NEWG_TileIsleExtend__WIP(wp);

    EXPECT_EQ(memcmp(before.data(), _world_maps, before.size()), 0);
}
