#include <gtest/gtest.h>

#include <cstdlib>
#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
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

        square_scouted_p0 = static_cast<uint8_t *>(malloc(bit_field_size));
        square_scouted_p1 = static_cast<uint8_t *>(malloc(bit_field_size));
        _square_explored = static_cast<uint8_t *>(malloc(explored_size));

        ASSERT_NE(square_scouted_p0, nullptr);
        ASSERT_NE(square_scouted_p1, nullptr);
        ASSERT_NE(_square_explored, nullptr);

        memset(square_scouted_p0, 0, bit_field_size);
        memset(square_scouted_p1, 0, bit_field_size);
        memset(_square_explored, 0, explored_size);
    }

    void TearDown() override
    {
        free(_square_explored);
        free(square_scouted_p1);
        free(square_scouted_p0);
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