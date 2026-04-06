#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../src/NewGame.h"
#ifdef __cplusplus
}
#endif

#include <cstring> // Why is this included? For stu_strcat and stu_strcmp functions used in the tests, because C++ here rather than C.

/*
    String_List_Builer(int16_t * List_Size, int16_t Total, char * Dest)

    Builds a human-readable comma-separated list with " and " before the last item.
    - Increments *List_Size each call
    - If *List_Size == Total (last item) and *List_Size > 1: appends " and "
    - If *List_Size != Total and *List_Size > 1: appends ", "
    - If *List_Size == 1 (first item): appends nothing

    Usage pattern (from Draw_Special_Abilities_String):
        Retorts_Added = 0;
        for each retort:
            String_List_Builer(&Retorts_Added, retort_count, &Retorts_String[0]);
            stu_strcat(Retorts_String, retort_name);
*/


// Single item list: no separator should be appended
TEST(STR_ListSeparator_test, SingleItem_NoSeparator)
{
    char Dest[128] = "";
    int16_t List_Size = 0;
    int16_t Total = 1;

    // First (and only) item
    String_List_Builer(&List_Size, Total, Dest);
    stu_strcat(Dest, "Alchemy");

    EXPECT_EQ(List_Size, 1);
    EXPECT_STREQ(Dest, "Alchemy");
}


// Two item list: " and " between the two items
TEST(STR_ListSeparator_test, TwoItems_AndSeparator)
{
    char Dest[128] = "";
    int16_t List_Size = 0;
    int16_t Total = 2;

    // First item
    String_List_Builer(&List_Size, Total, Dest);
    stu_strcat(Dest, "Alchemy");

    EXPECT_EQ(List_Size, 1);
    EXPECT_STREQ(Dest, "Alchemy");

    // Second (last) item
    String_List_Builer(&List_Size, Total, Dest);
    stu_strcat(Dest, "Warlord");

    EXPECT_EQ(List_Size, 2);
    EXPECT_STREQ(Dest, "Alchemy and Warlord");
}


// Three item list: ", " after first, " and " before last
TEST(STR_ListSeparator_test, ThreeItems_CommaAndAnd)
{
    char Dest[128] = "";
    int16_t List_Size = 0;
    int16_t Total = 3;

    // First item
    String_List_Builer(&List_Size, Total, Dest);
    stu_strcat(Dest, "Alchemy");
    EXPECT_EQ(List_Size, 1);

    // Second item (not last)
    String_List_Builer(&List_Size, Total, Dest);
    stu_strcat(Dest, "Warlord");
    EXPECT_EQ(List_Size, 2);

    // Third (last) item
    String_List_Builer(&List_Size, Total, Dest);
    stu_strcat(Dest, "Myrran");
    EXPECT_EQ(List_Size, 3);

    EXPECT_STREQ(Dest, "Alchemy, Warlord and Myrran");
}


// Four item list: commas between non-last items, " and " before last
TEST(STR_ListSeparator_test, FourItems_MultipleCommasThenAnd)
{
    char Dest[128] = "";
    int16_t List_Size = 0;
    int16_t Total = 4;

    String_List_Builer(&List_Size, Total, Dest);
    stu_strcat(Dest, "Alchemy");

    String_List_Builer(&List_Size, Total, Dest);
    stu_strcat(Dest, "Warlord");

    String_List_Builer(&List_Size, Total, Dest);
    stu_strcat(Dest, "Myrran");

    String_List_Builer(&List_Size, Total, Dest);
    stu_strcat(Dest, "Channeler");

    EXPECT_EQ(List_Size, 4);
    EXPECT_STREQ(Dest, "Alchemy, Warlord, Myrran and Channeler");
}


// Verify List_Size is incremented correctly each call
TEST(STR_ListSeparator_test, ListSizeIncrementsCorrectly)
{
    char Dest[128] = "";
    int16_t List_Size = 0;
    int16_t Total = 5;

    for (int i = 0; i < 5; i++)
    {
        String_List_Builer(&List_Size, Total, Dest);
        stu_strcat(Dest, "X");
        EXPECT_EQ(List_Size, i + 1);
    }
}


// First item in any list: nothing appended to Dest
TEST(STR_ListSeparator_test, FirstItem_DestUnchanged)
{
    char Dest[128] = "";
    int16_t List_Size = 0;
    int16_t Total = 3;

    String_List_Builer(&List_Size, Total, Dest);

    // Dest should still be empty after the separator call
    // (no separator prepended before the first item)
    EXPECT_STREQ(Dest, "");
    EXPECT_EQ(List_Size, 1);
}


// Middle item (not first, not last): ", " appended
TEST(STR_ListSeparator_test, MiddleItem_CommaAppended)
{
    char Dest[128] = "";
    int16_t List_Size = 0;
    int16_t Total = 3;

    // First item
    String_List_Builer(&List_Size, Total, Dest);
    stu_strcat(Dest, "A");

    // Second item (middle)
    String_List_Builer(&List_Size, Total, Dest);
    // After separator, before appending item name, Dest should have ", "
    EXPECT_STREQ(Dest, "A, ");
}


// Last item (not first): " and " appended
TEST(STR_ListSeparator_test, LastItem_AndAppended)
{
    char Dest[128] = "";
    int16_t List_Size = 0;
    int16_t Total = 2;

    // First item
    String_List_Builer(&List_Size, Total, Dest);
    stu_strcat(Dest, "A");

    // Second (last) item
    String_List_Builer(&List_Size, Total, Dest);
    // After separator, before appending item name, Dest should have " and "
    EXPECT_STREQ(Dest, "A and ");
}