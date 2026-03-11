#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/MOM_Data.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/Mouse.h"
int16_t Ranged_Mouse_Image(int16_t attacker_idx, int16_t defender_idx);
#ifdef __cplusplus
}
#endif

#include <cstring>

// Private global in Combat.c used by Ranged_Mouse_Image().
extern "C" int16_t _attacker_sees_illusions;

class Ranged_Mouse_Image_test : public ::testing::Test
{
protected:
    struct s_BATTLE_UNIT test_units[2];
    struct s_BATTLE_UNIT * saved_battle_units = nullptr;
    int16_t saved_attacker_sees_illusions = 0;

    void SetUp() override
    {
        saved_battle_units = battle_units;
        saved_attacker_sees_illusions = _attacker_sees_illusions;

        memset(test_units, 0, sizeof(test_units));
        battle_units = &test_units[0];
        _attacker_sees_illusions = ST_FALSE;
    }

    void TearDown() override
    {
        battle_units = saved_battle_units;
        _attacker_sees_illusions = saved_attacker_sees_illusions;
    }

    int16_t Call()
    {
        return Ranged_Mouse_Image(/*attacker_idx=*/0, /*defender_idx=*/1);
    }
};

TEST_F(Ranged_Mouse_Image_test, InvalidRangedGroup_ReturnsRedCross)
{
    test_units[0].ranged_type = 0;

    EXPECT_EQ(Call(), crsr_RedCross);
}

TEST_F(Ranged_Mouse_Image_test, ValidMissile_NoInvisibility_ReturnsRanged)
{
    test_units[0].ranged_type = 20;  // 20 / 10 == rag_Missile

    EXPECT_EQ(Call(), crsr_Ranged);
}

TEST_F(Ranged_Mouse_Image_test, DefenderInvisibleByEnchantment_NoSight_ReturnsRedCross)
{
    test_units[0].ranged_type = 30;  // 30 / 10 == rag_Magic
    test_units[1].enchantments = UE_INVISIBILITY;
    _attacker_sees_illusions = ST_FALSE;

    EXPECT_EQ(Call(), crsr_RedCross);
}

TEST_F(Ranged_Mouse_Image_test, DefenderInvisibleByEnchantment_SeesIllusions_ReturnsRanged)
{
    test_units[0].ranged_type = 10;  // 10 / 10 == rag_Boulder
    test_units[1].enchantments = UE_INVISIBILITY;
    _attacker_sees_illusions = ST_TRUE;

    EXPECT_EQ(Call(), crsr_Ranged);
}

TEST_F(Ranged_Mouse_Image_test, DefenderInvisibleByAbility_NoSight_ReturnsRedCross)
{
    test_units[0].ranged_type = 20;  // 20 / 10 == rag_Missile
    test_units[1].Abilities = UA_INVISIBILITY;
    _attacker_sees_illusions = ST_FALSE;

    EXPECT_EQ(Call(), crsr_RedCross);
}

TEST_F(Ranged_Mouse_Image_test, AttackerTruesight_OverridesDefenderInvisibility)
{
    test_units[0].ranged_type = 20;  // 20 / 10 == rag_Missile
    test_units[0].enchantments = UE_TRUESIGHT;
    test_units[1].enchantments = UE_INVISIBILITY;
    _attacker_sees_illusions = ST_FALSE;

    EXPECT_EQ(Call(), crsr_Ranged);
}
