/*
    WIZARDS.EXE
        ovr117
        ovr118
*/

#include "../../STU/src/STU_DBG.h"

#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/GENDRAW.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_DAT.h"  /* _screen_seg */
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_SET.h"  /* magic_set */
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/SOUND.h"
#include "../../MoX/src/Timer.h"
#include "../../MoX/src/paragrph.h"

#include "CITYCALC.h"
#include "CMBMAGIC.h"
#include "Combat.h"
#include "../../MoX/src/Help.h"
#include "MainScr.h"   /* Allocate_Reduced_Map(); Play_Background_Music(); */
#include "NEXTTURN.h"
#include "SBookScr.h"  /* Spellbook_Screen_Draw(); */
#include "SPLMASTR.h"
#include "UNITTYPE.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../../ext/stu_compat.h"

#include "Spellbook.h"
#include "../../STU/src/STU_LOG.h"

// Combat.C
// WZD dseg:C520
extern int16_t _combat_structure;



// WZD dseg:5D1C                                                 BEGIN:  ovr117 - Strings

// WZD dseg:5D1C
char str_empty_string__ovr117[] = "";

// WZD dseg:5D1D
char cnst_Rsrch_Page1[] = "Research";

// WZD dseg:5D26
char cnst_Rsrch_Page2[] = "Spells";

// WZD dseg:5D2D
char cnst_Special_Cost[] = "Special";

// WZD dseg:5D35
char cnst_Instant_Cast[] = "Instant";

// WZD dseg:5D3D
char cnst_QuestionMark5[] = "?";

// WZD dseg:5D3F
char cnst_Space_MP_2[] = " MP";

// WZD dseg:5D43
char cnst_Item[] = "Item";

// WZD dseg:5D48
char book_lbx_file__ovr117[] = "book";

// WZD dseg:5D4D
char cnst_SpellGroup1[] = "Summoning";
// WZD dseg:5D57
char cnst_SpellGroup2[] = "Special Spells";
// WZD dseg:5D66
char cnst_SpellGroup3[] = "City Spells";
// WZD dseg:5D72
char cnst_SpellGroup4[] = "Enchantment";
// WZD dseg:5D7E
char cnst_SpellGroup5[] = "Unit Spells";
// WZD dseg:5D8A
char cnst_SpellGroup6[] = "Combat Spells";

// WZD dseg:5D98
char cnst_SpellRealm1[] = "Arcane";
// WZD dseg:5D9F
char cnst_SpellRealm2[] = "Chaos";
// WZD dseg:5DA5
char cnst_SpellRealm3[] = "Death";
// WZD dseg:5DAB
char cnst_SpellRealm4[] = "Life";
// WZD dseg:5DB0
char cnst_SpellRealm5[] = "Nature";
// WZD dseg:5DB7
char cnst_SpellRealm6[] = "Sorcery";

// WZD dseg:5DBF 00                                              align 2

// WZD dseg:5DBF                                                 END:  ovr117 - Strings



// WZD dseg:5DC0                                                 BEGIN:  ovr118 - Initialized Data

// WZD dseg:5DC0
// WZD dseg:5DCC
struct s_SPELL_DECODE g_spell_decode_x_r = {  53,  45,  37,  16,   6,   6 };
struct s_SPELL_DECODE g_spell_decode_x_l = {  64,  71,  79, 101, 111, 111 };

// WZD dseg:5DD8
char book_lbx_file__ovr118[] = "book";

// WZD dseg:5DDC
char str_empty_string__ovr118[] = "";

// WZD dseg:5DDD
char scroll_lbx_file__ovr118__1[] = "scroll";

// WZD dseg:5DE4
char cnst_ChooseResearch[] = "Choose a new spell to research";

// WZD dseg:5E03
char wizlab_lbx_file__ovr118[] = "Wizlab";

// WZD dseg:5E0A
char music_lbx_file__ovr118[] = "music";

// WZD dseg:5E10
char scroll_lbx_file__ovr118__2[] = "Scroll";

// WZD dseg:5E17
char specfx_lbx_file__ovr118[] = "SPECFX";

// WZD dseg:5E1E
char cnst_SP_Cost[] = "SP Cost :";

// WZD dseg:5E28
char spellscr_lbx_file__ovr118[] = "Spellscr";

// WZD dseg:5E31
char cnst_NewSpell_Msg[] = "You have mastered the spell of ";

// WZD dseg:5E51
char desc_lbx_file__ovr118[] = "Desc";

// WZD dseg:5E56
char hlpentry_lbx_file__ovr118[] = "hlpentry";

// WZD dseg:5E5F
char str_hotkey_ESC__ovr118[] = "\x1B";

// WZD dseg:5E61
char cnst_Catsing_Cost[] = "Casting Cost:";

// WZD dseg:5E6F
char cnst_SpaceOpenBrace4[] = " (";

// WZD dseg:5E72
char cnst_RT1turn[] = " turn)";

// WZD dseg:5E79
char cnst_RTmoreturns[] = " turns)";

// WZD dseg:5E81
char cnst_Research_Cost[] = "Research Cost:";

// WZD dseg:5E90 00 00                                           dw 0

// WZD dseg:5E90                                                 END:  ovr118 - Initialized Data



// WZD dseg:9448                                                 ¿ BEGIN: Spellbook (!Screen) - Uninitialized Data ?
// WZD dseg:9448                                                 BEGIN:  ovr118

// WZD dseg:9448
int16_t g_first_research_page;

// WZD dseg:944A
int16_t g_spellbook_last_left_page;

// WZD dseg:944C
int16_t g_combat_spellbook_left_page;

// WZD dseg:944E 00 00                                           dw 0
// WZD dseg:9450 00 00                                           dw 0

// WZD dseg:9452
SAMB_ptr _spellbook_small_text;

// WZD dseg:9454
// _spellbook_small_symbols SPBK_ICONS <0>
/*
00000000 struc SPBK_ICONS ; (sizeof=0xC, standard type)
00000000 Nature@ dw ?
00000002 Sorcery@ dw ?
00000004 Chaos@ dw ?
00000006 Life@ dw ?
00000008 Death@ dw ?
0000000A Arcane@ dw ?
0000000C ends SPBK_ICONS
*/
SAMB_ptr _spellbook_small_symbols[NUM_MAGIC_REALMS];

// WZD dseg:9460
SAMB_ptr _spellbook_big_seg;

// WZD dseg:9462 00 00                                           dw 0

// WZD dseg:9464
SAMB_ptr _spellbook_big_right_corner_seg;
// WZD dseg:9466
SAMB_ptr _spellbook_big_left_corner_seg;

// WZD dseg:9468
SAMB_ptr _spellbook_small_right_corner_seg;

// WZD dseg:946A
SAMB_ptr _spellbook_small_left_corner_seg;

// WZD dseg:946C
SAMB_ptr cityspel_seg;

// WZD dseg:946E
SAMB_ptr _spellbook_small_seg;

// WZD dseg:946E                                                 END:  ovr118



// WZD dseg:C8FA                                                 ¿ BEGIN: Spellbook (!Screen) - Uninitialized Data ?
// WZD dseg:C8FA                                                 BEGIN:  ovr117

// WZD dseg:C8FA
struct s_SPELL_BOOK_PAGE * m_spellbook_pages;

// WZD dseg:C8FC
/*

¿ also used for the magic dripping mouse cursor image ?
*/
int16_t g_spellbook_anim_stage;

// WZD dseg:C8FE
// drake178: IMG_SBK_BookRealms@ SPBK_ICONS
SAMB_ptr spellbook_symbols_bitm[NUM_MAGIC_REALMS];

// WZD dseg:C90A
// drake178:  g_spellbook_cast_mana_limit
/*
; the predicted amount of mana that the wizard will
; be able to channel into a spell each turn?
; (lower of mana+manaperturn and skill+heroskill)
; in combat, the highest castable cost
*/
/*
used to set turns_left for casting
counterpart to g_research_income_by_realm[] for researching
*/
int16_t g_spellbook_cast_mana_limit;

// WZD dseg:C90C
// drake178: NearAlloc_First!
/*

Build_Spellbook()
    for(itr1 = 0; itr1 < NUM_MAGIC_REALMS; itr1++)
    {
        research_bonus_percentage = Player_Spell_Research_Bonus(HUMAN_PLAYER_IDX, ((itr1 * 40) + 1));
        g_research_income_by_realm[itr1] = ((research_points * research_bonus_percentage) / 100);
    }

*/
SAMB_ptr g_research_income_by_realm;

// WZD dseg:C90E
/*
Build_Spellbook()
WIZ_ManaPerTurn = (((_players[HUMAN_PLAYER_IDX].mana_ratio * _players[HUMAN_PLAYER_IDX].Power_Base) / 100) - Player_Armies_And_Enchantments_Mana_Upkeep(HUMAN_PLAYER_IDX));
just used for the calculation for g_spellbook_cast_mana_limit
no reason this is module scoped?
*/
int16_t WIZ_ManaPerTurn;

// WZD dseg:C910
// drake178: 0Dh LBX_Alloc_Space paragraphs (first or next); NOT BIG ENOUGH TO HOLD MORE THAN 104 SPELLS!
int16_t * m_spellbook_spell_list;

// WZD dseg:C914
/*
    ¿ count of records in m_spellbook_pages ?
*/
int16_t m_spellbook_page_count;

/*
    spells...divided into categories
*/
// WZD dseg:C916
// drake178: Combat Spells or Sorcery
int16_t SBK_Group_6_Count;
// WZD dseg:C918
// drake178: Enchantments or Life
int16_t SBK_Group_5_Count;
// WZD dseg:C91A
// drake178: Unit Spells or Nature
int16_t SBK_Group_4_Count;
// WZD dseg:C91C
// drake178: City Spells or Death
int16_t SBK_Group_3_Count;
// WZD dseg:C91E
// drake178: Special Spells or Chaos
int16_t SBK_Group_2_Count;
// WZD dseg:C920
// drake178: Summoning Spells or Arcane
int16_t SBK_Group_1_Count;

// WZD dseg:C922 00 00                                           
// drake178: apprentice: 0 - choose research; 2 - apprentice book
// drake178: new spell anim: 0 - spell on left; 1 - spell on right
int16_t g_spellbook_mode;

// WZD dseg:C924
// drake178: ; set to the index + 1 (into the page) of the spell
// drake178: ; being cast while setting up the spellbook display
// drake178: ; in the apprentice, it is then reset to the page count
// drake178: ; without the research candidates
int16_t g_spellbook_left_page;

// WZD dseg:C926
// drake178:  Spell List Count, New Spell Index, ...
// ¿ MoO2  Module: MOX  _temp_sint... ?
/*

    count of spell_idx in m_spellbook_spell_list[]
        Spellbook_Group_Counts()
    set in Build_Spell_List()

Learn_Spell_Animation()
    used as spellbook page spell index of spell being learned, as passed in

AI_Kill_Lame_Units()
    ¿ count of *less-than_average* units removed ?

Combat_City_Capture()
    GUI_Multipurpose_Int is used here as a change accumulator for the summary scroll
*/
int16_t GUI_Multipurpose_Int;  /* NOTE(JimBalcomb,20240922): presently, now, only used in Combat.C */
/*

count for research_list[] coming out of Build_Research_List()
*/
int16_t m_spell_list_count;  // DNE in Dasm;  uses GUI_Multipurpose_Int

// WZD dseg:C928
/*
¿ 2 pages, 8 per page ?
¿ sometimes used as 4 x 4 ?
*/
char * g_spellbook_descriptions[NUM_SPELLBOOK_DESCRIPTIONS];

// WZD dseg:C968
SAMB_ptr wizlab_wizard_seg;
// WZD dseg:C96A
SAMB_ptr wizlab_familiar_seg;
// WZD dseg:C96C
SAMB_ptr wizlab_blue_column_seg;
// WZD dseg:C96E
SAMB_ptr wizlab_background_seg;
#define wizlab_podium_seg   wizlab_background_seg  // DNE in Dasm

// WZD dseg:C970 00                                              db    0
// WZD dseg:C971 00                                              db    0

// WZD dseg:C971                                                 END:  ovr118



/*
    WIZARDS.EXE  ovr117
*/

