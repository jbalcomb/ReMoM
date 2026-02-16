#include <gtest/gtest.h>
#include "../../MoM/src/ArmyList.h"

TEST(ArmyList, Build_Army_List_symbol_exists)
{
    // Verify we can take the address of a core function (link-time sanity check)
    void *ptr = (void *)Build_Army_List;
    ASSERT_NE(ptr, nullptr);
}
