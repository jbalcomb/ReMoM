#include <gtest/gtest.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

extern "C" {
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/Util.h"  /* Set_Bit_Field(), Clear_Bit_Field() */
#include "../src/Terrain.h"
#include "../src/TerrType.h"
#include "../../MOD/src/MOD_Lua.h"
#include "../../MOD/src/MOD_Rules.h"
}

/*
    Square_Production_Bonus() asks the loaded rules profile (assets/rules/<profile>/terrain.lua).
    The "original" profile must reproduce Square_Production_Bonus_Original(), the faithful 1.31 code, for every input.
*/

namespace
{

const std::string rules_dir = std::string(REMOM_SOURCE_DIR) + "/assets/rules/";

class Square_Production_Bonus_Rules_test : public ::testing::Test
{
protected:
    std::vector<uint8_t> world = std::vector<uint8_t>(NUM_PLANES * WORLD_SIZE * 2, 0);
    std::vector<uint8_t> shared_bits = std::vector<uint8_t>(NUM_PLANES * WORLD_SIZE, 0);
    uint8_t * saved_world;
    uint8_t * saved_shared_bits;

    static const int wx = 5;
    static const int wy = 7;
    static const int wp = 1;

    void SetUp() override
    {
        saved_world = _world_maps;
        saved_shared_bits = city_area_shared_bits;
        _world_maps = world.data();
        city_area_shared_bits = shared_bits.data();
        MOD_Lua_Shutdown();
    }

    void TearDown() override
    {
        MOD_Lua_Shutdown();
        _world_maps = saved_world;
        city_area_shared_bits = saved_shared_bits;
    }

    void Set_Square(int16_t terrain_value, bool shared)
    {
        int bit = (wy * WORLD_WIDTH) + wx;
        std::memcpy(&world[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx) * 2], &terrain_value, 2);
        if(shared) { Set_Bit_Field(bit, &city_area_shared_bits[wp * WORLD_SIZE]); }
        else       { Clear_Bit_Field(bit, &city_area_shared_bits[wp * WORLD_SIZE]); }
    }

    void Load(const std::string & profile)
    {
        ASSERT_EQ(MOD_Rules_Load((rules_dir + profile).c_str()), 0) << MOD_Lua_Last_Error();
    }

    /* Every 16-bit map value, Gaia's Blessing 0 / 1 / 2 (the C code tests `!= ST_TRUE`), shared or not:
       the number of inputs where the loaded profile disagrees with the 1.31 C code. */
    int Count_Differences_From_Original(std::string * first)
    {
        int differences = 0;
        for(int32_t value = INT16_MIN; value <= INT16_MAX; value++)
        {
            for(int shared = 0; shared <= 1; shared++)
            {
                Set_Square((int16_t)value, shared != 0);
                for(int16_t gaia = 0; gaia <= 2; gaia++)
                {
                    int16_t expected = Square_Production_Bonus_Original(wx, wy, wp, gaia);
                    int16_t actual = Square_Production_Bonus(wx, wy, wp, gaia);
                    if(expected != actual)
                    {
                        if(differences == 0 && first != nullptr)
                        {
                            *first = "map value " + std::to_string(value) + " shared " + std::to_string(shared) + " gaia " + std::to_string(gaia) + ": C " + std::to_string(expected) + ", Lua " + std::to_string(actual);
                        }
                        differences++;
                    }
                }
            }
        }
        return differences;
    }

    int16_t Bonus(int16_t terrain_type, int16_t gaia = 0, bool shared = false)
    {
        Set_Square(terrain_type, shared);
        return Square_Production_Bonus(wx, wy, wp, gaia);
    }
};

TEST_F(Square_Production_Bonus_Rules_test, Original_Profile_Matches_The_C_Code_For_Every_Input)
{
    Load("original");
    std::string first;
    EXPECT_EQ(Count_Differences_From_Original(&first), 0) << "first difference: " << first;
}

TEST_F(Square_Production_Bonus_Rules_test, Original_Profile_Is_Really_Asked)
{
    /* A profile that answers 42 for everything proves the C code is not answering. */
    std::string dir = ::testing::TempDir() + "rules_answer_42";
    std::system(("mkdir -p '" + dir + "'").c_str());
    FILE * fp = std::fopen((dir + "/profile.lua").c_str(), "w");
    ASSERT_NE(fp, nullptr);
    std::fputs("function rules.square_production_bonus(t, g, s) return 42 end\n", fp);
    std::fclose(fp);
    ASSERT_EQ(MOD_Rules_Load(dir.c_str()), 0) << MOD_Lua_Last_Error();
    EXPECT_EQ(Bonus(tt_Mountain1), 42);
}

