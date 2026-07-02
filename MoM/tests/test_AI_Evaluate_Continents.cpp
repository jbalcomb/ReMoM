#include <gtest/gtest.h>

#include <cstdlib>
#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/EMS/EMS.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DAT.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../src/DIPLODEF.h"
#include "../src/AIDUDES.h"
#include "../src/AIMOVE.h"
#include "../src/UNITTYPE.h"
#ifdef __cplusplus
}
#endif

class AIEvaluateContinentsTest : public ::testing::Test
{
protected:
    struct s_CITY * saved_cities = nullptr;
    struct s_FORTRESS * saved_fortresses = nullptr;
    struct s_UNIT * saved_units = nullptr;
    uint8_t * saved_landmasses = nullptr;
    int16_t * saved_countdown = nullptr;
    SAMB_ptr saved_datah = nullptr;
    SAMB_ptr saved_ems_pfba = nullptr;
    SAMB_ptr saved_contxxx = nullptr;
    uint8_t * saved_settler_targets[NUM_PLANES] = { nullptr, nullptr };
    uint8_t * saved_settler_targets_wx[NUM_PLANES] = { nullptr, nullptr };
    uint8_t * saved_settler_targets_wy[NUM_PLANES] = { nullptr, nullptr };
    int16_t * saved_war_targets[NUM_PLANES] = { nullptr, nullptr };
    int16_t saved_city_count = 0;
    int16_t saved_unit_count = 0;
    int16_t saved_num_players = 0;
    struct s_AI_CONTINENTS saved_ai_continents = {};

    void SetUp() override
    {
        int wp = 0;
        int landmass_idx = 0;

        saved_cities = _CITIES;
        saved_fortresses = _FORTRESSES;
        saved_units = _UNITS;
        saved_landmasses = _landmasses;
        saved_countdown = _ai_reevaluate_continents_countdown;
        saved_datah = _EMMDATAH_seg;
        saved_ems_pfba = EMS_PFBA;
        saved_contxxx = EmmHndl_CONTXXX;
        saved_city_count = _cities;
        saved_unit_count = _units;
        saved_num_players = _num_players;
        saved_ai_continents = _ai_continents;

        for(wp = 0; wp < NUM_PLANES; wp++)
        {
            saved_settler_targets[wp] = _ai_landmass_settler_targets[wp];
            saved_settler_targets_wx[wp] = _ai_landmass_settler_targets_wx_array[wp];
            saved_settler_targets_wy[wp] = _ai_landmass_settler_targets_wy_array[wp];
            saved_war_targets[wp] = _ai_landmass_war_targets[wp];
        }

        _CITIES = static_cast<struct s_CITY *>(calloc(NUM_CITIES, sizeof(struct s_CITY)));
        ASSERT_NE(_CITIES, nullptr);
        _FORTRESSES = static_cast<struct s_FORTRESS *>(calloc(NUM_FORTRESSES, sizeof(struct s_FORTRESS)));
        ASSERT_NE(_FORTRESSES, nullptr);
        _UNITS = static_cast<struct s_UNIT *>(calloc(NUM_UNITS, sizeof(struct s_UNIT)));
        ASSERT_NE(_UNITS, nullptr);
        _landmasses = static_cast<uint8_t *>(calloc(WORLD_SIZE * NUM_PLANES, sizeof(uint8_t)));
        ASSERT_NE(_landmasses, nullptr);
        _ai_reevaluate_continents_countdown = static_cast<int16_t *>(calloc(NUM_PLAYERS, sizeof(int16_t)));
        ASSERT_NE(_ai_reevaluate_continents_countdown, nullptr);

        for(wp = 0; wp < NUM_PLANES; wp++)
        {
            _ai_landmass_settler_targets[wp] = static_cast<uint8_t *>(calloc(NUM_PLAYERS, sizeof(uint8_t)));
            _ai_landmass_settler_targets_wx_array[wp] = static_cast<uint8_t *>(calloc(NUM_PLAYERS, sizeof(uint8_t)));
            _ai_landmass_settler_targets_wy_array[wp] = static_cast<uint8_t *>(calloc(NUM_PLAYERS, sizeof(uint8_t)));
            _ai_landmass_war_targets[wp] = static_cast<int16_t *>(calloc(NUM_PLAYERS, sizeof(int16_t)));
            ASSERT_NE(_ai_landmass_settler_targets[wp], nullptr);
            ASSERT_NE(_ai_landmass_settler_targets_wx_array[wp], nullptr);
            ASSERT_NE(_ai_landmass_settler_targets_wy_array[wp], nullptr);
            ASSERT_NE(_ai_landmass_war_targets[wp], nullptr);
        }

        EmmHndl_CONTXXX = Allocate_Space(4096);
        ASSERT_NE(EmmHndl_CONTXXX, nullptr);
        memset(EmmHndl_CONTXXX, 0, (4096 * SZ_PARAGRAPH_B));
        _EMMDATAH_seg = EmmHndl_CONTXXX;
        CONTXXX_Map();

        for(wp = 0; wp < NUM_PLANES; wp++)
        {
            memset(g_ai_evaluation_map[wp], 0, WORLD_SIZE * sizeof(uint16_t));

            for(landmass_idx = 0; landmass_idx < NUM_LANDMASSES; landmass_idx++)
            {
                _ai_landmass_land_squares_heads[wp][landmass_idx] = ST_UNDEFINED;
                _ai_landmass_dock_squares_heads[wp][landmass_idx] = ST_UNDEFINED;
                _ai_continents.plane[wp].player[0].type_array[landmass_idx] = 0;
                _ai_continents.plane[wp].player[0].wx_array[landmass_idx] = 0;
                _ai_continents.plane[wp].player[0].wy_array[landmass_idx] = 0;
            }
        }

        memset(_players, 0, sizeof(_players));
        _cities = 0;
        _units = 0;
        _num_players = 2;
    }

