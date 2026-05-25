#include <gtest/gtest.h>

#include <cstdlib>
#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/EMS/EMS.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../src/AIDUDES.h"
#include "../src/AIMOVE.h"
#include "../src/UNITTYPE.h"
#ifdef __cplusplus
}
#endif

extern "C"
{
    extern int16_t _turn;
    extern int16_t cp_drafted_unit_count;
    extern int16_t G_Seafaring_Count;
    extern int16_t G_Pushout_Lowest_Value;
    extern int16_t G_Seafaring_Lowest_Value;
    extern int16_t G_Pushout_UL_Indices[MAX_STACK];
    extern int16_t G_Pushout_CX_IDs[MAX_STACK];
    extern int16_t G_Pushout_Values[MAX_STACK];
    extern int16_t G_Pushout_Unit_Indices[MAX_STACK];
    extern int16_t G_Seafaring_UL_Indices[MAX_STACK];
    extern int16_t G_Seafaring_CX_IDs[MAX_STACK];
    extern int16_t G_Seafaring_Values[MAX_STACK];
    extern int16_t G_Seafaring_Unit_Indices[MAX_STACK];
}

class AIStacksSurveyExpeditionForcesTest : public ::testing::Test
{
protected:
    struct s_UNIT * saved_units = nullptr;
    int16_t saved_unit_count = 0;
    int16_t saved_turn = 0;
    SAMB_ptr saved_ems_pfba = nullptr;
    SAMB_ptr saved_emm_handle = nullptr;

    void SetUp() override
    {
        int stack_idx = 0;
        int list_idx = 0;

        saved_units = _UNITS;
        saved_unit_count = _units;
        saved_turn = _turn;
        saved_ems_pfba = EMS_PFBA;
        saved_emm_handle = EmmHndl_CONTXXX;

        _UNITS = static_cast<struct s_UNIT *>(calloc(MAX_AI_STACKS, sizeof(struct s_UNIT)));
        ASSERT_NE(_UNITS, nullptr);
        _units = 0;

        EmmHndl_CONTXXX = Allocate_Space(4096);
        ASSERT_NE(EmmHndl_CONTXXX, nullptr);
        memset(EmmHndl_CONTXXX, 0, (4096 * SZ_PARAGRAPH_B));
        EMM_Map_CONTXXX__WIP();

        _ai_own_stack_count = 0;
        cp_drafted_unit_count = 0;
        G_Seafaring_Count = 0;
        G_Pushout_Lowest_Value = 0;
        G_Seafaring_Lowest_Value = 0;

        for(stack_idx = 0; stack_idx < MAX_AI_STACKS; stack_idx++)
        {
            _ai_own_stack_type[stack_idx] = AISTK_DNE;
            _ai_own_stack_unit_count[stack_idx] = 0;

            for(list_idx = 0; list_idx < MAX_STACK; list_idx++)
            {
                _ai_own_stack_unit_list[stack_idx][list_idx] = ST_UNDEFINED;
            }
        }

        for(list_idx = 0; list_idx < MAX_STACK; list_idx++)
        {
            G_Pushout_UL_Indices[list_idx] = ST_UNDEFINED;
            G_Pushout_CX_IDs[list_idx] = ST_UNDEFINED;
            G_Pushout_Values[list_idx] = ST_UNDEFINED;
            G_Pushout_Unit_Indices[list_idx] = ST_UNDEFINED;
            G_Seafaring_UL_Indices[list_idx] = ST_UNDEFINED;
            G_Seafaring_CX_IDs[list_idx] = ST_UNDEFINED;
            G_Seafaring_Values[list_idx] = ST_UNDEFINED;
            G_Seafaring_Unit_Indices[list_idx] = ST_UNDEFINED;
        }
    }

    void TearDown() override
    {
        free(EmmHndl_CONTXXX);
        EmmHndl_CONTXXX = saved_emm_handle;
        EMS_PFBA = saved_ems_pfba;

        free(_UNITS);
        _UNITS = saved_units;
        _units = saved_unit_count;
        _turn = saved_turn;
    }

