#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../src/MAPGEN.h"
#ifdef __cplusplus
}
#endif

TEST(Init_New_Game_test, DISABLED_Smoke_DoesNotCrash)
{
    ASSERT_NO_FATAL_FAILURE(Init_New_Game());
}
