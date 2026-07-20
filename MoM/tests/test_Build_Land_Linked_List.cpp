#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/Allocate_Pool.h"  /* Pool_Init() - static pool reset between tests */
#include "../../MoX/src/EMS/EMS.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../src/AIDUDES.h"
#ifdef __cplusplus
}
#endif

#include <cstdlib>
#include <cstring>

class Build_Land_Linked_List_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        Pool_Init();  // EmmHndl_CONTXXX is static-pool-backed (Allocate_Space); reset the arena each test.

        _landmasses = (uint8_t *)malloc(WORLD_SIZE * NUM_PLANES);
        ASSERT_NE(_landmasses, nullptr);
        memset(_landmasses, 0, WORLD_SIZE * NUM_PLANES);

        EmmHndl_CONTXXX = Allocate_Space(4096);
        ASSERT_NE(EmmHndl_CONTXXX, nullptr);
        memset(EmmHndl_CONTXXX, 0, (4096 * SZ_PARAGRAPH_B));
    }

    void TearDown() override
    {
        _ai_landmass_land_squares_heads[ARCANUS_PLANE] = nullptr;
        _ai_landmass_land_squares_heads[MYRROR_PLANE] = nullptr;
        _ai_landmass_land_squares_lists[ARCANUS_PLANE] = nullptr;
        _ai_landmass_land_squares_lists[MYRROR_PLANE] = nullptr;
        _ai_landmass_land_squares_wx_array[ARCANUS_PLANE] = nullptr;
        _ai_landmass_land_squares_wx_array[MYRROR_PLANE] = nullptr;
        _ai_landmass_land_squares_wy_array[ARCANUS_PLANE] = nullptr;
        _ai_landmass_land_squares_wy_array[MYRROR_PLANE] = nullptr;
        EMS_PFBA = nullptr;

        // EmmHndl_CONTXXX is static-pool-backed (Allocate_Space) -- not freed here;
        // Pool_Init() in SetUp reclaims it.  _landmasses is real malloc -- free it.
        EmmHndl_CONTXXX = nullptr;

        free(_landmasses);
        _landmasses = nullptr;
    }

    void Set_Landmass_Id(int16_t wx, int16_t wy, int16_t wp, uint8_t landmass_idx)
    {
        SET_LANDMASS(wx, wy, wp, landmass_idx);
    }
};

TEST_F(Build_Land_Linked_List_test, AllOceanLeavesLandmassChainsEmpty)
{
    Build_Land_Linked_List();

    EXPECT_EQ(_ai_landmass_land_squares_heads[ARCANUS_PLANE][0], ST_UNDEFINED);
    EXPECT_EQ(_ai_landmass_land_squares_heads[ARCANUS_PLANE][1], ST_UNDEFINED);
    EXPECT_EQ(_ai_landmass_land_squares_heads[MYRROR_PLANE][0], ST_UNDEFINED);
    EXPECT_EQ(_ai_landmass_land_squares_heads[MYRROR_PLANE][1], ST_UNDEFINED);
}

TEST_F(Build_Land_Linked_List_test, TwoTilesOnSameLandmassFormOrderedChain)
{
    const int16_t wp = ARCANUS_PLANE;
    const int16_t first_wx = 10;
    const int16_t first_wy = 10;
    const int16_t second_wx = 11;
    const int16_t second_wy = 10;
    int16_t first_node_idx = 0;
    int16_t second_node_idx = 0;

    Set_Landmass_Id(first_wx, first_wy, wp, 1);
    Set_Landmass_Id(second_wx, second_wy, wp, 1);

    Build_Land_Linked_List();

    EXPECT_EQ(_ai_landmass_land_squares_heads[wp][0], ST_UNDEFINED);

    first_node_idx = _ai_landmass_land_squares_heads[wp][1];
    ASSERT_NE(first_node_idx, ST_UNDEFINED);
    EXPECT_EQ(_ai_landmass_land_squares_wx_array[wp][first_node_idx], first_wx);
    EXPECT_EQ(_ai_landmass_land_squares_wy_array[wp][first_node_idx], first_wy);

    second_node_idx = _ai_landmass_land_squares_lists[wp][first_node_idx];
    ASSERT_NE(second_node_idx, ST_UNDEFINED);
    EXPECT_EQ(_ai_landmass_land_squares_wx_array[wp][second_node_idx], second_wx);
    EXPECT_EQ(_ai_landmass_land_squares_wy_array[wp][second_node_idx], second_wy);
    EXPECT_EQ(_ai_landmass_land_squares_lists[wp][second_node_idx], ST_UNDEFINED);
}