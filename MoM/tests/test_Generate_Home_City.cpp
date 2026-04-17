#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_BITS.h"
#include "../../MoX/src/MOX_DAT.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/random.h"
#include "../src/MAPGEN.h"
#include "../src/TerrType.h"
#include "../src/UNITTYPE.h"
#ifdef __cplusplus
}
#endif

#include <cstdlib>
#include <cstring>


/*
    Generate_Home_Cities() places fortresses (starting city locations) for
    each player and then creates the actual city records and starting units.

    The fortress placement loop validates each random location by checking:
      1. Not ocean
      2. Minimum distance from other fortresses
      3. Minimum distance from nodes, towers, and lairs
      4. Sufficient maximum population:  City_Maximum_Size_NewGame(wx, wy, wp) > (8 - player_idx/3)

    BUG: The max-population comparison at MAPGEN.c:998 is inverted.
    The code reads:
        if(DBG_pop_min < DBG_pop_max) { REJECT } else { ACCEPT }
    This REJECTS locations with sufficient food and ACCEPTS locations with
    insufficient food (e.g., desert with max_pop = 0).

    These tests set _num_players = 0 so that only fortress placement runs
    (no city records, no unit creation, no LBX file loading).  The test then
    checks that each placed fortress satisfies the population requirement.
*/


class GenerateHomeCityTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _FORTRESSES = static_cast<struct s_FORTRESS *>(malloc(sizeof(struct s_FORTRESS) * NUM_PLAYERS));
        _CITIES = static_cast<struct s_CITY *>(malloc(sizeof(struct s_CITY) * NUM_CITIES));
        _NODES = static_cast<struct s_NODE *>(malloc(sizeof(struct s_NODE) * NUM_NODES));
        _TOWERS = static_cast<struct s_TOWER *>(malloc(sizeof(struct s_TOWER) * NUM_TOWERS));
        _LAIRS = static_cast<struct s_LAIR *>(malloc(sizeof(struct s_LAIR) * NUM_LAIRS));
        _UNITS = static_cast<struct s_UNIT *>(malloc(sizeof(struct s_UNIT) * NUM_UNITS));
        _world_maps = static_cast<uint8_t *>(malloc(WORLD_SIZE * NUM_PLANES * sizeof(uint16_t)));

        ASSERT_NE(_FORTRESSES, nullptr);
        ASSERT_NE(_CITIES, nullptr);
        ASSERT_NE(_NODES, nullptr);
        ASSERT_NE(_TOWERS, nullptr);
        ASSERT_NE(_LAIRS, nullptr);
        ASSERT_NE(_UNITS, nullptr);
        ASSERT_NE(_world_maps, nullptr);

        p_world_map = (int16_t (*)[WORLD_HEIGHT][WORLD_WIDTH])_world_maps;

        memset(_FORTRESSES, 0, sizeof(struct s_FORTRESS) * NUM_PLAYERS);
        memset(_CITIES, 0, sizeof(struct s_CITY) * NUM_CITIES);
        memset(_UNITS, 0, sizeof(struct s_UNIT) * NUM_UNITS);

        // Place nodes, towers, and lairs far outside the valid coordinate
        // range so they never interfere with fortress distance checks.
        for (int16_t i = 0; i < NUM_NODES; i++)
        {
            _NODES[i].wx = 0;
            _NODES[i].wy = 0;
            _NODES[i].wp = -1;
            _NODES[i].power = 0;
            _NODES[i].type = nt_Sorcery;
        }
        for (int16_t i = 0; i < NUM_TOWERS; i++)
        {
            _TOWERS[i].wx = 0;
            _TOWERS[i].wy = 0;
        }
        for (int16_t i = 0; i < NUM_LAIRS; i++)
        {
            _LAIRS[i].wx = 0;
            _LAIRS[i].wy = 0;
            _LAIRS[i].wp = -1;
            _LAIRS[i].type = 0;
            _LAIRS[i].intact = 0;
        }

        // All players on Arcanus, no special traits
        memset(_players, 0, sizeof(_players));

        _num_players = 0;  // skip city/unit creation (avoids LBX dependency)
        _cities = 0;
        _units = 0;
        _difficulty = god_Normal;
        _magic = 1;

        Set_Random_Seed(12345);
    }

    void TearDown() override
    {
        free(_world_maps);   _world_maps = nullptr;  p_world_map = nullptr;
        free(_UNITS);        _UNITS = nullptr;
        free(_LAIRS);        _LAIRS = nullptr;
        free(_TOWERS);       _TOWERS = nullptr;
        free(_NODES);        _NODES = nullptr;
        free(_CITIES);       _CITIES = nullptr;
        free(_FORTRESSES);   _FORTRESSES = nullptr;
    }

    // Fill the entire map on a given plane with a single terrain type
    void Fill_Map(int16_t wp, int16_t terrain_type)
    {
        for (int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
        {
            for (int16_t wx = 0; wx < WORLD_WIDTH; wx++)
            {
                SET_TERRAIN_TYPE(wx, wy, wp, terrain_type);
            }
        }
    }
};



