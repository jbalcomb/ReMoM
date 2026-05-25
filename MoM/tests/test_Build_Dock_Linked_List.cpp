#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/EMS/EMS.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../src/AIDUDES.h"
#include "../src/TerrType.h"
#ifdef __cplusplus
}
#endif

#include <cstdlib>
#include <cstring>

class Build_Dock_Linked_List_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _world_maps = (uint8_t *)Allocate_Space(602);
        ASSERT_NE(_world_maps, nullptr);
        memset(_world_maps, 0, WORLD_SIZE * NUM_PLANES * sizeof(uint16_t));

        _landmasses = (uint8_t *)malloc(WORLD_SIZE * NUM_PLANES);
        ASSERT_NE(_landmasses, nullptr);
        memset(_landmasses, 0, WORLD_SIZE * NUM_PLANES);

        EmmHndl_CONTXXX = Allocate_Space(4096);
        ASSERT_NE(EmmHndl_CONTXXX, nullptr);
        memset(EmmHndl_CONTXXX, 0, (4096 * SZ_PARAGRAPH_B));
    }

    void TearDown() override
    {
        _ai_landmass_dock_squares_heads[ARCANUS_PLANE] = nullptr;
        _ai_landmass_dock_squares_heads[MYRROR_PLANE] = nullptr;
        _ai_landmass_dock_squares_lists[ARCANUS_PLANE] = nullptr;
        _ai_landmass_dock_squares_lists[MYRROR_PLANE] = nullptr;
        _ai_landmass_dock_squares_wx_array[ARCANUS_PLANE] = nullptr;
        _ai_landmass_dock_squares_wx_array[MYRROR_PLANE] = nullptr;
        _ai_landmass_dock_squares_wy_array[ARCANUS_PLANE] = nullptr;
        _ai_landmass_dock_squares_wy_array[MYRROR_PLANE] = nullptr;
        EMS_PFBA = nullptr;

        free(EmmHndl_CONTXXX);
        EmmHndl_CONTXXX = nullptr;

        free(_landmasses);
        _landmasses = nullptr;

        free(_world_maps);
        _world_maps = nullptr;
    }

    void Set_Terrain(int16_t wx, int16_t wy, int16_t wp, int16_t terrain_type)
    {
        SET_TERRAIN_TYPE(wx, wy, wp, terrain_type);
    }

    void Set_Landmass_Id(int16_t wx, int16_t wy, int16_t wp, uint8_t landmass_idx)
    {
        SET_LANDMASS(wx, wy, wp, landmass_idx);
    }
};

TEST_F(Build_Dock_Linked_List_test, OpenOceanWithoutAdjacentShorelineLeavesDockChainsEmpty)
{
    Build_Dock_Linked_List();

    EXPECT_EQ(_ai_landmass_dock_squares_heads[ARCANUS_PLANE][0], ST_UNDEFINED);
    EXPECT_EQ(_ai_landmass_dock_squares_heads[ARCANUS_PLANE][1], ST_UNDEFINED);
    EXPECT_EQ(_ai_landmass_dock_squares_heads[MYRROR_PLANE][0], ST_UNDEFINED);
    EXPECT_EQ(_ai_landmass_dock_squares_heads[MYRROR_PLANE][1], ST_UNDEFINED);
}

TEST_F(Build_Dock_Linked_List_test, ShorelineLandSquareCreatesSingleNodeForItsLandmass)
{
    const int16_t wp = ARCANUS_PLANE;
    const int16_t wx = 20;
    const int16_t wy = 20;
    int16_t node_idx = 0;

    Set_Terrain(wx, wy, wp, tt_Shore1_Fst);
    Set_Landmass_Id(wx, wy, wp, 1);

    Build_Dock_Linked_List();

    EXPECT_EQ(_ai_landmass_dock_squares_heads[wp][0], ST_UNDEFINED);

    node_idx = _ai_landmass_dock_squares_heads[wp][1];
    ASSERT_NE(node_idx, ST_UNDEFINED);
    EXPECT_EQ(_ai_landmass_dock_squares_wx_array[wp][node_idx], wx);
    EXPECT_EQ(_ai_landmass_dock_squares_wy_array[wp][node_idx], wy);
    EXPECT_EQ(_ai_landmass_dock_squares_lists[wp][node_idx], ST_UNDEFINED);
}