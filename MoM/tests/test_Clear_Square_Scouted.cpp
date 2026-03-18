#include <gtest/gtest.h>

#include <cstdlib>
#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DAT.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/Util.h"
#include "../src/Explore.h"
#ifdef __cplusplus
}
#endif

class Clear_Square_Scouted_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const size_t bit_field_size = (WORLD_SIZE / 8);
        const size_t explored_size = (WORLD_SIZE * NUM_PLANES);
        const size_t units_size = (sizeof(struct s_UNIT) * 4);

        square_scouted_p0 = static_cast<uint8_t *>(malloc(bit_field_size));
        square_scouted_p1 = static_cast<uint8_t *>(malloc(bit_field_size));
        _square_explored = static_cast<uint8_t *>(malloc(explored_size));
        _UNITS = static_cast<struct s_UNIT *>(malloc(units_size));

        ASSERT_NE(square_scouted_p0, nullptr);
        ASSERT_NE(square_scouted_p1, nullptr);
        ASSERT_NE(_square_explored, nullptr);
        ASSERT_NE(_UNITS, nullptr);

        memset(square_scouted_p0, 0, bit_field_size);
        memset(square_scouted_p1, 0, bit_field_size);
        memset(_square_explored, 0, explored_size);
        memset(_UNITS, 0, units_size);
        memset(_players, 0, sizeof(_players));

        _human_player_idx = 0;
        _units = 4;
    }

    void TearDown() override
    {
        free(_UNITS);
        free(_square_explored);
        free(square_scouted_p1);
        free(square_scouted_p0);
        _UNITS = nullptr;
        _square_explored = nullptr;
        square_scouted_p0 = nullptr;
        square_scouted_p1 = nullptr;
    }
};

TEST_F(Clear_Square_Scouted_test, CheckSquareScoutedReturnsTrueWhenScoutedAndExplored)
{
    const int16_t world_x = 8;
    const int16_t world_y = 10;

    Set_Square_Scouted(world_x, world_y, 0);
    SET_SQUARE_EXPLORED(world_x, world_y, 0, 0x0F);

    EXPECT_EQ(Check_Square_Scouted(world_x, world_y, 0), ST_TRUE);
}

TEST_F(Clear_Square_Scouted_test, CheckSquareScoutedReturnsFalseWhenNotExplored)
{
    const int16_t world_x = 9;
    const int16_t world_y = 10;

    Set_Square_Scouted(world_x, world_y, 0);

    EXPECT_EQ(Check_Square_Scouted(world_x, world_y, 0), ST_FALSE);
}

TEST_F(Clear_Square_Scouted_test, CheckSquareScoutedReturnsFalseWhenExploredButNotScouted)
{
    const int16_t world_x = 10;
    const int16_t world_y = 10;

    SET_SQUARE_EXPLORED(world_x, world_y, 1, 0x0F);

    EXPECT_EQ(Check_Square_Scouted(world_x, world_y, 1), ST_FALSE);
}

TEST_F(Clear_Square_Scouted_test, SetSquareScoutedSetsRequestedPlaneOnly)
{
    const int16_t world_x = 14;
    const int16_t world_y = 11;
    const int16_t world_square_idx = (world_y * WORLD_WIDTH) + world_x;

    Set_Square_Scouted(world_x, world_y, 0);

    EXPECT_EQ(Test_Bit_Field(world_square_idx, square_scouted_p0), ST_TRUE);
    EXPECT_EQ(Test_Bit_Field(world_square_idx, square_scouted_p1), ST_FALSE);
}

TEST_F(Clear_Square_Scouted_test, SetSquareScoutedSetsOnlyTargetSquare)
{
    const int16_t set_x = 21;
    const int16_t set_y = 3;
    const int16_t unset_x = 22;
    const int16_t unset_y = 3;
    const int16_t set_square_idx = (set_y * WORLD_WIDTH) + set_x;
    const int16_t unset_square_idx = (unset_y * WORLD_WIDTH) + unset_x;

    Set_Square_Scouted(set_x, set_y, 1);

    EXPECT_EQ(Test_Bit_Field(set_square_idx, square_scouted_p1), ST_TRUE);
    EXPECT_EQ(Test_Bit_Field(unset_square_idx, square_scouted_p1), ST_FALSE);
    EXPECT_EQ(Test_Bit_Field(set_square_idx, square_scouted_p0), ST_FALSE);
}

