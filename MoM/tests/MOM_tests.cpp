/*
GoogleTest FAQ
Why should test suite names and test names not contain underscore?
Note: GoogleTest reserves underscore (_) for special-purpose keywords, such as the DISABLED_ prefix, in addition to the following rationale.

Underscore (_) is special, as C++ reserves the following to be used by the compiler and the standard library:

any identifier that starts with an _ followed by an upper-case letter, and
any identifier that contains two consecutive underscores (i.e. __) anywhere in its name.
User code is prohibited from using such identifiers.

So for simplicity, we just ask the users to avoid _ in TestSuiteName and TestName. The rule is more constraining than necessary, but it’s simple and easy to remember. It also gives GoogleTest some wiggle room in case its implementation needs to change in the future.
*/
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
#include "../../MoX/src/Util.h"  /* Delete_Structure() */

#include "../src/CITYCALC.h"
#include "../src/NEXTTURN.h"
#include "../src/Spells130.h"       /* Hero_Slot_Types () */
#include "../src/Spells132.h"       /* WIZ_HireHero() */
#include "../src/UNITTYPE.h"

#include "../../STU/src/STU_DBG.h"  /* Debug_Log_Startup(), Debug_Log_Shutdown() */



/*
    Forward Declare, Private
*/
// Spells132.c
// WZD dseg:68A8
char names_lbx_file__ovr132[];

#ifdef __cplusplus
}
#endif

void Set_Up(void)
{
    Debug_Log_Startup();
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
    Debug_Log_Shutdown();
}

class MOM_test2 : public testing::Test
 {

    void SetUp()
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
    
    void TearDown()
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

 };

/*

NEXTTURN.c
int16_t Create_Unit__WIP(int16_t unit_type, int16_t owner_idx, int16_t wx, int16_t wy, int16_t wp, int16_t R_Param)

*/
TEST(MOM_test, MOM_Create_Unit)
{
    _units = 0;
    _UNITS = (struct s_UNIT * )Allocate_Space((sizeof(struct s_UNIT) * 2) / SZ_PARAGRAPH_B);

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

    /*
        Tear-Down
    */
    free(_UNITS);
    _units = 0;
}