    void TearDown() override
    {
        int wp = 0;

        free(EmmHndl_CONTXXX);
        EmmHndl_CONTXXX = saved_contxxx;
        _EMMDATAH_seg = saved_datah;
        EMS_PFBA = saved_ems_pfba;

        for(wp = 0; wp < NUM_PLANES; wp++)
        {
            free(_ai_landmass_settler_targets[wp]);
            free(_ai_landmass_settler_targets_wx_array[wp]);
            free(_ai_landmass_settler_targets_wy_array[wp]);
            free(_ai_landmass_war_targets[wp]);

            _ai_landmass_settler_targets[wp] = saved_settler_targets[wp];
            _ai_landmass_settler_targets_wx_array[wp] = saved_settler_targets_wx[wp];
            _ai_landmass_settler_targets_wy_array[wp] = saved_settler_targets_wy[wp];
            _ai_landmass_war_targets[wp] = saved_war_targets[wp];
        }

        free(_ai_reevaluate_continents_countdown);
        free(_landmasses);
        free(_UNITS);
        free(_FORTRESSES);
        free(_CITIES);

        _ai_reevaluate_continents_countdown = saved_countdown;
        _landmasses = saved_landmasses;
        _UNITS = saved_units;
        _FORTRESSES = saved_fortresses;
        _CITIES = saved_cities;
        _cities = saved_city_count;
        _units = saved_unit_count;
        _num_players = saved_num_players;
        _ai_continents = saved_ai_continents;
    }

