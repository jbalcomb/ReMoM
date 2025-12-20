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



void Set_Up(void)
{
    _units = 0;
    _UNITS = (struct s_UNIT *)Allocate_Space(2028);  // 2028 PR, 32448 B
    _HEROES2[0] = (struct s_HEROES *)Allocate_Space(28);  // 28 PR, 448 B
    _HEROES2[1] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
    _HEROES2[2] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
    _HEROES2[3] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
    _HEROES2[4] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
    _HEROES2[5] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
    hero_names_table = (struct s_INACTV_HERO *)Allocate_Space(37);  // 37 PR, 592 B  ... ~ (36) 16-byte structs
    spell_data_table = (struct s_SPELL_DATA *)Allocate_Space(485);  // 485 PR, 7760 B; actual: 215 * 36 = 7740
    _FORTRESSES = (struct s_FORTRESS *)Allocate_Space(3);

}
void Tear_Down(void)
{
    free(_FORTRESSES);
    free(spell_data_table);
    free(hero_names_table);
    free(_HEROES2[5]);
    free(_HEROES2[4]);
    free(_HEROES2[3]);
    free(_HEROES2[2]);
    free(_HEROES2[1]);
    free(_HEROES2[0]);
    free(_UNITS);
    _units = 0;
}


TEST(MOM_test, MOM_Create_Unit)
{
    Set_Up();

    EXPECT_EQ(_units, 0);

    Create_Unit__WIP(ut_LizSettlers, 0, 1, 1, 0, 2000);

    EXPECT_EQ(_units, 1);

    EXPECT_EQ(_UNITS[0].type, ut_LizSettlers);
    EXPECT_EQ(_UNITS[0].owner_idx, 0);
    EXPECT_EQ(_UNITS[0].wx, 1);
    EXPECT_EQ(_UNITS[0].wy, 1);
    EXPECT_EQ(_UNITS[0].wp, 0);

    Create_Unit__WIP(ut_LizHalberdiers, 1, 2, 2, 1, 2000);

    EXPECT_EQ(_units, 2);

    EXPECT_EQ(_UNITS[1].type, ut_LizHalberdiers);
    EXPECT_EQ(_UNITS[1].owner_idx, 1);
    EXPECT_EQ(_UNITS[1].wx, 2);
    EXPECT_EQ(_UNITS[1].wy, 2);
    EXPECT_EQ(_UNITS[1].wp, 1);

    Kill_Unit(0, kt_Normal);

    Kill_Unit(1, kt_Disappeared);

    EXPECT_EQ(_units, 2);

    Delete_Dead_Units();

    EXPECT_EQ(_units, 0);

    Tear_Down();
}

TEST(MOM_test, MOM_Delete_Dead_Units)
{
    Set_Up();

    EXPECT_EQ(_units, 0);

    Create_Unit__WIP(ut_LizSettlers, 0, 1, 1, 0, 2000);

    EXPECT_EQ(_units, 1);
    
    Kill_Unit(0, kt_Normal);

    EXPECT_EQ(_units, 1);

    Delete_Dead_Units();  // decrements _units
    
    EXPECT_EQ(_units, 0);

    Tear_Down();
}
