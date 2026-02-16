/*

NOTE(JimBalcomb,202510091924): was to be some normal and hero units kill tests, but lost track, and create hero wont work :(..

MOM_Create_Unit()       twice, diff params, test those and all defaults
MOM_Kill_Unit_kt0()     
MOM_Kill_Unit_kt1()
MOM_Kill_Unit_kt2()

HIRE.c
    int16_t Hire_Hero_Popup(int16_t hero_slot_idx, int16_t unit_type_idx, int16_t hire_type)
        Hire_Success = WIZ_HireHero(HUMAN_PLAYER_IDX, unit_type_idx, hero_slot_idx, ST_FALSE);
Spells132.c
    int16_t WIZ_HireHero(int16_t player_idx, int16_t unit_type_idx, int16_t hero_slot_idx, int16_t saved_flag)
        Create_Unit__WIP(unit_type_idx, player_idx, FORTX(), FORTY(), FORTP(), -1);

*/
#include <gtest/gtest.h>

// #include "MOM_Test_Helpers.hpp"
#include "MOM_Test_Helpers.cpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/Allocate.h" /* _cities[], _UNITS[] */
#include "../../MoX/src/LBX_Load.h" /* LBX_Load_Data_Static() */
#include "../../MoX/src/MOM_Data.h" /* _cities[], _UNITS[] */
#include "../../MoX/src/MOX_DAT.h"  /* _players[] */
#include "../../MoX/src/MOX_DEF.h"  /* HUMAN_PLAYER_IDX */
#include "../../MoX/src/MOX_UPD.h"  /* UNITS_FINISHED */

#include "../src/CITYCALC.h"
#include "../src/NEXTTURN.h"
#include "../src/Spells130.h"       /* Hero_Slot_Types () */
#include "../src/Spells132.h"       /* WIZ_HireHero() */
#include "../src/UNITTYPE.h"

/*
    Forward Declare, Private
*/
// Spells132.c
// WZD dseg:68A8
char names_lbx_file__ovr132[];

#ifdef __cplusplus
}
#endif

class MyClassTestFixture : public ::testing::Test {
protected:
    // // Declare objects that will be used by all tests in this fixture
    // MyClass* my_object; 

