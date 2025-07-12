
#include "MOM_DBG.h"

#include "MOX/MOM_Data.h"
#include "MOX/MOX_DEF.h"
#include "MOX/MOX_TYPE.h"

#include "MOM_DEF.h"

#include <assert.h>
#include <math.h>       /* sqrt() */
#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), fseek(); */
#include <stdlib.h>     /* abs(); itoa(); */
#include <string.h>     /* memset(), strcat(), strcpy(); */



int16_t DBG_trigger_event = ST_FALSE;
/*
    et_None                 =  0,
    et_Meteor               =  1,
    et_Gift                 =  2,
    et_Disjunction          =  3,
    et_Marriage             =  4,
    et_Earthquake           =  5,
    et_Pirates              =  6,
    et_Plague               =  7,
    et_Rebellion            =  8,
    et_Donation             =  9,
    et_Depletion            = 10,
    et_New_Mine             = 11,
    et_Population_Boom      = 12,
    et_Good_Moon            = 13,
    et_Bad_Moon             = 14,
    et_Conjunction_Chaos    = 15,
    et_Conjunction_Nature   = 16,
    et_Conjunction_Sorcery  = 17,
    et_Mana_Short           = 18
*/
/*
    Plague
    Population Boom
    Good Moon
    Bad Moon
    Conjuction - Chaos
    Conjuction - Nature
    Conjuction - Sorcery
    Mana Short
*/
int16_t DBG_trigger_event_plague = ST_FALSE;
int16_t DBG_trigger_event_population_boom = ST_FALSE;
int16_t DBG_trigger_event_good_moon = ST_TRUE;
int16_t DBG_trigger_event_bad_moon = ST_FALSE;
int16_t DBG_trigger_event_conjunction_chaos = ST_FALSE;
int16_t DBG_trigger_event_conjunction_nature = ST_FALSE;
int16_t DBG_trigger_event_conjunction_sorcery = ST_FALSE;
int16_t DBG_trigger_event_mana_short = ST_FALSE;
/*
    MSG_UnitLost_Count
    MSG_UnitKilled_Count
    MSG_CityGrowth_Count
    MSG_CityDeath_Count
    MSG_BldLost_Count
    MSG_UEsLost_Count
    MSG_CEsLost_Count
    MSG_GEs_Lost
*/
int16_t DBG_trigger_offer_item = ST_FALSE;
int16_t DBG_trigger_offer_merc = ST_FALSE;
int16_t DBG_trigger_offer_hero = ST_FALSE;

int16_t DBG_trigger_complete_research_spell = ST_FALSE;

uint8_t DBG_debug_flag = ST_FALSE;



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
_landmasses

_NODES
_FORTRESSES
_TOWERS
_LAIRS
_ITEMS
_CITIES
_UNITS

_map_square_terrain_specials
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
    // _landmasses
    assert(sizeof(struct s_NODE) == NODE_RECORD_SIZE);          //   48  _NODES
    assert(sizeof(struct s_FORTRESS) == FORTRESS_RECORD_SIZE);  //    4  _FORTRESSES
    assert(sizeof(struct s_TOWER) == TOWER_RECORD_SIZE);        //    4  _TOWERS
    assert(sizeof(struct s_LAIR) == LAIR_RECORD_SIZE);          //   24  _LAIRS
    assert(sizeof(struct s_ITEM) == ITEM_RECORD_SIZE);          //   50  _ITEMS
    assert(sizeof(struct s_CITY) == CITY_RECORD_SIZE);          //  114  _CITIES
    assert(sizeof(struct s_UNIT) == UNIT_RECORD_SIZE);          //   32  _UNITS

    // _map_square_terrain_specials
    // _square_explored
    // movement_mode_cost_maps
    // events_table
    // _map_square_flags
    assert(sizeof(grand_vizier) == 2);
    // TBL_Premade_Items
    // hero_names_table

}