/*
    Test: Human player fortress must be on a square with enough food.

    Map layout: upper half is desert (max_pop = 0), lower half is grasslands
    (max_pop ~= 31).  With the buggy inverted comparison, the function accepts
    the first desert square it finds (8 >= 0) and rejects grasslands (8 < 31).

    The assertion checks that the human player's fortress (player_idx 0) has
    City_Maximum_Size_NewGame > pop_min.

    Expected result with the bug:   FAIL  (fortress on desert, max_pop = 0)
    Expected result after the fix:  PASS  (fortress on grasslands, max_pop > 8)
*/
TEST_F(GenerateHomeCityTest, HumanFortressHasSufficientMaxPop)
{
    // Arrange: desert top half, grasslands bottom half, both planes
    for (int16_t wp = 0; wp < NUM_PLANES; wp++)
    {
        for (int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
        {
            for (int16_t wx = 0; wx < WORLD_WIDTH; wx++)
            {
                if (wy < 20)
                {
                    SET_TERRAIN_TYPE(wx, wy, wp, tt_Desert1);
                }
                else
                {
                    SET_TERRAIN_TYPE(wx, wy, wp, tt_Grasslands1);
                }
            }
        }
    }

    // Act
    Generate_Home_Cities();

    // Assert: human player fortress (player_idx 0) should be on viable terrain
    int16_t human_wx = _FORTRESSES[HUMAN_PLAYER_IDX].wx;
    int16_t human_wy = _FORTRESSES[HUMAN_PLAYER_IDX].wy;
    int16_t human_wp = _FORTRESSES[HUMAN_PLAYER_IDX].wp;
    int16_t max_pop = City_Maximum_Size_NewGame(human_wx, human_wy, human_wp);
    int16_t pop_min = (8 - (HUMAN_PLAYER_IDX / 3));

    EXPECT_GT(max_pop, pop_min) << "Human fortress at (" << human_wx << ", " << human_wy << ", " << human_wp << ") has max_pop " << max_pop << " but needs > " << pop_min << ". The population comparison in Generate_Home_Cities is inverted.";
}



/*
    Test: ALL fortresses must be on squares with enough food.

    Same map layout as above.  Checks every player's fortress, not just
    the human player's.

    Expected result with the bug:   FAIL
    Expected result after the fix:  PASS
*/
TEST_F(GenerateHomeCityTest, AllFortressesHaveSufficientMaxPop)
{
    // Arrange: desert top half, grasslands bottom half, both planes
    for (int16_t wp = 0; wp < NUM_PLANES; wp++)
    {
        for (int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
        {
            for (int16_t wx = 0; wx < WORLD_WIDTH; wx++)
            {
                if (wy < 20)
                {
                    SET_TERRAIN_TYPE(wx, wy, wp, tt_Desert1);
                }
                else
                {
                    SET_TERRAIN_TYPE(wx, wy, wp, tt_Grasslands1);
                }
            }
        }
    }

    // Act
    Generate_Home_Cities();

    // Assert: every fortress should be on terrain with sufficient max_pop
    for (int16_t player_idx = 0; player_idx < NUM_PLAYERS; player_idx++)
    {
        int16_t fw = _FORTRESSES[player_idx].wx;
        int16_t fh = _FORTRESSES[player_idx].wy;
        int16_t fp = _FORTRESSES[player_idx].wp;
        int16_t max_pop = City_Maximum_Size_NewGame(fw, fh, fp);
        int16_t pop_min = (8 - (player_idx / 3));

        EXPECT_GT(max_pop, pop_min) << "Fortress for player " << player_idx << " at (" << fw << ", " << fh << ", " << fp << ") has max_pop " << max_pop << " but needs > " << pop_min;
    }
}
