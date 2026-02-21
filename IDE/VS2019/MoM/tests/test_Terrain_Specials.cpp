#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/MOM_Data.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/random.h"
#include "../src/MAPGEN.h"
#include "../src/TerrType.h"
#ifdef __cplusplus
}
#endif

#include <cstring>

class Terrain_Special_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        Seed_Random(12345);
    }
};

TEST_F(Terrain_Special_test, Desert_Arcanus_ReturnsGems)
{
    // Act multiple times to check distribution
    int gems_count = 0;
    int quork_count = 0;
    
    for (int i = 0; i < 100; i++)
    {
        Seed_Random(i);
        int16_t special = Desert_Terrain_Special(ARCANUS_PLANE);
        if (special == ts_Gems) gems_count++;
        if (special == ts_QuorkCrystals) quork_count++;
    }
    
    // Assert - Gems should be more common (6/9) than Quork (3/9)
    EXPECT_GT(gems_count, quork_count) << "Gems should be more common than Quork on Arcanus";
    EXPECT_GT(gems_count, 0) << "Should sometimes return Gems";
    EXPECT_GT(quork_count, 0) << "Should sometimes return Quork";
}

TEST_F(Terrain_Special_test, Desert_Myrror_ReturnsValidSpecial)
{
    // Act multiple times to check all possibilities
    bool found_gems = false;
    bool found_quork = false;
    bool found_crysx = false;
    
    for (int i = 0; i < 100; i++)
    {
        Seed_Random(i);
        int16_t special = Desert_Terrain_Special(MYRROR_PLANE);
        
        if (special == ts_Gems) found_gems = true;
        if (special == ts_QuorkCrystals) found_quork = true;
        if (special == ts_CrysxCrystals) found_crysx = true;
    }
    
    // Assert - Myrror can return all three types
    EXPECT_TRUE(found_gems) << "Should sometimes return Gems";
    EXPECT_TRUE(found_quork) << "Should sometimes return Quork";
    EXPECT_TRUE(found_crysx) << "Should sometimes return Crysx (Myrror only)";
}

TEST_F(Terrain_Special_test, Hills_Arcanus_ReturnsValidSpecial)
{
    // Act multiple times to check distribution
    bool found_iron = false;
    bool found_coal = false;
    bool found_silver = false;
    bool found_gold = false;
    bool found_mithril = false;
    bool found_adamantium = false;
    
    for (int i = 0; i < 200; i++)
    {
        Seed_Random(i);
        int16_t special = Hills_Terrain_Special(ARCANUS_PLANE);
        
        if (special == ts_Iron) found_iron = true;
        if (special == ts_Coal) found_coal = true;
        if (special == ts_Silver) found_silver = true;
        if (special == ts_Gold) found_gold = true;
        if (special == ts_Mithril) found_mithril = true;
        if (special == ts_Adamantium) found_adamantium = true;
    }
    
    // Assert - Arcanus hills should have all except Adamantium
    EXPECT_TRUE(found_iron) << "Should sometimes return Iron";
    EXPECT_TRUE(found_coal) << "Should sometimes return Coal";
    EXPECT_TRUE(found_silver) << "Should sometimes return Silver";
    EXPECT_TRUE(found_gold) << "Should sometimes return Gold";
    EXPECT_TRUE(found_mithril) << "Should sometimes return Mithril";
    EXPECT_FALSE(found_adamantium) << "Arcanus should never return Adamantium";
}

TEST_F(Terrain_Special_test, Hills_Myrror_ReturnsValidSpecial)
{
    // Act multiple times to check distribution
    bool found_adamantium = false;
    bool found_mithril = false;
    
    for (int i = 0; i < 200; i++)
    {
        Seed_Random(i + 1000);
        int16_t special = Hills_Terrain_Special(MYRROR_PLANE);
        
        if (special == ts_Adamantium) found_adamantium = true;
        if (special == ts_Mithril) found_mithril = true;
    }
    
    // Assert - Myrror can return Adamantium
    EXPECT_TRUE(found_adamantium) << "Myrror should sometimes return Adamantium";
    EXPECT_TRUE(found_mithril) << "Myrror should sometimes return Mithril";
}

TEST_F(Terrain_Special_test, Mountain_Arcanus_ReturnsValidSpecial)
{
    // Act multiple times to check distribution
    bool found_iron = false;
    bool found_coal = false;
    bool found_silver = false;
    bool found_gold = false;
    bool found_mithril = false;
    bool found_adamantium = false;
    
    for (int i = 0; i < 200; i++)
    {
        Seed_Random(i);
        int16_t special = Mountain_Terrain_Special(ARCANUS_PLANE);
        
        if (special == ts_Iron) found_iron = true;
        if (special == ts_Coal) found_coal = true;
        if (special == ts_Silver) found_silver = true;
        if (special == ts_Gold) found_gold = true;
        if (special == ts_Mithril) found_mithril = true;
        if (special == ts_Adamantium) found_adamantium = true;
    }
    
    // Assert - Arcanus mountains should have all except Adamantium
    EXPECT_TRUE(found_iron) << "Should sometimes return Iron";
    EXPECT_TRUE(found_coal) << "Should sometimes return Coal";
    EXPECT_TRUE(found_silver) << "Should sometimes return Silver";
    EXPECT_TRUE(found_gold) << "Should sometimes return Gold";
    EXPECT_TRUE(found_mithril) << "Should sometimes return Mithril";
    EXPECT_FALSE(found_adamantium) << "Arcanus should never return Adamantium";
}

TEST_F(Terrain_Special_test, Mountain_Myrror_CanReturnAdamantium)
{
    // Act multiple times to check distribution
    bool found_adamantium = false;
    
    for (int i = 0; i < 200; i++)
    {
        Seed_Random(i + 2000);
        int16_t special = Mountain_Terrain_Special(MYRROR_PLANE);
        
        if (special == ts_Adamantium) found_adamantium = true;
    }
    
    // Assert
    EXPECT_TRUE(found_adamantium) << "Myrror mountains should sometimes return Adamantium";
}

TEST_F(Terrain_Special_test, Mountain_Coal_MoreCommonThanSilver)
{
    // Act - count occurrences over many samples
    int coal_count = 0;
    int silver_count = 0;
    
    for (int i = 0; i < 200; i++)
    {
        Seed_Random(i);
        int16_t special = Mountain_Terrain_Special(ARCANUS_PLANE);
        if (special == ts_Coal) coal_count++;
        if (special == ts_Silver) silver_count++;
    }
    
    // Assert - Coal (5/18) should be more common than Silver (2/18)
    EXPECT_GT(coal_count, silver_count) << "Coal should be more common than Silver in mountains";
}