TEST_F(Square_Production_Bonus_Rules_test, Without_A_Profile_The_C_Code_Answers)
{
    EXPECT_STREQ(MOD_Rules_Profile(), "");
    EXPECT_EQ(Bonus(tt_Mountain1), 5);
    EXPECT_EQ(Bonus(tt_Forest2, 1), 6);
}

TEST_F(Square_Production_Bonus_Rules_test, Community_Patch_Profile_Is_Still_The_Original)
{
    /* No Community Patch change to this rule is confirmed yet (assets/rules/community_patch/terrain.lua). */
    Load("community_patch");
    EXPECT_EQ(Count_Differences_From_Original(nullptr), 0);
}

TEST_F(Square_Production_Bonus_Rules_test, Com2_Profile_Uses_CoM2_Terrain_Values)
{
    Load("com2");
    EXPECT_EQ(Bonus(tt_Mountain1), 7);                     /* 1.31: 5 */
    EXPECT_EQ(Bonus(tt_Mountains_Fst + 3), 7);             /* mountain range */
    EXPECT_EQ(Bonus(tt_ChaosNode), 7);
    EXPECT_EQ(Bonus(tt_Hills_Fst), 3);
    EXPECT_EQ(Bonus(tt_Desert1), 3);                       /* 1.31: 0 */
    EXPECT_EQ(Bonus(tt_Desert_Fst + 10), 3);
    EXPECT_EQ(Bonus(tt_Forest1), 3);
    EXPECT_EQ(Bonus(tt_Forest1, 1), 9);                    /* ForestGaiaProduction; 1.31: 6 */
    EXPECT_EQ(Bonus(tt_NatureNode, 1), 3);                 /* 1.31: 6 */
    EXPECT_EQ(Bonus(tt_Grasslands1), 0);
    EXPECT_EQ(Bonus(tt_Tundra_1st), 0);
    EXPECT_EQ(Bonus(tt_Volcano), 0);
    EXPECT_EQ(Bonus(tt_Mountain1, 0, true), 3);            /* shared: halved, as 1.31 (unconfirmed for CoM2) */
}

TEST_F(Square_Production_Bonus_Rules_test, A_Script_Error_Falls_Back_To_The_C_Code)
{
    std::string dir = ::testing::TempDir() + "rules_broken";
    std::system(("mkdir -p '" + dir + "'").c_str());
    FILE * fp = std::fopen((dir + "/profile.lua").c_str(), "w");
    ASSERT_NE(fp, nullptr);
    std::fputs("function rules.square_production_bonus(t, g, s) error('boom') end\n", fp);
    std::fclose(fp);
    ASSERT_EQ(MOD_Rules_Load(dir.c_str()), 0) << MOD_Lua_Last_Error();
    EXPECT_EQ(Bonus(tt_Mountain1), 5);
    EXPECT_NE(std::string(MOD_Lua_Last_Error()).find("boom"), std::string::npos);
}

TEST_F(Square_Production_Bonus_Rules_test, Profiles_Cannot_Load_Files_Outside_A_Rules_Folder)
{
    std::string dir = ::testing::TempDir() + "rules_escape";
    std::system(("mkdir -p '" + dir + "'").c_str());
    FILE * fp = std::fopen((dir + "/profile.lua").c_str(), "w");
    ASSERT_NE(fp, nullptr);
    std::fputs("rules.load('../../etc/passwd')\n", fp);
    std::fclose(fp);
    EXPECT_EQ(MOD_Rules_Load(dir.c_str()), -1);
    EXPECT_STREQ(MOD_Rules_Profile(), "");
    EXPECT_EQ(Bonus(tt_Mountain1), 5);
}

TEST_F(Square_Production_Bonus_Rules_test, Unknown_Profile_Fails_And_Leaves_The_C_Code)
{
    EXPECT_EQ(MOD_Rules_Load("no_such_profile"), -1);
    EXPECT_STREQ(MOD_Rules_Profile(), "");
    EXPECT_EQ(Bonus(tt_Mountain1), 5);
}

}  /* namespace */
