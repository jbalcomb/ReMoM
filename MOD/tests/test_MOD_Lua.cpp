#include <gtest/gtest.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

extern "C" {
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/random.h"
#include "../src/MOD_Hook.h"
#include "../src/MOD_Load.h"
#include "../src/MOD_Lua.h"
}

namespace
{

class MOD_Lua_test : public ::testing::Test
{
protected:
    struct s_CITY cities[2];
    struct s_CITY * saved_cities = nullptr;
    int16_t saved_count = 0;

    void SetUp() override
    {
        MOD_Lua_Shutdown();
        saved_cities = _CITIES;
        saved_count = _cities;
        memset(cities, 0, sizeof(cities));
        memset(cities[0].bldg_status, bs_NotBuilt, sizeof(cities[0].bldg_status));
        memset(cities[1].bldg_status, bs_NotBuilt, sizeof(cities[1].bldg_status));
        cities[0].owner_idx = 0;
        cities[0].race = 5;
        cities[0].size = 4;
        cities[1].owner_idx = 2;
        cities[1].race = 9;
        cities[1].size = 7;
        cities[1].bldg_status[3] = bs_Built;
        cities[1].bldg_status[4] = bs_Replaced;
        _CITIES = cities;
        _cities = 2;
        ASSERT_EQ(MOD_Lua_Init(), 0);
    }

    void TearDown() override
    {
        MOD_Lua_Shutdown();
        _CITIES = saved_cities;
        _cities = saved_count;
    }

    /* Evaluates `expr` inside a city_food handler and returns the number it produces. */
    int32_t Eval(const std::string & expr)
    {
        std::string chunk = "game.on('city_food', function(c, v) return " + expr + " end)";
        EXPECT_EQ(MOD_Lua_Run_String("eval", chunk.c_str()), 0) << MOD_Lua_Last_Error();
        int32_t value = MOD_Hook_City_Value(MOD_HOOK_CITY_FOOD, 0, 0);
        MOD_Lua_Shutdown();
        MOD_Lua_Init();
        return value;
    }
};

}  // namespace

TEST_F(MOD_Lua_test, Hook_ReplacesValue)
{
    ASSERT_EQ(MOD_Lua_Run_String("t", "game.on('city_gold', function(city, gold) return gold * 2 end)"), 0) << MOD_Lua_Last_Error();
    EXPECT_EQ(MOD_Hook_Count(MOD_HOOK_CITY_GOLD), 1);
    EXPECT_EQ(MOD_Hook_City_Value(MOD_HOOK_CITY_GOLD, 1, 21), 42);
    EXPECT_EQ(MOD_Hook_City_Value(MOD_HOOK_CITY_FOOD, 1, 21), 21);
}

TEST_F(MOD_Lua_test, Hook_SeesCityData)
{
    ASSERT_EQ(MOD_Lua_Run_String("t",
        "game.on('city_research', function(c, v)\n"
        "  return v + game.citysize(c) * 10 + game.cityowner(c) + game.isbuilt(c, 3) * 2 + game.isbuilt(c, 4) * 4\n"
        "end)"), 0) << MOD_Lua_Last_Error();
    EXPECT_EQ(MOD_Hook_City_Value(MOD_HOOK_CITY_RESEARCH, 1, 5), 5 + 70 + 2 + 2 + 4);
    EXPECT_EQ(MOD_Hook_City_Value(MOD_HOOK_CITY_RESEARCH, 0, 5), 5 + 40);
}

TEST_F(MOD_Lua_test, Api_MatchesCasNames)
{
    EXPECT_EQ(Eval("game.maxcity()"), 1);
    EXPECT_EQ(Eval("game.cityrace(1)"), 9);
    EXPECT_EQ(Eval("game.cityowner(1)"), 2);
}

TEST_F(MOD_Lua_test, ScriptError_KeepsValueAndRecordsError)
{
    ASSERT_EQ(MOD_Lua_Run_String("t", "game.on('city_gold', function(c, v) error('boom') end)"), 0);
    EXPECT_EQ(MOD_Hook_City_Value(MOD_HOOK_CITY_GOLD, 0, 9), 9);
    EXPECT_NE(std::string(MOD_Lua_Last_Error()).find("boom"), std::string::npos);
}

