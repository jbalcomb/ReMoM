// Init_Summoning_Circle_And_Spell_Of_Mastery() (MGC o56p5): the Spell of Mastery research cost starts at the spell table's cost, less
// 3000 for a wizard with 11 books in ANY realm. The asm tests each realm with `cmp [spellranks+n], 11 / jz loc_52776` (the subtract)
// and skips it only after the fifth (`jnz loc_52785`): a logical OR. The reconstruction had &&, so no wizard ever got the 3000 off.

#include <gtest/gtest.h>

#include <cstdlib>
#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DAT.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../src/INITGAME.h"
#include "../src/Spellbook.h"
#ifdef __cplusplus
}
#endif

static const int16_t TEST_SOM_RESEARCH_COST = 20000;  // any value above 3000 that fits s_SPELL_DATA.research_cost (int16_t)

class Init_Summoning_Circle_And_Spell_Of_Mastery_test : public ::testing::Test
{
protected:
    struct s_SPELL_DATA * saved_spell_data_table = nullptr;
    struct s_FORTRESS * saved_fortresses = nullptr;
    int16_t saved_num_players = 0;
    struct s_WIZARD saved_players[NUM_PLAYERS];

    void SetUp() override
    {
        saved_spell_data_table = spell_data_table;
        saved_fortresses = _FORTRESSES;
        saved_num_players = _num_players;
        memcpy(saved_players, _players, sizeof(saved_players));

        spell_data_table = static_cast<struct s_SPELL_DATA *>(calloc(spl_Spell_Of_Mastery + 1, sizeof(struct s_SPELL_DATA)));
        _FORTRESSES = static_cast<struct s_FORTRESS *>(calloc(NUM_FORTRESSES, sizeof(struct s_FORTRESS)));
        ASSERT_NE(spell_data_table, nullptr);
        ASSERT_NE(_FORTRESSES, nullptr);
        spell_data_table[spl_Spell_Of_Mastery].research_cost = TEST_SOM_RESEARCH_COST;
        memset(_players, 0, sizeof(saved_players));
        _num_players = 1;
    }

    void TearDown() override
    {
        free(spell_data_table);
        free(_FORTRESSES);
        spell_data_table = saved_spell_data_table;
        _FORTRESSES = saved_fortresses;
        _num_players = saved_num_players;
        memcpy(_players, saved_players, sizeof(saved_players));
    }
};

TEST_F(Init_Summoning_Circle_And_Spell_Of_Mastery_test, NoRealmAtElevenKeepsFullCost)
{
    int16_t ranks[5] = { 10, 0, 3, 1, 0 };
    memcpy(_players[0].spellranks, ranks, sizeof(ranks));
    Init_Summoning_Circle_And_Spell_Of_Mastery();
    EXPECT_EQ(_players[0].som_research_cost, (uint16_t)TEST_SOM_RESEARCH_COST);
}

TEST_F(Init_Summoning_Circle_And_Spell_Of_Mastery_test, ElevenInAnyOneRealmTakes3000Off)
{
    for(int realm = 0; realm < 5; realm++)
    {
        memset(_players[0].spellranks, 0, sizeof(_players[0].spellranks));
        _players[0].spellranks[realm] = 11;
        _players[0].som_research_cost = 0;
        Init_Summoning_Circle_And_Spell_Of_Mastery();
        EXPECT_EQ(_players[0].som_research_cost, (uint16_t)(TEST_SOM_RESEARCH_COST - 3000)) << "11 books in realm " << realm;
    }
}

TEST_F(Init_Summoning_Circle_And_Spell_Of_Mastery_test, ElevenInEveryRealmTakes3000OffOnce)
{
    int16_t ranks[5] = { 11, 11, 11, 11, 11 };
    memcpy(_players[0].spellranks, ranks, sizeof(ranks));
    Init_Summoning_Circle_And_Spell_Of_Mastery();
    EXPECT_EQ(_players[0].som_research_cost, (uint16_t)(TEST_SOM_RESEARCH_COST - 3000));
}

TEST_F(Init_Summoning_Circle_And_Spell_Of_Mastery_test, SummonCircleIsTheFortress)
{
    _FORTRESSES[0].wx = 17;
    _FORTRESSES[0].wy = 23;
    _FORTRESSES[0].wp = 1;
    Init_Summoning_Circle_And_Spell_Of_Mastery();
    EXPECT_EQ(_players[0].summon_wx, 17);
    EXPECT_EQ(_players[0].summon_wy, 23);
    EXPECT_EQ(_players[0].summon_wp, 1);
}
