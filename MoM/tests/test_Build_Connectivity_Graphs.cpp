#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/MOM_DAT.h"      /* movement_mode_cost_maps, connectivity_grid_land/sea, s_MOVE_MODE_COST_MAPS (via MovePath.h) */
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/EMS/EMS.h"      /* _EMMDATAH_seg */
#include "../src/MAPGEN.h"
#ifdef __cplusplus
}
#endif

#include <cstdint>
#include <cstdlib>
#include <cstring>

/*
    Build_Connectivity_Graphs(wp) is the per-plane wrapper: it maps the EMM cost-map bank,
    then builds two section-connectivity grids for the plane --
        walking.moves2  -> connectivity_grid_land[wp * 96]
        sailing.moves2  -> connectivity_grid_sea[wp * 96]
    via Build_Connectivity_Graph(). No Random() -> deterministic.

    Backing the globals for a unit test:
    - EMMDATAH_Map() early-returns NULL when _EMMDATAH_seg == NULL (EMS.c), and the wrapper
      ignores its return, so nulling that seg lets movement_mode_cost_maps be a plain malloc
      rather than EMM-paged.
    - movement_mode_cost_maps gets WORLD_OVERFLOW of trailing slack for the OG out-of-bounds
      reads past the last plane's sailing submap.
    - connectivity_grid_land/sea are sized like the game's 224-byte allocation (NUM_PLANES*96 used + slack;
      the east-column OGBUG writes one byte past a plane's 96-byte record).

    With every passable tile == 0 and impassable == 0xFF, an all-passable plane yields the
    fully-connected section pattern (0xEE top row / 0xFF interior / 0xBB bottom row, matching
    test_Build_Connectivity_Graph), and an all-impassable plane yields all-zero.
*/

class Build_Connectivity_Graphs_test : public ::testing::Test
{
protected:
    struct s_MOVE_MODE_COST_MAPS * saved_mmcm = nullptr;
    int8_t * saved_uu1 = nullptr;
    int8_t * saved_uu2 = nullptr;
    SAMB_ptr saved_datah = nullptr;

    static constexpr int UU_BYTES = 224;   /* matches the game alloc: NUM_PLANES*96 used + slack */

    void SetUp() override
    {
        saved_mmcm  = movement_mode_cost_maps;
        saved_uu1   = connectivity_grid_land;
        saved_uu2   = connectivity_grid_sea;
        saved_datah = _EMMDATAH_seg;

        _EMMDATAH_seg = NULL;   /* EMMDATAH_Map() becomes a harmless no-op in the wrapper */

        const size_t mmcm_bytes = (sizeof(struct s_MOVE_MODE_COST_MAPS) * NUM_PLANES) + WORLD_OVERFLOW;
        movement_mode_cost_maps = static_cast<struct s_MOVE_MODE_COST_MAPS *>(malloc(mmcm_bytes));
        ASSERT_NE(movement_mode_cost_maps, nullptr);
        memset(movement_mode_cost_maps, 0, mmcm_bytes);   /* passable baseline incl. trailing padding */

        connectivity_grid_land = static_cast<int8_t *>(malloc(UU_BYTES));
        connectivity_grid_sea = static_cast<int8_t *>(malloc(UU_BYTES));
        ASSERT_NE(connectivity_grid_land, nullptr);
        ASSERT_NE(connectivity_grid_sea, nullptr);
        memset(connectivity_grid_land, 0xCC, UU_BYTES);                 /* sentinel: must be fully overwritten */
        memset(connectivity_grid_sea, 0xCC, UU_BYTES);
    }

    void TearDown() override
    {
        free(movement_mode_cost_maps);
        free(connectivity_grid_land);
        free(connectivity_grid_sea);
        movement_mode_cost_maps = saved_mmcm;
        connectivity_grid_land = saved_uu1;
        connectivity_grid_sea = saved_uu2;
        _EMMDATAH_seg = saved_datah;
    }

    /* Fill one plane: every submap = plane_fill, then override the sailing submap = sailing_fill. */
    void Fill_Plane(int wp, int8_t plane_fill, int8_t sailing_fill)
    {
        memset(&movement_mode_cost_maps[wp], plane_fill, sizeof(struct s_MOVE_MODE_COST_MAPS));
        memset(&movement_mode_cost_maps[wp].sailing.moves2[0], sailing_fill, WORLD_SIZE);
    }
};

TEST_F(Build_Connectivity_Graphs_test, RoutesWalkingToUU1AndSailingToUU2PerPlane)
{
    const int8_t PASS = 0x00;   /* passable (entry cost 0) */
    const int8_t WALL = (int8_t)0xFF;   /* impassable (-1) */

    /* Plane 0: walking passable, sailing impassable.  Plane 1: walking impassable, sailing passable.
       This makes walking != sailing and plane0 != plane1, so the routing and the wp*96 stride are
       both pinned in one shot. */
    Fill_Plane(ARCANUS_PLANE, PASS, WALL);
    Fill_Plane(MYRROR_PLANE,  WALL, PASS);

    Build_Connectivity_Graphs(ARCANUS_PLANE);
    Build_Connectivity_Graphs(MYRROR_PLANE);

    const uint8_t * uu1 = reinterpret_cast<const uint8_t *>(connectivity_grid_land);   /* walking */
    const uint8_t * uu2 = reinterpret_cast<const uint8_t *>(connectivity_grid_sea);   /* sailing */

    for(int y = 0; y < 8; y++)
    {
        uint8_t pattern = (y == 0) ? 0xEE : (y == 7) ? 0xBB : 0xFF;
        for(int x = 0; x < 12; x++)
        {
            int s = (y * 12) + x;
            EXPECT_EQ(uu1[(0 * 96) + s], pattern) << "connectivity_grid_land (walking) plane 0 section " << s;
            EXPECT_EQ(uu1[(1 * 96) + s], 0x00)    << "connectivity_grid_land (walking) plane 1 section " << s;
            EXPECT_EQ(uu2[(0 * 96) + s], 0x00)    << "connectivity_grid_sea (sailing) plane 0 section " << s;
            EXPECT_EQ(uu2[(1 * 96) + s], pattern) << "connectivity_grid_sea (sailing) plane 1 section " << s;
        }
    }
}
