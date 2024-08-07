
#include "MoX.H"



int16_t current_screen;
int16_t previous_screen;





/*
_HEROES2

_num_players
_landsize
_magic
_difficulty
_cities
_units
_turn
_unit

_players

_world_maps  // 4800 * 2 = 9600

UU_TBL_1
UU_TBL_2
TBL_Landmasses

_NODES
_FORTRESSES
_TOWERS
_LAIRS
_ITEMS
_CITIES
_UNITS

TBL_Terr_Specials
TBL_Scouting
movement_mode_cost_maps
events_table
TBL_Terrain_Flags

grand_vizier

TBL_Premade_Items
hero_names_table

*/

void Test_Run(void)
{
    size_t test_size;

    test_size = sizeof(struct s_WIZARD);
    test_size = sizeof(struct s_NODE);
    test_size = sizeof(struct s_FORTRESS);
    test_size = sizeof(struct s_TOWER);
    test_size = sizeof(struct s_LAIR);
    test_size = sizeof(struct s_ITEM);
    test_size = sizeof(struct s_CITY);
    test_size = sizeof(struct s_UNIT);

    assert(sizeof(struct s_HERO) == HERO_RECORD_SIZE);  // _HEROES2

    assert(sizeof(_num_players) == 2);
    assert(sizeof(_landsize) == 2);
    assert(sizeof(_magic) == 2);
    assert(sizeof(_difficulty) == 2);
    assert(sizeof(_cities) == 2);
    assert(sizeof(_units) == 2);
    assert(sizeof(_turn) == 2);
    assert(sizeof(_unit) == 2);

    assert(sizeof(struct s_WIZARD) == WIZARD_RECORD_SIZE);  // 1224  _players
    // _world_maps  // 4800 * 2 = 9600
    // UU_TBL_1
    // UU_TBL_2
    // TBL_Landmasses
assert(sizeof(struct s_NODE) == NODE_RECORD_SIZE);          //   48  _NODES
assert(sizeof(struct s_FORTRESS) == FORTRESS_RECORD_SIZE);  //    4  _FORTRESSES
assert(sizeof(struct s_TOWER) == TOWER_RECORD_SIZE);        //    4  _TOWERS
assert(sizeof(struct s_LAIR) == LAIR_RECORD_SIZE);          //   24  _LAIRS
assert(sizeof(struct s_ITEM) == ITEM_RECORD_SIZE);          //   50  _ITEMS
assert(sizeof(struct s_CITY) == CITY_RECORD_SIZE);          //  114  _CITIES
assert(sizeof(struct s_UNIT) == UNIT_RECORD_SIZE);          //   32  _UNITS

    // TBL_Terr_Specials
    // TBL_Scouting
    // movement_mode_cost_maps
    // events_table
    // TBL_Terrain_Flags
    assert(sizeof(grand_vizier) == 2);
    // TBL_Premade_Items
    // hero_names_table

}
