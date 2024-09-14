
#include "MoX.H"



int16_t current_screen;
int16_t previous_screen;



int16_t DBG_trigger_event = ST_FALSE;
int16_t DBG_trigger_offer_item = ST_FALSE;
int16_t DBG_trigger_offer_merc = ST_FALSE;
int16_t DBG_trigger_offer_hero = ST_FALSE;



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
_square_explored
movement_mode_cost_maps
events_table
_map_square_flags

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
    // _square_explored
    // movement_mode_cost_maps
    // events_table
    // _map_square_flags
    assert(sizeof(grand_vizier) == 2);
    // TBL_Premade_Items
    // hero_names_table

}
