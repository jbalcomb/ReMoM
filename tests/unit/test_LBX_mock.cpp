#include <gtest/gtest.h>
#include "../mocks/MockLBX.h"

TEST(LBX_mock, LBX_Load_Data_Static_returns_success)
{
    int r = LBX_Load_Data_Static("names.lbx", NULL, 0);
    ASSERT_NE(r, 0);
}