    int16_t Add_Unit(int16_t unit_type)
    {
        const int16_t unit_idx = _units;

        _UNITS[unit_idx].type = unit_type;
        _UNITS[unit_idx].enchantments = 0;
        _UNITS[unit_idx].mutations = 0;
        _units++;

        return unit_idx;
    }

    void Seed_Stack(int stack_idx, int stack_type, int unit_count, const int16_t * unit_indices)
    {
        int list_idx = 0;

        _ai_own_stack_type[stack_idx] = static_cast<uint8_t>(stack_type);
        _ai_own_stack_unit_count[stack_idx] = static_cast<int16_t>(unit_count);

        for(list_idx = 0; list_idx < MAX_STACK; list_idx++)
        {
            _ai_own_stack_unit_list[stack_idx][list_idx] = ST_UNDEFINED;
        }

        for(list_idx = 0; list_idx < unit_count; list_idx++)
        {
            _ai_own_stack_unit_list[stack_idx][list_idx] = unit_indices[list_idx];
        }

        if((stack_idx + 1) > _ai_own_stack_count)
        {
            _ai_own_stack_count = static_cast<int16_t>(stack_idx + 1);
        }
    }
};

TEST_F(AIStacksSurveyExpeditionForcesTest, StackHookFiltersNonMilitaryUnitsBeforeDrafting)
{
    const int16_t mixed_units[MAX_STACK] =
    {
        Add_Unit(ut_LizHalberdiers),
        Add_Unit(ut_HMenEngineers),
        Add_Unit(ut_LizSettlers)
    };

    Seed_Stack(0, AISTK_Unknown, 3, mixed_units);
    G_Pushout_Lowest_Value = 10000;
    G_Seafaring_Lowest_Value = 10000;

    AI_Stacks_Survey_Expedition_Forces_Stack_Test_Hook(0, 3, 3);

    ASSERT_EQ(cp_drafted_unit_count, 1);
    EXPECT_EQ(G_Pushout_UL_Indices[0], 0);
    EXPECT_EQ(G_Pushout_CX_IDs[0], 0);
    EXPECT_EQ(G_Pushout_Unit_Indices[0], mixed_units[0]);
}

TEST_F(AIStacksSurveyExpeditionForcesTest, StackHookSkipsUndefinedSlots)
{
    const int16_t sparse_units[MAX_STACK] =
    {
        ST_UNDEFINED,
        Add_Unit(ut_LizHalberdiers)
    };

    Seed_Stack(2, AISTK_Unknown, 2, sparse_units);
    G_Pushout_Lowest_Value = 10000;
    G_Seafaring_Lowest_Value = 10000;

    AI_Stacks_Survey_Expedition_Forces_Stack_Test_Hook(2, 2, 2);

    ASSERT_EQ(cp_drafted_unit_count, 1);
    EXPECT_EQ(G_Pushout_CX_IDs[0], 2);
    EXPECT_EQ(G_Pushout_UL_Indices[0], 1);
    EXPECT_EQ(G_Pushout_Unit_Indices[0], sparse_units[1]);
}

TEST_F(AIStacksSurveyExpeditionForcesTest, StackHookHonorsExcessCountLimit)
{
    const int16_t combat_units[MAX_STACK] =
    {
        Add_Unit(ut_LizHalberdiers),
        Add_Unit(ut_LizSwordsmen)
    };

    Seed_Stack(4, AISTK_Garrison, 2, combat_units);
    G_Pushout_Lowest_Value = 10000;
    G_Seafaring_Lowest_Value = 10000;

    AI_Stacks_Survey_Expedition_Forces_Stack_Test_Hook(4, 2, 1);

    ASSERT_EQ(cp_drafted_unit_count, 1);
    EXPECT_EQ(G_Pushout_CX_IDs[0], 4);
    EXPECT_TRUE(
        (G_Pushout_UL_Indices[0] == 0)
        ||
        (G_Pushout_UL_Indices[0] == 1)
    );
    EXPECT_TRUE(
        (G_Pushout_Unit_Indices[0] == combat_units[0])
        ||
        (G_Pushout_Unit_Indices[0] == combat_units[1])
    );
    EXPECT_NE(G_Pushout_Unit_Indices[0], ST_UNDEFINED);
}

