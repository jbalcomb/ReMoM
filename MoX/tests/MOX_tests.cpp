#include <gtest/gtest.h>

// Where'd this go?  #include "Util.h"

#include "../src/Util.h"



TEST(MOX_test, MOX_One_Plus_One)
{
    ASSERT_EQ((1 + 1), 2);
}

TEST(MOX_test, MOX_String_To_Upper)
{
    // char string[4] = { 's', 'u', 'p', '\0'};
    // String_To_Upper(string);
    std::string string = "sup";
    // TODO  Valgrind  ==415298== 4 bytes in 1 blocks are definitely lost in loss record 1 of 1
    char * buffer = new char[string.length() + 1];  // Create a modifiable buffer and copy the string
    stu_strcpy(buffer, string.c_str());
    String_To_Upper(buffer);
    ASSERT_STREQ("SUP", buffer);
}

TEST(MOX_test, MOX_String_To_Lower)
{
    // char string[4] = { 'S', 'U', 'P', '\0'};
    // String_To_Lower(string);
    std::string string = "SUP";
    // TODO  Valgrind  ==415298== 4 bytes in 1 blocks are definitely lost in loss record 1 of 1
    char * buffer = new char[string.length() + 1];  // Create a modifiable buffer and copy the string
    stu_strcpy(buffer, string.c_str());
    String_To_Lower(buffer);
    ASSERT_STREQ("sup", buffer);
}
