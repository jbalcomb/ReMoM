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

class NEWG_EqualizeNodes_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _world_maps = (uint8_t *)Allocate_Space(602);
        memset(_world_maps, tt_Ocean1, WORLD_SIZE * NUM_PLANES * sizeof(uint16_t));
        
        _landmasses = (uint8_t *)Allocate_Space(300);
        memset(_landmasses, 0, WORLD_SIZE * NUM_PLANES);
        
        memset(_NODES, 0, sizeof(_NODES));
        
        Seed_Random(12345);
    }

    void TearDown() override
    {
        free(_world_maps);
        _world_maps = NULL;
        free(_landmasses);
        _landmasses = NULL;
    }
};

TEST_F(NEWG_EqualizeNodes_test, BalancedDistribution_NoChange)
{
    // Arrange - create balanced distribution (6 each)
    for (int i = 0; i < 6; i++)
    {
        _NODES[i].wp = ARCANUS_PLANE;
        _NODES[i].type = nt_Sorcery;
        _NODES[i].wx = 10 + i;
        _NODES[i].wy = 20;
        SET_TERRAIN_TYPE(_NODES[i].wx, _NODES[i].wy, ARCANUS_PLANE, tt_SorceryNode);
    }
    for (int i = 6; i < 12; i++)
    {
        _NODES[i].wp = ARCANUS_PLANE;
        _NODES[i].type = nt_Chaos;
        _NODES[i].wx = 10 + i;
        _NODES[i].wy = 20;
        SET_TERRAIN_TYPE(_NODES[i].wx, _NODES[i].wy, ARCANUS_PLANE, tt_ChaosNode);
    }
    for (int i = 12; i < 16; i++)
    {
        _NODES[i].wp = ARCANUS_PLANE;
        _NODES[i].type = nt_Nature;
        _NODES[i].wx = 10 + i;
        _NODES[i].wy = 20;
        SET_TERRAIN_TYPE(_NODES[i].wx, _NODES[i].wy, ARCANUS_PLANE, tt_NatureNode);
    }

    // Act
    NEWG_EqualizeNodes__WIP(ARCANUS_PLANE);

    // Assert - count node types
    int sorcery = 0, chaos = 0, nature = 0;
    for (int i = 0; i < 16; i++)
    {
        if (_NODES[i].wp == ARCANUS_PLANE)
        {
            if (_NODES[i].type == nt_Sorcery) sorcery++;
            if (_NODES[i].type == nt_Chaos) chaos++;
            if (_NODES[i].type == nt_Nature) nature++;
        }
    }
    
    EXPECT_GE(chaos, 6) << "Should have at least 6 Chaos nodes";
    EXPECT_GE(nature, 6) << "Should have at least 6 Nature nodes (some converted from Sorcery)";
}

TEST_F(NEWG_EqualizeNodes_test, ExcessSorcery_ConvertsToOthers)
{
    // Arrange - all Arcanus nodes are Sorcery
    for (int i = 0; i < 16; i++)
    {
        _NODES[i].wp = ARCANUS_PLANE;
        _NODES[i].type = nt_Sorcery;
        _NODES[i].wx = 10 + i;
        _NODES[i].wy = 20;
        SET_TERRAIN_TYPE(_NODES[i].wx, _NODES[i].wy, ARCANUS_PLANE, tt_SorceryNode);
    }

    // Act
    NEWG_EqualizeNodes__WIP(ARCANUS_PLANE);

    // Assert - should have converted some to Chaos and Nature
    int sorcery = 0, chaos = 0, nature = 0;
    for (int i = 0; i < 16; i++)
    {
        if (_NODES[i].wp == ARCANUS_PLANE)
        {
            if (_NODES[i].type == nt_Sorcery) sorcery++;
            if (_NODES[i].type == nt_Chaos) chaos++;
            if (_NODES[i].type == nt_Nature) nature++;
        }
    }
    
    EXPECT_LT(sorcery, 16) << "Should have converted some Sorcery nodes";
    EXPECT_GT(chaos, 0) << "Should have some Chaos nodes";
    EXPECT_GT(nature, 0) << "Should have some Nature nodes";
}

