#include <gtest/gtest.h>

extern "C" {
#include "../src/MOD_Hook.h"
}

namespace
{

int32_t Add_Ten(int16_t hook_id, int16_t subject_idx, int32_t value, void * user_data)
{
    return value + 10;
}

int32_t Double_It(int16_t hook_id, int16_t subject_idx, int32_t value, void * user_data)
{
    return value * 2;
}

int32_t Record_Subject(int16_t hook_id, int16_t subject_idx, int32_t value, void * user_data)
{
    *static_cast<int16_t *>(user_data) = subject_idx;
    return value;
}

class MOD_Hook_test : public ::testing::Test
{
protected:
    void SetUp() override { MOD_Hook_Clear_All(); }
    void TearDown() override { MOD_Hook_Clear_All(); }
};

}  // namespace

TEST_F(MOD_Hook_test, NoHandler_ReturnsValueUnchanged)
{
    EXPECT_EQ(MOD_Hook_City_Value(MOD_HOOK_CITY_GOLD, 3, 17), 17);
    EXPECT_EQ(MOD_Hook_Count(MOD_HOOK_CITY_GOLD), 0);
}

TEST_F(MOD_Hook_test, Handler_AppliesOnlyToItsHook)
{
    ASSERT_EQ(MOD_Hook_Register(MOD_HOOK_CITY_GOLD, Add_Ten, nullptr), 0);
    EXPECT_EQ(MOD_Hook_City_Value(MOD_HOOK_CITY_GOLD, 0, 5), 15);
    EXPECT_EQ(MOD_Hook_City_Value(MOD_HOOK_CITY_FOOD, 0, 5), 5);
}

TEST_F(MOD_Hook_test, Chain_RunsInRegistrationOrder)
{
    ASSERT_EQ(MOD_Hook_Register(MOD_HOOK_CITY_PRODUCTION, Add_Ten, nullptr), 0);
    ASSERT_EQ(MOD_Hook_Register(MOD_HOOK_CITY_PRODUCTION, Double_It, nullptr), 0);
    EXPECT_EQ(MOD_Hook_City_Value(MOD_HOOK_CITY_PRODUCTION, 0, 1), 22);  /* (1 + 10) * 2 */
}

TEST_F(MOD_Hook_test, Handler_ReceivesSubjectIndex)
{
    int16_t seen = -1;
    ASSERT_EQ(MOD_Hook_Register(MOD_HOOK_CITY_POWER, Record_Subject, &seen), 0);
    MOD_Hook_City_Value(MOD_HOOK_CITY_POWER, 42, 0);
    EXPECT_EQ(seen, 42);
}

TEST_F(MOD_Hook_test, Register_RejectsBadInput)
{
    EXPECT_EQ(MOD_Hook_Register(-1, Add_Ten, nullptr), -1);
    EXPECT_EQ(MOD_Hook_Register(MOD_HOOK_COUNT, Add_Ten, nullptr), -1);
    EXPECT_EQ(MOD_Hook_Register(MOD_HOOK_CITY_GOLD, nullptr, nullptr), -1);
}

TEST_F(MOD_Hook_test, Register_RejectsFullChain)
{
    for(int itr = 0; itr < MOD_HOOK_MAX_HANDLERS; itr++)
    {
        ASSERT_EQ(MOD_Hook_Register(MOD_HOOK_CITY_RESEARCH, Add_Ten, nullptr), 0);
    }
    EXPECT_EQ(MOD_Hook_Register(MOD_HOOK_CITY_RESEARCH, Add_Ten, nullptr), -1);
    EXPECT_EQ(MOD_Hook_City_Value(MOD_HOOK_CITY_RESEARCH, 0, 0), 10 * MOD_HOOK_MAX_HANDLERS);
}

TEST_F(MOD_Hook_test, Result_ClampedToCityFieldRange)
{
    ASSERT_EQ(MOD_Hook_Register(MOD_HOOK_CITY_GOLD, Double_It, nullptr), 0);
    EXPECT_EQ(MOD_Hook_City_Value(MOD_HOOK_CITY_GOLD, 0, 200), 255);
    EXPECT_EQ(MOD_Hook_City_Value(MOD_HOOK_CITY_GOLD, 0, -5), 0);
    ASSERT_EQ(MOD_Hook_Register(MOD_HOOK_CITY_FOOD, Double_It, nullptr), 0);
    EXPECT_EQ(MOD_Hook_City_Value(MOD_HOOK_CITY_FOOD, 0, 100), 127);
    EXPECT_EQ(MOD_Hook_City_Value(MOD_HOOK_CITY_FOOD, 0, -100), -128);
}

TEST_F(MOD_Hook_test, NoHandler_DoesNotClamp)
{
    EXPECT_EQ(MOD_Hook_City_Value(MOD_HOOK_CITY_GOLD, 0, 300), 300);
}

TEST_F(MOD_Hook_test, ClearAll_RemovesHandlers)
{
    ASSERT_EQ(MOD_Hook_Register(MOD_HOOK_CITY_GOLD, Add_Ten, nullptr), 0);
    MOD_Hook_Clear_All();
    EXPECT_EQ(MOD_Hook_City_Value(MOD_HOOK_CITY_GOLD, 0, 5), 5);
}

TEST_F(MOD_Hook_test, Names_RoundTrip)
{
    for(int16_t itr = 0; itr < MOD_HOOK_COUNT; itr++)
    {
        ASSERT_NE(MOD_Hook_Name(itr), nullptr);
        EXPECT_EQ(MOD_Hook_Find(MOD_Hook_Name(itr)), itr);
    }
    EXPECT_STREQ(MOD_Hook_Name(MOD_HOOK_CITY_GOLD), "city_gold");
    EXPECT_EQ(MOD_Hook_Find("no_such_hook"), -1);
    EXPECT_EQ(MOD_Hook_Find(nullptr), -1);
    EXPECT_EQ(MOD_Hook_Name(MOD_HOOK_COUNT), nullptr);
}
