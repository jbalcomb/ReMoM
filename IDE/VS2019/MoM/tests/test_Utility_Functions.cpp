#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/MOM_Data.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../src/MAPGEN.h"
#include "../src/TerrType.h"
#ifdef __cplusplus
}
#endif

#include <cstring>

class Set_Square_Explored_Bits_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _SQUARE_EXPLORED = (uint8_t *)Allocate_Space(300);
        memset(_SQUARE_EXPLORED, 0, WORLD_SIZE * NUM_PLANES);
    }

    void TearDown() override
    {
        free(_SQUARE_EXPLORED);
        _SQUARE_EXPLORED = NULL;
    }
};

TEST_F(Set_Square_Explored_Bits_test, SetsValidCoordinates)
{
    // Act
    Set_Square_Explored_Bits(ARCANUS_PLANE, 30, 20, SCT_TopLeft);

    // Assert
    EXPECT_EQ(GET_SQUARE_EXPLORED(30, 20, ARCANUS_PLANE), SCT_TopLeft);
}

TEST_F(Set_Square_Explored_Bits_test, IgnoresNegativeX)
{
    // Act
    Set_Square_Explored_Bits(ARCANUS_PLANE, -1, 20, SCT_TopLeft);

    // Assert - should not crash, and value should remain 0
    EXPECT_EQ(GET_SQUARE_EXPLORED(0, 20, ARCANUS_PLANE), 0);
}

TEST_F(Set_Square_Explored_Bits_test, IgnoresNegativeY)
{
    // Act
    Set_Square_Explored_Bits(ARCANUS_PLANE, 30, -1, SCT_TopLeft);

    // Assert
    EXPECT_EQ(GET_SQUARE_EXPLORED(30, 0, ARCANUS_PLANE), 0);
}

TEST_F(Set_Square_Explored_Bits_test, IgnoresXOutOfBounds)
{
    // Act
    Set_Square_Explored_Bits(ARCANUS_PLANE, WORLD_WIDTH, 20, SCT_TopLeft);

    // Assert - should not crash
    EXPECT_EQ(GET_SQUARE_EXPLORED(WORLD_WIDTH - 1, 20, ARCANUS_PLANE), 0);
}

TEST_F(Set_Square_Explored_Bits_test, IgnoresYOutOfBounds)
{
    // Act
    Set_Square_Explored_Bits(ARCANUS_PLANE, 30, WORLD_HEIGHT, SCT_TopLeft);

    // Assert
    EXPECT_EQ(GET_SQUARE_EXPLORED(30, WORLD_HEIGHT - 1, ARCANUS_PLANE), 0);
}

TEST_F(Set_Square_Explored_Bits_test, SetsMultipleBits)
{
    // Act
    Set_Square_Explored_Bits(ARCANUS_PLANE, 30, 20, SCT_TopLeft | SCT_TopRight);

    // Assert
    EXPECT_EQ(GET_SQUARE_EXPLORED(30, 20, ARCANUS_PLANE), (SCT_TopLeft | SCT_TopRight));
}

TEST_F(Set_Square_Explored_Bits_test, SetsAllFourCorners)
{
    // Act
    Set_Square_Explored_Bits(ARCANUS_PLANE, 30, 20, 
        SCT_TopLeft | SCT_TopRight | SCT_BottomLeft | SCT_BottomRight);

    // Assert
    EXPECT_EQ(GET_SQUARE_EXPLORED(30, 20, ARCANUS_PLANE), 
        (SCT_TopLeft | SCT_TopRight | SCT_BottomLeft | SCT_BottomRight));
}

TEST_F(Set_Square_Explored_Bits_test, WorksOnMyrrorPlane)
{
    // Act
    Set_Square_Explored_Bits(MYRROR_PLANE, 30, 20, SCT_BottomRight);

    // Assert
    EXPECT_EQ(GET_SQUARE_EXPLORED(30, 20, MYRROR_PLANE), SCT_BottomRight);
}

TEST_F(Set_Square_Explored_Bits_test, EdgeCoordinatesAtZero)
{
    // Act
    Set_Square_Explored_Bits(ARCANUS_PLANE, 0, 0, SCT_TopLeft);

    // Assert
    EXPECT_EQ(GET_SQUARE_EXPLORED(0, 0, ARCANUS_PLANE), SCT_TopLeft);
}

TEST_F(Set_Square_Explored_Bits_test, EdgeCoordinatesAtMax)
{
    // Act
    Set_Square_Explored_Bits(ARCANUS_PLANE, WORLD_WIDTH - 1, WORLD_HEIGHT - 1, SCT_BottomRight);

    // Assert
    EXPECT_EQ(GET_SQUARE_EXPLORED(WORLD_WIDTH - 1, WORLD_HEIGHT - 1, ARCANUS_PLANE), SCT_BottomRight);
}

class Draw_Building_The_Worlds_test : public ::testing::Test
{
protected:
    // Note: This function interacts with graphics, so we can only test it doesn't crash
};