TEST_F(Clear_Square_Scouted_test, ClearsRequestedPlaneOnly)
{
    const int16_t world_x = 12;
    const int16_t world_y = 7;
    const int16_t world_square_idx = (world_y * WORLD_WIDTH) + world_x;

    Set_Square_Scouted(world_x, world_y, 0);
    Set_Square_Scouted(world_x, world_y, 1);

    ASSERT_EQ(Test_Bit_Field(world_square_idx, square_scouted_p0), ST_TRUE);
    ASSERT_EQ(Test_Bit_Field(world_square_idx, square_scouted_p1), ST_TRUE);

    Clear_Square_Scouted(world_x, world_y, 0);

    EXPECT_EQ(Test_Bit_Field(world_square_idx, square_scouted_p0), ST_FALSE);
    EXPECT_EQ(Test_Bit_Field(world_square_idx, square_scouted_p1), ST_TRUE);
}

TEST_F(Clear_Square_Scouted_test, ClearsOnlyTargetSquare)
{
    const int16_t clear_x = 5;
    const int16_t clear_y = 9;
    const int16_t keep_x = 6;
    const int16_t keep_y = 9;
    const int16_t clear_square_idx = (clear_y * WORLD_WIDTH) + clear_x;
    const int16_t keep_square_idx = (keep_y * WORLD_WIDTH) + keep_x;

    Set_Square_Scouted(clear_x, clear_y, 0);
    Set_Square_Scouted(keep_x, keep_y, 0);

    ASSERT_EQ(Test_Bit_Field(clear_square_idx, square_scouted_p0), ST_TRUE);
    ASSERT_EQ(Test_Bit_Field(keep_square_idx, square_scouted_p0), ST_TRUE);

    Clear_Square_Scouted(clear_x, clear_y, 0);

    EXPECT_EQ(Test_Bit_Field(clear_square_idx, square_scouted_p0), ST_FALSE);
    EXPECT_EQ(Test_Bit_Field(keep_square_idx, square_scouted_p0), ST_TRUE);
}

TEST_F(Clear_Square_Scouted_test, ClearSquareScoutedFlagsClearsPlaneZeroOnly)
{
    const size_t bit_field_size = (WORLD_SIZE / 8);

    memset(square_scouted_p0, 0xFF, bit_field_size);
    memset(square_scouted_p1, 0x5A, bit_field_size);

    Clear_Square_Scouted_Flags(0);

    for(size_t itr_bit_field = 0; itr_bit_field < bit_field_size; itr_bit_field++)
    {
        EXPECT_EQ(square_scouted_p0[itr_bit_field], 0) << "byte index " << itr_bit_field;
        EXPECT_EQ(square_scouted_p1[itr_bit_field], 0x5A) << "byte index " << itr_bit_field;
    }
}

TEST_F(Clear_Square_Scouted_test, ClearSquareScoutedFlagsClearsNonZeroPlaneOnly)
{
    const size_t bit_field_size = (WORLD_SIZE / 8);

    memset(square_scouted_p0, 0xA5, bit_field_size);
    memset(square_scouted_p1, 0xFF, bit_field_size);

    Clear_Square_Scouted_Flags(1);

    for(size_t itr_bit_field = 0; itr_bit_field < bit_field_size; itr_bit_field++)
    {
        EXPECT_EQ(square_scouted_p0[itr_bit_field], 0xA5) << "byte index " << itr_bit_field;
        EXPECT_EQ(square_scouted_p1[itr_bit_field], 0) << "byte index " << itr_bit_field;
    }
}