// NOTE(JimBalcomb,202510091913): locks up the exe; not sure why, gave up quick; debug tests as regular code? fix this and add the other kill units tests!
// /*
// Spells132.c int16_t WIZ_HireHero(int16_t player_idx, int16_t unit_type_idx, int16_t hero_slot_idx, int16_t saved_flag)
// */
// TEST(MOM_test, MOM_WIZ_HireHero)
// {
//     _units = 0;
//     _UNITS = (struct s_UNIT * )Allocate_Space((sizeof(struct s_UNIT) * 2) / SZ_PARAGRAPH_B);
//     _HEROES2[0] = (struct s_HEROES *)Allocate_Space(28);  // 28 PR, 448 B
//     // struct s_FORTRESS * _FORTRESSES;
//     // _FORTRESSES = (struct s_FORTRESS *)Allocate_Space(3);
//     _FORTRESSES = (struct s_FORTRESS *)Allocate_Space(3);
// 
//     _FORTRESSES[0].wx = 0;
//     _FORTRESSES[0].wy = 0;
//     _FORTRESSES[0].wp = 0;
//     _FORTRESSES[0].active = ST_TRUE;
// 
//     EXPECT_EQ(_units, 0);
//     
//     WIZ_HireHero(0, ut_Warlock, 0, ST_FALSE);
//     // int16_t WIZ_HireHero(int16_t player_idx, int16_t unit_type_idx, int16_t hero_slot_idx, int16_t saved_flag)
//     int16_t player_idx = 0;
//     int16_t unit_type_idx = ut_Warlock;
//     int16_t hero_slot_idx = 0;
//     int16_t saved_flag = ST_FALSE;
//     int16_t itr = 0;
//     Create_Unit__WIP(unit_type_idx, player_idx, FORTX(), FORTY(), FORTP(), ST_UNDEFINED);
//     _UNITS[(_units - 1)].Finished = ST_FALSE;  // default is ST_TRUE
//     _UNITS[(_units - 1)].moves2 = _UNITS[(_units - 1)].moves2_max;  // default is 0
//     _UNITS[(_units - 1)].Hero_Slot = hero_slot_idx;  // default is ST_UNDEFINED
//     _players[player_idx].Heroes[hero_slot_idx].unit_idx = (_units - 1);
//     Hero_Slot_Types(unit_type_idx, _players[player_idx].Heroes[hero_slot_idx].Item_Slots);
//     for(itr = 0; itr < NUM_HERO_ITEM_SLOTS; itr++)
//     {
//         _players[HUMAN_PLAYER_IDX].Heroes[hero_slot_idx].Items[itr] = ST_UNDEFINED;
//     }
//     if(saved_flag == ST_TRUE)
//     {
//         if(player_idx == HUMAN_PLAYER_IDX)
//         {
//             strcpy(_players[player_idx].Heroes[hero_slot_idx].name, hero_names_table[unit_type_idx].name);
//             _UNITS[(_units - 1)].XP = hero_names_table[unit_type_idx].experience_points;
//             _UNITS[(_units - 1)].Level = Calc_Unit_Level((_units - 1));
//         }
//         else
//         {
//             LBX_Load_Data_Static(names_lbx_file__ovr132, 0, (SAMB_ptr)_players[player_idx].Heroes[hero_slot_idx].name, ((player_idx * 35) + unit_type_idx), 1, 13);
//             _UNITS[(_units - 1)].Level = abs(_HEROES2[player_idx]->heroes[unit_type_idx].Level);
//             _UNITS[(_units - 1)].XP = TBL_Experience[_UNITS[(_units - 1)].Level];
//         }
//     }
//     else
//     {
//         LBX_Load_Data_Static(names_lbx_file__ovr132, 0, (SAMB_ptr)_players[player_idx].Heroes[hero_slot_idx].name, ((player_idx * 35) + unit_type_idx), 1, 13);
//         SETMAX(_HEROES2[player_idx]->heroes[unit_type_idx].Level, HL_GRANDLORD);
//     }
//     _UNITS[(_units - 1)].Level = _HEROES2[player_idx]->heroes[unit_type_idx].Level;
//     _UNITS[(_units - 1)].XP = TBL_Experience[_HEROES2[player_idx]->heroes[unit_type_idx].Level];
//     // return ST_TRUE;
// 
// 
// 
//     EXPECT_EQ(_units, 1);
// 
//     EXPECT_EQ(_UNITS[0].wx, 2);                     // _UNITS[_units].wx = wx;
//     EXPECT_EQ(_UNITS[0].wy, 2);                     // _UNITS[_units].wy = wy;
//     EXPECT_EQ(_UNITS[0].wp, 1);                     // _UNITS[_units].wp = wp;
//     EXPECT_EQ(_UNITS[0].owner_idx, 1);              // _UNITS[_units].owner_idx = owner_idx;
//     // _UNITS[_units].moves2_max = _unit_type_table[unit_type].Move_Halves;
//     EXPECT_EQ(_UNITS[0].type, ut_Warlock);   // _UNITS[_units].type = unit_type;
//     // DIFF  EXPECT_EQ(_UNITS[1].Hero_Slot, ST_UNDEFINED);       // _UNITS[_units].Hero_Slot = ST_UNDEFINED;
//     EXPECT_EQ(_UNITS[0].in_tower, ST_FALSE);        // _UNITS[_units].in_tower = ST_FALSE;
//     // DIFF  EXPECT_EQ(_UNITS[1].Finished, ST_TRUE);             // _UNITS[_units].Finished = ST_TRUE;
//     // DIFF  EXPECT_EQ(_UNITS[1].moves2, 0);                     // _UNITS[_units].moves2 = 0;
//     // _UNITS[_units].Sight_Range = _unit_type_table[unit_type].Sight;
//     EXPECT_EQ(_UNITS[0].dst_wx, 0);                 // _UNITS[_units].dst_wx = 0;
//     EXPECT_EQ(_UNITS[0].dst_wy, 0);                 // _UNITS[_units].dst_wy = 0;
//     EXPECT_EQ(_UNITS[0].Status, us_Ready);          // _UNITS[_units].Status = us_Ready;
//     EXPECT_EQ(_UNITS[0].Level, 0);                  // _UNITS[_units].Level = 0;
//     EXPECT_EQ(_UNITS[0].XP, 0);                     // _UNITS[_units].XP = 0;
//     EXPECT_EQ(_UNITS[0].Damage, 0);                 // _UNITS[_units].Damage = 0;
//     EXPECT_EQ(_UNITS[0].Draw_Priority, 0);          // _UNITS[_units].Draw_Priority = 0;
//     EXPECT_EQ(_UNITS[0].enchantments, 0);           // _UNITS[_units].enchantments = 0;
//     EXPECT_EQ(_UNITS[0].mutations, 0);              // _UNITS[_units].mutations = 0;
//     EXPECT_EQ(_UNITS[0].Move_Failed, ST_FALSE);     // _UNITS[_units].Move_Failed = ST_FALSE;
//     EXPECT_EQ(_UNITS[0].Rd_Constr_Left, ST_UNDEFINED);        // _UNITS[_units].Rd_Constr_Left = ST_UNDEFINED;
// 
//     EXPECT_NE(_UNITS[(_units - 1)].Hero_Slot, ST_UNDEFINED);  // _UNITS[(_units - 1)].Hero_Slot = hero_slot_idx;  // default is ST_UNDEFINED
//     EXPECT_EQ(_UNITS[0].Finished, ST_FALSE);        // _UNITS[(_units - 1)].Finished = ST_FALSE;  // default is ST_TRUE
//     EXPECT_EQ(_UNITS[0].moves2, 0);                 // _UNITS[(_units - 1)].moves2 = _UNITS[(_units - 1)].moves2_max;  // default is 0
// 
//     // EXPECT_EQ(_players[0].Heroes[0].unit_idx = (_units - 1));
//     EXPECT_EQ(_players[0].Heroes[0].unit_idx, 0);
// 
//     // Hero_Slot_Types(unit_type_idx, _players[player_idx].Heroes[hero_slot_idx].Item_Slots);
//     EXPECT_EQ(_players[0].Heroes[0].Item_Slots[0], ist_Sword_Slot);  // 1
//     EXPECT_EQ(_players[0].Heroes[0].Item_Slots[1], ist_Armor_Slot);  // 5
//     EXPECT_EQ(_players[0].Heroes[0].Item_Slots[2], ist_Misc_Slot);   // 6
// 
//     // for(itr = 0; itr < NUM_HERO_ITEM_SLOTS; itr++)
//     //     _players[HUMAN_PLAYER_IDX].Heroes[hero_slot_idx].Items[itr] = ST_UNDEFINED;
//     EXPECT_EQ(_players[0].Heroes[0].Items[0], ST_UNDEFINED);
//     EXPECT_EQ(_players[0].Heroes[0].Items[1], ST_UNDEFINED);
//     EXPECT_EQ(_players[0].Heroes[0].Items[2], ST_UNDEFINED);
// 
//     //     LBX_Load_Data_Static(names_lbx_file__ovr132, 0, (SAMB_ptr)_players[player_idx].Heroes[hero_slot_idx].name, ((player_idx * 35) + unit_type_idx), 1, 13);
// 
//     EXPECT_EQ(_HEROES2[0]->heroes[ut_Warlock].Level, HL_GRANDLORD);
// 
//     // _UNITS[(_units - 1)].Level = _HEROES2[player_idx]->heroes[unit_type_idx].Level;
//     EXPECT_EQ(_UNITS[0].Level, 999);
// 
//     // _UNITS[(_units - 1)].XP = TBL_Experience[_HEROES2[player_idx]->heroes[unit_type_idx].Level];
//     EXPECT_EQ(_UNITS[0].XP, 999);
// 
// 
// 
//     free(_FORTRESSES);
//     free(_HEROES2[0]);
//     free(_UNITS);
//     _units = 0;
// 
// }




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
TEST_F(MOM_test2, MOM_Kill_Unit_kt0)
{
//     _units = 0;
//     _UNITS = (struct s_UNIT *)Allocate_Space(2028);  // 2028 PR, 32448 B
//     _HEROES2[0] = (struct s_HEROES *)Allocate_Space(28);  // 28 PR, 448 B
//     _HEROES2[1] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
//     _HEROES2[2] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
//     _HEROES2[3] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
//     _HEROES2[4] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
//     _HEROES2[5] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
//     hero_names_table = (struct s_INACTV_HERO *)Allocate_Space(37);  // 37 PR, 592 B  ... ~ (36) 16-byte structs
//     spell_data_table = (struct s_SPELL_DATA *)Allocate_Space(485);  // 485 PR, 7760 B; actual: 215 * 36 = 7740

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

//     /*
//         Tear-Down
//     */
//     free(spell_data_table);
//     free(hero_names_table);
//     free(_HEROES2[5]);
//     free(_HEROES2[4]);
//     free(_HEROES2[3]);
//     free(_HEROES2[2]);
//     free(_HEROES2[1]);
//     free(_HEROES2[0]);
//     free(_UNITS);
//     _units = 0;
// // _units = 0;
// // _UNITS = (struct s_UNIT *)Allocate_Space(2028);  // 2028 PR, 32448 B
// // _HEROES2[0] = (struct s_HEROES *)Allocate_Space(28);  // 28 PR, 448 B
// // _HEROES2[1] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
// // _HEROES2[2] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
// // _HEROES2[3] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
// // _HEROES2[4] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
// // _HEROES2[5] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
// // hero_names_table = (struct s_INACTV_HERO *)Allocate_Space(37);  // 37 PR, 592 B  ... ~ (36) 16-byte structs
// // spell_data_table = (struct s_SPELL_DATA *)Allocate_Space(485);  // 485 PR, 7760 B; actual: 215 * 36 = 7740
}

