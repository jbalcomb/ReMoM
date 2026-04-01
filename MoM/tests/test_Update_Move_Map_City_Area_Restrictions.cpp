#include <gtest/gtest.h>

#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../src/CMBTDEF.h"
#include "../src/Combat.h"
#ifdef __cplusplus
}
#endif

namespace
{
    constexpr uint8_t INF_VALUE = 0xFF;

    int Cell_Index(int16_t cgx, int16_t cgy)
    {
        return ((cgy * COMBAT_GRID_WIDTH) + cgx);
    }
}

class Update_Move_Map_City_Area_Restrictions_test : public ::testing::Test
{
protected:
    struct s_BATTLEFIELD test_battlefield;
    struct s_BATTLE_UNIT test_battle_units[1];
    uint8_t test_move_cost_map[COMBAT_GRID_CELL_COUNT];

    struct s_BATTLEFIELD * saved_battlefield = nullptr;
    struct s_BATTLE_UNIT * saved_battle_units = nullptr;
    uint8_t * saved_move_cost_map = nullptr;

    void SetUp() override
    {
        saved_battlefield = battlefield;
        saved_battle_units = battle_units;
        saved_move_cost_map = _cmbt_movepath_cost_map;

        memset(&test_battlefield, 0, sizeof(test_battlefield));
        memset(test_battle_units, 0, sizeof(test_battle_units));
        memset(test_move_cost_map, 0, sizeof(test_move_cost_map));

        battlefield = &test_battlefield;
        battle_units = &test_battle_units[0];
        _cmbt_movepath_cost_map = &test_move_cost_map[0];

        test_battlefield.Central_Structure = CS_None;
        test_battlefield.walled = ST_TRUE;
    }

    void TearDown() override
    {
        _cmbt_movepath_cost_map = saved_move_cost_map;
        battle_units = saved_battle_units;
        battlefield = saved_battlefield;
    }
};

TEST_F(Update_Move_Map_City_Area_Restrictions_test, OutsideUnit_BlocksWallCellsButLeavesGateOpen)
{
    test_battle_units[0].cgx = 0;
    test_battle_units[0].cgy = 0;

    test_battlefield.walls[0][0] = ST_TRUE;
    test_battlefield.walls[1][1] = ST_TRUE;
    test_battlefield.walls[2][3] = ST_TRUE;

    Update_Move_Map_City_Area_Restrictions(/*battle_unit_idx=*/0);

    EXPECT_EQ(test_move_cost_map[Cell_Index(MIN_CGX_CITY, MIN_CGY_CITY)], INF_VALUE);
    EXPECT_EQ(test_move_cost_map[Cell_Index(MIN_CGX_CITY + 1, MIN_CGY_CITY + 1)], INF_VALUE);
    EXPECT_EQ(test_move_cost_map[Cell_Index(CGX_LAIR, CGY_LAIR)], INF_VALUE);
    EXPECT_EQ(test_move_cost_map[Cell_Index(CGX_GATE, CGY_GATE)], 0);
    EXPECT_EQ(test_move_cost_map[Cell_Index(MIN_CGX_CITY + 2, MIN_CGY_CITY + 1)], 0);
}

TEST_F(Update_Move_Map_City_Area_Restrictions_test, InsideUnit_BlocksPerimeterButKeepsGateExitOpen)
{
    test_battlefield.Central_Structure = CS_City;

    test_battle_units[0].cgx = (MIN_CGX_CITY + 1);
    test_battle_units[0].cgy = (MIN_CGY_CITY + 1);

    test_battlefield.walls[1][3] = ST_TRUE;
    test_battlefield.walls[2][3] = ST_TRUE;

    Update_Move_Map_City_Area_Restrictions(/*battle_unit_idx=*/0);

    EXPECT_EQ(test_move_cost_map[Cell_Index(MAX_CGX_CITY + 1, MIN_CGY_CITY + 1)], INF_VALUE);
    EXPECT_EQ(test_move_cost_map[Cell_Index(MAX_CGX_CITY + 1, CGY_GATE)], 0);
    EXPECT_EQ(test_move_cost_map[Cell_Index(MAX_CGX_CITY, MIN_CGY_CITY + 1)], 0);
}

TEST_F(Update_Move_Map_City_Area_Restrictions_test, FlyingUnitSkipsCityWallRestrictions)
{
    test_battle_units[0].cgx = 0;
    test_battle_units[0].cgy = 0;
    test_battle_units[0].Move_Flags = MV_FLYING;

    test_battlefield.walls[1][2] = ST_TRUE;

    Update_Move_Map_City_Area_Restrictions(/*battle_unit_idx=*/0);

    EXPECT_EQ(test_move_cost_map[Cell_Index(MIN_CGX_CITY + 2, MIN_CGY_CITY + 1)], 0);
    EXPECT_EQ(test_move_cost_map[Cell_Index(CGX_LAIR, CGY_LAIR)], INF_VALUE);
}