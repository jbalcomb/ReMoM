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

#include <cstring>
#include <cstdlib>

class NEWG_ClearLandmasses__WIP_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _landmasses = static_cast<uint8_t *>(malloc(WORLD_SIZE * NUM_PLANES));
        ASSERT_NE(_landmasses, nullptr);
        memset(_landmasses, 0x7F, WORLD_SIZE * NUM_PLANES);
        m_landmasses_ctr = 42;
    }

    void TearDown() override
    {
        free(_landmasses);
        _landmasses = nullptr;
    }
};

TEST_F(NEWG_ClearLandmasses__WIP_test, ClearsOnlyArcanusAndResetsCounter)
{
    Init_Landmasses(ARCANUS_PLANE);

    EXPECT_EQ(m_landmasses_ctr, 1);

    for(int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(int16_t wx = 0; wx < WORLD_WIDTH; wx++)
        {
            EXPECT_EQ(GET_LANDMASS(wx, wy, ARCANUS_PLANE), 0);
            EXPECT_EQ(GET_LANDMASS(wx, wy, MYRROR_PLANE), 0x7F);
        }
    }
}

TEST_F(NEWG_ClearLandmasses__WIP_test, ClearsOnlyMyrrorAndResetsCounter)
{
    Init_Landmasses(MYRROR_PLANE);

    EXPECT_EQ(m_landmasses_ctr, 1);

    for(int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(int16_t wx = 0; wx < WORLD_WIDTH; wx++)
        {
            EXPECT_EQ(GET_LANDMASS(wx, wy, MYRROR_PLANE), 0);
            EXPECT_EQ(GET_LANDMASS(wx, wy, ARCANUS_PLANE), 0x7F);
        }
    }
}