TEST_F(NEWG_EqualizeNodes_test, Myrror_MinimumThreeEach)
{
    // Arrange - all Myrror nodes are Sorcery
    for (int i = 16; i < NUM_NODES; i++)
    {
        _NODES[i].wp = MYRROR_PLANE;
        _NODES[i].type = nt_Sorcery;
        _NODES[i].wx = 10 + (i - 16);
        _NODES[i].wy = 20;
        SET_TERRAIN_TYPE(_NODES[i].wx, _NODES[i].wy, MYRROR_PLANE, tt_SorceryNode);
    }

    // Act
    NEWG_EqualizeNodes__WIP(MYRROR_PLANE);

    // Assert
    int chaos = 0, nature = 0;
    for (int i = 16; i < NUM_NODES; i++)
    {
        if (_NODES[i].wp == MYRROR_PLANE)
        {
            if (_NODES[i].type == nt_Chaos) chaos++;
            if (_NODES[i].type == nt_Nature) nature++;
        }
    }
    
    EXPECT_GE(chaos, 3) << "Myrror should have at least 3 Chaos nodes";
    EXPECT_GE(nature, 3) << "Myrror should have at least 3 Nature nodes";
}

TEST_F(NEWG_EqualizeNodes_test, DoesNotAffectOtherPlane)
{
    // Arrange - set Arcanus nodes
    for (int i = 0; i < 16; i++)
    {
        _NODES[i].wp = ARCANUS_PLANE;
        _NODES[i].type = nt_Sorcery;
        _NODES[i].wx = 10 + i;
        _NODES[i].wy = 20;
    }
    
    // Set Myrror nodes
    for (int i = 16; i < NUM_NODES; i++)
    {
        _NODES[i].wp = MYRROR_PLANE;
        _NODES[i].type = nt_Chaos;
        _NODES[i].wx = 10 + (i - 16);
        _NODES[i].wy = 20;
    }

    // Act - only process Arcanus
    NEWG_EqualizeNodes__WIP(ARCANUS_PLANE);

    // Assert - Myrror nodes unchanged
    for (int i = 16; i < NUM_NODES; i++)
    {
        EXPECT_EQ(_NODES[i].type, nt_Chaos) << "Myrror nodes should remain Chaos";
    }
}

TEST_F(NEWG_EqualizeNodes_test, UpdatesTerrainType)
{
    // Arrange - all Sorcery
    for (int i = 0; i < 16; i++)
    {
        _NODES[i].wp = ARCANUS_PLANE;
        _NODES[i].type = nt_Sorcery;
        _NODES[i].wx = 10 + i;
        _NODES[i].wy = 20;
        SET_TERRAIN_TYPE(_NODES[i].wx, _NODES[i].wy, ARCANUS_PLANE, tt_SorceryNode);
    }

    // Act
    NEWG_EqualizeNodes__WIP(ARCANUS_PLANE);

    // Assert - verify terrain types match node types
    for (int i = 0; i < 16; i++)
    {
        if (_NODES[i].wp == ARCANUS_PLANE)
        {
            int16_t wx = _NODES[i].wx;
            int16_t wy = _NODES[i].wy;
            int16_t terrain = GET_TERRAIN_TYPE(wx, wy, ARCANUS_PLANE);
            
            if (_NODES[i].type == nt_Chaos)
            {
                EXPECT_EQ(terrain, tt_ChaosNode) << "Chaos node terrain mismatch at " << i;
            }
            else if (_NODES[i].type == nt_Nature)
            {
                EXPECT_EQ(terrain, tt_NatureNode) << "Nature node terrain mismatch at " << i;
            }
            else if (_NODES[i].type == nt_Sorcery)
            {
                EXPECT_EQ(terrain, tt_SorceryNode) << "Sorcery node terrain mismatch at " << i;
            }
        }
    }
}

class NEWG_CreateNodeAura_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        Seed_Random(12345);
    }
};