TEST_F(MOD_Lua_test, NonNumberResult_KeepsValue)
{
    ASSERT_EQ(MOD_Lua_Run_String("t", "game.on('city_gold', function(c, v) return 'lots' end)"), 0);
    EXPECT_EQ(MOD_Hook_City_Value(MOD_HOOK_CITY_GOLD, 0, 9), 9);
}

TEST_F(MOD_Lua_test, BadCity_IsAnErrorNotACrash)
{
    ASSERT_EQ(MOD_Lua_Run_String("t", "game.on('city_gold', function(c, v) return game.citysize(99) end)"), 0);
    EXPECT_EQ(MOD_Hook_City_Value(MOD_HOOK_CITY_GOLD, 0, 9), 9);
    EXPECT_NE(std::string(MOD_Lua_Last_Error()).find("no city 99"), std::string::npos);
}

TEST_F(MOD_Lua_test, UnknownHook_FailsToLoad)
{
    EXPECT_EQ(MOD_Lua_Run_String("t", "game.on('city_happiness', function(c, v) return v end)"), -1);
    EXPECT_NE(std::string(MOD_Lua_Last_Error()).find("unknown hook"), std::string::npos);
}

TEST_F(MOD_Lua_test, SyntaxError_FailsToLoad)
{
    EXPECT_EQ(MOD_Lua_Run_String("t", "game.on('city_gold', function(c, v) return v"), -1);
}

TEST_F(MOD_Lua_test, Sandbox_HasNoFileOrOsAccess)
{
    EXPECT_EQ(MOD_Lua_Run_String("t",
        "assert(io == nil) assert(os == nil) assert(package == nil) assert(debug == nil)\n"
        "assert(dofile == nil) assert(loadfile == nil) assert(load == nil) assert(require == nil)\n"
        "assert(math.randomseed == nil)"), 0) << MOD_Lua_Last_Error();
}

TEST_F(MOD_Lua_test, Random_UsesGameRng)
{
    std::vector<int> from_c;
    std::vector<int> from_lua;

    Set_Random_Seed(12345);
    for(int itr = 0; itr < 5; itr++)
    {
        from_c.push_back(Random(100) - 1);
    }

    Set_Random_Seed(12345);
    ASSERT_EQ(MOD_Lua_Run_String("t", "game.on('city_food', function(c, v) return game.rnd(100) end)"), 0) << MOD_Lua_Last_Error();
    for(int itr = 0; itr < 5; itr++)
    {
        from_lua.push_back(MOD_Hook_City_Value(MOD_HOOK_CITY_FOOD, 0, 0));
    }

    EXPECT_EQ(from_c, from_lua);
}

TEST_F(MOD_Lua_test, MathRandom_UsesGameRng)
{
    Set_Random_Seed(777);
    int expected = Random(6);

    Set_Random_Seed(777);
    EXPECT_EQ(Eval("math.random(6)"), expected);
}

TEST_F(MOD_Lua_test, Shutdown_RemovesLuaHandlers)
{
    ASSERT_EQ(MOD_Lua_Run_String("t", "game.on('city_gold', function(c, v) return 0 end)"), 0);
    MOD_Lua_Shutdown();
    EXPECT_EQ(MOD_Hook_Count(MOD_HOOK_CITY_GOLD), 0);
    EXPECT_EQ(MOD_Hook_City_Value(MOD_HOOK_CITY_GOLD, 0, 9), 9);
}