TEST_F(AIStacksSurveyExpeditionForcesTest, ResetsAndHonorsStackEligibilityRules)
{
    const int16_t unknown_units[MAX_STACK] =
    {
        Add_Unit(ut_LizHalberdiers)
    };
    const int16_t roamer_units[MAX_STACK] =
    {
        ST_UNDEFINED,
        Add_Unit(ut_LizHalberdiers)
    };
    const int16_t small_garrison_units[MAX_STACK] =
    {
        Add_Unit(ut_LizHalberdiers),
        Add_Unit(ut_LizHalberdiers),
        Add_Unit(ut_LizHalberdiers),
        Add_Unit(ut_LizHalberdiers),
        Add_Unit(ut_LizHalberdiers)
    };
    const int16_t large_garrison_units[MAX_STACK] =
    {
        Add_Unit(ut_LizHalberdiers),
        Add_Unit(ut_LizHalberdiers),
        Add_Unit(ut_LizHalberdiers),
        Add_Unit(ut_LizHalberdiers),
        Add_Unit(ut_LizHalberdiers),
        Add_Unit(ut_LizHalberdiers)
    };
    const int16_t fortress_units[MAX_STACK] =
    {
        Add_Unit(ut_LizHalberdiers),
        Add_Unit(ut_LizHalberdiers),
        Add_Unit(ut_LizHalberdiers),
        Add_Unit(ut_LizHalberdiers),
        Add_Unit(ut_LizHalberdiers),
        Add_Unit(ut_LizHalberdiers)
    };

    cp_drafted_unit_count = 7;
    G_Seafaring_Count = 5;
    G_Pushout_Lowest_Value = 123;
    G_Seafaring_Lowest_Value = 456;
    _turn = 99;

    Seed_Stack(0, AISTK_Unknown, 1, unknown_units);
    Seed_Stack(1, AISTK_Roamer, 2, roamer_units);
    Seed_Stack(2, AISTK_Garrison, 5, small_garrison_units);
    Seed_Stack(3, AISTK_Garrison, 6, large_garrison_units);
    Seed_Stack(4, AISTK_FortressGarrison, 6, fortress_units);

    AI_Stacks_Survey_Expedition_Forces_Test_Hook();

    EXPECT_EQ(cp_drafted_unit_count, 3);
    EXPECT_EQ(G_Pushout_Lowest_Value, 10000);
    EXPECT_EQ(G_Seafaring_Lowest_Value, 10000);
}

TEST_F(AIStacksSurveyExpeditionForcesTest, FlyingCombatUnitAlsoEntersSeafaringPool)
{
    const int16_t flying_units[MAX_STACK] =
    {
        Add_Unit(ut_Pegasai)
    };

    _turn = 10;
    Seed_Stack(0, AISTK_Unknown, 1, flying_units);

    AI_Stacks_Survey_Expedition_Forces_Test_Hook();

    ASSERT_EQ(cp_drafted_unit_count, 1);
    ASSERT_EQ(G_Seafaring_Count, 1);
    EXPECT_EQ(G_Pushout_Unit_Indices[0], flying_units[0]);
    EXPECT_EQ(G_Seafaring_Unit_Indices[0], flying_units[0]);
}

TEST_F(AIStacksSurveyExpeditionForcesTest, FortressGarrisonStopsContributingAtTurn100)
{
    const int16_t fortress_units[MAX_STACK] =
    {
        Add_Unit(ut_LizHalberdiers),
        Add_Unit(ut_LizHalberdiers),
        Add_Unit(ut_LizHalberdiers),
        Add_Unit(ut_LizHalberdiers),
        Add_Unit(ut_LizHalberdiers),
        Add_Unit(ut_LizHalberdiers)
    };

    _turn = 100;
    Seed_Stack(0, AISTK_FortressGarrison, 6, fortress_units);

    AI_Stacks_Survey_Expedition_Forces_Test_Hook();

    EXPECT_EQ(cp_drafted_unit_count, 0);
    EXPECT_EQ(G_Seafaring_Count, 0);
}