#include "pch.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

#ifdef __cplusplus
extern "C" {
#endif
#include "Allocate.h"  /* _cities[], _UNITS[] */
#include "MOM_Data.h"  /* _cities[], _UNITS[] */
#include "CITYCALC.h"
#include "NEXTTURN.h"
#include "UNITTYPE.h"
#ifdef __cplusplus
}
#endif

TEST(MOM_test, MOM_Create_Unit)
{
    _units = 0;
    _UNITS = (struct s_UNIT * )Allocate_Space((sizeof(struct s_UNIT) * 2) / SZ_PARAGRAPH_B);

    ASSERT_EQ(_units, 0);

    Create_Unit__WIP(ut_LizSettlers, 0, 1, 1, 0, 2000);

    ASSERT_EQ(_units, 1);

    ASSERT_EQ(_UNITS[0].type, ut_LizSettlers);
    ASSERT_EQ(_UNITS[1].owner_idx, 0);
    ASSERT_EQ(_UNITS[1].wx, 1);
    ASSERT_EQ(_UNITS[1].wy, 1);
    ASSERT_EQ(_UNITS[1].wp, 0);

    Create_Unit__WIP(ut_LizHalberdiers, 1, 2, 2, 1, 2000);

    ASSERT_EQ(_units, 2);

    ASSERT_EQ(_UNITS[1].type, ut_LizHalberdiers);
    ASSERT_EQ(_UNITS[1].owner_idx, 1);
    ASSERT_EQ(_UNITS[1].wx, 2);
    ASSERT_EQ(_UNITS[1].wy, 2);
    ASSERT_EQ(_UNITS[1].wp, 1);

    Kill_Unit(1,1);
    Kill_Unit(0,1);
    Delete_Dead_Units();

}
