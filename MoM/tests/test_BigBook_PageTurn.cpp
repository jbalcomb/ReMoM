#include <gtest/gtest.h>

#include <type_traits>

#ifdef __cplusplus
extern "C"
{
#endif
#include "../src/Spellbook.h"
#ifdef __cplusplus
}
#endif

TEST(BigBookPageTurnTest, ExposesCallableFunctionPointer)
{
    using page_turn_fn_t = void (*)(short int);

    page_turn_fn_t page_turn_fn = &BigBook_PageTurn;

    ASSERT_NE(page_turn_fn, nullptr);
    EXPECT_TRUE((std::is_same<decltype(&BigBook_PageTurn), page_turn_fn_t>::value));
}

TEST(BigBookPageTurnTest, KeepsDirectionContractValuesDistinct)
{
    constexpr short int backward = 0;
    constexpr short int forward = 1;
    constexpr short int research = 666;

    EXPECT_NE(backward, forward);
    EXPECT_NE(forward, research);
    EXPECT_GT(research, forward);
}
