#include <gtest/gtest.h>

// #include "MOM_Test_Helpers.hpp"
#include "MOM_Test_Helpers.cpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/Allocate.h" /* _cities[], _UNITS[] */
#include "../../MoX/src/MOM_Data.h" /* _cities[], _UNITS[] */
#include "../../MoX/src/MOX_DAT.h"  /* _players[] */
#include "../../MoX/src/MOX_DEF.h"  /* HUMAN_PLAYER_IDX */
#include "../../MoX/src/MOX_UPD.h"  /* UNITS_FINISHED */

#include "../src/CITYCALC.h"
#include "../src/NEXTTURN.h"
#include "../src/UNITTYPE.h"
#ifdef __cplusplus
}
#endif

/*

NEXTTURN.c
int16_t Create_Unit__WIP(int16_t unit_type, int16_t owner_idx, int16_t wx, int16_t wy, int16_t wp, int16_t R_Param)

*/
TEST(MOM_test, MOM_Create_Unit)
{
    _units = 0;
    _UNITS = (struct s_UNIT * )Allocate_Space((sizeof(struct s_UNIT) * 2) / SZ_PARAGRAPH_B);

    ASSERT_EQ(_units, 0);

    Create_Unit__WIP(ut_LizSettlers, 0, 1, 1, 0, 2000);

    ASSERT_EQ(_units, 1);

    ASSERT_EQ(_UNITS[0].type, ut_LizSettlers);
    ASSERT_EQ(_UNITS[0].owner_idx, 0);
    ASSERT_EQ(_UNITS[0].wx, 1);
    ASSERT_EQ(_UNITS[0].wy, 1);
    ASSERT_EQ(_UNITS[0].wp, 0);
    // _UNITS[_units].wx = wx;
    // _UNITS[_units].wy = wy;
    // _UNITS[_units].wp = wp;
    // _UNITS[_units].owner_idx = owner_idx;
    // _UNITS[_units].moves2_max = _unit_type_table[unit_type].Move_Halves;
    // _UNITS[_units].type = unit_type;
    // _UNITS[_units].Hero_Slot = -1;
    // _UNITS[_units].in_tower = ST_FALSE;
    // _UNITS[_units].Finished = ST_TRUE;
    // _UNITS[_units].moves2 = 0;
    // _UNITS[_units].Sight_Range = _unit_type_table[unit_type].Sight;
    // _UNITS[_units].dst_wx = 0;
    // _UNITS[_units].dst_wy = 0;
    // _UNITS[_units].Status = us_Ready;
    // _UNITS[_units].Level = 0;
    // _UNITS[_units].XP = 0;
    // _UNITS[_units].Damage = 0;
    // _UNITS[_units].Draw_Priority = 0;
    // _UNITS[_units].enchantments = 0;
    // _UNITS[_units].mutations = 0;
    // _UNITS[_units].Move_Failed = ST_FALSE;
    // _UNITS[_units].Rd_Constr_Left = -1;

    Create_Unit__WIP(ut_LizHalberdiers, 1, 2, 2, 1, 2000);

    ASSERT_EQ(_units, 2);

    ASSERT_EQ(_UNITS[1].type, ut_LizHalberdiers);
    ASSERT_EQ(_UNITS[1].owner_idx, 1);
    ASSERT_EQ(_UNITS[1].wx, 2);
    ASSERT_EQ(_UNITS[1].wy, 2);
    ASSERT_EQ(_UNITS[1].wp, 1);

    /*
        Tear-Down
    */
    free(_UNITS);
    _units = 0;
}

