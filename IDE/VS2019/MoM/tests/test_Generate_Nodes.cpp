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

class Generate_Nodes_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _world_maps = (uint8_t *)Allocate_Space(602);
        memset(_world_maps, tt_Ocean1, WORLD_SIZE * NUM_PLANES * sizeof(uint16_t));
        
        _landmasses = (uint8_t *)Allocate_Space(300);
        memset(_landmasses, 0, WORLD_SIZE * NUM_PLANES);
        
        memset(_NODES, 0, sizeof(_NODES));
        
        // Set some land tiles
        for (int y = 2; y < 37; y++)
        {
            for (int x = 3; x < 57; x++)
            {
                SET_TERRAIN_TYPE(x, y, ARCANUS_PLANE, tt_Grasslands1);
                SET_TERRAIN_TYPE(x, y, MYRROR_PLANE, tt_Grasslands1);
            }
        }
        
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

TEST_F(Generate_Nodes_test, Creates30Nodes)
{
    // Act
    Generate_Nodes();

    // Assert
    int active_nodes = 0;
    for (int i = 0; i < NUM_NODES; i++)
    {
        if (_NODES[i].power > 0)
        {
            active_nodes++;
        }
    }
    EXPECT_EQ(active_nodes, NUM_NODES) << "Should create exactly 30 nodes";
}

TEST_F(Generate_Nodes_test, First16NodesAreArcanus)
{
    // Act
    Generate_Nodes();

    // Assert
    for (int i = 0; i < 16; i++)
    {
        EXPECT_EQ(_NODES[i].wp, ARCANUS_PLANE) << "First 16 nodes should be on Arcanus, node " << i;
    }
}

TEST_F(Generate_Nodes_test, Last14NodesAreMyrror)
{
    // Act
    Generate_Nodes();

    // Assert
    for (int i = 16; i < NUM_NODES; i++)
    {
        EXPECT_EQ(_NODES[i].wp, MYRROR_PLANE) << "Last 14 nodes should be on Myrror, node " << i;
    }
}

TEST_F(Generate_Nodes_test, ArcanusNodesPowerInRange)
{
    // Act
    Generate_Nodes();

    // Assert - Arcanus nodes have power 4-9
    for (int i = 0; i < 16; i++)
    {
        EXPECT_GE(_NODES[i].power, 4) << "Arcanus node " << i << " power too low";
        EXPECT_LE(_NODES[i].power, 9) << "Arcanus node " << i << " power too high";
    }
}

TEST_F(Generate_Nodes_test, MyrrorNodesPowerInRange)
{
    // Act
    Generate_Nodes();

    // Assert - Myrror nodes have power 9-19
    for (int i = 16; i < NUM_NODES; i++)
    {
        EXPECT_GE(_NODES[i].power, 9) << "Myrror node " << i << " power too low";
        EXPECT_LE(_NODES[i].power, 19) << "Myrror node " << i << " power too high";
    }
}

TEST_F(Generate_Nodes_test, NodesWithinMapBounds)
{
    // Act
    Generate_Nodes();

    // Assert
    for (int i = 0; i < NUM_NODES; i++)
    {
        EXPECT_GE(_NODES[i].wx, 3) << "Node " << i << " x coordinate too small";
        EXPECT_LT(_NODES[i].wx, 57) << "Node " << i << " x coordinate too large";
        EXPECT_GE(_NODES[i].wy, 2) << "Node " << i << " y coordinate too small";
        EXPECT_LT(_NODES[i].wy, 37) << "Node " << i << " y coordinate too large";
    }
}

TEST_F(Generate_Nodes_test, NodesInitiallyUnowned)
{
    // Act
    Generate_Nodes();

    // Assert
    for (int i = 0; i < NUM_NODES; i++)
    {
        EXPECT_EQ(_NODES[i].owner_idx, ST_UNDEFINED) << "Node " << i << " should start unowned";
    }
}

TEST_F(Generate_Nodes_test, NodesFlagsInitialized)
{
    // Act
    Generate_Nodes();

    // Assert
    for (int i = 0; i < NUM_NODES; i++)
    {
        EXPECT_EQ(_NODES[i].flags, 0) << "Node " << i << " flags should be initialized to 0";
    }
}