TEST_F(Clear_Square_Scouted_test, ContactOtherPlayerSetsMutualContactWhenForeignUnitIsVisible)
{
    const int16_t unit_idx = 0;
    const int16_t owner_idx = 1;
    const int16_t world_x = 17;
    const int16_t world_y = 12;
    const int16_t world_p = 0;

    _UNITS[unit_idx].owner_idx = owner_idx;
    _UNITS[unit_idx].wx = static_cast<int8_t>(world_x);
    _UNITS[unit_idx].wy = static_cast<int8_t>(world_y);
    _UNITS[unit_idx].wp = static_cast<int8_t>(world_p);
    _UNITS[unit_idx].in_tower = ST_FALSE;
    _UNITS[unit_idx].enchantments = 0;

    SET_SQUARE_EXPLORED(world_x, world_y, world_p, 0x0F);
    Set_Square_Scouted(world_x, world_y, world_p);

    Contact_Other_Player(unit_idx, world_p);

    EXPECT_EQ(_players[_human_player_idx].Dipl.Contacted[owner_idx], ST_TRUE);
    EXPECT_EQ(_players[owner_idx].Dipl.Contacted[_human_player_idx], ST_TRUE);
}

TEST_F(Clear_Square_Scouted_test, ContactOtherPlayerIgnoresHumanOwnedUnits)
{
    const int16_t unit_idx = 0;
    const int16_t world_x = 18;
    const int16_t world_y = 12;
    const int16_t world_p = 0;

    _UNITS[unit_idx].owner_idx = static_cast<int8_t>(_human_player_idx);
    _UNITS[unit_idx].wx = static_cast<int8_t>(world_x);
    _UNITS[unit_idx].wy = static_cast<int8_t>(world_y);
    _UNITS[unit_idx].wp = static_cast<int8_t>(world_p);

    SET_SQUARE_EXPLORED(world_x, world_y, world_p, 0x0F);
    Set_Square_Scouted(world_x, world_y, world_p);

    Contact_Other_Player(unit_idx, world_p);

    EXPECT_EQ(_players[_human_player_idx].Dipl.Contacted[1], ST_FALSE);
    EXPECT_EQ(_players[1].Dipl.Contacted[_human_player_idx], ST_FALSE);
}

TEST_F(Clear_Square_Scouted_test, ContactOtherPlayerIgnoresInvisibleUnits)
{
    const int16_t unit_idx = 0;
    const int16_t owner_idx = 1;
    const int16_t world_x = 19;
    const int16_t world_y = 12;
    const int16_t world_p = 0;

    _UNITS[unit_idx].owner_idx = owner_idx;
    _UNITS[unit_idx].wx = static_cast<int8_t>(world_x);
    _UNITS[unit_idx].wy = static_cast<int8_t>(world_y);
    _UNITS[unit_idx].wp = static_cast<int8_t>(world_p);
    _UNITS[unit_idx].enchantments = UE_INVISIBILITY;

    SET_SQUARE_EXPLORED(world_x, world_y, world_p, 0x0F);
    Set_Square_Scouted(world_x, world_y, world_p);

    Contact_Other_Player(unit_idx, world_p);

    EXPECT_EQ(_players[_human_player_idx].Dipl.Contacted[owner_idx], ST_FALSE);
    EXPECT_EQ(_players[owner_idx].Dipl.Contacted[_human_player_idx], ST_FALSE);
}

TEST_F(Clear_Square_Scouted_test, ContactOtherPlayerRequiresSquareToBeScouted)
{
    const int16_t unit_idx = 0;
    const int16_t owner_idx = 1;
    const int16_t world_x = 20;
    const int16_t world_y = 12;
    const int16_t world_p = 0;

    _UNITS[unit_idx].owner_idx = owner_idx;
    _UNITS[unit_idx].wx = static_cast<int8_t>(world_x);
    _UNITS[unit_idx].wy = static_cast<int8_t>(world_y);
    _UNITS[unit_idx].wp = static_cast<int8_t>(world_p);
    _UNITS[unit_idx].enchantments = 0;

    SET_SQUARE_EXPLORED(world_x, world_y, world_p, 0x0F);

    Contact_Other_Player(unit_idx, world_p);

    EXPECT_EQ(_players[_human_player_idx].Dipl.Contacted[owner_idx], ST_FALSE);
    EXPECT_EQ(_players[owner_idx].Dipl.Contacted[_human_player_idx], ST_FALSE);
}