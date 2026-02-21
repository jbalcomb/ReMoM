#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/MOM_Data.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../src/MAPGEN.h"
#ifdef __cplusplus
}
#endif

#include <cstdlib>

class SetUpperLairGuardianCountTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _LAIRS = static_cast<struct s_LAIR *>(malloc(sizeof(struct s_LAIR) * NUM_LAIRS));
        ASSERT_NE(_LAIRS, nullptr);

        for(int16_t itr = 0; itr < NUM_LAIRS; itr++)
        {
            _LAIRS[itr].guard1_count = 0;
            _LAIRS[itr].guard2_count = 0;
        }
    }

    void TearDown() override
    {
        free(_LAIRS);
        _LAIRS = nullptr;
    }
};

TEST_F(SetUpperLairGuardianCountTest, CopiesLowNibbleToHighNibble)
{
    for(int16_t itr = 0; itr < NUM_LAIRS; itr++)
    {
        uint8_t guard1 = static_cast<uint8_t>(itr % 10);
        uint8_t guard2 = static_cast<uint8_t>((itr + 3) % 10);
        _LAIRS[itr].guard1_count = guard1;
        _LAIRS[itr].guard2_count = guard2;
    }

    Set_Upper_Lair_Guardian_Count();

    for(int16_t itr = 0; itr < NUM_LAIRS; itr++)
    {
        uint8_t guard1 = static_cast<uint8_t>(itr % 10);
        uint8_t guard2 = static_cast<uint8_t>((itr + 3) % 10);
        uint8_t expected_guard1 = static_cast<uint8_t>(guard1 | (guard1 << 4));
        uint8_t expected_guard2 = static_cast<uint8_t>(guard2 | (guard2 << 4));

        EXPECT_EQ(_LAIRS[itr].guard1_count, expected_guard1);
        EXPECT_EQ(_LAIRS[itr].guard2_count, expected_guard2);
    }
}

TEST_F(SetUpperLairGuardianCountTest, LeavesZeroCountsAsZero)
{
    Set_Upper_Lair_Guardian_Count();

    for(int16_t itr = 0; itr < NUM_LAIRS; itr++)
    {
        EXPECT_EQ(_LAIRS[itr].guard1_count, 0);
        EXPECT_EQ(_LAIRS[itr].guard2_count, 0);
    }
}
