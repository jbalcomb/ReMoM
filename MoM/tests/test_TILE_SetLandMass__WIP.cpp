#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/MOM_Data.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../src/MAPGEN.h"
extern int16_t m_landmasses_ctr;
#ifdef __cplusplus
}
#endif

#include <cstdlib>
#include <cstring>

class TILE_SetLandMass__WIP_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _landmasses = static_cast<uint8_t *>(malloc(WORLD_SIZE * NUM_PLANES));
        ASSERT_NE(_landmasses, nullptr);
        memset(_landmasses, 0, WORLD_SIZE * NUM_PLANES);
    }

    void TearDown() override
    {
        free(_landmasses);
        _landmasses = nullptr;
    }
};

TEST_F(TILE_SetLandMass__WIP_test, NoAdjacentLandmass_WritesZeroToLeftTile_AndDoesNotIncrementCounter)
{
    const int16_t wp = ARCANUS_PLANE;
    const int16_t wx = 10;
    const int16_t wy = 10;

    SET_LANDMASS((wx - 1), wy, wp, 55);
    m_landmasses_ctr = 7;

    Build_Landmass(wp, wx, wy);

    EXPECT_EQ(GET_LANDMASS((wx - 1), wy, wp), 0);
    EXPECT_EQ(m_landmasses_ctr, 7);
}

TEST_F(TILE_SetLandMass__WIP_test, AdjacentLandmass_WritesCurrentCounterToLeftTile_AndIncrementsCounter)
{
    const int16_t wp = ARCANUS_PLANE;
    const int16_t wx = 10;
    const int16_t wy = 10;

    SET_LANDMASS(wx, (wy - 1), wp, 5);
    m_landmasses_ctr = 7;

    Build_Landmass(wp, wx, wy);

    EXPECT_EQ(GET_LANDMASS((wx - 1), wy, wp), 7);
    EXPECT_EQ(m_landmasses_ctr, 8);
    EXPECT_EQ(GET_LANDMASS(wx, (wy - 1), wp), 5);
}