/*
3 tests, one including hero with items

kill_type e_KILL_TYPE kt_Normal, kt_Dismissed, kt_Disappeared

    _UNITS[unit_idx].Level = Unit_Base_Level(unit_idx);
    if((kill_type == kt_Dismissed) || (_UNITS[unit_idx].type == ut_Chosen))
        _UNITS[unit_idx].Finished = ST_TRUE;    AKA UNITS_FINISHED(unit_idx, ST_TRUE);
        if(_UNITS[unit_idx].Hero_Slot > -1)
            ...
    else  / * ((kill_type != 1) && (_UNITS[unit_idx].type != ut_Chosen)) * /
        UNITS_FINISHED(unit_idx, ST_TRUE);
        if(_UNITS[unit_idx].Hero_Slot > -1)
            ...
    UNITS_OWNER(unit_idx, ST_UNDEFINED);
    UNITS_WP(unit_idx, ST_UNDEFINED);  // ¿ here because `wp = 9` is used for dead combat summon units ?



Kill_Unit() DEPENDS ON ...

string      strcpy()
CITYCALC    Unit_Base_Level()
Items       Remove_Item()

struct s_UNIT * _UNITS;
struct s_HEROES * _HEROES2[NUM_PLAYERS];
struct s_INACTV_HERO * hero_names_table;

_UNITS[].Finished
_UNITS[].Hero_Slot
_UNITS[].Level
_UNITS[].owner_idx
_UNITS[].type
_UNITS[].XP

_players[].Heroes[].Items[]
_players[].Heroes[].name
_players[].Heroes[].unit_idx

_HEROES2[]->heroes[].Level

hero_names_table[].experience_points
hero_names_table[].name

Unit_Base_Level() depends on...
spell_data_table[]
TBL_Experience[]

ALLOC.c
Allocate_Data_Space()
spell_data_table = (struct s_SPELL_DATA *)Allocate_Space(485);  // 485 PR, 7760 B; actual: 215 * 36 = 7740
MOM_Data.c
int16_t TBL_Experience[9] = {0, 20, 60, 120, 200, 300, 450, 600, 1000};
*/
/*
Kill_Unit()
    _UNITS[unit_idx].Level = Unit_Base_Level(unit_idx);
CITYCALC    int16_t Unit_Base_Level(int16_t unit_idx)

*/
// TEST(MOM_test, MOM_Unit_Base_Level)
// {
//     _units = 0;
//     _UNITS = (struct s_UNIT * )Allocate_Space((sizeof(struct s_UNIT) * 2) / SZ_PARAGRAPH_B);
// 
//     Create_Unit__WIP(ut_LizSettlers, 0, 1, 1, 0, 2000);
// 
//     ASSERT_EQ(Unit_Base_Level(0), 0);
// 
// }
// TEST(MOM_test, MOM_Remove_Item)
// {
//     
// }
TEST(MOM_test, MOM_Kill_Unit_kt0)
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

    EXPECT_EQ(_units, 0);

    Create_Unit__WIP(ut_LizSettlers, 0, 1, 1, 0, 2000);

    EXPECT_EQ(_units, 1);

    EXPECT_EQ(_UNITS[0].type, ut_LizSettlers);
    EXPECT_EQ(_UNITS[0].owner_idx, 0);
    EXPECT_EQ(_UNITS[0].wx, 1);
    EXPECT_EQ(_UNITS[0].wy, 1);
    EXPECT_EQ(_UNITS[0].wp, 0);

    Create_Unit__WIP(ut_LizSettlers, 0, 1, 1, 0, 2000);

    EXPECT_EQ(_units, 2);

    EXPECT_EQ(_UNITS[1].type, ut_LizSettlers);
    EXPECT_EQ(_UNITS[1].owner_idx, 0);
    EXPECT_EQ(_UNITS[1].wx, 1);
    EXPECT_EQ(_UNITS[1].wy, 1);
    EXPECT_EQ(_UNITS[1].wp, 0);

    Create_Unit__WIP(ut_LizSettlers, 0, 1, 1, 0, 2000);

    EXPECT_EQ(_units, 3);

    EXPECT_EQ(_UNITS[2].type, ut_LizSettlers);
    EXPECT_EQ(_UNITS[2].owner_idx, 0);
    EXPECT_EQ(_UNITS[2].wx, 1);
    EXPECT_EQ(_UNITS[2].wy, 1);
    EXPECT_EQ(_UNITS[2].wp, 0);
    
    Kill_Unit(0, kt_Normal);

    ASSERT_EQ(_UNITS[0].owner_idx, ST_UNDEFINED);
    ASSERT_EQ(_UNITS[0].wp, ST_UNDEFINED);

    Kill_Unit(1, kt_Dismissed);

    ASSERT_EQ(_UNITS[1].owner_idx, ST_UNDEFINED);
    ASSERT_EQ(_UNITS[1].wp, ST_UNDEFINED);

    Kill_Unit(2, kt_Disappeared);

    ASSERT_EQ(_UNITS[2].owner_idx, ST_UNDEFINED);
    ASSERT_EQ(_UNITS[2].wp, ST_UNDEFINED);
 
    /*
        Tear-Down
    */
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
// _units = 0;
// _UNITS = (struct s_UNIT *)Allocate_Space(2028);  // 2028 PR, 32448 B
// _HEROES2[0] = (struct s_HEROES *)Allocate_Space(28);  // 28 PR, 448 B
// _HEROES2[1] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
// _HEROES2[2] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
// _HEROES2[3] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
// _HEROES2[4] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
// _HEROES2[5] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
// hero_names_table = (struct s_INACTV_HERO *)Allocate_Space(37);  // 37 PR, 592 B  ... ~ (36) 16-byte structs
// spell_data_table = (struct s_SPELL_DATA *)Allocate_Space(485);  // 485 PR, 7760 B; actual: 215 * 36 = 7740
}

TEST(MOM_test, MOM_Kill_Unit_kt1)
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


    
    Kill_Unit(0, kt_Dismissed);

    ASSERT_EQ(_UNITS[0].owner_idx, ST_UNDEFINED);
    ASSERT_EQ(_UNITS[0].wp, ST_UNDEFINED);

    /*
        Tear-Down
    */
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

TEST(MOM_test, MOM_Kill_Unit_kt2)
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


    
    Kill_Unit(0, kt_Disappeared);

//             if(kill_type != kt_Disappeared)  /* ¿ must be type 0 kt_Normal ? */
//             {
//             }
//             else
//             {
//                 _HEROES2[_UNITS[unit_idx].owner_idx]->heroes[_UNITS[unit_idx].type].Level = -20;

    ASSERT_EQ(_UNITS[0].owner_idx, ST_UNDEFINED);
    ASSERT_EQ(_UNITS[0].wp, ST_UNDEFINED);

    /*
        Tear-Down
    */
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

/*

*/
// TEST(MOM_test, MOM_Destroy_City)
// {
// 
// }
