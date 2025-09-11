#include <gtest/gtest.h>

// Where'd this go?  #include "Util.h"

#include "../src/Util.h"



TEST(MOX_test, MOX_Ridiculous)
{
    Ridiculous();
}

TEST(MOX_test, MOX_One_Plus_One)
{
    ASSERT_EQ(1+1, 2);
}

TEST(MOX_test, MOX_String_To_Upper)
{
    // char string[4] = { 's', 'u', 'p', '\0'};
    // String_To_Upper(string);
    std::string string = "sup";
    char * buffer = new char[string.length() + 1];  // Create a modifiable buffer and copy the string
    strcpy(buffer, string.c_str());
    String_To_Upper(buffer);
    ASSERT_STREQ("SUP", buffer);
}

TEST(MOX_test, MOX_String_To_Lower)
{
    // char string[4] = { 'S', 'U', 'P', '\0'};
    // String_To_Lower(string);
    std::string string = "SUP";
    char * buffer = new char[string.length() + 1];  // Create a modifiable buffer and copy the string
    strcpy(buffer, string.c_str());
    String_To_Lower(buffer);
    ASSERT_STREQ("sup", buffer);
}
