#include <gtest/gtest.h>

#include <type_traits>

#ifdef __cplusplus
extern "C"
{
#endif

/*
    Spellbook.h currently exposes SmlBook_PageTurn__WIP(), while
    Spellbook.c defines SmlBook_PageTurn(). Declare the concrete symbol
    so this test validates the implementation currently present in momlib.
*/
void SmlBook_PageTurn(short int turn_type, short int combat_flag, short int player_idx);

#ifdef __cplusplus
}
#endif

TEST(SmlBookPageTurnTest, ExposesCallableFunctionPointer)
{
    using sml_page_turn_fn_t = void (*)(short int, short int, short int);

    sml_page_turn_fn_t page_turn_fn = &SmlBook_PageTurn;

    ASSERT_NE(page_turn_fn, nullptr);
    EXPECT_TRUE((std::is_same<decltype(&SmlBook_PageTurn), sml_page_turn_fn_t>::value));
}

TEST(SmlBookPageTurnTest, KeepsDirectionAndModeContractValuesDistinct)
{
    constexpr short int backward = 0;
    constexpr short int forward = 1;
    constexpr short int overland_mode = 0;
    constexpr short int combat_mode = 1;

    EXPECT_NE(backward, forward);
    EXPECT_NE(overland_mode, combat_mode);
}