TEST_F(MOM_test2, MOM_Kill_Unit_kt1)
{
//     _units = 0;
//     _UNITS = (struct s_UNIT *)Allocate_Space(2028);  // 2028 PR, 32448 B
//     _HEROES2[0] = (struct s_HEROES *)Allocate_Space(28);  // 28 PR, 448 B
//     _HEROES2[1] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
//     _HEROES2[2] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
//     _HEROES2[3] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
//     _HEROES2[4] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
//     _HEROES2[5] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
//     hero_names_table = (struct s_INACTV_HERO *)Allocate_Space(37);  // 37 PR, 592 B  ... ~ (36) 16-byte structs
//     spell_data_table = (struct s_SPELL_DATA *)Allocate_Space(485);  // 485 PR, 7760 B; actual: 215 * 36 = 7740

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

//     /*
//         Tear-Down
//     */
//     free(spell_data_table);
//     free(hero_names_table);
//     free(_HEROES2[5]);
//     free(_HEROES2[4]);
//     free(_HEROES2[3]);
//     free(_HEROES2[2]);
//     free(_HEROES2[1]);
//     free(_HEROES2[0]);
//     free(_UNITS);
//     _units = 0;
}

TEST_F(MOM_test2, MOM_Kill_Unit_kt2)
{
//     _units = 0;
//     _UNITS = (struct s_UNIT *)Allocate_Space(2028);  // 2028 PR, 32448 B
//     _HEROES2[0] = (struct s_HEROES *)Allocate_Space(28);  // 28 PR, 448 B
//     _HEROES2[1] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
//     _HEROES2[2] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
//     _HEROES2[3] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
//     _HEROES2[4] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
//     _HEROES2[5] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
//     hero_names_table = (struct s_INACTV_HERO *)Allocate_Space(37);  // 37 PR, 592 B  ... ~ (36) 16-byte structs
//     spell_data_table = (struct s_SPELL_DATA *)Allocate_Space(485);  // 485 PR, 7760 B; actual: 215 * 36 = 7740

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

    EXPECT_EQ(_UNITS[0].owner_idx, ST_UNDEFINED);
    EXPECT_EQ(_UNITS[0].wp, ST_UNDEFINED);

//     /*
//         Tear-Down
//     */
//     free(spell_data_table);
//     free(hero_names_table);
//     free(_HEROES2[5]);
//     free(_HEROES2[4]);
//     free(_HEROES2[3]);
//     free(_HEROES2[2]);
//     free(_HEROES2[1]);
//     free(_HEROES2[0]);
//     free(_UNITS);
//     _units = 0;
}



