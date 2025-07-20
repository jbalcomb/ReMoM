
/*
https://cfd.university/learn/automating-cfd-solver-and-library-compilation-using-cmake/how-to-automate-testing-with-cmake-ctest-and-gtest/
All tests shown below follow the AAA (arrange, act, assert) principle.
This helps us to organise our tests into a predictiable format and makes them easier to read.
I have written about that as well in the gtest article, where you can find a more detailed description of this as well.
https://cfd.university/learn/the-complete-guide-to-software-testing-for-cfd-applications/the-test-driven-development-for-writing-bug-free-cfd-code/#aioseo-writing-the-test
*/

#include "gtest/gtest.h"

#include "../../src/MainScr_Maps.h"



TEST(Learning_Test, Test_Center_Map)
{
    // Arrange
    // ComplexNumber a(3.0, 4.0);
    int16_t _map_wx, _map_wy, _prev_wx, _prev_wy, _map_wp;

    // Act
    // auto magnitude = a.magnitude();

    // Main_Screen()
    //     if(abs(input_field_idx) == _minimap_grid_field)
    //         _prev_world_x = reduced_map_window_wx + _minimap_grid_x;  // ...is the 'wx' of the clicked square
    //         _prev_world_y = reduced_map_window_wy + _minimap_grid_y;  // ...is the 'wy' of the clicked square
    //         _map_x = _prev_world_x;
    //         _map_y = _prev_world_y;
    //         Center_Map(&_map_x, &_map_y, _prev_world_x, _prev_world_y, _map_plane);
    Center_Map(&_map_wx, &_map_wy, _prev_wx, _prev_wy, _map_wp);

    // Assert
    ASSERT_EQ(_map_wx, 5);
    ASSERT_EQ(_map_wy, 5);
}

int main(int argc, char **argv)
{

    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();

}