TEST_F(MOD_Lua_test, LoadDirectory_RunsMainLua)
{
    const std::string dir = ::testing::TempDir() + "mod_load_test";
    std::string scripts = dir + "/scripts";
#ifdef _WIN32
    std::system(("mkdir \"" + scripts + "\"").c_str());
#else
    std::system(("mkdir -p '" + scripts + "'").c_str());
#endif
    FILE * ini = std::fopen((dir + "/mod.ini").c_str(), "w");
    ASSERT_NE(ini, nullptr);
    std::fputs("[Mod]\nname=Test Mod\nversion=0.1\n", ini);
    std::fclose(ini);
    FILE * lua = std::fopen((scripts + "/main.lua").c_str(), "w");
    ASSERT_NE(lua, nullptr);
    std::fputs("game.on('city_power', function(c, v) return v + 3 end)\n", lua);
    std::fclose(lua);

    EXPECT_EQ(MOD_Load_Directory(dir.c_str()), 0) << MOD_Lua_Last_Error();
    EXPECT_EQ(MOD_Hook_City_Value(MOD_HOOK_CITY_POWER, 0, 4), 7);
}

TEST_F(MOD_Lua_test, LoadDirectory_MissingScriptFails)
{
    EXPECT_EQ(MOD_Load_Directory("this/folder/does/not/exist"), -1);
}

/* A mod folder written to the test's temp directory: mod.ini [CAS] binds scripts to hooks. */
class MOD_Load_CAS_test : public MOD_Lua_test
{
protected:
    std::string dir;

    void Write(const std::string & name, const char * text)
    {
        FILE * fp = fopen((dir + "/" + name).c_str(), "w");
        ASSERT_NE(fp, nullptr);
        fputs(text, fp);
        fclose(fp);
    }

    void SetUp() override
    {
        MOD_Lua_test::SetUp();
        dir = ::testing::TempDir() + "remom_cas_mod_" + ::testing::UnitTest::GetInstance()->current_test_info()->name();   /* ctest runs tests in parallel */
#ifdef _WIN32
        std::system(("mkdir \"" + dir + "\"").c_str());
#else
        std::system(("mkdir -p '" + dir + "'").c_str());
#endif
    }

    void TearDown() override
    {
        MOD_Unload();
        MOD_Lua_test::TearDown();
    }
};

TEST_F(MOD_Load_CAS_test, Binds_Hooks_With_Master_And_Com2_Names)
{
    Write("mod.ini", "[Mod]\nname=CAS test\n[CAS]\nmaster=MASTER.CAS\ncity_gold=Gold.CAS\nEconomyFood=Food.CAS\nEconomyUnrest=Unrest.CAS\n");
    Write("MASTER.CAS", "BONUS=100;");
    Write("Gold.CAS", "RESULT = VALUE + BONUS + CITYOWNER(CITYID);");
    Write("Food.CAS", "ADDRESLINE \"Farmers\", 6; ADDRESLINE \"Granary\", 2;");
    ASSERT_EQ(MOD_Load_Directory(dir.c_str()), 0);
    EXPECT_EQ(MOD_Hook_Count(MOD_HOOK_CITY_GOLD), 1);
    EXPECT_EQ(MOD_Hook_City_Value(MOD_HOOK_CITY_GOLD, 1, 5), 107);       /* city 1 is owned by player 2 */
    EXPECT_EQ(MOD_Hook_City_Value(MOD_HOOK_CITY_FOOD, 0, 99), 8);         /* result lines replace the value */
}

TEST_F(MOD_Load_CAS_test, Script_Error_Keeps_Value)
{
    Write("mod.ini", "[CAS]\ncity_gold=Gold.CAS\n");
    Write("Gold.CAS", "RESULT = BOOKS(1,2);");                          /* not implemented in ReMoM yet */
    ASSERT_EQ(MOD_Load_Directory(dir.c_str()), 0);
    EXPECT_EQ(MOD_Hook_City_Value(MOD_HOOK_CITY_GOLD, 0, 9), 9);
}

TEST_F(MOD_Load_CAS_test, Broken_Script_Fails_To_Load)
{
    Write("mod.ini", "[CAS]\ncity_gold=Gold.CAS\n");
    Write("Gold.CAS", "IF 1 THEN RESULT = 2;");
    EXPECT_EQ(MOD_Load_Directory(dir.c_str()), -1);
}