/*

_units

_UNITS[]
_players[]

Delete_Structure()

*/
TEST_F(MOM_test2, MOM_Delete_Dead_Units)
{
    // Set_Up();

    EXPECT_EQ(_units, 0);

    Create_Unit__WIP(ut_LizSettlers, 0, 1, 1, 0, 2000);

    EXPECT_EQ(_units, 1);
    
    Kill_Unit(0, kt_Normal);

    EXPECT_EQ(_units, 1);

    Delete_Dead_Units();  // decrements _units
    // ... [ctest] minkernel\crts\ucrt\src\appcrt\stdio\output.cpp(34) : Assertion failed: stream != nullptr
//     int itr_units;
//     int unit_type;
//     int itr_players;
//     int itr_heroes;
//     for(itr_units = 0; itr_units < _units; itr_units++)
//     {
//         unit_type = _UNITS[itr_units].type;
//         if(
//             (_UNITS[itr_units].owner_idx < HUMAN_PLAYER_IDX)
//             ||
//             (_UNITS[itr_units].owner_idx > NEUTRAL_PLAYER_IDX)
//         )
//         {
//             Delete_Structure(itr_units, (uint8_t *)&_UNITS[0], sizeof(struct s_UNIT), _units);
// 
//             for(itr_players = 0; itr_players < _num_players; itr_players++)
//                 for(itr_heroes = 0; itr_heroes < NUM_HEROES; itr_heroes++)
//                     if((_players[itr_players].Heroes[itr_heroes].unit_idx != ST_UNDEFINED) && (_players[itr_players].Heroes[itr_heroes].unit_idx > itr_units))
//                         _players[itr_players].Heroes[itr_heroes].unit_idx -= 1;
// 
//             _units -= 1;
//         }
//     }

    EXPECT_EQ(_units, 0);

    // Tear_Down();
}



/*

*/
// TEST(MOM_test, MOM_Destroy_City)
// {
// 
// }