    // SetUp() is called before each test runs
    void SetUp() override {
        // // Initialize resources (e.g., create new objects, open files/DB connections)
        // my_object = new MyClass();
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

    // TearDown() is called after each test runs
    void TearDown() override {
        // // Clean up resources (e.g., delete objects, close connections)
        // delete my_object;
        // my_object = nullptr; // Best practice to prevent dangling pointers
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

    // You can also add helper functions here
};

TEST_F(MyClassTestFixture, MOM_Create_Unit)
{
    EXPECT_EQ(_units, 0);
    Create_Unit__WIP(ut_LizSettlers, 0, 1, 1, 0, 2000);
    EXPECT_EQ(_units, 1);
    EXPECT_EQ(_UNITS[0].wx, 1);                     // _UNITS[_units].wx = wx;
    EXPECT_EQ(_UNITS[0].wy, 1);                     // _UNITS[_units].wy = wy;
    EXPECT_EQ(_UNITS[0].wp, 0);                     // _UNITS[_units].wp = wp;
    EXPECT_EQ(_UNITS[0].owner_idx, 0);              // _UNITS[_units].owner_idx = owner_idx;
    // _UNITS[_units].moves2_max = _unit_type_table[unit_type].Move_Halves;
    EXPECT_EQ(_UNITS[0].type, ut_LizSettlers);      // _UNITS[_units].type = unit_type;
    EXPECT_EQ(_UNITS[0].Hero_Slot, ST_UNDEFINED);             // _UNITS[_units].Hero_Slot = -1;
    EXPECT_EQ(_UNITS[0].in_tower, ST_FALSE);        // _UNITS[_units].in_tower = ST_FALSE;
    EXPECT_EQ(_UNITS[0].Finished, ST_TRUE);         // _UNITS[_units].Finished = ST_TRUE;
    EXPECT_EQ(_UNITS[0].moves2, 0);                 // _UNITS[_units].moves2 = 0;
    // _UNITS[_units].Sight_Range = _unit_type_table[unit_type].Sight;
    EXPECT_EQ(_UNITS[0].dst_wx, 0);                 // _UNITS[_units].dst_wx = 0;
    EXPECT_EQ(_UNITS[0].dst_wy, 0);                 // _UNITS[_units].dst_wy = 0;
    EXPECT_EQ(_UNITS[0].Status, us_Ready);          // _UNITS[_units].Status = us_Ready;
    EXPECT_EQ(_UNITS[0].Level, 0);                  // _UNITS[_units].Level = 0;
    EXPECT_EQ(_UNITS[0].XP, 0);                     // _UNITS[_units].XP = 0;
    EXPECT_EQ(_UNITS[0].Damage, 0);                 // _UNITS[_units].Damage = 0;
    EXPECT_EQ(_UNITS[0].Draw_Priority, 0);          // _UNITS[_units].Draw_Priority = 0;
    EXPECT_EQ(_UNITS[0].enchantments, 0);           // _UNITS[_units].enchantments = 0;
    EXPECT_EQ(_UNITS[0].mutations, 0);              // _UNITS[_units].mutations = 0;
    EXPECT_EQ(_UNITS[0].Move_Failed, ST_FALSE);     // _UNITS[_units].Move_Failed = ST_FALSE;
    EXPECT_EQ(_UNITS[0].Rd_Constr_Left, -1);        // _UNITS[_units].Rd_Constr_Left = -1;
    Create_Unit__WIP(ut_LizHalberdiers, 1, 2, 2, 1, 2000);
    EXPECT_EQ(_units, 2);
    EXPECT_EQ(_UNITS[1].wx, 2);                         // _UNITS[_units].wx = wx;
    EXPECT_EQ(_UNITS[1].wy, 2);                         // _UNITS[_units].wy = wy;
    EXPECT_EQ(_UNITS[1].wp, 1);                         // _UNITS[_units].wp = wp;
    EXPECT_EQ(_UNITS[1].owner_idx, 1);                  // _UNITS[_units].owner_idx = owner_idx;
    // _UNITS[_units].moves2_max = _unit_type_table[unit_type].Move_Halves;
    EXPECT_EQ(_UNITS[1].type, ut_LizHalberdiers);       // _UNITS[_units].type = unit_type;
    EXPECT_EQ(_UNITS[1].Hero_Slot, ST_UNDEFINED);       // _UNITS[_units].Hero_Slot = ST_UNDEFINED;
    EXPECT_EQ(_UNITS[1].in_tower, ST_FALSE);            // _UNITS[_units].in_tower = ST_FALSE;
    EXPECT_EQ(_UNITS[1].Finished, ST_TRUE);             // _UNITS[_units].Finished = ST_TRUE;
    EXPECT_EQ(_UNITS[1].moves2, 0);                     // _UNITS[_units].moves2 = 0;
    // _UNITS[_units].Sight_Range = _unit_type_table[unit_type].Sight;
    EXPECT_EQ(_UNITS[1].dst_wx, 0);                     // _UNITS[_units].dst_wx = 0;
    EXPECT_EQ(_UNITS[1].dst_wy, 0);                     // _UNITS[_units].dst_wy = 0;
    EXPECT_EQ(_UNITS[1].Status, us_Ready);              // _UNITS[_units].Status = us_Ready;
    EXPECT_EQ(_UNITS[1].Level, 0);                      // _UNITS[_units].Level = 0;
    EXPECT_EQ(_UNITS[1].XP, 0);                         // _UNITS[_units].XP = 0;
    EXPECT_EQ(_UNITS[1].Damage, 0);                     // _UNITS[_units].Damage = 0;
    EXPECT_EQ(_UNITS[1].Draw_Priority, 0);              // _UNITS[_units].Draw_Priority = 0;
    EXPECT_EQ(_UNITS[1].enchantments, 0);               // _UNITS[_units].enchantments = 0;
    EXPECT_EQ(_UNITS[1].mutations, 0);                  // _UNITS[_units].mutations = 0;
    EXPECT_EQ(_UNITS[1].Move_Failed, ST_FALSE);         // _UNITS[_units].Move_Failed = ST_FALSE;
    EXPECT_EQ(_UNITS[1].Rd_Constr_Left, ST_UNDEFINED);  // _UNITS[_units].Rd_Constr_Left = ST_UNDEFINED;
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
//     EXPECT_EQ(Unit_Base_Level(0), 0);
// 
// }
// TEST(MOM_test, MOM_Remove_Item)
// {
//     
// }
TEST_F(MyClassTestFixture, MOM_Kill_Unit_kt0)
{
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
    EXPECT_EQ(_UNITS[0].owner_idx, ST_UNDEFINED);
    EXPECT_EQ(_UNITS[0].wp, ST_UNDEFINED);
    Kill_Unit(1, kt_Dismissed);
    EXPECT_EQ(_UNITS[1].owner_idx, ST_UNDEFINED);
    EXPECT_EQ(_UNITS[1].wp, ST_UNDEFINED);
    Kill_Unit(2, kt_Disappeared);
    EXPECT_EQ(_UNITS[2].owner_idx, ST_UNDEFINED);
    EXPECT_EQ(_UNITS[2].wp, ST_UNDEFINED);
 }

TEST_F(MyClassTestFixture, MOM_Kill_Unit_kt1)
{
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
    EXPECT_EQ(_UNITS[0].owner_idx, ST_UNDEFINED);
    EXPECT_EQ(_UNITS[0].wp, ST_UNDEFINED);
}

TEST_F(MyClassTestFixture, MOM_Kill_Unit_kt2)
{
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
    EXPECT_EQ(_UNITS[0].owner_idx, ST_UNDEFINED);
    EXPECT_EQ(_UNITS[0].wp, ST_UNDEFINED);
}

TEST_F(MyClassTestFixture, MOM_Delete_Dead_Units)
{
    EXPECT_EQ(_units, 0);
    Create_Unit__WIP(ut_LizSettlers, 0, 1, 1, 0, 2000);
    EXPECT_EQ(_units, 1);
    Kill_Unit(0, kt_Normal);
    EXPECT_EQ(_units, 1);
    Delete_Dead_Units();  // decrements _units
    EXPECT_EQ(_units, 0);
}



/*

*/
// TEST(MOM_test, MOM_Destroy_City)
// {
// 
// }
