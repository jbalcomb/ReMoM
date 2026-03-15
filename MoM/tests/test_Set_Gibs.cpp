#include <gtest/gtest.h>
#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/MOM_DAT.h"

void Set_Gibs(int16_t battle_unit_idx, int16_t Damage);
extern int16_t m_gibs_frames[20];

#ifdef __cplusplus
}
#endif

class Set_Gibs_test : public ::testing::Test
{
protected:
    struct s_BATTLE_UNIT test_battle_units[1];
    struct s_BATTLE_UNIT * saved_battle_units = nullptr;
    int16_t saved_gibs_frames[20];

    void SetUp() override
    {
        saved_battle_units = battle_units;
        memcpy(saved_gibs_frames, m_gibs_frames, sizeof(saved_gibs_frames));

        memset(test_battle_units, 0, sizeof(test_battle_units));
        battle_units = &test_battle_units[0];

        test_battle_units[0].hits = 4;
        test_battle_units[0].Cur_Figures = 8;
    }

    void TearDown() override
    {
        memcpy(m_gibs_frames, saved_gibs_frames, sizeof(saved_gibs_frames));
        battle_units = saved_battle_units;
    }
};

TEST_F(Set_Gibs_test, PositiveDamage_SetsMinimumOneGibWhenFormulaRoundsToZero)
{
    Set_Gibs(/*battle_unit_idx=*/0, /*Damage=*/1);

    EXPECT_EQ(test_battle_units[0].gibs, 1);
    EXPECT_EQ(test_battle_units[0].Atk_FigLoss, 1);
}

TEST_F(Set_Gibs_test, LargeDamage_ClampsGibsToFiveAndFigureLossToCurrentFigures)
{
    test_battle_units[0].Cur_Figures = 2;

    Set_Gibs(/*battle_unit_idx=*/0, /*Damage=*/50);

    EXPECT_EQ(test_battle_units[0].gibs, 5);
    EXPECT_EQ(test_battle_units[0].Atk_FigLoss, 2);
}

TEST_F(Set_Gibs_test, ZeroCurrentFigures_UsesDefaultGibValueAndNoFigureLoss)
{
    test_battle_units[0].Cur_Figures = 0;

    Set_Gibs(/*battle_unit_idx=*/0, /*Damage=*/10);

    EXPECT_EQ(test_battle_units[0].gibs, 3);
    EXPECT_EQ(test_battle_units[0].Atk_FigLoss, 0);
}

TEST_F(Set_Gibs_test, RefreshesAllGibFramesWithRandom)
{
    Set_Gibs(/*battle_unit_idx=*/0, /*Damage=*/2);

    for(int idx = 0; idx < 20; idx++)
    {
        // Random(4) - 1 can produce values from -1 to at least 3
        // depending on the actual Random() implementation
        EXPECT_TRUE(m_gibs_frames[idx] >= -1 && m_gibs_frames[idx] <= 3);
    }
}
