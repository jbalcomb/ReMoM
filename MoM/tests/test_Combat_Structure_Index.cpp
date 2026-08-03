/*
    Combat central-structure square index.

    Ground truth is the disassembly. Two independent listings write the central-structure
    square as a raw literal displacement into _cmbt_movepath_cost_map:

        ovr091/Assign_Combat_Grids.asm:85-89
            les     bx, [battlefield]
            cmp     [es:bx+s_BATTLEFIELD.center_square_structure], CS_None
            jz      short loc_780C6
            mov     bx, [_cmbt_movepath_cost_map]
            mov     [byte ptr bx+0EDh], -1

        ovr124/BU_SetCityMovement.asm:38-41
            cmp     [es:bx+s_BATTLEFIELD.center_square_structure], CS_City
            jz      short loc_A778E
            mov     bx, [_cmbt_movepath_cost_map]
            mov     [byte ptr bx+0EDh], e_INF

    The index convention is (cgy * COMBAT_GRID_WIDTH) + cgx — ovr091/Assign_Combat_Grids.asm:38-48
    loads cgy, multiplies by 21, then adds cgx. So 0EDh = 237 = (11 * 21) + 6, i.e. cgx 6 / cgy 11.

    These tests pin that literal and check that every name the codebase has for that square
    agrees with it.
*/

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

    /* The displacement written by ovr091/Assign_Combat_Grids.asm:89 and ovr124/BU_SetCityMovement.asm:41. */
    constexpr int OG_CENTRAL_STRUCTURE_DISPLACEMENT = 0xED;

    int Cell_Index(int16_t cgx, int16_t cgy)
    {
        return ((cgy * COMBAT_GRID_WIDTH) + cgx);
    }
}


/*
    Constant-level checks. These are deliberately EXPECT_EQ rather than static_assert so a
    mismatch reports as a failing test instead of breaking the build for everyone.
*/

TEST(Combat_Structure_Index, Index_Convention_Is_Row_Major_On_COMBAT_GRID_WIDTH)
{
    /* Guards the arithmetic the rest of this file depends on. */
    EXPECT_EQ(COMBAT_GRID_WIDTH, 21);
    EXPECT_EQ(Cell_Index(6, 11), OG_CENTRAL_STRUCTURE_DISPLACEMENT);
}

TEST(Combat_Structure_Index, OG_Displacement_Decodes_To_cgx6_cgy11)
{
    EXPECT_EQ((OG_CENTRAL_STRUCTURE_DISPLACEMENT / COMBAT_GRID_WIDTH), 11);   /* cgy */
    EXPECT_EQ((OG_CENTRAL_STRUCTURE_DISPLACEMENT % COMBAT_GRID_WIDTH), 6);    /* cgx */
}

TEST(Combat_Structure_Index, CGX_LAIR_CGY_LAIR_Match_The_OG_Displacement)
{
    /* CMBTDEF.h's pair is the one that already agrees with the disassembly. */
    EXPECT_EQ(CGX_LAIR, 6);
    EXPECT_EQ(CGY_LAIR, 11);
    EXPECT_EQ(Cell_Index(CGX_LAIR, CGY_LAIR), OG_CENTRAL_STRUCTURE_DISPLACEMENT);
}

TEST(Combat_Structure_Index, COMBAT_STRUCTURE_IDX_Matches_The_OG_Displacement)
{
    /*
        Combat.h's COMBAT_STRUCTURE_IDX must name the same square the OG writes.
        Any disagreement here means the macro blocks a different cell than the original.
    */
    EXPECT_EQ(COMBAT_STRUCTURE_IDX, OG_CENTRAL_STRUCTURE_DISPLACEMENT);
}

TEST(Combat_Structure_Index, COMBAT_STRUCTURE_CG_Coordinates_Match_CMBTDEF)
{
    /* Both headers describe the same square; they must not disagree about which axis is which. */
    EXPECT_EQ(COMBAT_STRUCTURE_CGX, CGX_LAIR);
    EXPECT_EQ(COMBAT_STRUCTURE_CGY, CGY_LAIR);
}


/*
    Behavioural check. Update_Move_Map_City_Area_Restrictions is the one function in this
    area that computes the square via CGX_LAIR/CGY_LAIR rather than COMBAT_STRUCTURE_IDX,
    so running it shows which cell actually gets blocked.

    Fixture mirrors test_Update_Move_Map_City_Area_Restrictions.cpp.
*/
class Combat_Structure_Index_behaviour : public ::testing::Test
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

        /* Lair / Node / Tower shape: a central structure, no city walls. */
        test_battlefield.center_square_structure = CS_None;
        test_battlefield.walled = ST_FALSE;

        /* Flying short-circuits the wall/perimeter branches, leaving only the
           central-structure write from the top of the function. */
        test_battle_units[0].cgx = 0;
        test_battle_units[0].cgy = 0;
        test_battle_units[0].Move_Flags = MV_FLYING;
    }

    void TearDown() override
    {
        _cmbt_movepath_cost_map = saved_move_cost_map;
        battle_units = saved_battle_units;
        battlefield = saved_battlefield;
    }
};

TEST_F(Combat_Structure_Index_behaviour, Blocked_Cell_Is_The_OG_Displacement)
{
    Update_Move_Map_City_Area_Restrictions(/*battle_unit_idx=*/0);

    EXPECT_EQ(test_move_cost_map[OG_CENTRAL_STRUCTURE_DISPLACEMENT], INF_VALUE);
}

TEST_F(Combat_Structure_Index_behaviour, Blocked_Cell_Is_The_One_COMBAT_STRUCTURE_IDX_Names)
{
    /*
        The cell the OG blocks and the cell COMBAT_STRUCTURE_IDX names must be the same cell.
        If this fails while the test above passes, the macro points somewhere else.
    */
    Update_Move_Map_City_Area_Restrictions(/*battle_unit_idx=*/0);

    EXPECT_EQ(test_move_cost_map[COMBAT_STRUCTURE_IDX], INF_VALUE);
}