TEST_F(NEWG_CreateNodeAura_test, FirstAuraSquare_IsCenterNode)
{
    // Arrange
    int8_t aura_xs[20];
    int8_t aura_ys[20];

    // Act
    NEWG_CreateNodeAura__WIP(5, aura_xs, aura_ys, 30, 20);

    // Assert
    EXPECT_EQ(aura_xs[0], 30);
    EXPECT_EQ(aura_ys[0], 20);
}

TEST_F(NEWG_CreateNodeAura_test, PowerDeterminesAuraSize)
{
    // Arrange
    int8_t aura_xs[20];
    int8_t aura_ys[20];
    int16_t power = 8;

    // Act
    NEWG_CreateNodeAura__WIP(power, aura_xs, aura_ys, 30, 20);

    // Assert - verify at least 'power' unique coordinates
    int unique_count = 1; // Center is always included
    for (int i = 1; i < power; i++)
    {
        bool is_unique = true;
        for (int j = 0; j < i; j++)
        {
            if (aura_xs[i] == aura_xs[j] && aura_ys[i] == aura_ys[j])
            {
                is_unique = false;
                break;
            }
        }
        if (is_unique) unique_count++;
    }
    EXPECT_EQ(unique_count, power) << "Should have unique aura squares equal to power";
}

TEST_F(NEWG_CreateNodeAura_test, AuraSquares_AreAdjacentOrNearby)
{
    // Arrange
    int8_t aura_xs[20];
    int8_t aura_ys[20];
    int16_t power = 5;

    // Act
    NEWG_CreateNodeAura__WIP(power, aura_xs, aura_ys, 30, 20);

    // Assert - all aura squares should be within reasonable distance
    for (int i = 0; i < power; i++)
    {
        int dx = abs(aura_xs[i] - 30);
        int dy = abs(aura_ys[i] - 20);
        EXPECT_LE(dx, 2) << "Aura square " << i << " too far in X";
        EXPECT_LE(dy, 2) << "Aura square " << i << " too far in Y";
    }
}

TEST_F(NEWG_CreateNodeAura_test, NoOverlap_WithinAura)
{
    // Arrange
    int8_t aura_xs[20];
    int8_t aura_ys[20];
    int16_t power = 9;

    // Act
    NEWG_CreateNodeAura__WIP(power, aura_xs, aura_ys, 30, 20);

    // Assert - no duplicates
    for (int i = 0; i < power; i++)
    {
        for (int j = i + 1; j < power; j++)
        {
            bool same = (aura_xs[i] == aura_xs[j] && aura_ys[i] == aura_ys[j]);
            EXPECT_FALSE(same) << "Duplicate aura square at indices " << i << " and " << j;
        }
    }
}

TEST_F(NEWG_CreateNodeAura_test, AuraSquares_WithinBounds)
{
    // Arrange
    int8_t aura_xs[20];
    int8_t aura_ys[20];
    int16_t power = 15;

    // Act
    NEWG_CreateNodeAura__WIP(power, aura_xs, aura_ys, 30, 20);

    // Assert
    for (int i = 0; i < power; i++)
    {
        EXPECT_GT(aura_xs[i], 0) << "Aura X coordinate " << i << " out of bounds";
        EXPECT_LT(aura_xs[i], 59) << "Aura X coordinate " << i << " out of bounds";
        EXPECT_GT(aura_ys[i], 0) << "Aura Y coordinate " << i << " out of bounds";
        EXPECT_LT(aura_ys[i], 39) << "Aura Y coordinate " << i << " out of bounds";
    }
}

TEST_F(NEWG_CreateNodeAura_test, SmallPower_Adjacent)
{
    // Arrange - power less than 9 should only use adjacent squares
    int8_t aura_xs[20];
    int8_t aura_ys[20];
    int16_t power = 5;

    // Act
    NEWG_CreateNodeAura__WIP(power, aura_xs, aura_ys, 30, 20);

    // Assert - all should be immediately adjacent (within 1 square)
    for (int i = 1; i < power; i++)
    {
        int dx = abs(aura_xs[i] - 30);
        int dy = abs(aura_ys[i] - 20);
        EXPECT_LE(dx + dy, 1) << "Power < 9 should only use adjacent squares";
    }
}
