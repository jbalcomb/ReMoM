#include <gtest/gtest.h>
#include "Util.h"

TEST(MOX_test, MOX_One_Plus_One)
{
    ASSERT_EQ(1+1, 2);
}

TEST(MOX_test, MOX_String_To_Upper)
{
    char string[4] = { 's', 'u', 'p', '\0'};
    String_To_Upper(string);
    ASSERT_STREQ("SUP", string);
}

TEST(MOX_test, MOX_String_To_Lower)
{
    char string[4] = { 'S', 'U', 'P', '\0'};
    String_To_Lower(string);
    ASSERT_STREQ("sup", string);
}