    void Set_Landmass(int16_t wp, int16_t wx, int16_t wy, uint8_t landmass_idx)
    {
        _landmasses[(wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx] = landmass_idx;
    }

    int16_t Add_City(int16_t owner_idx, int16_t wp, int16_t wx, int16_t wy)
    {
        const int16_t city_idx = _cities;

        _CITIES[city_idx].owner_idx = static_cast<int8_t>(owner_idx);
        _CITIES[city_idx].wp = static_cast<int8_t>(wp);
        _CITIES[city_idx].wx = static_cast<int8_t>(wx);
        _CITIES[city_idx].wy = static_cast<int8_t>(wy);
        _cities += 1;

        return city_idx;
    }

    int16_t Add_Unit(int16_t owner_idx, int16_t unit_type, int16_t wp, int16_t wx, int16_t wy)
    {
        const int16_t unit_idx = _units;

        _UNITS[unit_idx].owner_idx = static_cast<int8_t>(owner_idx);
        _UNITS[unit_idx].type = static_cast<uint8_t>(unit_type);
        _UNITS[unit_idx].wp = static_cast<int8_t>(wp);
        _UNITS[unit_idx].wx = static_cast<int8_t>(wx);
        _UNITS[unit_idx].wy = static_cast<int8_t>(wy);
        _UNITS[unit_idx].dst_wx = static_cast<int8_t>(wx);
        _UNITS[unit_idx].dst_wy = static_cast<int8_t>(wy);
        _UNITS[unit_idx].Status = us_Ready;
        _units += 1;

        return unit_idx;
    }

    void Seed_Land_Node(int16_t wp, int16_t landmass_idx, int16_t node_idx, int16_t wx, int16_t wy, int16_t next_node_idx)
    {
        _ai_landmass_land_squares_heads[wp][landmass_idx] = node_idx;
        _ai_landmass_land_squares_wx_array[wp][node_idx] = static_cast<int8_t>(wx);
        _ai_landmass_land_squares_wy_array[wp][node_idx] = static_cast<int8_t>(wy);
        _ai_landmass_land_squares_lists[wp][node_idx] = next_node_idx;
    }

    void Seed_Dock_Node(int16_t wp, int16_t landmass_idx, int16_t node_idx, int16_t wx, int16_t wy, int16_t next_node_idx)
    {
        _ai_landmass_dock_squares_heads[wp][landmass_idx] = node_idx;
        _ai_landmass_dock_squares_wx_array[wp][node_idx] = static_cast<int8_t>(wx);
        _ai_landmass_dock_squares_wy_array[wp][node_idx] = static_cast<int8_t>(wy);
        _ai_landmass_dock_squares_lists[wp][node_idx] = next_node_idx;
    }
};

TEST_F(AIEvaluateContinentsTest, PositiveCountdownWithoutStaleTargetReturnsEarly)
{
    _ai_reevaluate_continents_countdown[0] = 2;
    _ai_continents.plane[ARCANUS_PLANE].player[0].type_array[1] = lmt_Contested;
    _ai_continents.plane[ARCANUS_PLANE].player[0].wx_array[1] = 17;
    _ai_continents.plane[ARCANUS_PLANE].player[0].wy_array[1] = 23;
    _ai_landmass_settler_targets[ARCANUS_PLANE][0] = 9;
    _ai_landmass_settler_targets_wx_array[ARCANUS_PLANE][0] = 11;
    _ai_landmass_settler_targets_wy_array[ARCANUS_PLANE][0] = 12;

    AI_Evaluate_Continents(0);

    EXPECT_EQ(_ai_reevaluate_continents_countdown[0], 1);
    EXPECT_EQ(_ai_continents.plane[ARCANUS_PLANE].player[0].type_array[1], lmt_Contested);
    EXPECT_EQ(_ai_continents.plane[ARCANUS_PLANE].player[0].wx_array[1], 17);
    EXPECT_EQ(_ai_continents.plane[ARCANUS_PLANE].player[0].wy_array[1], 23);
    EXPECT_EQ(_ai_landmass_settler_targets[ARCANUS_PLANE][0], 9);
    EXPECT_EQ(_ai_landmass_settler_targets_wx_array[ARCANUS_PLANE][0], 11);
    EXPECT_EQ(_ai_landmass_settler_targets_wy_array[ARCANUS_PLANE][0], 12);
}

TEST_F(AIEvaluateContinentsTest, CountdownOneTriggersEmptyWorldReevaluation)
{
    _ai_reevaluate_continents_countdown[0] = 1;

    AI_Evaluate_Continents(0);

    EXPECT_GE(_ai_reevaluate_continents_countdown[0], 25);
    EXPECT_LE(_ai_reevaluate_continents_countdown[0], 43);
    EXPECT_EQ(_ai_continents.plane[ARCANUS_PLANE].player[0].type_array[0], lmt_NoOwnCityAndAllyHasCity);
    EXPECT_EQ(_ai_continents.plane[MYRROR_PLANE].player[0].type_array[0], lmt_NoOwnCityAndAllyHasCity);
    EXPECT_EQ(_ai_landmass_settler_targets[ARCANUS_PLANE][0], 0);
    EXPECT_EQ(_ai_landmass_settler_targets[MYRROR_PLANE][0], 0);
    EXPECT_EQ(_ai_continents.plane[ARCANUS_PLANE].player[0].type_array[1], lmt_NoOwnCityAndAllyHasCity);
    EXPECT_EQ(_ai_continents.plane[ARCANUS_PLANE].player[0].wx_array[1], 0);
    EXPECT_EQ(_ai_continents.plane[ARCANUS_PLANE].player[0].wy_array[1], 0);
}

TEST_F(AIEvaluateContinentsTest, SeededLandmassesProduceMeaningfulClassificationsAndStageSquares)
{
    _ai_reevaluate_continents_countdown[0] = 1;
    _players[0].Dipl.Dipl_Status[1] = DIPL_Alliance;

    Set_Landmass(ARCANUS_PLANE, 10, 10, 1);
    Set_Landmass(ARCANUS_PLANE, 12, 10, 1);
    Set_Landmass(ARCANUS_PLANE, 20, 20, 2);

    Add_City(0, ARCANUS_PLANE, 10, 10);
    Add_City(1, ARCANUS_PLANE, 20, 20);

    g_ai_evaluation_map[ARCANUS_PLANE][(10 * WORLD_WIDTH) + 10] = 1;

    Build_Land_Linked_List();

    AI_Evaluate_Continents(0);

    EXPECT_EQ(_ai_continents.plane[ARCANUS_PLANE].player[0].type_array[1], lmt_Contested);
    EXPECT_EQ(_ai_continents.plane[ARCANUS_PLANE].player[0].wx_array[1], 10);
    EXPECT_EQ(_ai_continents.plane[ARCANUS_PLANE].player[0].wy_array[1], 10);

    EXPECT_EQ(_ai_continents.plane[ARCANUS_PLANE].player[0].type_array[2], lmt_NoOwnCityAndAllyHasCity);
    EXPECT_EQ(_ai_continents.plane[ARCANUS_PLANE].player[0].wx_array[2], 20);
    EXPECT_EQ(_ai_continents.plane[ARCANUS_PLANE].player[0].wy_array[2], 20);
}

TEST_F(AIEvaluateContinentsTest, Regression_ReevaluationWithUnit_CurrentImplementationDies)
{
    _ai_reevaluate_continents_countdown[0] = 1;
    Add_Unit(0, ut_LizHalberdiers, ARCANUS_PLANE, 10, 10);

    ASSERT_DEATH(
        {
            AI_Evaluate_Continents(0);
        },
        ".*");
}