TEST_F(Draw_Building_The_Worlds_test, ZeroPercentDoesNotCrash)
{
    // Act/Assert - should not crash
    EXPECT_NO_THROW(Draw_Building_The_Worlds(0));
}

TEST_F(Draw_Building_The_Worlds_test, HalfwayDoesNotCrash)
{
    // Act/Assert
    EXPECT_NO_THROW(Draw_Building_The_Worlds(50));
}

TEST_F(Draw_Building_The_Worlds_test, CompleteDoesNotCrash)
{
    // Act/Assert
    EXPECT_NO_THROW(Draw_Building_The_Worlds(100));
}

TEST_F(Draw_Building_The_Worlds_test, OverHundredDoesNotCrash)
{
    // Act/Assert
    EXPECT_NO_THROW(Draw_Building_The_Worlds(150));
}

class Get_Useable_City_Area_NewGame_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _world_maps = (uint8_t *)Allocate_Space(602);
        memset(_world_maps, tt_Ocean1, WORLD_SIZE * NUM_PLANES * sizeof(uint16_t));
    }

    void TearDown() override
    {
        free(_world_maps);
        _world_maps = NULL;
    }
};

TEST_F(Get_Useable_City_Area_NewGame_test, CenterPosition_Returns21Squares)
{
    // Arrange
    int16_t wx_array[25];
    int16_t wy_array[25];

    // Act
    int16_t count = Get_Useable_City_Area_NewGame(30, 20, ARCANUS_PLANE, wx_array, wy_array);

    // Assert
    EXPECT_EQ(count, 21) << "Center position should have full 21-square diamond";
}

TEST_F(Get_Useable_City_Area_NewGame_test, TopEdge_ReturnsReducedArea)
{
    // Arrange
    int16_t wx_array[25];
    int16_t wy_array[25];

    // Act
    int16_t count = Get_Useable_City_Area_NewGame(30, 0, ARCANUS_PLANE, wx_array, wy_array);

    // Assert
    EXPECT_LT(count, 21) << "Top edge should have fewer squares due to Y boundary";
    EXPECT_GT(count, 0) << "Should still have some squares";
}

TEST_F(Get_Useable_City_Area_NewGame_test, BottomEdge_ReturnsReducedArea)
{
    // Arrange
    int16_t wx_array[25];
    int16_t wy_array[25];

    // Act
    int16_t count = Get_Useable_City_Area_NewGame(30, WORLD_HEIGHT - 1, ARCANUS_PLANE, wx_array, wy_array);

    // Assert
    EXPECT_LT(count, 21) << "Bottom edge should have fewer squares";
}

TEST_F(Get_Useable_City_Area_NewGame_test, LeftEdge_WrapsAround)
{
    // Arrange
    int16_t wx_array[25];
    int16_t wy_array[25];

    // Act
    int16_t count = Get_Useable_City_Area_NewGame(0, 20, ARCANUS_PLANE, wx_array, wy_array);

    // Assert
    EXPECT_EQ(count, 21) << "Left edge should wrap around and still have 21 squares";
}

TEST_F(Get_Useable_City_Area_NewGame_test, RightEdge_WrapsAround)
{
    // Arrange
    int16_t wx_array[25];
    int16_t wy_array[25];

    // Act
    int16_t count = Get_Useable_City_Area_NewGame(WORLD_WIDTH - 1, 20, ARCANUS_PLANE, wx_array, wy_array);

    // Assert
    EXPECT_EQ(count, 21) << "Right edge should wrap around and still have 21 squares";
}

TEST_F(Get_Useable_City_Area_NewGame_test, CoordinatesAreValid)
{
    // Arrange
    int16_t wx_array[25];
    int16_t wy_array[25];

    // Act
    int16_t count = Get_Useable_City_Area_NewGame(30, 20, ARCANUS_PLANE, wx_array, wy_array);

    // Assert
    for (int i = 0; i < count; i++)
    {
        EXPECT_GE(wx_array[i], 0) << "X coordinate " << i << " should be >= 0";
        EXPECT_LT(wx_array[i], WORLD_WIDTH) << "X coordinate " << i << " should be < WORLD_WIDTH";
        EXPECT_GE(wy_array[i], 0) << "Y coordinate " << i << " should be >= 0";
        EXPECT_LT(wy_array[i], WORLD_HEIGHT) << "Y coordinate " << i << " should be < WORLD_HEIGHT";
    }
}

TEST_F(Get_Useable_City_Area_NewGame_test, IncludesCenterSquare)
{
    // Arrange
    int16_t wx_array[25];
    int16_t wy_array[25];

    // Act
    int16_t count = Get_Useable_City_Area_NewGame(30, 20, ARCANUS_PLANE, wx_array, wy_array);

    // Assert - check that (30,20) is in the list
    bool found_center = false;
    for (int i = 0; i < count; i++)
    {
        if (wx_array[i] == 30 && wy_array[i] == 20)
        {
            found_center = true;
            break;
        }
    }
    EXPECT_TRUE(found_center) << "City area should include the center square";
}
