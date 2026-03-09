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

class CreateLairTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _LAIRS = static_cast<struct s_LAIR *>(malloc(sizeof(struct s_LAIR) * NUM_LAIRS));
        ASSERT_NE(_LAIRS, nullptr);

        memset(_LAIRS, 0xFF, sizeof(struct s_LAIR) * NUM_LAIRS);

        _difficulty = god_Normal;
        _magic = 1;
    }

    void TearDown() override
    {
        free(_LAIRS);
        _LAIRS = nullptr;
    }
};

TEST_F(CreateLairTest, SetsCoreRecordFieldsForExplicitLairTypes)
{
    Create_Lair(0, ARCANUS_PLANE, 10, 11, 3, 0, 800);
    EXPECT_EQ(_LAIRS[0].type, lt_Tower);
    EXPECT_EQ(_LAIRS[0].wp, ARCANUS_PLANE);
    EXPECT_EQ(_LAIRS[0].wx, 10);
    EXPECT_EQ(_LAIRS[0].wy, 11);
    EXPECT_EQ(_LAIRS[0].intact, ST_TRUE);

    Create_Lair(1, MYRROR_PLANE, 20, 21, 3, 1, 800);
    EXPECT_EQ(_LAIRS[1].type, lt_Sorcery_Node);

    Create_Lair(2, ARCANUS_PLANE, 30, 31, 3, 2, 800);
    EXPECT_EQ(_LAIRS[2].type, lt_Nature_Node);

    Create_Lair(3, MYRROR_PLANE, 40, 22, 3, 3, 800);
    EXPECT_EQ(_LAIRS[3].type, lt_Chaos_Node);
}

TEST_F(CreateLairTest, DefaultLairTypeFallsWithinRandomEncounterRange)
{
    for(int16_t itr = 0; itr < 20; itr++)
    {
        Create_Lair((20 + itr), ARCANUS_PLANE, 5, 5, 1, 4, 800);

        EXPECT_GE(_LAIRS[(20 + itr)].type, lt_Cave);
        EXPECT_LE(_LAIRS[(20 + itr)].type, lt_Fallen_Temple);
    }
}

TEST_F(CreateLairTest, GuardCountsStayWithinDocumentedBounds)
{
    Create_Lair(50, ARCANUS_PLANE, 12, 13, 3, 1, 2200);

    EXPECT_GE(_LAIRS[50].guard1_count, 0);
    EXPECT_GE(_LAIRS[50].guard2_count, 0);
    EXPECT_LE((_LAIRS[50].guard1_count + _LAIRS[50].guard2_count), 9);

    if(_LAIRS[50].guard1_unit_type == 0)
    {
        EXPECT_EQ(_LAIRS[50].guard1_count, 0);
    }

    if(_LAIRS[50].guard2_unit_type == 0)
    {
        EXPECT_EQ(_LAIRS[50].guard2_count, 0);
    }
}
