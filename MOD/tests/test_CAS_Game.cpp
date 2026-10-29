#include <gtest/gtest.h>

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

extern "C" {
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_DAT.h"
#include "../../MoM/src/EVENTS.h"
#include "../../MoM/src/Terrain.h"
#include "../../MoM/src/TerrType.h"
#include "../src/CAS.h"
#include "../src/CAS_Game.h"
}

namespace
{

/* A small world on grassland: CAS city 1 (ReMoM index 0; player 0, Arcanus 10,10) and city 2 (index 1; player 2, Myrror 59,5). */
class CAS_Game_test : public ::testing::Test
{
protected:
    std::vector<struct s_CITY> cities = std::vector<struct s_CITY>(NUM_CITIES);
    std::vector<int16_t> world = std::vector<int16_t>(NUM_PLANES * WORLD_SIZE, tt_Grasslands1);
    std::vector<uint8_t> specials = std::vector<uint8_t>(NUM_PLANES * WORLD_SIZE, 0);
    std::vector<struct s_FORTRESS> fortresses = std::vector<struct s_FORTRESS>(NUM_FORTRESSES);
    std::vector<struct s_UNIT> units = std::vector<struct s_UNIT>(NUM_UNITS);
    struct s_EVENT_DATA events;

    struct s_CITY * saved_cities;
    int16_t saved_city_count, saved_unit_count, saved_difficulty, saved_num_players;
    int16_t (*saved_world)[WORLD_HEIGHT][WORLD_WIDTH];
    uint8_t * saved_specials;
    struct s_FORTRESS * saved_fortresses;
    struct s_UNIT * saved_units;
    struct s_EVENT_DATA * saved_events;
    struct s_WIZARD saved_players[NUM_PLAYERS];

    void SetUp() override
    {
        saved_cities = _CITIES; saved_city_count = _cities; saved_unit_count = _units; saved_difficulty = _difficulty;
        saved_num_players = _num_players; saved_world = p_world_map; saved_specials = _map_square_terrain_specials;
        saved_fortresses = _FORTRESSES; saved_units = _UNITS; saved_events = events_table;
        std::memcpy(saved_players, _players, sizeof(saved_players));

        std::memset(_players, 0, sizeof(saved_players));
        std::memset(&events, 0, sizeof(events));
        for(auto & c : cities) { std::memset(c.bldg_status, bs_NotBuilt, sizeof(c.bldg_status)); }
        cities[0].owner_idx = 0; cities[0].wp = 0; cities[0].wx = 10; cities[0].wy = 10; cities[0].population = 7; cities[0].race = 4;
        cities[1].owner_idx = 2; cities[1].wp = 1; cities[1].wx = 59; cities[1].wy = 5; cities[1].population = 3;
        _CITIES = cities.data(); _cities = 2;
        p_world_map = (int16_t (*)[WORLD_HEIGHT][WORLD_WIDTH])world.data();
        _map_square_terrain_specials = specials.data();
        _FORTRESSES = fortresses.data();
        _UNITS = units.data(); _units = 0;
        events_table = &events;
        _num_players = 3;
        _difficulty = god_Normal;
        CAS_Game_Reset_Slots();
        CAS_Clear_Master();
    }

    void TearDown() override
    {
        _CITIES = saved_cities; _cities = saved_city_count; _units = saved_unit_count; _difficulty = saved_difficulty;
        _num_players = saved_num_players; p_world_map = saved_world; _map_square_terrain_specials = saved_specials;
        _FORTRESSES = saved_fortresses; _UNITS = saved_units; events_table = saved_events;
        std::memcpy(_players, saved_players, sizeof(saved_players));
    }

