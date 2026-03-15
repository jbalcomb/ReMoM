#include <gtest/gtest.h>
#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_SET.h"

void Clear_Gibs(int16_t battle_unit_idx);
void Make_Missiles(int16_t missile_count, int16_t Targets, int16_t SrcX, int16_t SrcY, int16_t TgtX, int16_t TgtY, int16_t Type);
void Melee_Animation(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx, int16_t attacker_damage, int16_t defender_damage, int16_t cgx, int16_t cgy);
#ifdef __cplusplus
}
#endif

class Clear_Gibs_test : public ::testing::Test
{
protected:
    struct s_BATTLE_UNIT test_battle_units[1];
    struct s_BATTLE_UNIT * saved_battle_units = nullptr;

    void SetUp() override
    {
        saved_battle_units = battle_units;

        memset(test_battle_units, 0, sizeof(test_battle_units));
        battle_units = &test_battle_units[0];
    }

    void TearDown() override
    {
        battle_units = saved_battle_units;
    }
};

TEST_F(Clear_Gibs_test, ClearsGibsAndFigureLossToZero)
{
    test_battle_units[0].gibs = 5;
    test_battle_units[0].Atk_FigLoss = 8;

    Clear_Gibs(/*battle_unit_idx=*/0);

    EXPECT_EQ(test_battle_units[0].gibs, 0);
    EXPECT_EQ(test_battle_units[0].Atk_FigLoss, 0);
}

TEST_F(Clear_Gibs_test, IdempotentClear_ZeroRemainsZero)
{
    test_battle_units[0].gibs = 0;
    test_battle_units[0].Atk_FigLoss = 0;

    Clear_Gibs(/*battle_unit_idx=*/0);

    EXPECT_EQ(test_battle_units[0].gibs, 0);
    EXPECT_EQ(test_battle_units[0].Atk_FigLoss, 0);
}

TEST_F(Clear_Gibs_test, MultipleCalls_ConsistentlyClearsState)
{
    test_battle_units[0].gibs = 3;
    test_battle_units[0].Atk_FigLoss = 4;

    Clear_Gibs(/*battle_unit_idx=*/0);
    EXPECT_EQ(test_battle_units[0].gibs, 0);
    EXPECT_EQ(test_battle_units[0].Atk_FigLoss, 0);

    test_battle_units[0].gibs = 2;
    test_battle_units[0].Atk_FigLoss = 1;

    Clear_Gibs(/*battle_unit_idx=*/0);
    EXPECT_EQ(test_battle_units[0].gibs, 0);
    EXPECT_EQ(test_battle_units[0].Atk_FigLoss, 0);
}


class Melee_Animation_test : public ::testing::Test
{
protected:
    struct s_BATTLE_UNIT test_battle_units[2];
    struct s_BATTLE_UNIT * saved_battle_units = nullptr;
    struct s_MAGIC_SET saved_magic_set;
    // Keep this fixture local to combat animation state behavior.

    void SetUp() override
    {
        saved_battle_units = battle_units;
        saved_magic_set = magic_set;

        memset(test_battle_units, 0, sizeof(test_battle_units));
        battle_units = &test_battle_units[0];

        magic_set.sound_effects = ST_FALSE;
        magic_set.movement_animations = ST_FALSE;

        test_battle_units[0].cgx = 4;
        test_battle_units[0].cgy = 5;
        test_battle_units[0].Cur_Figures = 6;
        test_battle_units[0].hits = 4;

        test_battle_units[1].cgx = 10;
        test_battle_units[1].cgy = 11;
        test_battle_units[1].Cur_Figures = 7;
        test_battle_units[1].hits = 5;
    }

    void TearDown() override
    {
        magic_set = saved_magic_set;
        battle_units = saved_battle_units;
    }
};

TEST_F(Melee_Animation_test, Regression_UnitVsUnit_CurrentImplementationDiesWithoutCombatScreenInit)
{
    ASSERT_DEATH(
        {
            Melee_Animation(
                /*attacker_battle_unit_idx=*/0,
                /*defender_battle_unit_idx=*/1,
                /*attacker_damage=*/3,
                /*defender_damage=*/4,
                /*cgx=*/0,
                /*cgy=*/0);
        },
        ".*");
}

TEST_F(Melee_Animation_test, Regression_UnitVsCell_CurrentImplementationDiesWithoutCombatScreenInit)
{
    ASSERT_DEATH(
        {
            Melee_Animation(
                /*attacker_battle_unit_idx=*/0,
                /*defender_battle_unit_idx=*/99,
                /*attacker_damage=*/2,
                /*defender_damage=*/0,
                /*cgx=*/13,
                /*cgy=*/3);
        },
        ".*");
}


TEST(Make_Missiles_test, Regression_CurrentImplementationDiesWithoutMissileBufferInit)
{
    ASSERT_DEATH(
        {
            Make_Missiles(
                /*missile_count=*/1,
                /*Targets=*/1,
                /*SrcX=*/6,
                /*SrcY=*/8,
                /*TgtX=*/10,
                /*TgtY=*/12,
                /*Type=*/1);
        },
        ".*");
}
