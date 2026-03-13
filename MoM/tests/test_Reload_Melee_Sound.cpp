#include <gtest/gtest.h>
#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../src/UNITTYPE.h"

SAMB_ptr Reload_Melee_Sound(int16_t battle_unit_idx, uint32_t * sound_seg_size);
#ifdef __cplusplus
}
#endif

class Reload_Melee_Sound_test : public ::testing::Test
{
protected:
    struct s_BATTLE_UNIT test_battle_units[1];
    struct s_UNIT test_units[1];

    struct s_BATTLE_UNIT * saved_battle_units = nullptr;
    struct s_UNIT * saved_units = nullptr;
    int16_t saved_sound_value = 0;

    void SetUp() override
    {
        memset(test_battle_units, 0, sizeof(test_battle_units));
        memset(test_units, 0, sizeof(test_units));

        saved_battle_units = battle_units;
        saved_units = _UNITS;
        saved_sound_value = _unit_type_table[0].Sound;

        battle_units = &test_battle_units[0];
        _UNITS = &test_units[0];

        test_battle_units[0].unit_idx = 0;
        test_units[0].type = 0;
    }

    void TearDown() override
    {
        _unit_type_table[0].Sound = saved_sound_value;
        _UNITS = saved_units;
        battle_units = saved_battle_units;
    }
};

TEST_F(Reload_Melee_Sound_test, Regression_LowNibble8_CurrentImplementationDies)
{
    _unit_type_table[0].Sound = 0x08;

    ASSERT_DEATH(
        {
            uint32_t sound_seg_size = 0;
            (void)Reload_Melee_Sound(0, &sound_seg_size);
        },
        ".*");
}

TEST_F(Reload_Melee_Sound_test, Regression_LowNibbleNon8_CurrentImplementationDies)
{
    _unit_type_table[0].Sound = 0x05;

    ASSERT_DEATH(
        {
            uint32_t sound_seg_size = 0;
            (void)Reload_Melee_Sound(0, &sound_seg_size);
        },
        ".*");
}