    void Set_Terrain(int wp, int wx, int wy, int16_t type) { world[(wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx] = type; }
    void Set_Special(int wp, int wx, int wy, uint8_t value) { specials[(wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx] = value; }

    /* Runs `RESULT = <expression>;` and returns RESULT; INT32_MIN on a script error. */
    int32_t Eval(const std::string & expression)
    {
        std::string source = "RESULT = " + expression + ";";
        CAS_Script * script = CAS_Load_String("test.CAS", source.c_str());
        EXPECT_NE(script, nullptr) << CAS_Last_Error();
        if(script == nullptr) { return INT32_MIN; }
        int32_t result = 0;
        struct s_CAS_INPUT input = { "RESULT", 0 };
        int rc = CAS_Run(script, &input, 1, "RESULT", &result);
        CAS_Free(script);
        return (rc == 0) ? result : INT32_MIN;
    }

    void Run(const std::string & source)
    {
        CAS_Script * script = CAS_Load_String("test.CAS", source.c_str());
        ASSERT_NE(script, nullptr) << CAS_Last_Error();
        int32_t result = 0;
        EXPECT_EQ(CAS_Run(script, nullptr, 0, "RESULT", &result), 0) << CAS_Last_Error();
        CAS_Free(script);
    }
};

TEST_F(CAS_Game_test, Buildings_Use_Com2_Ids)
{
    cities[0].bldg_status[bt_Temple] = bs_Built;
    cities[0].bldg_status[bt_CityWalls] = bs_Replaced;
    cities[0].bldg_status[bt_Armory] = bs_Built;
    EXPECT_EQ(Eval("ISBUILT(1,40)"), 1);                     /* BTemple */
    EXPECT_EQ(Eval("ISBUILT(1,37)"), 1);                     /* BCityWalls */
    EXPECT_EQ(Eval("ISBUILT(1,39)"), 1);                     /* BArmory */
    EXPECT_EQ(Eval("ISBUILT(1,4)"), 0);                      /* BColosseum: CoM2 only, though 4 is 1.31's Armory */
    EXPECT_EQ(Eval("ISBUILT(1,23)"), 0);                     /* BAmplifyingTower: 1.31's 23 is the Temple */
    EXPECT_EQ(Eval("ISBUILT(1,999)"), 0);
    EXPECT_EQ(Eval("ISBUILT(3,40)"), INT32_MIN);             /* no city 3 */
    EXPECT_EQ(Eval("ISBUILT(0,40)"), 0);                     /* 0: no city */
}

TEST_F(CAS_Game_test, Buildings_Owned)
{
    cities[0].bldg_status[bt_Bank] = bs_Built;
    cities[1].bldg_status[bt_Bank] = bs_Built;
    cities[1].enchantments[PROSPERITY] = 3;
    EXPECT_EQ(Eval("BUILDINGSOWNED(0,27,0)"), 1);
    EXPECT_EQ(Eval("BUILDINGSOWNED(2,27,0)"), 1);
    EXPECT_EQ(Eval("BUILDINGSOWNED(2,27,20)"), 1);           /* with CEProsperity */
    EXPECT_EQ(Eval("BUILDINGSOWNED(0,27,20)"), 0);
}

TEST_F(CAS_Game_test, City_Enchantments_Return_The_Caster)
{
    cities[0].enchantments[FAMINE] = 3;                      /* cast by player 2 (stored as player + 1) */
    EXPECT_EQ(Eval("CITYENCHANT(1,8)"), 2);                  /* CEDrought */
    EXPECT_EQ(Eval("CITYENCHANT(1,1)"), -1);
    EXPECT_EQ(Eval("CITYENCHANT(1,29)"), -1);                /* CEChaosBless: CoM2 only */
}

TEST_F(CAS_Game_test, Wizard_Books_Retorts_Globals_Spells)
{
    _players[1].spellranks[sbr_Chaos] = 6;
    _players[1].alchemy = 1;
    _players[1].Globals[WIND_MASTERY] = 1;
    _players[1].spells_list[13 - 1] = 2;                     /* SIceBolt known */
    EXPECT_EQ(Eval("BOOKS(1,3)"), 6);                        /* Chaos = 3 */
    EXPECT_EQ(Eval("BOOKS(1,6)"), 0);                        /* Arcane */
    EXPECT_EQ(Eval("RETORT(1,1)"), 1);                       /* Alchemist */
    EXPECT_EQ(Eval("RETORT(1,9)"), 0);                       /* Procurator: CoM2 only */
    EXPECT_EQ(Eval("HASGLOBAL(1,27)"), 1);                   /* GEWindMastery: 1.31 index 4 */
    EXPECT_EQ(Eval("HASGLOBAL(1,4)"), 0);                    /* GEAuraofMajesty */
    EXPECT_EQ(Eval("SPELLSTATE(1,13)"), 2);
    EXPECT_EQ(Eval("SPELLSTATE(1,390)"), 0);                 /* Warlord's own spell */
    EXPECT_EQ(Eval("BOOKS(9,1)"), INT32_MIN);
}

TEST_F(CAS_Game_test, Ore_And_Terrain_In_Range)
{
    Set_Special(0, 11, 11, TS_IRON);
    Set_Special(0, 12, 10, TS_IRON | TS_WILDGAME);
    Set_Special(0, 12, 12, TS_IRON);                         /* corner of the 5x5: outside the city area */
    Set_Terrain(0, 9, 8, tt_Forest2);
    Set_Terrain(0, 10, 12, tt_Hills_Fst + 2);
    Set_Terrain(0, 8, 10, tt_RiverM_1st);
    EXPECT_EQ(Eval("OREINRANGE(1,6)"), 2);                   /* OreIron */
    EXPECT_EQ(Eval("OREINRANGE(1,1)"), 1);                   /* OreWildgame */
    EXPECT_EQ(Eval("OREINRANGE(1,5)"), 0);                   /* OreOrihalcon: CoM2 only */
    EXPECT_EQ(Eval("TERRAININRANGE(1,12)"), 1);              /* LTForest */
    EXPECT_EQ(Eval("TERRAININRANGE(1,5)"), 1);               /* LTHill */
    EXPECT_EQ(Eval("TERRAININRANGE(1,3)"), 1);               /* LTRiverMouth */
    EXPECT_EQ(Eval("TERRAININRANGE(1,4)"), 18);              /* the rest is grassland */
}

TEST_F(CAS_Game_test, City_Area_Wraps_In_X)
{
    Set_Special(1, 1, 5, TS_GEMS);                           /* two squares east of x=59 */
    EXPECT_EQ(Eval("OREINRANGE(2,10)"), 1);
}

TEST_F(CAS_Game_test, Map_Uses_Planes_1_And_2)
{
    EXPECT_EQ(Eval("PLANESIZEX(1)"), 60);
    EXPECT_EQ(Eval("PLANESIZEY(2)"), 40);
    EXPECT_EQ(Eval("CITYONTILE(1,10,10)"), 1);
    EXPECT_EQ(Eval("CITYONTILE(2,59,5)"), 2);
    EXPECT_EQ(Eval("CITYONTILE(2,119,5)"), 2);               /* x wraps */
    EXPECT_EQ(Eval("CITYONTILE(2,59,40)"), 0);               /* off the map */
    EXPECT_EQ(Eval("CITYONTILE(1,59,5)"), 0);
    EXPECT_EQ(Eval("CITYOWNER(CITYONTILE(1,59,5))"), -1);    /* Warlord reads the owner of "no city" */
}

TEST_F(CAS_Game_test, Units_On_A_Tile)
{
    _units = 3;
    units[0].owner_idx = 1; units[0].wp = 0; units[0].wx = 4; units[0].wy = 4;
    units[1].owner_idx = 2; units[1].wp = 1; units[1].wx = 4; units[1].wy = 4;
    units[2].owner_idx = 4; units[2].wp = 0; units[2].wx = 4; units[2].wy = 4;
    EXPECT_EQ(Eval("NOFUNITSONTILE(1,4,4)"), 2);
    EXPECT_EQ(Eval("UNITONTILE(1,4,4,2)"), 3);                /* unit index 2 */
    EXPECT_EQ(Eval("GETSTAT(UNITONTILE(1,4,4,1),80,1)"), 1); /* SOwner */
    EXPECT_EQ(Eval("UNITONTILE(1,5,5,1)"), 0);
    EXPECT_EQ(Eval("GETSTAT(UNITONTILE(1,5,5,1),80,1)"), -1);
    EXPECT_EQ(Eval("CITYONTILE(3)"), 0);                     /* unit 3 stands on no city */
    EXPECT_EQ(Eval("GETSTAT(0,1,1)"), INT32_MIN);            /* other stats come with the unit hooks */
}

TEST_F(CAS_Game_test, Fortress_Banished_Defeated)
{
    fortresses[0].active = 1; fortresses[0].wp = 0; fortresses[0].wx = 10; fortresses[0].wy = 10;
    fortresses[1].active = 0; _players[1].casting_spell_idx = 214;
    fortresses[2].active = 0;
    EXPECT_EQ(Eval("ISFORTRESS(1)"), 1);
    EXPECT_EQ(Eval("ISFORTRESS(2)"), 0);
    EXPECT_EQ(Eval("BANISHED(1)"), 1);
    EXPECT_EQ(Eval("DEFEATED(1)"), 0);
    EXPECT_EQ(Eval("DEFEATED(2)"), 1);
    EXPECT_EQ(Eval("BANISHED(0) + DEFEATED(0)"), 0);
}

TEST_F(CAS_Game_test, Diplomacy_And_Wizard_Facts)
{
    _players[0].Dipl.Dipl_Status[1] = FINAL_WAR;
    _players[0].Dipl.Contacted[1] = 1;
    _players[0].Dipl.Visible_Rel[1] = -40;
    _players[1].capital_race = 12;
    _players[1].researching_spell_idx = 55;
    EXPECT_EQ(Eval("TREATY(0,1)"), 3);                       /* Final War is War to CoM2 */
    EXPECT_EQ(Eval("CONTACT(0,1)"), 1);
    EXPECT_EQ(Eval("CONTACT(0,2)"), 0);
    EXPECT_EQ(Eval("RELATION(0,1)"), -40);
    EXPECT_EQ(Eval("HOMERACE(1)"), 12);
    EXPECT_EQ(Eval("RESEARCHING(1)"), 55);
    EXPECT_EQ(Eval("NOFWIZARDS"), 2);
}

TEST_F(CAS_Game_test, Events)
{
    events.Good_Moon_Status = es_Ongoing;
    events.Good_Moon_Duration = 4;
    events.Population_Boom_Status = es_Starting;
    events.Population_Boom_Data = 1;
    EXPECT_EQ(Eval("EVENTTURN(4)"), 4);                      /* EvGoodMoon */
    EXPECT_EQ(Eval("EVENTTURN(5)"), 0);                      /* EvBadMoon */
    EXPECT_EQ(Eval("EVENTTURN(16)"), 1);                     /* EvPopBoom, just started */
    EXPECT_EQ(Eval("EVENTVAL(16)"), 2);                      /* the city, 1-based */
    EXPECT_EQ(Eval("EVENTTURN(7)"), 0);                      /* EvMerchant: CoM2 only */
}

TEST_F(CAS_Game_test, City_Facts_And_Options)
{
    EXPECT_EQ(Eval("POPUNITS(1)"), 7);
    EXPECT_EQ(Eval("CITYRACE(1)"), 4);
    EXPECT_EQ(Eval("CITYOWNER(2)"), 2);
    EXPECT_EQ(Eval("MAXCITY"), 2);
    EXPECT_EQ(Eval("SCORINGOPTION(3)"), 0);
    EXPECT_EQ(Eval("DIFFICULTY"), 3);                        /* Normal */
    _difficulty = god_Impossible;
    EXPECT_EQ(Eval("DIFFICULTY"), 7);
}

TEST_F(CAS_Game_test, Script_Slots)
{
    Run("SETCITYDATA 2, 7, 42; SETWIZARDDATA(2, 100, -5);");
    EXPECT_EQ(Eval("GETCITYDATA(2,7)"), 42);
    EXPECT_EQ(Eval("GETWIZARDDATA(2,100)"), -5);
    EXPECT_EQ(Eval("GETCITYDATA(1,7)"), 0);
    EXPECT_EQ(Eval("GETCITYDATA(2,0)"), INT32_MIN);          /* slots are 1..100 */
    EXPECT_EQ(Eval("GETCITYDATA(2,101)"), INT32_MIN);
    CAS_Game_Reset_Slots();
    EXPECT_EQ(Eval("GETCITYDATA(2,7)"), 0);
}

}  /* namespace */