// WZD o117p01
void Build_Spell_List(int16_t type, int16_t spell_list[])
{
    int16_t spell_idx = 0;
    int16_t itr_realms = 0;
    int16_t itr_spells = 0;
    int16_t spell_status = 0;
    int16_t spells_list_status = 0;  // DNE in Dasm
    for(itr_realms = 0; itr_realms < NUM_MAGIC_REALMS; itr_realms++)
    {
        for(itr_spells = 0; itr_spells < NUM_SPELLS_PER_MAGIC_REALM; itr_spells++)
        {
            spell_status = 0;
            spell_idx = ((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + (itr_spells + 1));
            switch(type)
            {
                case slt_Overland:  /* Overland */
                {
                    if(
                        (_players[HUMAN_PLAYER_IDX].spells_list[((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr_spells)] == sls_Known)
                        &&
                        (spell_data_table[spell_idx].Eligibility > -1)
                        &&
                        (spell_data_table[spell_idx].Eligibility != 5)
                    )
                    {
                        spell_status = 1;
                    }
                } break;
                case slt_Combat:  /* Combat */
                {
                    if(
                        (_players[HUMAN_PLAYER_IDX].spells_list[((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr_spells)] == sls_Known)
                        &&
                        (spell_data_table[spell_idx].Eligibility != 1)
                        &&
                        (spell_data_table[spell_idx].Eligibility != 5)
                    )
                    {
                        spell_status = 1;
                    }
                } break;
                case slt_Library:  /* Apprentice */
                {
                    if(_players[HUMAN_PLAYER_IDX].spells_list[((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr_spells)] > sls_Unknown)
                    {
                        if(_players[HUMAN_PLAYER_IDX].spells_list[((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr_spells)] != sls_Known)
                        {
                            spell_status = 2;  /* ¿ researchable ? */
                        }
                        else
                        {
                            spell_status = 1;  /* ¿ castable ? */
                        }
                    }
                } break;
            }
            if(spell_status > 0)  /* ¿ knowable or known ? */
            {
                if(spell_status != 1)  /* ¿ known ? */
                {
                    spell_list[m_spell_list_count] = -(spell_idx);  /* ¿ researchable ? */
                }
                else
                {
                    spell_list[m_spell_list_count] = spell_idx;  /* ¿ castable ? */
                }
                m_spell_list_count++;
            }
        }
    }
}


// WZD o117p02
/*
Combat Spellbook Add Spells From Magic Realm
OON XREF
    NX_j_SBK_AddRealm()
    Combat_Spellbook_Build()
*/
void Combat_Spellbook_Add_Spells_From_Magic_Realm(int16_t * spell_list, int16_t magic_realm)
{
    int16_t zz_flag = 0;
    int16_t realm_idx = 0;
    int16_t itr_spell_per_magic_realm = 0;
    int16_t spell_idx = 0;
    realm_idx = magic_realm;
    for(itr_spell_per_magic_realm = 0; itr_spell_per_magic_realm < NUM_SPELLS_PER_MAGIC_REALM; itr_spell_per_magic_realm++)
    {
        zz_flag = 0;
        spell_idx = ((realm_idx * NUM_SPELLS_PER_MAGIC_REALM) + itr_spell_per_magic_realm + 1);
        if(spell_data_table[spell_idx].Eligibility != 1)  // ¿ knowable ?
        {
            spell_list[m_spell_list_count] = spell_idx;
            m_spell_list_count++;
        }
    }
}


// WZD o117p03
void Spellbook_Group_Counts(void)
{
    int16_t itr = 0;
    if(magic_set.spell_book_ordering == ST_TRUE)
    {
        for(itr = 0; itr < m_spell_list_count; itr++)
        {
            switch(spell_data_table[abs(m_spellbook_spell_list[itr])].spell_book_category)
            {
                case 0:
                {
                    SBK_Group_2_Count++;  // "Special Spells"
                } break;
                case 1:
                {
                    SBK_Group_1_Count++;  // "Summoning"
                } break;
                case 2:
                {
                    SBK_Group_5_Count++;  // "Enchantment"
                } break;
                case 3:
                {
                    SBK_Group_3_Count++;  // "City Spells"
                } break;
                case 4:
                {
                    SBK_Group_4_Count++;  // "Unit Spells"
                } break;
                case 5:
                {
                    SBK_Group_6_Count++;  // "Combat Spells"
                } break;
            }
        }
    }
    else
    {
        for(itr = 0; itr < m_spell_list_count; itr++)
        {
            switch(spell_data_table[abs(m_spellbook_spell_list[itr])].magic_realm)
            {
                case sbr_Nature:
                {
                    SBK_Group_4_Count++;  // "Nature"
                } break;
                case sbr_Sorcery:
                {
                    SBK_Group_6_Count++;  // "Sorcery"
                } break;
                case sbr_Chaos:
                {
                    SBK_Group_2_Count++;  // "Chaos"
                } break;
                case sbr_Life:
                {
                    SBK_Group_5_Count++;  // "Life"
                } break;
                case sbr_Death:
                {
                    SBK_Group_3_Count++;  // "Death"
                } break;
                case sbr_Arcane:
                {
                    SBK_Group_1_Count++;  // "Arcane"
                } break;
            }
        }
    }
}


// WZD o117p04
/*
    spell_list_type   {0,1,2}
    page_spell_count   {4,6,8}
spell_list_type only used in call to Build_Spell_List()
(which would seem to mean it is not actually "book type")
*/
/**
 * @brief Build the spellbook page list and related casting and research data for the human player.
 *
 * Initializes the temporary spell list, computes per-realm research incomes, derives the current
 * casting mana limit for the book display, counts spell groups, allocates the page table, and fills
 * the generated pages. When building the big spellbook, this routine also appends the two research
 * candidate pages after the grouped spell pages.
 *
 * @param spell_list_type Selects which spells are gathered into the spell list.
 * Use the spell list type values consumed by Build_Spell_List(), such as overland, combat, or
 * apprentice/library mode.
 * @param page_spell_count Number of spell entries to place on each generated page.
 * Typical values are the small-book and big-book page capacities.
 *
 * @note This routine updates module-scoped spellbook state including m_spellbook_spell_list,
 * m_spellbook_pages, m_spell_list_count, m_spellbook_page_count, g_research_income_by_realm, and
 * g_spellbook_cast_mana_limit.
 * @note The build always targets the human player state and uses HUMAN_PLAYER_IDX throughout.
 */
void Build_Spellbook(int16_t spell_list_type, int16_t page_spell_count)
{
    int16_t hero_casting_skill = 0;
    int16_t itr2 = 0;
    int16_t total_pages = 0;
    int16_t bogus_placeholder = 0;  // DNE in Dasm;  uses total_pages
    int16_t research_bonus_percentage;  // DNE in Dasm;  uses total_pages
    int16_t research_points = 0;
    int16_t itr1 = 0;
    int16_t itr_magic_realms = 0;
    m_spellbook_spell_list = (int16_t *)Allocate_First_Block(_screen_seg, 13);  // 13 PR, 208 B
    g_research_income_by_realm = Near_Allocate_First(12);
    // spells...divided into categories
    SBK_Group_3_Count = 0;      // City Spells or Death
    SBK_Group_2_Count = 0;      // Special Spells or Chaos
    SBK_Group_1_Count = 0;      // Summoning Spells or Arcane
    SBK_Group_5_Count = 0;      // Enchantments or Life
    SBK_Group_4_Count = 0;      // Unit Spells or Nature
    SBK_Group_6_Count = 0;      // Combat Spells or Sorcery
    m_spell_list_count = 0;
    m_spellbook_page_count = 0;
    Players_Update_Magic_Power();
    Player_Magic_Power_Distribution(&bogus_placeholder, &bogus_placeholder, &research_points, HUMAN_PLAYER_IDX);
    for(itr_magic_realms = 0; itr_magic_realms < NUM_MAGIC_REALMS; itr_magic_realms++)
    {
        research_bonus_percentage = Player_Spell_Research_Bonus(HUMAN_PLAYER_IDX, ((itr_magic_realms * NUM_SPELLS_PER_MAGIC_REALM) + 1));  // {1, 41, 81, 121, 161, 201}
        g_research_income_by_realm[itr_magic_realms] = research_points + ((research_points * research_bonus_percentage) / 100);
    }
    _players[HUMAN_PLAYER_IDX].Nominal_Skill = Player_Base_Casting_Skill(HUMAN_PLAYER_IDX);
    WIZ_ManaPerTurn = (((_players[HUMAN_PLAYER_IDX].mana_ratio * _players[HUMAN_PLAYER_IDX].Power_Base) / 100) - Player_Armies_And_Enchantments_Mana_Upkeep(HUMAN_PLAYER_IDX));
    hero_casting_skill = Player_Hero_Casting_Skill(HUMAN_PLAYER_IDX);
    // set g_spellbook_cast_mana_limit to min
    if((WIZ_ManaPerTurn + _players[HUMAN_PLAYER_IDX].mana_reserve) > (_players[HUMAN_PLAYER_IDX].Nominal_Skill + hero_casting_skill))
    {
        g_spellbook_cast_mana_limit = (_players[HUMAN_PLAYER_IDX].Nominal_Skill + hero_casting_skill);
    }
    else if((WIZ_ManaPerTurn + _players[HUMAN_PLAYER_IDX].mana_reserve) > 0)
    {
        g_spellbook_cast_mana_limit = (WIZ_ManaPerTurn + _players[HUMAN_PLAYER_IDX].mana_reserve);
    }
    else
    {
        g_spellbook_cast_mana_limit = 0;
    }
    Build_Spell_List(spell_list_type, m_spellbook_spell_list);
    Spellbook_Group_Counts();
    total_pages =  ((SBK_Group_3_Count + page_spell_count - 1) / page_spell_count);
    total_pages += ((SBK_Group_2_Count + page_spell_count - 1) / page_spell_count);
    total_pages += ((SBK_Group_1_Count + page_spell_count - 1) / page_spell_count);
    total_pages += ((SBK_Group_5_Count + page_spell_count - 1) / page_spell_count);
    total_pages += ((SBK_Group_4_Count + page_spell_count - 1) / page_spell_count);
    total_pages += ((SBK_Group_6_Count + page_spell_count - 1) / page_spell_count);
    m_spellbook_pages = (struct s_SPELL_BOOK_PAGE *)Near_Allocate_Next(((size_t)(total_pages + 3) * sizeof(struct s_SPELL_BOOK_PAGE)));
    for(itr1 = 0; (total_pages + 3) > itr1; itr1++)
    {
        m_spellbook_pages[itr1].count = 0;
        stu_strcpy(m_spellbook_pages[itr1].title, "");
        for(itr2 = 0; itr2 < page_spell_count; itr2++)
        {
            m_spellbook_pages[itr1].spell[itr2] = 0;  /* ¿ NO_SPELL ? */
        }
    }
    Spellbook_Add_Group_Pages(page_spell_count);
    /*
        BEGIN: "Research Spells"
    */
    if(page_spell_count == NUM_SPELLS_PER_PAGE_BIG)  /* means we're building the "big book" */
    {
        g_first_research_page = m_spellbook_page_count;
        itr2 = 0;
        for(itr1 = 0; itr1 < NUM_SPELLS_PER_PAGE_BIG; itr1++)
        {
            if(_players[HUMAN_PLAYER_IDX].research_spells[(0 + itr1)] > 0)
            {
                m_spellbook_spell_list[(itr1 - itr2)] = _players[HUMAN_PLAYER_IDX].research_spells[(0 + itr1)];
            }
            else
            {
                itr2++;
            }
        }
        Spellbook_Add_Page((4 - itr2), ST_UNDEFINED, cnst_Rsrch_Page1, page_spell_count);
        itr2 = 0;
        for(itr1 = 0; itr1 < NUM_SPELLS_PER_PAGE_BIG; itr1++)
        {
            if(_players[HUMAN_PLAYER_IDX].research_spells[(4 + itr1)] > 0)
            {
                m_spellbook_spell_list[(itr1 - itr2)] = _players[HUMAN_PLAYER_IDX].research_spells[(4 + itr1)];
            }
            else
            {
                itr2++;
            }
        }
        Spellbook_Add_Page((4 - itr2), ST_UNDEFINED, cnst_Rsrch_Page2, page_spell_count);
    }
    /*
        END: "Research Spells"
    */
}


// WZD o117p05
// drake178: sub_9A9E8()
// ¿ ~ MoO2  Module: COLCALC  Player_N_Turns_Until_Research_Complete_() ?
int16_t UU_IDK_turn_to_cast__STUB(int16_t player_idx)
{

    return 999;

}


// WZD o117p06
int16_t Combat_Spellbook_Build(int16_t caster_idx)
{
    int16_t item_embed_spell_idx = 0;
    int16_t already_in_list = 0;
    int16_t itr_spellbook_spell_list = 0;
    int16_t spellbook_has_castable_spell = 0;
    int16_t index_on_page = 0;
    int16_t itr1 = 0;
    int16_t unit_type = 0;
    int16_t total_pages = 0;
    Near_Allocate_Mark();
    Mark_Block(_screen_seg);
    m_spellbook_spell_list = (int16_t *)Allocate_Next_Block(_screen_seg, 13);
    SBK_Group_3_Count = 0;  // ; City Spells or Death
    SBK_Group_2_Count = 0;  // ; Special Spells or Chaos
    SBK_Group_1_Count = 0;  // ; Summoning Spells or Arcane
    SBK_Group_5_Count = 0;  // ; Enchantments or Life
    SBK_Group_4_Count = 0;  // ; Unit Spells or Nature
    SBK_Group_6_Count = 0;  // ; Combat Spells or Sorcery
    m_spell_list_count = 0;
    m_spellbook_page_count = 0;
    if(caster_idx >= CASTER_IDX_BASE)
    {
        unit_type = Combat_Casting_Cost_Multiplier(HUMAN_PLAYER_IDX);
        g_spellbook_cast_mana_limit = _players[HUMAN_PLAYER_IDX].Cmbt_Skill_Left;
        /* OGBUG: rounds the wrong way ; BUG: x0.5 multiplier results in a negative signed ; value if the player has over 16,383 mana */
        if(((_players[HUMAN_PLAYER_IDX].mana_reserve * 10) / unit_type) < g_spellbook_cast_mana_limit)
        {
            g_spellbook_cast_mana_limit = ((_players[HUMAN_PLAYER_IDX].mana_reserve * 10) / unit_type);
        }
    }
    else
    {
        g_spellbook_cast_mana_limit = battle_units[caster_idx].mana;
    }
    if(caster_idx < CASTER_IDX_BASE)
    {
        if((battle_units[caster_idx].Attribs_1 & (USA_CASTER_20 | USA_CASTER_40)) != 0)
        {
            Combat_Spellbook_Add_Spells_From_Magic_Realm(m_spellbook_spell_list, (battle_units[caster_idx].race - 16));
        }
        else
        {
            Build_Spell_List(slt_Combat, m_spellbook_spell_list);
            unit_type = _UNITS[battle_units[caster_idx].unit_idx].type;
            for(itr1 = 0; itr1 < 4; itr1++)
            {
                already_in_list = ST_FALSE;
                if(_HEROES2[HUMAN_PLAYER_IDX]->heroes[unit_type].Spells[itr1] > 0)
                {
                    for(itr_spellbook_spell_list = 0; itr_spellbook_spell_list < m_spell_list_count; itr_spellbook_spell_list++)
                    {
                        if(_HEROES2[HUMAN_PLAYER_IDX]->heroes[unit_type].Spells[itr1] == m_spellbook_spell_list[itr_spellbook_spell_list])
                        {
                            already_in_list = ST_TRUE;
                        }
                    }
                    if(already_in_list == ST_FALSE)
                    {
                        m_spellbook_spell_list[m_spell_list_count] = _HEROES2[HUMAN_PLAYER_IDX]->heroes[unit_type].Spells[itr1];
                        m_spell_list_count++;
                    }
                }
            }
            if(battle_units[caster_idx].item_charges > 0)
            {
                unit_type = _UNITS[battle_units[caster_idx].unit_idx].Hero_Slot;
                /* OGBUG: this may not be the hero's original owner; should use unit owner, not current battle_unit controller */
                item_embed_spell_idx = _ITEMS[_players[battle_units[caster_idx].controller_idx].Heroes[unit_type].Items[0]].embed_spell_idx;
                if(item_embed_spell_idx > 0)
                {
                    already_in_list = ST_FALSE;
                    for(itr_spellbook_spell_list = 0; itr_spellbook_spell_list < m_spell_list_count; itr_spellbook_spell_list++)
                    {
                        if(m_spellbook_spell_list[itr_spellbook_spell_list] == item_embed_spell_idx)
                        {
                            already_in_list = ST_TRUE;
                        }
                    }
                    if(already_in_list == ST_FALSE)
                    {
                        m_spellbook_spell_list[m_spell_list_count] = item_embed_spell_idx;
                        m_spell_list_count++;
                    }
                }
            }
        }
    }
    else
    {
        Build_Spell_List(slt_Combat, m_spellbook_spell_list);
    }
    spellbook_has_castable_spell = ST_FALSE;
    if(caster_idx <= (CASTER_IDX_BASE - 1))
    {
        if(battle_units[caster_idx].item_charges > 0)
        {
            /* OGBUG: just because it is in an item, the spell may not necessarily be castable (e.g. Earth Elemental on a water square) */
            spellbook_has_castable_spell = ST_TRUE;
        }
        else
        {
            for(itr1 = 0; itr1 < m_spell_list_count; itr1++)
            {
                /* OGBUG: ignores Evil Omens */
                if(spell_data_table[abs(m_spellbook_spell_list[itr1])].casting_cost <= g_spellbook_cast_mana_limit)
                {
                    if(
                        !(
                            (spell_data_table[abs(m_spellbook_spell_list[itr1])].type == scc_Summoning)
                            &&
                            (_combat_structure == cs_OceanTerrainType)
                            &&
                            ((_unit_type_table[spell_data_table[abs(m_spellbook_spell_list[itr1])].unit_type].Move_Flags & MV_FLYING) == 0)
                            &&
                            ((_unit_type_table[spell_data_table[abs(m_spellbook_spell_list[itr1])].unit_type].Abilities & UA_NONCORPOREAL) == 0)
                        )
                        &&
                        !(
                            (m_spellbook_spell_list[itr1] == spl_Cracks_Call)  /* OGBUG: should be abs(spell_idx) */
                            &&
                            (_combat_structure == cs_OceanTerrainType)
                        )
                    )
                    {
                        spellbook_has_castable_spell = ST_TRUE;
                    }
                }
            }
        }
    }
    else
    {
        for(itr1 = 0; itr1 < m_spell_list_count; itr1++)
        {
            if(Casting_Cost((caster_idx - CASTER_IDX_BASE), abs(m_spellbook_spell_list[itr1]), 1) <= g_spellbook_cast_mana_limit)
            {
                if(
                    !(
                        (spell_data_table[abs(m_spellbook_spell_list[itr1])].type == scc_Summoning)
                        &&
                        (_combat_structure == cs_OceanTerrainType)
                        &&
                        ((_unit_type_table[spell_data_table[abs(m_spellbook_spell_list[itr1])].unit_type].Move_Flags & MV_FLYING) == 0)
                        &&
                        ((_unit_type_table[spell_data_table[abs(m_spellbook_spell_list[itr1])].unit_type].Abilities & UA_NONCORPOREAL) == 0)
                    )
                    &&
                    !(
                        (m_spellbook_spell_list[itr1] == spl_Cracks_Call)  /* OGBUG: should be abs(spell_idx) */
                        &&
                        (_combat_structure == cs_OceanTerrainType)
                    )
                )
                {
                    spellbook_has_castable_spell = ST_TRUE;
                }
            }
        }
    }
    Spellbook_Group_Counts();
    total_pages =  ((SBK_Group_3_Count + 5) / NUM_SPELLS_PER_PAGE_SML);
    total_pages += ((SBK_Group_2_Count + 5) / NUM_SPELLS_PER_PAGE_SML);
    total_pages += ((SBK_Group_1_Count + 5) / NUM_SPELLS_PER_PAGE_SML);
    total_pages += ((SBK_Group_5_Count + 5) / NUM_SPELLS_PER_PAGE_SML);
    total_pages += ((SBK_Group_4_Count + 5) / NUM_SPELLS_PER_PAGE_SML);
    total_pages += ((SBK_Group_6_Count + 5) / NUM_SPELLS_PER_PAGE_SML);
    m_spellbook_pages = (struct s_SPELL_BOOK_PAGE *)_cmbt_path_data;
    // ¿ ~ Spellbook_Add_Page() ?
    for(itr1 = 0; (total_pages + 3) > itr1; itr1++)
    {
        m_spellbook_pages[itr1].count = 0;
        stu_strcpy(m_spellbook_pages[itr1].title, str_empty_string__ovr117);
        for(index_on_page = 0; index_on_page < NUM_SPELLS_PER_PAGE_SML; index_on_page++)
        {
            m_spellbook_pages[itr1].spell[index_on_page] = spl_NONE;
        }
    }
    Spellbook_Add_Group_Pages(NUM_SPELLS_PER_PAGE_SML);
    Release_Block(_screen_seg);
    if(g_combat_spellbook_left_page > m_spellbook_page_count)
    {
        g_combat_spellbook_left_page = (m_spellbook_page_count - 2);
    }
    return spellbook_has_castable_spell;
}


// WZD o117p07
void SmlBook_Compose(struct s_SPELL_BOOK_PAGE spell_book_page, SAMB_ptr spellbook_bitmap)
{
    char spell_name[LEN_SPELL_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char temp_string[LEN_TEMP_BUFFER] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t string_width = 0;
    int16_t turns_to_cast = 0;
    int16_t Cost_Limit = 0;
    int16_t casting_cost = 0;
    int16_t spell_idx = 0;
    uint8_t colors[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t itr1 = 0;
    int16_t itr2 = 0;
    Draw_Picture_To_Bitmap(_spellbook_small_text, spellbook_bitmap);
    for(itr1 = 0; itr1 < 6; itr1++)
    {
        if(spell_book_page.count > itr1)
        {
            colors[0] = 184;
            for(itr2 = 1; itr2 < 5; itr2++)
            {
                colors[itr2] = 187;
            }
            Set_Font_Colors_15(1, &colors[0]);
            spell_idx = spell_book_page.spell[itr1];
            if(abs(spell_idx) != _players[HUMAN_PLAYER_IDX].casting_spell_idx)
            {
                casting_cost = Casting_Cost(HUMAN_PLAYER_IDX, abs(spell_idx), 0);
            }
            else
            {
                casting_cost = _players[HUMAN_PLAYER_IDX].casting_cost_remaining;
            }
            if(g_spellbook_cast_mana_limit != 0)
            {
                turns_to_cast = (casting_cost / g_spellbook_cast_mana_limit);
            }
            else
            {
                turns_to_cast = 40;
            }
            if(turns_to_cast > 40)
            {
                turns_to_cast = 40;
            }
            if(turns_to_cast < 1)
            {
                turns_to_cast = 1;
                if(_players[HUMAN_PLAYER_IDX].Skill_Left <= _players[HUMAN_PLAYER_IDX].mana_reserve)
                {
                    Cost_Limit = _players[HUMAN_PLAYER_IDX].Skill_Left;
                }
                else
                {
                    Cost_Limit = _players[HUMAN_PLAYER_IDX].mana_reserve;
                }
                if(Cost_Limit >= casting_cost)
                {
                    if(spell_data_table[abs(spell_idx)].type != scc_Crafting_Spell)
                    {
                        string_width = Get_String_Width(cnst_Instant_Cast);
                        Clear_Bitmap_Region(9, (7 + (itr1 * 22)), (12 + string_width), (12 + (itr1 * 22)), spellbook_bitmap);
                        Print_To_Bitmap(9, (7 + (itr1 * 22)), cnst_Instant_Cast, spellbook_bitmap);
                    }
                    else
                    {
                        string_width = Get_String_Width(cnst_Special_Cost);
                        Clear_Bitmap_Region(9, (7 + (itr1 * 22)), (12 + string_width), (12 + (itr1 * 22)), spellbook_bitmap);
                        Print_To_Bitmap(9, (7 + (itr1 * 22)), cnst_Special_Cost, spellbook_bitmap);
                    }
                }
                Clear_Bitmap_Region(0, (7 + (itr1 * 22)), 8, (12 + (itr1 * 22)), spellbook_bitmap);
                Clipped_Copy_Bitmap(1, (7 + (itr1 * 22)), spellbook_bitmap, spellbook_symbols_bitm[spell_data_table[abs(spell_idx)].magic_realm]);
            }
            else  /* (turns_to_cast >= 1) */
            {
                if(turns_to_cast >= 20)
                {
                    Clear_Bitmap_Region(0, (7 + (itr1 * 22)), 121, (12 + (itr1 * 22)), spellbook_bitmap);
                    Clear_Bitmap_Region(0, (13 + (itr1 * 22)), (3 + ((turns_to_cast - 20) * 6)), (19 + (itr1 * 22)), spellbook_bitmap);
                }
                else
                {
                    Clear_Bitmap_Region(0, (7 + (itr1 * 22)), (3 + (turns_to_cast * 6)), (12 + (itr1 * 22)), spellbook_bitmap);
                }
                for(itr2 = 0; itr2 < turns_to_cast; itr2++)
                {
                    if(itr2 >= 20)
                    {
                        Clipped_Copy_Bitmap((1 + ((itr2 % 20) * 6)), (13 + (itr1 * 22)), spellbook_bitmap, spellbook_symbols_bitm[spell_data_table[abs(spell_idx)].magic_realm]);
                    }
                    else
                    {
                        Clipped_Copy_Bitmap((1 + (itr2 * 6)), (7 + (itr1 * 22)), spellbook_bitmap, spellbook_symbols_bitm[spell_data_table[abs(spell_idx)].magic_realm]);
                    }
                }
            }
            if(abs(spell_idx) != _players[HUMAN_PLAYER_IDX].casting_spell_idx)
            {
                colors[0] = 184;
                for(itr2 = 1; itr2 < 5; itr2++)
                {
                    colors[itr2] = 187;
                }
            }
            else
            {
                for(itr2 = 0; itr2 < 5; itr2++)
                {
                    colors[itr2] = 198;
                }
            }
            Set_Font_Colors_15(1, &colors[0]);
            stu_strcpy(spell_name, spell_data_table[abs(spell_idx)].name);
            string_width = Get_String_Width(spell_name);
            Clear_Bitmap_Region(0, (itr1 * 22), (3 + string_width), (5 + (itr1 * 22)), spellbook_bitmap);
            Print_To_Bitmap(0, (itr1 * 22), spell_name, spellbook_bitmap);
            if(abs(spell_idx) != _players[HUMAN_PLAYER_IDX].casting_spell_idx)
            {
                casting_cost = Casting_Cost(HUMAN_PLAYER_IDX, abs(spell_idx), 0);
            }
            else
            {
                casting_cost = _players[HUMAN_PLAYER_IDX].casting_cost_remaining;
            }
            if(casting_cost < 0)
            {
                casting_cost = 0;
            }
            if(
                (casting_cost != 0)
                ||
                (spell_data_table[abs(spell_idx)].type != scc_Crafting_Spell)
            )
            {
                stu_itoa(casting_cost, temp_string, 10);
            }
            else
            {
                stu_strcpy(temp_string, cnst_QuestionMark5);
            }
            stu_strcat(temp_string, cnst_Space_MP_2);
            string_width = Get_String_Width(temp_string);
            Clear_Bitmap_Region((118 - string_width), (itr1 * 22), 121, (5 + (itr1 * 22)), spellbook_bitmap);
            Print_Right_To_Bitmap(121, (itr1 * 22), temp_string, spellbook_bitmap);
        }
        else
        {
            Clear_Bitmap_Region(0, (itr1 * 22), 121, (17 + (itr1 * 22)), spellbook_bitmap);
        }
    }
}


// WZD o117p08
/*
Up   J NX_j_Combat_Spellbook_Compose jmp     Combat_Spellbook_Compose         
Up   p Combat_Spellbook_Draw+91      call    near ptr Combat_Spellbook_Compose
     p Combat_Spellbook_Draw+127     call    near ptr Combat_Spellbook_Compose
Down p SmlBook_PageTurn+16C          call    near ptr Combat_Spellbook_Compose
Down p SmlBook_PageTurn+261          call    near ptr Combat_Spellbook_Compose
Down p SmlBook_PageTurn+3A4          call    near ptr Combat_Spellbook_Compose
*/
void Combat_Spellbook_Compose(struct s_SPELL_BOOK_PAGE spell_book_page, SAMB_ptr spellbook_bitmap, int16_t caster_idx)
{
    char spell_name[LEN_SPELL_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char temp_string[LEN_TEMP_BUFFER] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Max_Spendable = 0;
    int16_t Icon_Count = 0;
    int16_t casting_cost = 0;
    int16_t spell_idx = 0;
    uint8_t colors[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t itr1 = 0;
    int16_t itr2 = 0;
    Draw_Picture_To_Bitmap(_spellbook_small_text, spellbook_bitmap);
    for(itr1 = 0; itr1 < 6; itr1++)
    {
        if(spell_book_page.count > itr1)
        {
            spell_idx = spell_book_page.spell[itr1];
            if(caster_idx < (CASTER_IDX_BASE - 1))
            {
                casting_cost = spell_data_table[abs(spell_idx)].casting_cost;
            }
            else
            {
                casting_cost = Casting_Cost(HUMAN_PLAYER_IDX, abs(spell_idx), 1);
            }
            /*
                BEGIN:  Icon_Count
            */
            if(casting_cost == 0)
            {
                Icon_Count = 40;
            }
            else
            {
                if(caster_idx >= CASTER_IDX_BASE)
                {
                    Icon_Count = Combat_Casting_Cost_Multiplier(HUMAN_PLAYER_IDX);
                    Max_Spendable = _players[HUMAN_PLAYER_IDX].Cmbt_Skill_Left;
                    // ; BUG: rounds the wrong way
                    // ; BUG: x0.5 multiplier results in a negative signed
                    // ; value if the player has over 16,383 mana
                    if(((_players[HUMAN_PLAYER_IDX].mana_reserve * 10) / Icon_Count) < Max_Spendable)
                    {
                        Max_Spendable = ((_players[HUMAN_PLAYER_IDX].mana_reserve * 10) / Icon_Count);
                    }
                    if(casting_cost > Max_Spendable)
                    {
                        Icon_Count = 0;
                    }
                    else
                    {
                        Icon_Count = (Max_Spendable / casting_cost);
                    }
                }
                else  /* (caster_idx < CASTER_IDX_BASE) */
                {
                    if(casting_cost > g_spellbook_cast_mana_limit)
                    {
                        Icon_Count = 0;
                    }
                    else
                    {
                        Icon_Count = (g_spellbook_cast_mana_limit / casting_cost);
                    }
                    if(battle_units[caster_idx].item_charges > 0)
                    {
                        if(_ITEMS[_players[HUMAN_PLAYER_IDX].Heroes[_UNITS[battle_units[caster_idx].unit_idx].Hero_Slot].Items[0]].embed_spell_idx == spell_idx)
                        {
                            Icon_Count = battle_units[caster_idx].item_charges;
                            casting_cost = 22222;
                        }
                    }
                }
                if(
                    (
                        (spell_data_table[abs(spell_idx)].type == scc_City_Enchantment_Positive)
                        ||
                        (abs(spell_idx) == spl_Wall_Of_Stone)
                    )
                    &&
                    (
                        (_combat_attacker_player == HUMAN_PLAYER_IDX)
                        ||
                        (_combat_environ != cnv_Enemy_City)
                    )
                )
                {
                    Icon_Count = 0;
                }
                if(
                    (spell_data_table[abs(spell_idx)].type == scc_Summoning)
                    &&
                    (_combat_structure == cs_OceanTerrainType)
                    &&
                    ((_unit_type_table[spell_data_table[abs(spell_idx)].unit_type].Move_Flags & MV_FLYING) == 0)
                    &&
                    ((_unit_type_table[spell_data_table[abs(spell_idx)].unit_type].Abilities & UA_NONCORPOREAL) == 0)
                )
                {
                    Icon_Count = 0;
                }
                if(
                    (spell_idx == spl_Cracks_Call)  /* OGBUG: should be `(abs(spell_idx) == spl_Cracks_Call)` */
                    &&
                    (_combat_structure == cs_OceanTerrainType)
                )
                {
                    Icon_Count = 0;
                }
            }
            if(Icon_Count > 40)
            {
                Icon_Count = 40;
            }
            /*
                END:  Icon_Count
            */
            if(Icon_Count < 1)
            {
                for(itr2 = 0; itr2 < 5; itr2++)
                {
                    colors[itr2] = 26;
                }
            }
            else
            {
                if(Icon_Count < 20)
                {
                    Clear_Bitmap_Region(0, (7 + (itr1 * 22)), (3 + (Icon_Count * 6)), (12 + (itr1 * 22)), spellbook_bitmap);
                }
                else
                {
                    Clear_Bitmap_Region(0, (7 + (itr1 * 22)), 121, (12 + (itr1 * 22)), spellbook_bitmap);
                    Clear_Bitmap_Region(0, (13 + (itr1 * 22)), (3 + ((Icon_Count - 20) * 6)), (19 + (itr1 * 22)), spellbook_bitmap);
                }
                for(itr2 = 0; itr2 < Icon_Count; itr2++)
                {
                    if(itr2 < 20)
                    {
                        // ; decoded image headers appended to LBX_Sandbox_Segment
                        Clipped_Copy_Bitmap((1 + (itr2 * 6)), (7 + (itr1 * 22)), spellbook_bitmap, spellbook_symbols_bitm[spell_data_table[abs(spell_idx)].magic_realm]);
                    }
                    else
                    {
                        Clipped_Copy_Bitmap((1 + ((itr2 % 20) * 6)), (13 + (itr1 * 22)), spellbook_bitmap, spellbook_symbols_bitm[spell_data_table[abs(spell_idx)].magic_realm]);
                    }
                }
                colors[0] = 184;
                for(itr2 = 1; itr2 < 5; itr2++)
                {
                    colors[itr2] = 187;
                }
            }
            Set_Font_Colors_15(1, &colors[0]);
            stu_strcpy(spell_name, spell_data_table[abs(spell_idx)].name);
            Max_Spendable = Get_String_Width(spell_name);
            Clear_Bitmap_Region(0, (itr1 * 22), (3 + Max_Spendable), (5 + (itr1 * 22)), spellbook_bitmap);
            Print_To_Bitmap(0, (itr1 * 22), spell_name, spellbook_bitmap);
            if(casting_cost == 0)
            {
                stu_strcpy(temp_string, cnst_QuestionMark5);
            }
            else
            {
                if(casting_cost == 22222)
                {
                    stu_strcpy(temp_string, cnst_Item);
                }
                else
                {
                    stu_itoa(casting_cost, temp_string, 10);
                    stu_strcat(temp_string, cnst_Space_MP_2);
                }
            }
            Max_Spendable = Get_String_Width(temp_string);
            Clear_Bitmap_Region((118 - Max_Spendable), (itr1 * 22), 121, (5 + (itr1 * 22)), spellbook_bitmap);
            Print_Right_To_Bitmap(120, (itr1 * 22), temp_string, spellbook_bitmap);
        }
        else  /* (spell_book_page.count <= itr1) */
        {
            Clear_Bitmap_Region(0, (itr1 * 22), 121, (17 + (itr1 * 22)), spellbook_bitmap);
        }
    }
}


// WZD o117p09
void SmlBook_Draw(int16_t x, int16_t y)
{
    SAMB_ptr spellbook_bitmap = 0;
    uint8_t colors[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t itr = 0;
    Mark_Block(_screen_seg);
    spellbook_bitmap = Allocate_Next_Block(_screen_seg, 1050);  // 1050 PR  16800 B
    // {Nature, Sorcery, Chaos, Life, Death, Arcane}
    for(itr = 0; itr < NUM_MAGIC_REALMS; itr++)
    {
        spellbook_symbols_bitm[itr] = Allocate_Next_Block(_screen_seg, 4);  // 4 PR  64 B
        Draw_Picture_To_Bitmap(_spellbook_small_symbols[itr], spellbook_symbols_bitm[itr]);
    }
    Cycle_Palette_Color(198, 41, 33, 29, 48, 43, 43, 3);
    FLIC_Draw(x, y, _spellbook_small_seg);
    if(g_spellbook_mode == 1)
    {
        if(g_spellbook_left_page != 0)
        {
            FLIC_Draw((x + 13), (y + 4), _spellbook_small_left_corner_seg);
        }
        if(
            (g_spellbook_left_page != m_spellbook_page_count)
            &&
            ((m_spellbook_page_count - 2) > g_spellbook_left_page)
        )
        {
            FLIC_Draw((x + 258), (y + 4), _spellbook_small_right_corner_seg);
        }
    }
    SmlBook_Compose(m_spellbook_pages[g_spellbook_left_page], spellbook_bitmap);
    Draw_Picture((x + 16), (y + 21), spellbook_bitmap);
    colors[0] = 53;
    for(itr = 1; itr < 5; itr++)
    {
        colors[itr] = 46;
    }
    Set_Font_Colors_15(4, &colors[0]);
    Print_Centered((x + 70), (y + 6), m_spellbook_pages[g_spellbook_left_page].title);
    Draw_Picture_To_Bitmap(_spellbook_small_text, spellbook_bitmap);
    SmlBook_Compose(m_spellbook_pages[(g_spellbook_left_page + 1)], spellbook_bitmap);
    Draw_Picture((x + 148), (y + 21), spellbook_bitmap);
    colors[0] = 53;
    for(itr = 1; itr < 5; itr++)
    {
        colors[itr] = 46;
    }
    Set_Font_Colors_15(4, &colors[0]);
    Print_Centered((x + 208), (y + 6), m_spellbook_pages[(g_spellbook_left_page + 1)].title);
    Release_Block(_screen_seg);
}


// WZD o117p10
void Combat_Spellbook_Draw(int16_t x, int16_t y, int16_t caster_idx)
{
    uint8_t colors[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t itr = 0;
    // DNE  Mark_Block(_screen_seg);
    // DNE  spellbook_bitmap = Allocate_Next_Block(_screen_seg, 1050);  // 1050 PR  16800 B
    FLIC_Draw(x, y, _spellbook_small_seg);
    if(g_spellbook_mode == 1)
    {
        if(g_spellbook_left_page != 0)
        {
            FLIC_Draw((x + 13), (y + 4), _spellbook_small_left_corner_seg);
        }
        if(
            (g_spellbook_left_page != m_spellbook_page_count)
            &&
            ((m_spellbook_page_count - 2) > g_spellbook_left_page)
        )
        {
            FLIC_Draw((x + 258), (y + 4), _spellbook_small_right_corner_seg);
        }
    }
    // SmlBook_Compose(m_spellbook_pages[g_spellbook_left_page], spellbook_bitmap);
    // spellbook_bitmap  allocated in SmlBook_Draw(), passed to SmlBook_Compose()
    Combat_Spellbook_Compose(m_spellbook_pages[g_spellbook_left_page], spl_anim_compose_seg, caster_idx);
    Draw_Picture((x + 16), (y + 21), spl_anim_compose_seg);
    colors[0] = 53;
    for(itr = 1; itr < 5; itr++)
    {
        colors[itr] = 46;
    }
    Set_Font_Colors_15(4, &colors[0]);
    Print_Centered((x + 70), (y + 6), m_spellbook_pages[g_spellbook_left_page].title);
    Draw_Picture_To_Bitmap(_spellbook_small_text, spl_anim_compose_seg);
    // SmlBook_Compose(m_spellbook_pages[(g_spellbook_left_page + 1)], spellbook_bitmap);
    Combat_Spellbook_Compose(m_spellbook_pages[(g_spellbook_left_page + 1)], spl_anim_compose_seg, caster_idx);
    Draw_Picture((x + 148), (y + 21), spl_anim_compose_seg);
    colors[0] = 53;
    for(itr = 1; itr < 5; itr++)
    {
        colors[itr] = 46;
    }
    Set_Font_Colors_15(4, &colors[0]);
    Print_Centered((x + 208), (y + 6), m_spellbook_pages[(g_spellbook_left_page + 1)].title);
    // DNE  Release_Block(_screen_seg);
}


// WZD o117p11
// BOOK.LBX  "PTURNSML"
void SmlBook_PageTurn(int16_t turn_type, int16_t combat_flag, int16_t player_idx)
{
    int16_t Stage_Text_Widths[4] = { 0, 0, 0, 0};
    int16_t ystart = 0;
    int16_t xstart = 0;
    uint8_t colors[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t itr_colors = 0;
    int16_t PageTurn_Stage = 0;
    SAMB_ptr page_bitmap = 0;
    SAMB_ptr page_turn_bitmap = 0;
    int16_t itr = 0;  // DNE in Dasm
    int16_t page_num = 0;
    xstart = 16;
    ystart = 10;
    Stage_Text_Widths[3] = 85;
    Stage_Text_Widths[0] = 85;
    Stage_Text_Widths[2] = 55;
    Stage_Text_Widths[1] = 55;
    Deactivate_Auto_Function();
    Open_File_Animation__HACK(book_lbx_file__ovr117, 0);
    if(combat_flag == ST_FALSE)
    {
        Allocate_Reduced_Map();
    }
    Mark_Block(_screen_seg);
    page_bitmap = Allocate_Next_Block(_screen_seg, 1040);
    page_turn_bitmap = Allocate_Next_Block(_screen_seg, 1400);
    for(itr = 0; itr < NUM_MAGIC_REALMS; itr++)
    {
        spellbook_symbols_bitm[itr] = Allocate_Next_Block(_screen_seg, 4);
        Draw_Picture_To_Bitmap(_spellbook_small_symbols[itr], spellbook_symbols_bitm[itr]);
    }
    Cycle_Palette_Color(198, 39, 31, 25, 50, 46, 47, 3);
    for(PageTurn_Stage = 0; PageTurn_Stage < 4; PageTurn_Stage++)
    {
        Set_Page_Off();
        Copy_Back_To_Off();
        if(turn_type == 1)
        {
            page_num = g_spellbook_left_page;
        }
        else
        {
            page_num = g_spellbook_left_page - 2;
        }
        if(page_num != 0 && g_spellbook_mode == ST_TRUE)
        {
            FLIC_Draw(xstart + 13, ystart + 4, _spellbook_small_left_corner_seg);
        }
        if(combat_flag == ST_FALSE)
        {
            SmlBook_Compose(m_spellbook_pages[page_num], page_bitmap);
        }
        else
        {
            Combat_Spellbook_Compose(m_spellbook_pages[page_num], page_bitmap, player_idx);
        }
        Draw_Picture(xstart + 16, ystart + 21, page_bitmap);
        colors[0] = 53;
        for(itr_colors = 1; itr_colors < 5; itr_colors++)
        {
            colors[itr_colors] = 46;
        }
        Set_Font_Colors_15(4, colors);
        Print_Centered(xstart + 70, ystart + 6, m_spellbook_pages[page_num].title);
        if(turn_type == 1)
        {
            page_num = g_spellbook_left_page + 3;
        }
        else
        {
            page_num = g_spellbook_left_page + 1;
        }
        if(page_num < m_spellbook_page_count && g_spellbook_mode == ST_TRUE)
        {
            FLIC_Draw(xstart + 258, ystart + 4, _spellbook_small_right_corner_seg);
        }
        if(combat_flag == ST_FALSE)
        {
            SmlBook_Compose(m_spellbook_pages[page_num], page_bitmap);
        }
        else
        {
            Combat_Spellbook_Compose(m_spellbook_pages[page_num], page_bitmap, player_idx);
        }
        Draw_Picture(xstart + 148, ystart + 21, page_bitmap);
        colors[0] = 53;
        for(itr_colors = 1; itr_colors < 5; itr_colors++)
        {
            colors[itr_colors] = 46;
        }
        Set_Font_Colors_15(4, colors);
        if(spell_data_table[m_spellbook_pages[page_num - 1].spell[0]].spell_book_category != spell_data_table[m_spellbook_pages[page_num].spell[0]].spell_book_category)
        {
            Print_Centered(xstart + 208, ystart + 6, m_spellbook_pages[page_num].title);
        }
        if(turn_type == 0)
        {
            Set_File_Animation_Frame(3 - PageTurn_Stage);
        }
        Draw_File_Animation__HACK();
        if(PageTurn_Stage < 2)
        {
            if(turn_type == 1)
            {
                page_num = g_spellbook_left_page + 1;
            }
            else
            {
                page_num = g_spellbook_left_page;
            }
        }
        else
        {
            if(turn_type == 1)
            {
                page_num = g_spellbook_left_page + 2;
            }
            else
            {
                page_num = g_spellbook_left_page - 1;
            }
        }
        if(combat_flag == ST_FALSE)
        {
            SmlBook_Compose(m_spellbook_pages[page_num], page_bitmap);
        }
        else
        {
            Combat_Spellbook_Compose(m_spellbook_pages[page_num], page_bitmap, player_idx);
        }
        Create_Picture(122, 175, page_turn_bitmap);
        Clipped_Copy_Bitmap(0, 48, page_turn_bitmap, page_bitmap);
        colors[0] = 53;
        for(itr_colors = 1; itr_colors < 5; itr_colors++)
        {
            colors[itr_colors] = 46;
        }
        Set_Font_Colors_15(4, colors);
        if(abs(page_num - g_spellbook_left_page) == 2 || spell_data_table[m_spellbook_pages[page_num - 1].spell[0]].spell_book_category != spell_data_table[m_spellbook_pages[page_num].spell[0]].spell_book_category
        )
        {
            Print_Centered_To_Bitmap(62, 30, m_spellbook_pages[page_num].title, page_turn_bitmap);
        }
        Scale_Bitmap(page_turn_bitmap, Stage_Text_Widths[PageTurn_Stage], 100);
        if((turn_type == 1 && PageTurn_Stage == 0) || (turn_type == 0 && PageTurn_Stage == 3))
        {
            Shear_Bitmap_Y( 0,   0,   3,  -4, page_turn_bitmap);
            Shear_Bitmap_Y( 4,  -5,   9, -10, page_turn_bitmap);
            Shear_Bitmap_Y(10, -11,  21, -16, page_turn_bitmap);
            Shear_Bitmap_Y(22, -16,  32, -19, page_turn_bitmap);
            Shear_Bitmap_Y(33, -19,  40, -19, page_turn_bitmap);
            Shear_Bitmap_Y(41, -18,  62, -13, page_turn_bitmap);
            Shear_Bitmap_Y(63, -12, 101,   5, page_turn_bitmap);
            Draw_Picture_Windowed(xstart + 148, ystart - 28, page_turn_bitmap);
        }
        if((turn_type == 1 && PageTurn_Stage == 1) || (turn_type == 0 && PageTurn_Stage == 2))
        {
            Shear_Bitmap_Y( 0,   0,  5,  -8, page_turn_bitmap);
            Shear_Bitmap_Y( 6, -11, 16, -21, page_turn_bitmap);
            Shear_Bitmap_Y(17, -22, 26, -27, page_turn_bitmap);
            Shear_Bitmap_Y(27, -27, 38, -29, page_turn_bitmap);
            Shear_Bitmap_Y(39, -29, 49, -27, page_turn_bitmap);
            Shear_Bitmap_Y(50, -26, 61, -21, page_turn_bitmap);
            Shear_Bitmap_Y(62, -20, 70, -12, page_turn_bitmap);
            Draw_Picture_Windowed(xstart + 148, ystart - 30, page_turn_bitmap);
        }
        if((turn_type == 1 && PageTurn_Stage == 2) || (turn_type == 0 && PageTurn_Stage == 1))
        {
            Shear_Bitmap_Y( 0, -16,  9, -25, page_turn_bitmap);
            Shear_Bitmap_Y(10, -25, 17, -29, page_turn_bitmap);
            Shear_Bitmap_Y(18, -29, 29, -32, page_turn_bitmap);
            Shear_Bitmap_Y(30, -33, 40, -30, page_turn_bitmap);
            Shear_Bitmap_Y(41, -29, 52, -25, page_turn_bitmap);
            Shear_Bitmap_Y(53, -24, 63, -14, page_turn_bitmap);
            Shear_Bitmap_Y(64, -12, 70,   0, page_turn_bitmap);
            Draw_Picture_Windowed(xstart + 71, ystart - 28, page_turn_bitmap);
        }
        if((turn_type == 1 && PageTurn_Stage == 3) || (turn_type == 0 && PageTurn_Stage == 0))
        {
            Shear_Bitmap_Y( 0,   0,  31, -16, page_turn_bitmap);
            Shear_Bitmap_Y(32, -16,  47, -22, page_turn_bitmap);
            Shear_Bitmap_Y(48, -22,  68, -27, page_turn_bitmap);
            Shear_Bitmap_Y(69, -27,  82, -23, page_turn_bitmap);
            Shear_Bitmap_Y(83, -23,  92, -18, page_turn_bitmap);
            Shear_Bitmap_Y(93, -18, 101, -10, page_turn_bitmap);
            Draw_Picture_Windowed(xstart + 35, ystart - 22, page_turn_bitmap);
        }
        PageFlip_FX();
    }
    Release_Block(_screen_seg);
    if(combat_flag != ST_FALSE)
    {
        Assign_Auto_Function(Combat_Spellbook_Screen_Draw, 2);
    }
    else
    {
        Assign_Auto_Function(Spellbook_Screen_Draw, 2);
    }
}


// WZD o117p12
// drake178: SBK_AddPages()
/*
; creates and fills pages in the m_spellbook_pages@
; allocation, including only spells that match the
; selected group criteria based on the spellbook order
; setting, although a group index of -1 ignores these
; groupings and includes all spells from the also
; global m_spellbook_spell_list@
*/
/*

XREF:
    SBK_BuildSpellbook()
    Spellbook_Add_Group_Pages()
    NX_j_SBK_AddPages__WIP()

group_idx   category or magic realm

*/
void Spellbook_Add_Page(int16_t group_spell_count, int16_t group_idx, char * title, int16_t page_spell_count)
{
    int16_t same_group;
    int16_t page_spell_counter;
    int16_t itr_spellbook_spell_list;
    int16_t prev_page_first_spell;

    page_spell_counter = 0;

    stu_strcpy(m_spellbook_pages[m_spellbook_page_count].title, title);

    itr_spellbook_spell_list = 0;

    while(group_spell_count > 0)
    {

        same_group = ST_FALSE;

        if(magic_set.spell_book_ordering != ST_TRUE)
        {
            if(
                (spell_data_table[abs(m_spellbook_spell_list[itr_spellbook_spell_list])].magic_realm == group_idx)
                ||
                (group_idx == ST_UNDEFINED)
            )
            {
                same_group = ST_TRUE;
            }
        }
        else
        {
            if(
                (spell_data_table[abs(m_spellbook_spell_list[itr_spellbook_spell_list])].spell_book_category == group_idx)
                ||
                (group_idx == ST_UNDEFINED)
            )
            {
                same_group = ST_TRUE;
            }
        }

        if(same_group == ST_TRUE)
        {

            page_spell_counter++;

            // ¿ page is full ?
            if((page_spell_count + 1) == page_spell_counter)
            {
                m_spellbook_pages[m_spellbook_page_count].count = (int8_t)page_spell_count;

                page_spell_counter = 1;

                m_spellbook_page_count++;

                if((m_spellbook_page_count % 2) == 1)  /* ¿ odd pages ? ¿ {0,1} or {1,2} ? */
                {
                    /* Copilot */  prev_page_first_spell = abs(m_spellbook_pages[(m_spellbook_page_count - 1)].spell[0]);
                    if(magic_set.spell_book_ordering != ST_TRUE)
                    {
                        // if(spell_data_table[m_spellbook_pages[(m_spellbook_page_count - 1)].spell[0]].magic_realm != group_idx)
                        /* Copilot */  if(spell_data_table[prev_page_first_spell].magic_realm != group_idx)
                        {
                            stu_strcpy(m_spellbook_pages[m_spellbook_page_count].title, title);
                        }
                    }
                    else
                    {
                        // if(spell_data_table[m_spellbook_pages[(m_spellbook_page_count - 1)].spell[0]].spell_book_category != group_idx)
                        /* Copilot */  if(spell_data_table[prev_page_first_spell].spell_book_category != group_idx)
                        {
                            stu_strcpy(m_spellbook_pages[m_spellbook_page_count].title, title);
                        }
                    }

                }
                else
                {
                    stu_strcpy(m_spellbook_pages[m_spellbook_page_count].title, title);
                }
                
            }

            group_spell_count--;

            // ~ NOT Apprentice, Choose Research, or Learn Spell  AKA Big Book; therefore, Spells button or Combat  AKA Small Book
            if(page_spell_count != NUM_SPELLS_PER_PAGE_BIG)
            {
                if(abs(m_spellbook_spell_list[itr_spellbook_spell_list]) == _players[HUMAN_PLAYER_IDX].casting_spell_idx)
                {
                    g_spellbook_left_page = page_spell_counter;
                }
            }

            m_spellbook_pages[m_spellbook_page_count].spell[(page_spell_counter - 1)] = m_spellbook_spell_list[itr_spellbook_spell_list];

        }

        itr_spellbook_spell_list++;

    }

    m_spellbook_pages[m_spellbook_page_count].count = (int8_t)page_spell_counter;

    m_spellbook_page_count++;
    
}

// WZD o117p13
// drake178: SBK_Fill()
void Spellbook_Add_Group_Pages(int16_t page_spell_count)
{

    if(magic_set.spell_book_ordering == ST_TRUE)
    {

        if(SBK_Group_1_Count > 0)
        {
            Spellbook_Add_Page(SBK_Group_1_Count, sbc_Summoning, cnst_SpellGroup1, page_spell_count);  // "Summoning"
        }

        if(SBK_Group_2_Count > 0)
        {
            Spellbook_Add_Page(SBK_Group_2_Count, sbc_Special, cnst_SpellGroup2, page_spell_count);  // "Special Spells"
        }

        if(SBK_Group_3_Count > 0)
        {
            Spellbook_Add_Page(SBK_Group_3_Count, sbc_City, cnst_SpellGroup3, page_spell_count);  // "City Spells"
        }

        if(SBK_Group_5_Count > 0)
        {
            Spellbook_Add_Page(SBK_Group_5_Count, sbc_Enchantment, cnst_SpellGroup4, page_spell_count);  // "Enchantment"
        }

        if(SBK_Group_4_Count > 0)
        {
            Spellbook_Add_Page(SBK_Group_4_Count, sbc_Unit, cnst_SpellGroup5, page_spell_count);  // "Unit Spells"
        }

        if(SBK_Group_6_Count > 0)
        {
            Spellbook_Add_Page(SBK_Group_6_Count, sbc_Combat, cnst_SpellGroup6, page_spell_count);  // "Combat Spells"
        }

    }
    else
    {

        if(SBK_Group_1_Count > 0)
        {
            Spellbook_Add_Page(SBK_Group_1_Count, sbr_Arcane, cnst_SpellRealm1, page_spell_count);  // "Arcane"
        }

        if(SBK_Group_2_Count > 0)
        {
            Spellbook_Add_Page(SBK_Group_2_Count, sbr_Chaos, cnst_SpellRealm2, page_spell_count);  // "Chaos"
        }

        if(SBK_Group_3_Count > 0)
        {
            Spellbook_Add_Page(SBK_Group_3_Count, sbr_Death, cnst_SpellRealm3, page_spell_count);  // "Death"
        }

        if(SBK_Group_5_Count > 0)
        {
            Spellbook_Add_Page(SBK_Group_5_Count, sbr_Life, cnst_SpellRealm4, page_spell_count);  // "Life"
        }

        if(SBK_Group_4_Count > 0)
        {
            Spellbook_Add_Page(SBK_Group_4_Count, sbr_Nature, cnst_SpellRealm5, page_spell_count);  // "Nature"
        }

        if(SBK_Group_6_Count > 0)
        {
            Spellbook_Add_Page(SBK_Group_6_Count, sbr_Sorcery, cnst_SpellRealm6, page_spell_count);  // "Sorcery"
        }

    }

    // have an odd number of pages
    if((m_spellbook_page_count % 2) == 1)
    {
        m_spellbook_pages[m_spellbook_page_count].count = 0;
        m_spellbook_page_count++;
    }

}




/*
    WIZARDS.EXE  ovr118
*/

// WZD o118p01
/**
 * @brief Animate a big spellbook page turn and restore the apprentice book draw loop.
 *
 * Loads the BOOK.LBX page-turn animation, composes static background pages,
 * renders the turning page frame-by-frame with stage-specific scaling and
 * vertical shift deformation, and flips the display each stage.
 *
 * @param direction Page-turn mode selector.
 * Use 0 for backward, 1 for forward, and 666 to jump forward to the
 * research candidate pages.
 *
 * @details
 * This routine depends on and mutates spellbook UI globals, including
 * page position and dog-ear state, and temporarily allocates screen-segment
 * buffers for composed page imagery and corner art.
 *
 * Side effects:
 * - Deactivates and then reassigns the auto-draw callback to j_BigBook_Draw.
 * - Loads animation/graphics resources from BOOK.LBX and SCROLL.LBX.
 * - Uses page-flip effects to present 4 staged frames of the turn.
 *
 * @note The value 666 is a legacy sentinel used by the original flow to
 * target the research candidate section directly.
 */
void BigBook_PageTurn(int16_t direction)
{
    int16_t stage_text_widths[4] = { 0, 0, 0, 0 };
    int16_t page_turn_stage = 0;
    int16_t left_page = 0;
    int16_t page_index = 0;
    SAMB_ptr pict_seg = NULL;
    stage_text_widths[0] = 90;
    stage_text_widths[1] = 60;
    stage_text_widths[2] = 60;
    stage_text_widths[3] = 90;
    Deactivate_Auto_Function();
    /* Load page turn animation from BOOK.LBX */
    Open_File_Animation__HACK(book_lbx_file__ovr118, 1);
    Mark_Block(_screen_seg);
    /* Allocate buffer for composing the turning page */
    pict_seg = Allocate_Next_Block(_screen_seg, 1800);
    /* Load corner graphics */
    _spellbook_big_left_corner_seg = LBX_Reload_Next(scroll_lbx_file__ovr118__1, 7, _screen_seg);
    _spellbook_big_right_corner_seg = LBX_Reload_Next(scroll_lbx_file__ovr118__1, 8, _screen_seg);
    for(page_turn_stage = 0; page_turn_stage < 4; ++page_turn_stage)
    {
        Set_Page_Off();
        Copy_Back_To_Off();
        /* Determine background page index (left side) */
        if(direction >= 1)
        {
            page_index = g_spellbook_left_page;
        }
        else
        {
            page_index = (g_spellbook_left_page - 2);
        }
        /* Draw static background (Left Page) */
        BigBook_Compose(page_index, pict_seg, 1);
        Draw_Picture_Windowed(25, -20, pict_seg);
        if(page_index != 0 && g_spellbook_mode > 1)
        {
            FLIC_Draw(15, 9, (void *)_spellbook_big_left_corner_seg);
        }
        /* Determine background page index (Right Side) */
        if(direction == 1)
        {
            page_index = g_spellbook_left_page + 3;
        }
        else
        {
            if(direction == 0)
            {
                page_index = g_spellbook_left_page;
            }
            else /* direction == 666 (Research) */
            {
                page_index = g_first_research_page;
            }
            page_index++;
        }
        /* Draw static background (Right Page) */
        BigBook_Compose(page_index, pict_seg, 0);
        Draw_Picture_Windowed(173, -20, pict_seg);
        if(m_spellbook_page_count - 2 > page_index && g_spellbook_mode > 1)
        {
            FLIC_Draw(289, 9, (void *)_spellbook_big_right_corner_seg);
        }
        /* Determine which page is "on top" (turning) */
        if(direction != 0)
        {
            /* Forward or Research */
            if(page_turn_stage < 2)
            {
                if(direction < 1)
                {
                    page_index = g_spellbook_left_page;
                    left_page = 1;
                }
                else
                {
                    page_index = g_spellbook_left_page + 1;
                    left_page = 0;
                }

            }
            else
            {
                if(direction == 1)
                {
                    page_index = (g_spellbook_left_page + 2);
                    left_page = 1;
                }
                else if(direction == 666)
                {
                    left_page = g_first_research_page;
                    left_page = 1;
                }
                else
                {
                    left_page = g_spellbook_left_page - 1;
                    left_page = 0;
                }
            }
        }
        else
        {
            /* Backward */
            Set_File_Animation_Frame(3 - page_turn_stage);
            if(page_turn_stage < 2)
            {
                page_index = g_spellbook_left_page;
                left_page = 1;
            }
            else
            {
                page_index = (g_spellbook_left_page - 1);
                left_page = 0;
            }
        }
        /* Draw the animation frame */
        Draw_File_Animation__HACK();
        /* Compose and transform the turning page bitmap */
        BigBook_Compose(page_index, pict_seg, left_page);
        Scale_Bitmap(pict_seg, stage_text_widths[page_turn_stage], 100);
        /* Apply distortion effects based on animation stage to simulate page curve */
        if((direction >= 1 && page_turn_stage == 0) || (direction == 0 && page_turn_stage == 3))
        {
            Shear_Bitmap_Y(0, 0, 4, -6, pict_seg);
            Shear_Bitmap_Y(5, -7, 12, -12, pict_seg);
            Shear_Bitmap_Y(13, -13, 29, -19, pict_seg);
            Shear_Bitmap_Y(30, -19, 41, -19, pict_seg);
            Shear_Bitmap_Y(42, -19, 67, -14, pict_seg);
            Shear_Bitmap_Y(68, -13, 84, -7, pict_seg);
            Shear_Bitmap_Y(85, -6, 115, 8, pict_seg);
            Draw_Picture_Windowed(165, -24, pict_seg);
        }
        if((direction >= 1 && page_turn_stage == 1) || (direction == 0 && page_turn_stage == 2))
        {
            Shear_Bitmap_Y(0, -4, 5, -9, pict_seg);
            Shear_Bitmap_Y(6, -10, 14, -18, pict_seg);
            Shear_Bitmap_Y(15, -19, 32, -26, pict_seg);
            Shear_Bitmap_Y(33, -26, 42, -28, pict_seg);
            Shear_Bitmap_Y(43, -28, 56, -25, pict_seg);
            Shear_Bitmap_Y(57, -25, 69, -18, pict_seg);
            Shear_Bitmap_Y(70, -18, 76, -12, pict_seg);
            Draw_Picture_Windowed(165, -26, pict_seg);
        }
        if((direction >= 1 && page_turn_stage == 2) || (direction == 0 && page_turn_stage == 1))
        {
            Shear_Bitmap_Y(0, -20, 6, -25, pict_seg);
            Shear_Bitmap_Y(7, -25, 16, -30, pict_seg);
            Shear_Bitmap_Y(17, -30, 24, -32, pict_seg);
            Shear_Bitmap_Y(25, -32, 32, -33, pict_seg);
            Shear_Bitmap_Y(33, -33, 47, -29, pict_seg);
            Shear_Bitmap_Y(48, -29, 61, -21, pict_seg);
            Shear_Bitmap_Y(62, -20, 70, -12, pict_seg);
            Shear_Bitmap_Y(71, -11, 75, 0, pict_seg);
            Draw_Picture_Windowed(80, -20, pict_seg);
        }
        if((direction >= 1 && page_turn_stage == 3) || (direction == 0 && page_turn_stage == 0))
        {
            Shear_Bitmap_Y(0, 0, 36, -18, pict_seg);
            Shear_Bitmap_Y(37, -18, 64, -26, pict_seg);
            Shear_Bitmap_Y(65, -26, 73, -27, pict_seg);
            Shear_Bitmap_Y(74, -27, 84, -26, pict_seg);
            Shear_Bitmap_Y(85, -25, 92, -23, pict_seg);
            Shear_Bitmap_Y(93, -22, 102, -18, pict_seg);
            Shear_Bitmap_Y(103, -17, 115, -6, pict_seg);
            Draw_Picture_Windowed(39, -22, pict_seg);
        }
        PageFlip_FX();
    }
    /* Reset drawing function and cleanup */
    Assign_Auto_Function(BigBook_Draw, 2);
    Release_Block(_screen_seg);
}


// WZD o118p02
void BigBook_Draw(void)
{
    SAMB_ptr pict_seg = NULL;
    /* Update the spellbook animation stage (0-7) */
    g_spellbook_anim_stage = ((g_spellbook_anim_stage + 1) % 8);
    /* Cycle palette colors for the glowing spellbook effect */
    Cycle_Palette_Color(198, 29, 41, 63, 44, 56, 63, 2);
    /* Allocate temporary buffer for page rendering from the screen segment */
    Mark_Block(_screen_seg);
    pict_seg = Allocate_Next_Block(_screen_seg, 1800);
    /* Load left and right corner dog-ear graphics from scroll.lbx */
    _spellbook_big_left_corner_seg = LBX_Reload_Next(scroll_lbx_file__ovr118__1, 7, _screen_seg);
    _spellbook_big_right_corner_seg = LBX_Reload_Next(scroll_lbx_file__ovr118__1, 8, _screen_seg);
    /* The BIGBOOK background is stored in VGA frame 3; copy it to the current buffer */
    Copy_Back_To_Off();
    /* Draw navigation dog-ears if in apprentice book mode (g_spellbook_mode > 1) */
    if(g_spellbook_mode > 1)
    {
        /* Left dog-ear (previous page) */
        if(g_spellbook_left_page != 0)
        {
            FLIC_Draw(15, 9, _spellbook_big_left_corner_seg);
        }
        /* Right dog-ear (next page) */
        if(g_spellbook_left_page != m_spellbook_page_count && g_spellbook_left_page < (m_spellbook_page_count - 2))
        {
            FLIC_Draw(289, 9, _spellbook_big_right_corner_seg);
        }
    }
    /* Compose and draw the left page content */
    BigBook_Compose(g_spellbook_left_page, pict_seg, 1);
    Draw_Picture_Windowed(25, -20, pict_seg);
    /* Compose and draw the right page content */
    BigBook_Compose(g_spellbook_left_page + 1, pict_seg, 0);
    Draw_Picture_Windowed(173, -20, pict_seg);
    /* If choosing research (g_spellbook_mode == 0), display the prompt text */
    if(g_spellbook_mode == 0)
    {
        Set_Font_Style_Outline_Heavy(5, 9, 0, 0);
        Set_Outline_Color(1);
        Print(43, 183, cnst_ChooseResearch);
    }
    /* Release the temporary allocation block */
    Release_Block(_screen_seg);
}


// WZD o118p03
void Learn_Spell_Animation_Draw(void)
{
    Copy_Back_To_Off();
    FLIC_Draw(149, 133, wizlab_podium_seg);
    Reset_Animation_Frame(wizlab_wizard_seg);
    FLIC_Draw(69, 75, wizlab_wizard_seg);
    FLIC_Draw(190, 160, wizlab_familiar_seg);
    Clipped_Draw(132, -3, wizlab_blue_column_seg);
}


// WZD o118p04
// ¿ MoO2  Cls() || Cls_On ?
void Fill_Black_Draw(void)
{
    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_BLACK);
}


// WZD o118p05
void Learn_Spell_Animation_Load(void)
{
    int16_t magic_realm = 0;
    magic_realm = _players[HUMAN_PLAYER_IDX].Prim_Realm;
    // WIZLAB.LBX, 000    "MERLIN"      ""
    // WIZLAB.LBX, 001    "RAVEN"       ""
    // WIZLAB.LBX, 002    "SHAREE"      ""
    // WIZLAB.LBX, 003    "LOPAN"       ""
    // WIZLAB.LBX, 004    "JAFAR"       ""
    // WIZLAB.LBX, 005    "OBERIC"      ""
    // WIZLAB.LBX, 006    "RJAK"        ""
    // WIZLAB.LBX, 007    "SSSRA"       ""
    // WIZLAB.LBX, 008    "TAURON"      ""
    // WIZLAB.LBX, 009    "FREYA"       ""
    // WIZLAB.LBX, 010    "HORUS"       ""
    // WIZLAB.LBX, 011    "ARIEL"       ""
    // WIZLAB.LBX, 012    "TLALOC"      ""
    // WIZLAB.LBX, 013    "KALI"        ""
    wizlab_wizard_seg = LBX_Reload(wizlab_lbx_file__ovr118, _players[HUMAN_PLAYER_IDX].wizard_id, _screen_seg);
    Load_Palette_From_Animation(wizlab_wizard_seg);
    // WIZLAB.LBX, 019    "WIZLAB"      ""
    wizlab_background_seg = LBX_Reload(wizlab_lbx_file__ovr118, 19, _screen_seg);
    Set_Page_Off();
    FLIC_Draw(0, 0, wizlab_background_seg);
    Copy_Off_To_Back();
    Set_Page_On();
    // WIZLAB.LBX, 020    "PODIUM"      ""
    wizlab_podium_seg = LBX_Reload(wizlab_lbx_file__ovr118, 20, _screen_seg);
    // WIZLAB.LBX, 014    "FAMILIAR"    "green"
    // WIZLAB.LBX, 015    "FAMILIAR"    "blue"
    // WIZLAB.LBX, 016    "FAMILIAR"    "red"
    // WIZLAB.LBX, 017    "FAMILIAR"    "white"
    // WIZLAB.LBX, 018    "FAMILIAR"    "black"
    wizlab_familiar_seg = LBX_Reload_Next(wizlab_lbx_file__ovr118, (14 + magic_realm), _screen_seg);
    // WIZLAB.LBX, 021    "BLUECOL"     ""
    wizlab_blue_column_seg = LBX_Reload_Next(wizlab_lbx_file__ovr118, 21, _screen_seg);
    // WIZLAB.LBX, 000    "MERLIN"      ""
    // WIZLAB.LBX, 001    "RAVEN"       ""
    // WIZLAB.LBX, 002    "SHAREE"      ""
    // WIZLAB.LBX, 003    "LOPAN"       ""
    // WIZLAB.LBX, 004    "JAFAR"       ""
    // WIZLAB.LBX, 005    "OBERIC"      ""
    // WIZLAB.LBX, 006    "RJAK"        ""
    // WIZLAB.LBX, 007    "SSSRA"       ""
    // WIZLAB.LBX, 008    "TAURON"      ""
    // WIZLAB.LBX, 009    "FREYA"       ""
    // WIZLAB.LBX, 010    "HORUS"       ""
    // WIZLAB.LBX, 011    "ARIEL"       ""
    // WIZLAB.LBX, 012    "TLALOC"      ""
    // WIZLAB.LBX, 013    "KALI"        ""
    wizlab_wizard_seg = LBX_Reload_Next(wizlab_lbx_file__ovr118, _players[HUMAN_PLAYER_IDX].wizard_id, _screen_seg);
}


// WZD o118p06
void Learn_Spell_Animation(int16_t spell_idx, int16_t research_flag)
{
    char spell_description[LEN_SPELL_DESCRIPTION] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t colors[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char spell_name[LEN_SPELL_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    SAMB_ptr learn_spell_notify_seg = 0;
    int16_t research_candidate_count = 0;
    int16_t input_field_idx = 0;
    char buffer[LEN_TEMP_BUFFER] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t full_screen_esc = 0;
    int16_t spell_found = 0;
    int16_t spell_page_side = 0;
    int16_t casting_cost = 0;
    int16_t itr = 0;
    int16_t spellbook_page = 0;
    int16_t itr_spellbook_page_spell_count = 0;
    g_active_spell_idx = spell_idx;
    if(magic_set.spell_animations != ST_TRUE)
    {
        Reset_First_Block(_screen_seg);
        Allocate_Reduced_Map();
        // SPELLSCR.LBX, 060  "SUMGREEN"    ""
        learn_spell_notify_seg = LBX_Reload_Next(spellscr_lbx_file__ovr118, 60, _screen_seg);
        Assign_Auto_Function(Fill_Black_Draw, 2);
        Set_Page_Off();
        Main_Screen_Draw();
        stu_strcpy(GUI_NearMsgString, cnst_NewSpell_Msg);  // "You have mastered the spell of "
        stu_strcpy(spell_name, spell_data_table[spell_idx].name);
        stu_strcat(GUI_NearMsgString, spell_name);
        Notify2(0, 40, 2, GUI_NearMsgString, 0, learn_spell_notify_seg, 0, 9, 0, 0, 0, 1, 0);
        Deactivate_Auto_Function();
    }
    else
    {
        if(magic_set.background_music == ST_TRUE)
        {
            Stop_All_Sounds__STUB();
            SND_Spell_Music = LBX_Reload(music_lbx_file__ovr118, MUSIC_New_Spell, SND_Music_Segment);
            SND_Spell_Music_size = lbxload_entry_length;
            Play_Sound(SND_Spell_Music, SND_Spell_Music_size);
        }
        Fade_Out();
        Set_Mouse_List(1, mouse_list_none);
        Learn_Spell_Animation_Load();
        Set_Page_Off();
        Learn_Spell_Animation_Draw();
        _page_flip_effect = pfe_TogglePagesFadeIn;
        PageFlip_FX();
        Clear_Fields();
        /* OGBUG: spellbook_page is promptly overwritten and never tested against input */
        spellbook_page = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, str_empty_string__ovr118[0], ST_UNDEFINED);
        Assign_Auto_Function(Learn_Spell_Animation_Draw, 2);
        for(itr = 0; ((itr < 60) && (Get_Input() == ST_FALSE)); itr++)
        {
            Mark_Time();
            Set_Page_Off();
            Learn_Spell_Animation_Draw();
            PageFlip_FX();
            Release_Time(2);
        }
        Clear_Fields();
        Assign_Auto_Function(Fill_Black_Draw, 3);
        Fade_Out();
        Set_Page_Off();
        Fill_Black_Draw();
        PageFlip_FX();
        Load_Palette(0, -1, ST_NULL);  // ; EMPERATO - main game palette
        Reset_Cycle_Palette_Color();
        Set_Palette_Changes(0, 254);
        Update_Remap_Color_Range(10, 11);
        Update_Remap_Color_Range(0, 0);
        Build_Spellbook(slt_Library, 4);
        // SCROLL.LBX, 006  "BIGBOOK"   ""
        _spellbook_big_seg = LBX_Reload(scroll_lbx_file__ovr118__2, 6, _screen_seg);
        Set_Page_Off();
        FLIC_Draw(0, 0, _spellbook_big_seg);
        Copy_Off_To_Back();
        Set_Page_On();
        spell_found = ST_FALSE;
        for(spellbook_page = 0; (((m_spellbook_page_count - 2) > spellbook_page) && (spell_found == ST_FALSE)); spellbook_page++)
        {
            for(itr_spellbook_page_spell_count = 0; ((m_spellbook_pages[spellbook_page].count > itr_spellbook_page_spell_count) && (spell_found == ST_FALSE)); itr_spellbook_page_spell_count++)
            {
                if(abs(m_spellbook_pages[spellbook_page].spell[itr_spellbook_page_spell_count]) == spell_idx)
                {
                    if((spellbook_page % 2) == 0)
                    {
                        g_spellbook_left_page = spellbook_page;
                        spell_page_side = 0;
                        g_spellbook_mode = 0;
                    }
                    else
                    {
                        spell_page_side = 1;
                        g_spellbook_mode = 1;
                        g_spellbook_left_page = (spellbook_page - 1);
                    }
                    m_spell_list_count = itr_spellbook_page_spell_count;
                    spell_found = ST_TRUE;
                }
            }
        }
        /* OGBUG: spell_found is always true; Player_Gets_Spell() marks the spell Known before the Learn_Spell_Animation() call */
        if(spell_found == ST_FALSE)
        {
            /* OGBUG: should be (spell_idx - 1), not (spell_idx - 2) */
            _players[HUMAN_PLAYER_IDX].spells_list[((((spell_idx - 2) / NUM_SPELLS_PER_MAGIC_REALM) * NUM_SPELLS_PER_MAGIC_REALM) + ((spell_idx - 1) % NUM_SPELLS_PER_MAGIC_REALM))] = 1;  /* S_Knowable */
            Build_Spellbook(slt_Library, 4);
            for(spellbook_page = 0; (((m_spellbook_page_count - 1) > spellbook_page) && (spell_found == ST_FALSE)); spellbook_page++)
            {
                for(itr_spellbook_page_spell_count = 0; ((m_spellbook_pages[spellbook_page].count > itr_spellbook_page_spell_count) && (spell_found == ST_FALSE)); itr_spellbook_page_spell_count++)
                {
                    if(abs(m_spellbook_pages[spellbook_page].spell[itr_spellbook_page_spell_count]) == spell_idx)
                    {
                        if((spellbook_page % 2) == 0)
                        {
                            g_spellbook_left_page = spellbook_page;
                            spell_page_side = 0;
                            g_spellbook_mode = 0;
                        }
                        else
                        {
                            spell_page_side = 1;
                            g_spellbook_mode = 1;
                            g_spellbook_left_page = (spellbook_page - 1);
                        }
                        m_spell_list_count = itr_spellbook_page_spell_count;
                        spell_found = ST_TRUE;
                    }
                }
            }
        }
        Reset_First_Block(_screen_seg);
        for(spellbook_page = 0; spellbook_page < NUM_SPELLBOOK_DESCRIPTIONS; spellbook_page++)
        {
            g_spellbook_descriptions[spellbook_page] = (char *)Allocate_Next_Block(_screen_seg, 12);
        }
        BigBook_Load_Spell_Descriptions(g_spellbook_left_page);
        Mark_Block(_screen_seg);
        g_gui_scratch_bitmap = Allocate_Next_Block(_screen_seg, 1800);  // 1800 PR, 28800 B
        spl_anim_compose_seg = Allocate_Next_Block(_screen_seg, 325);  // 325 PR, 5200 B
        // SPECFX.LBX, 049  "NEWSPELL"  ""
        spell_animation_seg = LBX_Reload_Next(specfx_lbx_file__ovr118, 49, _screen_seg);
        stu_strcpy(spell_description, g_spellbook_descriptions[((spell_page_side * 4) + itr_spellbook_page_spell_count - 1)]);
        Create_Picture(129, 37, spl_anim_compose_seg);
        casting_cost = Casting_Cost(HUMAN_PLAYER_IDX, g_active_spell_idx, ST_FALSE);
        for(itr = 0; itr < 16; itr++)
        {
            colors[itr] = 198;
        }
        Set_Font_Colors_15(7, &colors[0]);
        stu_strcpy(spell_name, spell_data_table[g_active_spell_idx].name);
        Print_To_Bitmap(0, 0, spell_name, spl_anim_compose_seg);
        Set_Font_Colors_15(6, &colors[0]);
        Print_To_Bitmap(0, 11, cnst_SP_Cost, spl_anim_compose_seg);
        stu_itoa(casting_cost, buffer, 10);
        Print_Right_To_Bitmap(42, 11, buffer, spl_anim_compose_seg);
        Set_Font_LF(0);
        Set_Font_Spacing(1);
        Print_Paragraph_To_Bitmap(0, 17, 128, spell_description, 0, spl_anim_compose_seg);
        Clear_Fields();
        g_spellbook_anim_stage = 0;
        full_screen_esc = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_UNDEFINED, ST_UNDEFINED);
        input_field_idx = ST_FALSE;
        Assign_Auto_Function(Learn_Spell_Animation_Decode_Draw, 2);
        for(g_spellbook_anim_stage = 0; ((g_spellbook_anim_stage < 45) && (input_field_idx == ST_FALSE)); g_spellbook_anim_stage++)
        {
            input_field_idx = Get_Input();
            if(g_spellbook_anim_stage < 15)
            {
                Cycle_Palette_Color(198, 18, 14, 9, 38, 38, 53, 2);
            }
            Set_Page_Off();
            Learn_Spell_Animation_Decode_Draw();
            PageFlip_FX();
            if(g_spellbook_anim_stage == 0)
            {
                _page_flip_effect = pfe_None;
            }
        }
        Release_Block(_screen_seg);
        Deactivate_Auto_Function();
        if(research_flag == ST_TRUE)
        {
            research_candidate_count = 0;
            spellbook_page = 0;
            while(spellbook_page < NUM_RESEARCH_SPELLS)
            {
                if(_players[HUMAN_PLAYER_IDX].research_spells[spellbook_page] > 0)
                {
                    research_candidate_count++;
                }
                spellbook_page++;
            }
            if(research_candidate_count == 0)
            {
                Stop_All_Sounds__STUB();
                Play_Background_Music();
                return;
            }
            if(g_spellbook_left_page < g_first_research_page)
            {
                BigBook_Load_Spell_Descriptions(g_spellbook_left_page + 2);
                BigBook_PageTurn(1);
                g_spellbook_left_page += 2;
                spellbook_page = 0;
                while(spellbook_page < NUM_RESEARCH_SPELLS)
                {
                    stu_strcpy(g_spellbook_descriptions[spellbook_page], g_spellbook_descriptions[spellbook_page + 8]);
                    spellbook_page++;
                }
                g_spellbook_mode = 1;
                Set_Page_Off();
                BigBook_Draw();
                PageFlip_FX();
            }
            if(g_spellbook_left_page < g_first_research_page)
            {
                BigBook_Load_Spell_Descriptions(g_first_research_page);
                BigBook_PageTurn(666);
                g_spellbook_left_page = g_first_research_page;
                spellbook_page = 0;
                while(spellbook_page < NUM_RESEARCH_SPELLS)
                {
                    stu_strcpy(g_spellbook_descriptions[spellbook_page], g_spellbook_descriptions[spellbook_page + 8]);
                    spellbook_page++;
                }
                g_spellbook_mode = 1;
                Set_Page_Off();
                BigBook_Draw();
                PageFlip_FX();
            }
            Set_Palette_Changes(0, 224);
            Update_Remap_Color_Range(10, 11);
        }
        g_spellbook_mode = 1;
        Set_Page_Off();
        BigBook_Draw();
        Set_Mouse_List(1, mouse_list_default);
        PageFlip_FX();
    }
}


// WZD o118p07
void Learn_Spell_Animation_Decode_Draw(void)
{
    int16_t spell_decode[34] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    memcpy(&spell_decode[28], &g_spell_decode_x_r, 12);
    memcpy(&spell_decode[22], &g_spell_decode_x_l, 12);
    Copy_Back_To_Off();
    if(g_spellbook_mode == 1)  /* research spell in on left page */
    {
        BigBook_Compose(g_spellbook_left_page, g_gui_scratch_bitmap, ST_TRUE);
        Draw_Picture_Windowed(25, -20, g_gui_scratch_bitmap);
    }
    else  /* research spell in on right page */
    {
        BigBook_Compose((g_spellbook_left_page + 1), g_gui_scratch_bitmap, ST_FALSE);
        Draw_Picture_Windowed(173, -20, g_gui_scratch_bitmap);
    }
    if(g_spellbook_mode == 0)
    {
        BigBook_Compose(g_spellbook_left_page, g_gui_scratch_bitmap, ST_TRUE);
    }
    else
    {
        BigBook_Compose((g_spellbook_left_page + 1), g_gui_scratch_bitmap, ST_FALSE);
    }                          
    if(g_spellbook_anim_stage < 22)
    {
        Clear_Bitmap_Region(0, (48 + (GUI_Multipurpose_Int * 37)), 129, (82 + (GUI_Multipurpose_Int * 37)), g_gui_scratch_bitmap);
    }
    else if(g_spellbook_anim_stage < 28)
    {
        Clear_Bitmap_Region(                                       0, (48 + (GUI_Multipurpose_Int * 37)), spell_decode[g_spellbook_anim_stage + 6], (82 + (GUI_Multipurpose_Int * 37)), g_gui_scratch_bitmap);
        Clear_Bitmap_Region(    spell_decode[g_spellbook_anim_stage], (48 + (GUI_Multipurpose_Int * 37)),                                      129, (82 + (GUI_Multipurpose_Int * 37)), g_gui_scratch_bitmap);
        Clear_Bitmap_Region(spell_decode[g_spellbook_anim_stage + 6],                                  0,     spell_decode[g_spellbook_anim_stage],                                 37, spl_anim_compose_seg);
    }
    if(g_spellbook_mode == 0)
    {
        Draw_Picture_Windowed(25, -20, g_gui_scratch_bitmap);
        if(g_spellbook_anim_stage < 27)
        {
            Draw_Picture(25, (28 + (GUI_Multipurpose_Int * 37)), spl_anim_compose_seg);
        }
        if(
            (g_spellbook_anim_stage > 15)
            &&
            (g_spellbook_anim_stage < 29)
        )
        {
            Set_Animation_Frame(spell_animation_seg, (g_spellbook_anim_stage - 16));
            FLIC_Draw(20, (21 + (GUI_Multipurpose_Int * 37)), spell_animation_seg);
        }
    }
    else
    {
        Draw_Picture_Windowed(173, -20, g_gui_scratch_bitmap);
        if(g_spellbook_anim_stage < 27)
        {
            Draw_Picture(173, (28 + (GUI_Multipurpose_Int * 37)), spl_anim_compose_seg);
        }
        if(
            (g_spellbook_anim_stage > 15)
            &&
            (g_spellbook_anim_stage < 29)
        )
        {
            Set_Animation_Frame(spell_animation_seg, (g_spellbook_anim_stage - 16));
            FLIC_Draw(168, (24 + (GUI_Multipurpose_Int * 37)), spell_animation_seg);
        }
    }
}


// WZD o118p08
void BigBook_Load_Spell_Descriptions(int16_t page)
{
    char buffer[LEN_SPELL_DESCRIPTION] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t itr = 0;
    int16_t count = 0;
    for(itr = 0; itr < NUM_SPELLBOOK_DESCRIPTIONS; itr++)
    {
        stu_strcpy(g_spellbook_descriptions[itr], str_empty_string__ovr118);
    }
    for(itr = g_spellbook_left_page; (g_spellbook_left_page + 1) >= itr; itr++)
    {
        if(
            (itr < 0)
            ||
            (itr >= m_spellbook_page_count)
        )
        {
            for(count = 0; count < NUM_SPELLS_PER_PAGE_BIG; count++)
            {
                stu_strcpy(g_spellbook_descriptions[(((itr - g_spellbook_left_page) * 4) + count)], str_empty_string__ovr118);
            }
        }
        else
        {
            for(count = 0; count < NUM_SPELLS_PER_PAGE_BIG; count++)
            {
                if(m_spellbook_pages[itr].count > count)
                {
                    LBX_Load_Data_Static(desc_lbx_file__ovr118, 0, (SAMB_ptr)buffer, abs(m_spellbook_pages[itr].spell[count]), 1, 110);
                    stu_strcpy(g_spellbook_descriptions[(((itr - g_spellbook_left_page) * 4) + count)], buffer);
                }
                else
                {
                    stu_strcpy(g_spellbook_descriptions[(((itr - g_spellbook_left_page) * 4) + count)], str_empty_string__ovr118);
                }
            }
        }
    }
    for(itr = page; (page + 1) >= itr; itr++)
    {
        if(
            (itr < 0)
            ||
            (itr >= m_spellbook_page_count)
        )
        {
            for(count = 0; count < NUM_SPELLS_PER_PAGE_BIG; count++)
            {
                stu_strcpy(g_spellbook_descriptions[((((itr - page) + 2) * 4) + count)], str_empty_string__ovr118);
            }
        }
        else
        {
            for(count = 0; count < NUM_SPELLS_PER_PAGE_BIG; count++)
            {
                if(m_spellbook_pages[itr].count > count)
                {
                    LBX_Load_Data_Static(desc_lbx_file__ovr118, 0, (SAMB_ptr)buffer, abs(m_spellbook_pages[itr].spell[count]), 1, 110);
                    stu_strcpy(g_spellbook_descriptions[((((itr - page) + 2) * 4) + count)], buffer);
                }
                else
                {
                    stu_strcpy(g_spellbook_descriptions[((((itr - page) + 2) * 4) + count)], str_empty_string__ovr118);
                }
            }
        }
    }
}


// WZD o118p09
void Apprentice_Screen(void)
{
    int16_t dogear_left_field = 0;
    int16_t dogear_right_field = 0;
    int16_t full_window_esc = 0;
    int16_t input_field_idx = 0;
    int16_t itr = 0;
    int16_t leave_screen = 0;
    Reset_Cycle_Palette_Color();
    Build_Spellbook(slt_Library, 4);  // spell_list_type, page_spell_count
    g_spellbook_left_page = g_first_research_page;
    g_spellbook_mode = 2;
    // SCROLL.LBX, 006  "BIGBOOK"   ""
    _spellbook_big_seg = LBX_Reload(scroll_lbx_file__ovr118__2, 6, _screen_seg);
    Set_Page_Off();
    FLIC_Draw(0, 0, _spellbook_big_seg);
    Copy_Off_To_Back();
    Set_Page_On();
    // "big book Help"
    LBX_Load_Data_Static(hlpentry_lbx_file__ovr118, 24, (SAMB_ptr)&_help_entries[0], 0, 10, 10);
    Reset_First_Block(_screen_seg);
    for(itr = 0; itr < 16; itr++)
    {
        g_spellbook_descriptions[itr] = (char *)Allocate_Next_Block(_screen_seg, 9);
    }
    BigBook_Load_Spell_Descriptions(g_spellbook_left_page);
    Assign_Auto_Function(BigBook_Draw, 2);
    Clear_Fields();
    dogear_left_field = Add_Hidden_Field(15, 7, 30, 22, str_empty_string__ovr118[0], ST_UNDEFINED);
    dogear_right_field = Add_Hidden_Field(290, 7, 305, 22, str_empty_string__ovr118[0], ST_UNDEFINED);
    full_window_esc = Add_Hidden_Field(SCREEN_XMIN, 25, SCREEN_XMAX, SCREEN_YMAX, str_hotkey_ESC__ovr118[0], ST_UNDEFINED);
    leave_screen = ST_FALSE;
    Set_Help_List((char *)&_help_entries[0], 10);
    while(leave_screen == ST_FALSE)
    {
        input_field_idx = Get_Input();
        for(itr = 0; itr < 4; itr++)
        {
            if(
                (m_spellbook_pages[g_spellbook_left_page].count > itr)
                &&
                (m_spellbook_pages[g_spellbook_left_page].spell[itr] > 0)
            )
            {
                if(abs(m_spellbook_pages[g_spellbook_left_page].spell[itr]) == spl_Spell_Of_Return)
                {
                    _help_entries[(2 + itr)].help_idx = HLP_SPELL_OF_RETURN;
                }
                else
                {
                    _help_entries[(2 + itr)].help_idx = abs(m_spellbook_pages[g_spellbook_left_page].spell[itr]);
                }
            }
            else
            {
                _help_entries[(2 + itr)].help_idx = ST_UNDEFINED;
            }
        }
        for(itr = 0; itr < 4; itr++)
        {
            if(
                (m_spellbook_pages[(g_spellbook_left_page + 1)].count > itr)
                &&
                (m_spellbook_pages[g_spellbook_left_page + 1].spell[itr] > 0)
            )
            {
                if(abs(m_spellbook_pages[g_spellbook_left_page + 1].spell[itr]) == spl_Spell_Of_Return)
                {
                    _help_entries[(6 + itr)].help_idx = HLP_SPELL_OF_RETURN;
                }
                else
                {
                    _help_entries[(6 + itr)].help_idx = abs(m_spellbook_pages[g_spellbook_left_page + 1].spell[itr]);
                }
            }
            else
            {
                _help_entries[(6 + itr)].help_idx = ST_UNDEFINED;
            }
        }
        if(input_field_idx == full_window_esc)
        {
            leave_screen = ST_TRUE;
        }
        if(input_field_idx == dogear_right_field)
        {
            if((m_spellbook_page_count - 2) > g_spellbook_left_page)
            {
                BigBook_Load_Spell_Descriptions((g_spellbook_left_page + 2));
                BigBook_PageTurn(1);
                g_spellbook_left_page += 2;
                for(itr = 0; itr < 8; itr++)
                {
                    stu_strcpy(g_spellbook_descriptions[(0 + itr)], g_spellbook_descriptions[(8 + itr)]);
                }
                Set_Page_Off();
                BigBook_Draw();
                PageFlip_FX();
            }
        }
        if(input_field_idx == dogear_left_field)
        {
            if(g_spellbook_left_page > 1)
            {
                BigBook_Load_Spell_Descriptions((g_spellbook_left_page - 2));
                BigBook_PageTurn(0);
                g_spellbook_left_page -= 2;
                for(itr = 0; itr < 8; itr++)
                {
                    stu_strcpy(g_spellbook_descriptions[(0 + itr)], g_spellbook_descriptions[(8 + itr)]);
                }
                Set_Page_Off();
                BigBook_Draw();
                PageFlip_FX();
            }
        }
        if(leave_screen == ST_FALSE)
        {
            Set_Page_Off();
            BigBook_Draw();
            PageFlip_FX();
        }
    }
    g_first_research_page = g_spellbook_left_page;
    Deactivate_Help_List();
    Deactivate_Auto_Function();
}


// WZD o118p10
void BigBook_Compose(int16_t page, SAMB_ptr pict_seg, int16_t flag)
{
    char spell_name[LEN_SPELL_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char spell_description[LEN_SPELL_DESCRIPTION] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t colors[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t description_batch = 0;
    int32_t research_cost = 0;
    int16_t turns_left = 0;
    int16_t casting_cost = 0;
    int16_t itr_spell_book_descriptions_count = 0;
    int16_t itr = 0;
    description_batch = (page - g_spellbook_left_page);
    if(description_batch < 0)
    {
        description_batch = (abs(description_batch) + ((description_batch + 2) * 2));
    }
    if(description_batch > 2)
    {
        description_batch = ((description_batch % 2) + 2);
    }
    colors[0] = 184;
    for(itr = 1; itr < 16; itr++)
    {
        colors[itr] = 186;
    }
    Create_Picture(129, 200, pict_seg);
    for(itr = 0; m_spellbook_pages[page].count > itr; itr++)
    {
        if(m_spellbook_pages[page].spell[itr] == _players[HUMAN_PLAYER_IDX].researching_spell_idx)
        {
            for(itr_spell_book_descriptions_count = 0; itr_spell_book_descriptions_count < 16; itr_spell_book_descriptions_count++)
            {
                colors[itr_spell_book_descriptions_count] = 198;
            }
            Set_Font_Colors_15(4, &colors[0]);
        }
        else
        {
            for(itr_spell_book_descriptions_count = 0; itr_spell_book_descriptions_count < 16; itr_spell_book_descriptions_count++)
            {
                colors[itr_spell_book_descriptions_count] = 186;
            }
            if(m_spellbook_pages[page].spell[itr] < 0)
            {
                    Set_Font_Colors_15(7, &colors[0]);
            }
            else
            {
                    Set_Font_Colors_15(4, &colors[0]);
            }
        }
        stu_strcpy(spell_name, spell_data_table[abs(m_spellbook_pages[page].spell[itr])].name);
        Print_To_Bitmap(0, (48 + (37 * itr)), spell_name, pict_seg);
        if(m_spellbook_pages[page].spell[itr] == _players[HUMAN_PLAYER_IDX].researching_spell_idx)
        {
            for(itr_spell_book_descriptions_count = 0; itr_spell_book_descriptions_count < 16; itr_spell_book_descriptions_count++)
            {
                colors[itr_spell_book_descriptions_count] = 198;
            }
            Set_Font_Colors_15(0, &colors[0]);
        }
        else
        {
            colors[0] = 184;
            colors[1] = 185;
            if(m_spellbook_pages[page].spell[itr] < 0)
            {
                Set_Font_Colors_15(6, &colors[0]);
            }
            else
            {
                Set_Font_Colors_15(0, &colors[0]);
            }
        }
        if(page < g_first_research_page)
        {
            casting_cost = Casting_Cost(HUMAN_PLAYER_IDX, abs(m_spellbook_pages[page].spell[itr]), ST_FALSE);
            if(g_spellbook_cast_mana_limit == 0)
            {
                turns_left = 999;
            }
            else
            {
                turns_left = (((casting_cost + g_spellbook_cast_mana_limit) - 1) / g_spellbook_cast_mana_limit);
            }
            stu_strcpy(spell_description, cnst_Catsing_Cost);
            stu_itoa(casting_cost, spell_name, 10);
            stu_strcat(spell_description, spell_name);
            stu_strcat(spell_description, cnst_SpaceOpenBrace4);
            stu_itoa(turns_left, spell_name, 10);
            stu_strcat(spell_description, spell_name);
            if(turns_left == 1)
            {
                stu_strcat(spell_description, cnst_RT1turn);
            }
            else
            {
                stu_strcat(spell_description, cnst_RTmoreturns);
            }
        }
        else
        {
            if(m_spellbook_pages[page].spell[itr] == _players[HUMAN_PLAYER_IDX].researching_spell_idx)
            {
                research_cost = _players[HUMAN_PLAYER_IDX].research_cost_remaining;
            }
            else if(abs(m_spellbook_pages[page].spell[itr]) == spl_Spell_Of_Mastery)
            {
                research_cost = _players[HUMAN_PLAYER_IDX].som_research_cost;
            }
            else
            {
                research_cost = spell_data_table[abs(m_spellbook_pages[page].spell[itr])].research_cost;
            }
            /* OGBUG: should be per spell, not per realm; this is an arbitrary estimate */
            if(g_research_income_by_realm[spell_data_table[abs(m_spellbook_pages[page].spell[itr])].magic_realm] == 0)
            {
                turns_left = 999;
            }
            else
            {
                turns_left = (((research_cost + g_research_income_by_realm[spell_data_table[abs(m_spellbook_pages[page].spell[itr])].magic_realm]) - 1) / g_research_income_by_realm[spell_data_table[abs(m_spellbook_pages[page].spell[itr])].magic_realm]);
            }
            stu_strcpy(spell_description, cnst_Research_Cost);
            stu_ltoa(research_cost, spell_name, 10);
            stu_strcat(spell_description, spell_name);
            stu_strcat(spell_description, cnst_SpaceOpenBrace4);
            stu_itoa(turns_left, spell_name, 10);
            stu_strcat(spell_description, spell_name);
            if(turns_left == 1)
            {
                stu_strcat(spell_description, cnst_RT1turn);
            }
            else
            {
                stu_strcat(spell_description, cnst_RTmoreturns);
            }
        }
        Print_To_Bitmap(0, (59 + (37 * itr)), spell_description, pict_seg);
        if(abs(m_spellbook_pages[page].spell[itr]) > 0)
        {
            Set_Font_LF(0);
            Set_Font_Spacing(1);
            stu_strcpy(spell_description, g_spellbook_descriptions[((description_batch * 4) + itr)]);
            Print_Paragraph_To_Bitmap(0, (65 + (37 * itr)), 128, spell_description, 0, pict_seg);  // Align-Left
        }
    }  /* END: for(itr = 0; m_spellbook_pages[page].count > itr; itr++) */
    if(
        (m_spellbook_pages[page].count <= 0)
        &&
        (page < g_first_research_page)
    )
    {
        return;
    }
    colors[ 0] = 184;
    colors[ 8] =  46;
    colors[ 1] =  55;
    colors[ 9] =  46;
    colors[ 2] =  47;
    colors[10] =  46;
    colors[ 3] =  39;
    colors[11] =  46;
    colors[ 4] =  46;
    colors[12] =  46;
    colors[ 5] =  46;
    colors[13] =  46;
    colors[ 6] =  46;
    colors[14] =  46;
    colors[ 7] =  46;
    colors[15] =  46;
    Set_Font_Colors_15(5, &colors[0]);
    if(abs((g_spellbook_left_page - page)) != 2)
    {
        if(page != g_spellbook_left_page)
        {
            if(
                spell_data_table[abs(m_spellbook_pages[page].spell[0])].spell_book_category
                ==
                spell_data_table[abs(m_spellbook_pages[(page - 1)].spell[0])].spell_book_category
            )
            {
                if(page < g_first_research_page)
                {
                    return;
                }
            }
        }
    }
    // "Summoning", "Special Spells", "City Spells", "Enchantment", "Unit Spells", "Combat Spells", "Research", "Spells"
    if(flag == ST_TRUE)
    {
        Print_Centered_To_Bitmap(66, 30, m_spellbook_pages[page].title, pict_seg);
    }
    else
    {
        Print_Centered_To_Bitmap(58, 30, m_spellbook_pages[page].title, pict_seg);
    }
}


// WZD o118p11
// ~ MoO2  Module: TECH  Tech_Select_()
void Spell_Research_Select(void)
{
    int16_t Spell_GUI_Controls[NUM_RESEARCH_SPELLS] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Top_Y = 0;
    int16_t Left_X = 0;
    int16_t input_field_idx = 0;
    int16_t itr = 0;  // _SI_
    int16_t candidate_count = 0;
    int16_t leave_screen = 0;
    leave_screen = ST_FALSE;
    for(itr = 0; itr < NUM_RESEARCH_SPELLS; itr++)
    {
        if(_players[HUMAN_PLAYER_IDX].research_spells[itr] > 0)
        {
            candidate_count++;
        }
    }
    if(candidate_count == 0)
    {
        return;
    }
    // SCROLL.LBX, 006  "BIGBOOK"   ""
    _spellbook_big_seg = LBX_Reload(scroll_lbx_file__ovr118__2, 6, _screen_seg);
    Set_Page_Off();
    FLIC_Draw(0, 0, _spellbook_big_seg);
    Copy_Off_To_Back();
    Build_Spellbook(slt_Library, 4);
    Set_Mouse_List(1, mouse_list_default);
    g_spellbook_mode = 0;
    Reset_First_Block(_screen_seg);
    for(itr = 0; itr < NUM_SPELLBOOK_DESCRIPTIONS; itr++)
    {
        g_spellbook_descriptions[itr] = (char *)Allocate_Next_Block(_screen_seg, 9);  // 9 PR, 144 B
    }
    g_spellbook_left_page = g_first_research_page;
    BigBook_Load_Spell_Descriptions(g_spellbook_left_page);
    Assign_Auto_Function(BigBook_Draw, 2);
    Clear_Fields();
    for(itr = 0; itr < NUM_RESEARCH_SPELLS; itr++)
    {
        Left_X = (25 + ((itr / 4) * 147));
        Top_Y = (25 + ((itr % 4) * 37));
        Spell_GUI_Controls[itr] = Add_Hidden_Field(Left_X, Top_Y, (Left_X + 110), (Top_Y + 33), str_empty_string__ovr118[0], ST_UNDEFINED);
        /* CLAUDE */ LOG_INFO(LOG_CAT_SPELLBOOK, "[RESEARCH] slot=%d field=%d spell=%d rect=(%d,%d)-(%d,%d) size=%dx%d", itr, Spell_GUI_Controls[itr], _players[HUMAN_PLAYER_IDX].research_spells[itr], Left_X, Top_Y, Left_X + 110, Top_Y + 33, 110, 33);
    }
    _page_flip_effect = pfe_Dissolve;
    Set_Page_Off();
    BigBook_Draw();
    PageFlip_FX();
    leave_screen = ST_FALSE;
    LBX_Load_Data_Static(hlpentry_lbx_file__ovr118, 24, (SAMB_ptr)&_help_entries[0], 0, 10, 10);
    Set_Help_List((char *)&_help_entries[0], 10);
    _help_entries[0].help_idx = ST_UNDEFINED;
    _help_entries[1].help_idx = ST_UNDEFINED;
    while(leave_screen == ST_FALSE)
    {
        input_field_idx = Get_Input();
        for(itr = 0; itr < 4; itr++)
        {
            if(
                (m_spellbook_pages[(g_spellbook_left_page + 0)].count > itr)
                &&
                (m_spellbook_pages[(g_spellbook_left_page + 0)].spell[itr] > 0)
            )
            {
                if(abs(m_spellbook_pages[(g_spellbook_left_page + 0)].spell[itr]) == spl_Spell_Of_Return)
                {
                    _help_entries[(2 + itr)].help_idx = HLP_SPELL_OF_RETURN;
                }
                else
                {
                    _help_entries[(2 + itr)].help_idx = abs(m_spellbook_pages[(g_spellbook_left_page + 0)].spell[itr]);
                }
            }
            else
            {
                _help_entries[(2 + itr)].help_idx = ST_UNDEFINED;
            }
        }
        for(itr = 0; itr < 4; itr++)
        {
            if(
                (m_spellbook_pages[(g_spellbook_left_page + 1)].count > itr)
                &&
                (m_spellbook_pages[(g_spellbook_left_page + 1)].spell[itr] > 0)
            )
            {
                if(abs(m_spellbook_pages[(g_spellbook_left_page + 1)].spell[itr]) == spl_Spell_Of_Return)
                {
                    _help_entries[(6 + itr)].help_idx = HLP_SPELL_OF_RETURN;
                }
                else
                {
                    _help_entries[(6 + itr)].help_idx = abs(m_spellbook_pages[(g_spellbook_left_page + 1)].spell[itr]);
                }
            }
            else
            {
                _help_entries[(6 + itr)].help_idx = ST_UNDEFINED;
            }
        }
        for(itr = 0; itr < NUM_RESEARCH_SPELLS; itr++)
        {
            if(Spell_GUI_Controls[itr] == input_field_idx)
            {
                /* CLAUDE */ LOG_INFO(LOG_CAT_SPELLBOOK, "[RESEARCH] CLICK matched slot=%d field=%d -> spell=%d (researching_spell_idx)", itr, input_field_idx, _players[HUMAN_PLAYER_IDX].research_spells[itr]);
                if(_players[HUMAN_PLAYER_IDX].research_spells[itr] > 0)
                {
                    _players[HUMAN_PLAYER_IDX].researching_spell_idx = _players[HUMAN_PLAYER_IDX].research_spells[itr];
                    assert(_players[HUMAN_PLAYER_IDX].researching_spell_idx > 0);
                    leave_screen = ST_TRUE;
                }
            }
        }
        if(leave_screen == ST_FALSE)
        {
            Set_Page_Off();
            BigBook_Draw();
            PageFlip_FX();
        }
    }
    Deactivate_Help_List();
    Deactivate_Auto_Function();
}


// WZD o118p12
// drake178: GAME_CheckResearch()
/*
; checks whether the spell currently being researched
; by the player is already known, and if so, triggers
; the research selection dialog
*/
/*



*/
void Check_Research_Spell_Is_Known(int16_t fade)
{
    int16_t spell_idx = 0;

    spell_idx = abs(_players[HUMAN_PLAYER_IDX].researching_spell_idx);  /* NOTE(JimBalcomb,20260310): this is the only place this ever gets abs()'d */

    if(_players[HUMAN_PLAYER_IDX].spells_list[((((spell_idx - 1) / NUM_SPELLS_PER_MAGIC_REALM) * NUM_SPELLS_PER_MAGIC_REALM) + ((spell_idx - 1) % NUM_SPELLS_PER_MAGIC_REALM))] == sls_Known)
    {

        if(fade == ST_TRUE)
        {

            Set_Page_Off();

            CLROFF()

            Toggle_Pages();

            Copy_On_To_Off_Page();

            Fade_In();

        }

        Spell_Research_Select();
        
        if(_players[HUMAN_PLAYER_IDX].researching_spell_idx == spl_Spell_Of_Mastery)
        {

            _players[HUMAN_PLAYER_IDX].research_cost_remaining = _players[HUMAN_PLAYER_IDX].som_research_cost;

        }
        else
        {

            _players[HUMAN_PLAYER_IDX].research_cost_remaining = spell_data_table[_players[HUMAN_PLAYER_IDX].researching_spell_idx].research_cost;

        }

        if(fade == ST_TRUE)
        {

            Fade_Out();

            CLROFF()

            Toggle_Pages();

            Copy_On_To_Off_Page();

            // DOMSDOS  Load_Palette(0, -1);  // EMPERATO - main game palette
            Load_Palette(0, -1, ST_NULL);

        }

    }

}
