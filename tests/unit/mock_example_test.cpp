#include <gtest/gtest.h>
#include "../mocks/MockSDL.h"

TEST(MockSDL, InitAndTicks)
{
    EXPECT_EQ(SDL_Init(0), 0);
    EXPECT_EQ(SDL_GetTicks(), 0u);
}
