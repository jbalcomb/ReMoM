/*
    WIZARDS.EXE
        ovr117
        ovr118
*/

#include "MOX/MOM_Data.H"
#include "MOX/MOX_DAT.H"  /* _screen_seg */
#include "MOX/MOX_SET.H"  /* magic_set */
#include "MOX/SOUND.H"

#include "MainScr.H"   /* Allocate_Reduced_Map(); */
#include "NEXTTURN.H"
#include "SBookScr.H"  /* Spellbook_Screen_Draw(); */
#include "Spellbook.H"

#include <stdlib.h>     /* abs(); itoa(); malloc(); */

// Combat.C
extern int16_t CMB_combat_structure;



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
char cnst_BookAnim_File[] = "book";

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
struct s_SPELL_DECODE TBL_SpellDecode_Rs = {  53,  45,  37,  16,   6,   6 };
struct s_SPELL_DECODE TBL_SpellDecode_Ls = {  64,  71,  79, 101, 111, 111 };

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
int16_t SBK_Candidate_Page;

// WZD dseg:944A
int16_t SBK_SomePageSaveVar;

// WZD dseg:944C
int16_t CMB_SpellBookPage;

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
int16_t SBK_NewSpellAnim_Stg;

// WZD dseg:C8FE
// drake178: IMG_SBK_BookRealms@ SPBK_ICONS
SAMB_ptr spellbook_symbols_bitm[NUM_MAGIC_REALMS];

// WZD dseg:C90A
// drake178:  SBK_BookManaLimit
/*
; the predicted amount of mana that the wizard will
; be able to channel into a spell each turn?
; (lower of mana+manaperturn and skill+heroskill)
; in combat, the highest castable cost
*/
/*
used to set turns_left for casting
counterpart to SBK_Research_Incomes[] for researching
*/
int16_t SBK_BookManaLimit;

// WZD dseg:C90C
// drake178: NearAlloc_First!
/*

SBK_BuildSpellbook__WIP()
    for(itr1 = 0; itr1 < NUM_MAGIC_REALMS; itr1++)
    {
        research_bonus_percentage = Player_Spell_Research_Bonus(HUMAN_PLAYER_IDX, ((itr1 * 40) + 1));
        SBK_Research_Incomes[itr1] = ((research_points * research_bonus_percentage) / 100);
    }

*/
SAMB_ptr SBK_Research_Incomes;

// WZD dseg:C90E
/*
SBK_BuildSpellbook__WIP()
WIZ_ManaPerTurn = (((_players[HUMAN_PLAYER_IDX].mana_ratio * _players[HUMAN_PLAYER_IDX].Power_Base) / 100) - Player_Armies_And_Enchantments_Mana_Upkeep(HUMAN_PLAYER_IDX));
just used for the calculation for SBK_BookManaLimit
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
int16_t SBK_Dogears;

// WZD dseg:C924
// drake178: ; set to the index + 1 (into the page) of the spell
// drake178: ; being cast while setting up the spellbook display
// drake178: ; in the apprentice, it is then reset to the page count
// drake178: ; without the research candidates
int16_t SBK_OpenPage;

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
char * SBK_Descriptions[NUM_SPELLBOOK_DESCRIPTIONS];

// WZD dseg:C968
SAMB_ptr wizlab_wizard_seg;
// WZD dseg:C96A
SAMB_ptr wizlab_familiar_seg;
// WZD dseg:C96C
SAMB_ptr wizlab_blue_column_seg;
// WZD dseg:C96E
SAMB_ptr wizlab_background_seg;  // DNE in Dasm
SAMB_ptr wizlab_podium_seg;

// WZD dseg:C970 00                                              db    0
// WZD dseg:C971 00                                              db    0

// WZD dseg:C971                                                 END:  ovr118



/*
    WIZARDS.EXE  ovr117
*/

// WZD o117p01
// drake178: SBK_AddHumanPlayer()
/*
; adds the list of the human player's spells into the
; specified buffer, with the count assumed to be stored
; in GUI_Multipurpose_Int, based on type:
;   0 - overland spellbook
;   1 - combat spellbook
;   2 - spprentice spellbook, with knowable spells
;       having their indexes bitwise negated
*/
/*

    set spell indices in spell list
    {0, 1, 2} {overland, combat, apprentice}

type
¿ {0,1,2} ?
0 - known overland spells only
1 - known combat spells only

in combat, if caster is unit/hero, adds its spells to the list after calling this

*/
void Build_Spell_List(int16_t type, int16_t spell_list[])
{
    int16_t spell_idx;
    int16_t itr_realms;  // _CX_
    int16_t itr_spells;  // _SI_
    int16_t spell_status;  // _DI_
    int16_t spells_list_status = 0;  // DNE in Dasm

    for(itr_realms = 0; itr_realms < NUM_MAGIC_REALMS; itr_realms++)
    {

        for(itr_spells = 0; itr_spells < NUM_SPELLS_PER_MAGIC_REALM; itr_spells++)
        {

            spell_status = 0;
            
            spell_idx = ((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + (itr_spells + 1));

            // if(((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr_spells) == spl_Just_Cause)
            // {
            //     STU_DEBUG_BREAK();
            // }
            // spells_list_status = _players[HUMAN_PLAYER_IDX].spells_list[((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr_spells)];
            // spells_list_status = _players[HUMAN_PLAYER_IDX].spells_list[spl_Just_Cause];


            switch(type)
            {

                case slt_Overland:  /* Overland */
                {
                    
                    if(
                        (_players[HUMAN_PLAYER_IDX].spells_list[((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr_spells)] == 2)  /* S_Known */
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
                        (_players[HUMAN_PLAYER_IDX].spells_list[((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr_spells)] == 2)  /* S_Known */
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

                    if(_players[HUMAN_PLAYER_IDX].spells_list[((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr_spells)] > 0)  /* S_Unknown */  /* ¿ knowable or known ? */
                    {
                        
                        if(_players[HUMAN_PLAYER_IDX].spells_list[((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr_spells)] != 2)  /* S_Known */
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
                    // DELETEME  spell_list[GUI_Multipurpose_Int] = -(spell_idx);  /* ¿ researchable ? */
                    spell_list[m_spell_list_count] = -(spell_idx);  /* ¿ researchable ? */
                }
                else
                {
                    // DELETEME  spell_list[GUI_Multipurpose_Int] = spell_idx;  /* ¿ castable ? */
                    spell_list[m_spell_list_count] = spell_idx;  /* ¿ castable ? */
                }

                // DELETEME  GUI_Multipurpose_Int++;
                m_spell_list_count++;

            }

        }

    }

}


// WZD o117p02
// drake178: SBK_AddRealm()
/*
; adds all combat castable spells (eligibility not 1)
; from the selected realm to the specified buffer, with
; its count assumed to be in GUI_Multipurpose_Int
*/
/*
Combat Spellbook Add Spells From Magic Realm

OON XREF
    NX_j_SBK_AddRealm()
    Combat_Spellbook_Build__WIP()
*/
void Combat_Spellbook_Add_Spells_From_Magic_Realm(int16_t * spell_list, int16_t magic_realm)
{
    int16_t zz_flag = 0;
    int16_t realm_idx = 0;
    int16_t itr_spell_per_magic_realm = 0;  // _CX_
    int16_t spell_idx = 0;  // _SI_

    realm_idx = magic_realm;

    for(itr_spell_per_magic_realm = 0; itr_spell_per_magic_realm < NUM_SPELLS_PER_MAGIC_REALM; itr_spell_per_magic_realm++)
    {

        zz_flag = 0;

        spell_idx = ((realm_idx * NUM_SPELLS_PER_MAGIC_REALM) + itr_spell_per_magic_realm + 1);

        if(spell_data_table[spell_idx].Eligibility != 1)  // ¿ knowable ?
        {

            spell_list[m_spell_list_count] = spell_idx;

        }

    }

}


// WZD o117p03
// drake178: SBK_CountSpellGroups()
/*
*/
/*
*/
void Spellbook_Group_Counts(void)
{
    int16_t itr;  // _SI_

    if(magic_set.spell_book_ordering == ST_TRUE)
    {
        // DELETEME  for(itr = 0; itr < GUI_Multipurpose_Int; itr++)
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
        // DELETEME  for(itr = 0; itr < GUI_Multipurpose_Int; itr++)
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
// drake178: SBK_BuildSpellbook()
/*
; creates a list of spells for the specified book type,
; then organizes them into the global near dynamic
; allocation m_spellbook_pages@ (created here)
; Book_Type corresponds to one of the following:
;   0 - overland spellbook
;   1 - combat spellbook
;   2 - Apprentice spellbook
; however, combat doesn't actually use this routine, as
; it has to account for unit spells too
*/
/*
    spell_list_type   {0,1,2}
    page_spell_count   {4,6,8}

spell_list_type only used in call to Build_Spell_List()
(which would seem to mean it is not actually "book type")

*/
void SBK_BuildSpellbook__WIP(int16_t spell_list_type, int16_t page_spell_count)
{
    int16_t hero_casting_skill = 0;
    int16_t itr2 = 0;
    int16_t Total_Pages = 0;
    int16_t bogus_placeholder = 0;  // DNE in Dasm;  uses Total_Pages
    int16_t research_bonus_percentage;  // DNE in Dasm;  uses Total_Pages
    int16_t research_points = 0;
    int16_t itr1 = 0;  // _SI_
    int16_t itr_magic_realms = 0;  // _SI_
    
    // m_spellbook_spell_list = SA_MK_FP0(Allocate_First_Block(_screen_seg, 13));
    m_spellbook_spell_list = (int16_t *)Allocate_First_Block(_screen_seg, 13);  // 13 PR, 208 B

    // ¿ BUG  should be 12 bytes, not 12 paragraphs ?
    SBK_Research_Incomes = Near_Allocate_First(12);  // 12 PR, 192 B

    // spells...divided into categories
    SBK_Group_3_Count = 0;      // City Spells or Death
    SBK_Group_2_Count = 0;      // Special Spells or Chaos
    SBK_Group_1_Count = 0;      // Summoning Spells or Arcane
    SBK_Group_5_Count = 0;      // Enchantments or Life
    SBK_Group_4_Count = 0;      // Unit Spells or Nature
    SBK_Group_6_Count = 0;      // Combat Spells or Sorcery

    // DELETEME  GUI_Multipurpose_Int = 0;   // Spell List Count, New Spell Index, ...
    m_spell_list_count = 0;

    m_spellbook_page_count = 0;
    
    Players_Update_Magic_Power();

// void Player_Magic_Power_Distribution(int16_t * mana_points, int16_t * skill_points, int16_t * research_points, int16_t player_idx)
//     *mana_points = mana_portion;
//     *skill_points = skill_portion;
//     *research_points = research_portion;
    Player_Magic_Power_Distribution(&bogus_placeholder, &bogus_placeholder, &research_points, HUMAN_PLAYER_IDX);

    for(itr_magic_realms = 0; itr_magic_realms < NUM_MAGIC_REALMS; itr_magic_realms++)
    {
        // int16_t Player_Spell_Research_Bonus(int16_t player_idx, int16_t spell_idx)
        research_bonus_percentage = Player_Spell_Research_Bonus(HUMAN_PLAYER_IDX, ((itr_magic_realms * NUM_SPELLS_PER_MAGIC_REALM) + 1));  // {1, 41, 81, 121, 161, 201}

        SBK_Research_Incomes[itr_magic_realms] = ((research_points * research_bonus_percentage) / 100);
    }

    _players[HUMAN_PLAYER_IDX].Nominal_Skill = Player_Base_Casting_Skill(HUMAN_PLAYER_IDX);
    
    WIZ_ManaPerTurn = (((_players[HUMAN_PLAYER_IDX].mana_ratio * _players[HUMAN_PLAYER_IDX].Power_Base) / 100) - Player_Armies_And_Enchantments_Mana_Upkeep(HUMAN_PLAYER_IDX));

    hero_casting_skill = Player_Hero_Casting_Skill(HUMAN_PLAYER_IDX);

    // set SBK_BookManaLimit to min
    if((WIZ_ManaPerTurn + _players[HUMAN_PLAYER_IDX].mana_reserve) > (_players[HUMAN_PLAYER_IDX].Nominal_Skill + hero_casting_skill))
    {
        SBK_BookManaLimit = (_players[HUMAN_PLAYER_IDX].Nominal_Skill + hero_casting_skill);
    }
    else if((WIZ_ManaPerTurn + _players[HUMAN_PLAYER_IDX].mana_reserve) > 0)
    {
        SBK_BookManaLimit = (WIZ_ManaPerTurn + _players[HUMAN_PLAYER_IDX].mana_reserve);
    }
    else
    {
        SBK_BookManaLimit = 0;
    }

    // void Build_Spell_List(int16_t spell_list_type, SAMB_ptr SpList_Ptr)
    Build_Spell_List(spell_list_type, m_spellbook_spell_list);

    // void Spellbook_Group_Counts(void)
    Spellbook_Group_Counts();

    Total_Pages =  ((SBK_Group_3_Count + page_spell_count - 1) / page_spell_count);
    Total_Pages += ((SBK_Group_2_Count + page_spell_count - 1) / page_spell_count);
    Total_Pages += ((SBK_Group_1_Count + page_spell_count - 1) / page_spell_count);
    Total_Pages += ((SBK_Group_5_Count + page_spell_count - 1) / page_spell_count);
    Total_Pages += ((SBK_Group_4_Count + page_spell_count - 1) / page_spell_count);
    Total_Pages += ((SBK_Group_6_Count + page_spell_count - 1) / page_spell_count);

    // Severity	Code	Description	Project	File	Line	Suppression State
    // Message	lnt - arithmetic - overflow	A sub - expression may overflow before being assigned to a wider type.ReMoM	C : \STU\devel\ReMoM\src\Spellbook.C	558
    m_spellbook_pages = (struct s_SPELL_BOOK_PAGE *)Near_Allocate_Next(((Total_Pages + 3) * sizeof(struct s_SPELL_BOOK_PAGE)));

    for(itr1 = 0; (Total_Pages + 3) > itr1; itr1++)
    {
        m_spellbook_pages[itr1].count = 0;
        strcpy(m_spellbook_pages[itr1].title, "");

        for(itr2 = 0; itr2 < page_spell_count; itr2++)
        {
            m_spellbook_pages[itr1].spell[itr2] = 0;  /* ¿ NO_SPELL ? */
        }
    }

    // void Spellbook_Add_Group_Pages(int16_t page_spell_count)
    Spellbook_Add_Group_Pages(page_spell_count);

    /*
        BEGIN: "Research Spells"
    */
    if(page_spell_count == NUM_SPELLS_PER_PAGE_BIG)  /* means we're building the "big book" */
    {
        SBK_Candidate_Page = m_spellbook_page_count;

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

        // void Spellbook_Add_Page(int16_t group_spell_count, int16_t group_idx, char * title, int16_t page_spell_count)
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

        // void Spellbook_Add_Page(int16_t group_spell_count, int16_t group_idx, char * title, int16_t page_spell_count)
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
// drake178: CMB_BuildSpellbook()
/*
; creates a combat spellbook for the selected entity
; into the SBK_BookPages@ allocation and performs
; a basic check to see if any of its spells may be cast
; (looks at casting cost and ocean tile eligibility)
; returns 1 if there are spells to cast, or 0 otherwise
; BUGs: uses a signed comparison when adding hero
; spells, resulting in indexes above 127 not being
; included (all Death and most Life spells); returns 1
; if there's an item charge even if it's not usable;
; ignores Evil Omens for units, and can overflow mana
; at the Fortress if above 16383
*/
/*

*/
int16_t Combat_Spellbook_Build__WIP(int16_t caster_idx)
{
    int16_t Charged_Spell_Index = 0;
    int16_t Already_Added = 0;
    int16_t itr_spellbook_spell_list = 0;
    int16_t Can_Cast = 0;
    int16_t Index_on_Page = 0;
    int16_t itr1 = 0;  // _SI_
    int16_t unit_type = 0;  // _DI_
    int16_t total_pages = 0;  // _DI_

    Near_Allocate_Mark();

    Mark_Block(_screen_seg);

    // DOMSDOS  m_spellbook_spell_list = SA_MK_FP0(Allocate_Next_Block(_screen_seg, 13));
    m_spellbook_spell_list = (int16_t *)Allocate_Next_Block(_screen_seg, 13);

    SBK_Group_3_Count = 0;  // ; City Spells or Death
    SBK_Group_2_Count = 0;  // ; Special Spells or Chaos
    SBK_Group_1_Count = 0;  // ; Summoning Spells or Arcane
    SBK_Group_5_Count = 0;  // ; Enchantments or Life
    SBK_Group_4_Count = 0;  // ; Unit Spells or Nature
    SBK_Group_6_Count = 0;  // ; Combat Spells or Sorcery

    // GUI_Multipurpose_Int__spell_list_count = 0;
    m_spell_list_count = 0;

    m_spellbook_page_count = 0;

    if(caster_idx >= CASTER_IDX_BASE)
    {

        unit_type = Combat_Casting_Cost_Multiplier(HUMAN_PLAYER_IDX);

        SBK_BookManaLimit = _players[HUMAN_PLAYER_IDX].Cmbt_Skill_Left;

        // ; BUG: rounds the wrong way
        // ; BUG: x0.5 multiplier results in a negative signed
        // ; value if the player has over 16,383 mana
        if(((_players[HUMAN_PLAYER_IDX].mana_reserve * 10) / unit_type) < SBK_BookManaLimit)
        {
            SBK_BookManaLimit = ((_players[HUMAN_PLAYER_IDX].mana_reserve * 10) / unit_type);
        }

    }
    else
    {

        SBK_BookManaLimit = battle_units[caster_idx].mana;

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

                Already_Added = ST_FALSE;

                // ; BUG: ignores all spells with index > 127
                if(_HEROES2[HUMAN_PLAYER_IDX]->heroes[unit_type].Spells[itr1] > 0)
                {

                    for(itr_spellbook_spell_list = 0; itr_spellbook_spell_list < m_spell_list_count; itr_spellbook_spell_list++)
                    {

                        if(_HEROES2[HUMAN_PLAYER_IDX]->heroes[unit_type].Spells[itr1] == m_spellbook_spell_list[itr_spellbook_spell_list])
                        {

                            Already_Added = ST_TRUE;

                        }

                    }

                    if(Already_Added == ST_FALSE)
                    {

                        m_spellbook_spell_list[m_spell_list_count] = _HEROES2[HUMAN_PLAYER_IDX]->heroes[unit_type].Spells[itr1];

                    }

                }

                if(battle_units[caster_idx].Item_Charges > 0)
                {

                    unit_type = _UNITS[battle_units[caster_idx].unit_idx].Hero_Slot;

                    // ; BUG: this may not be the hero's original owner
                    Charged_Spell_Index = _ITEMS[_players[battle_units[caster_idx].controller_idx].Heroes[unit_type].Items[0]].embed_spell_idx;
                    
                    if(Charged_Spell_Index > 0)
                    {

                        Already_Added = ST_FALSE;

                        for(itr_spellbook_spell_list = 0; itr_spellbook_spell_list < m_spell_list_count; itr_spellbook_spell_list++)
                        {

                            if(m_spellbook_spell_list[itr_spellbook_spell_list] == Charged_Spell_Index)
                            {

                                Already_Added = ST_TRUE;

                            }

                        }

                    }

                }

            }

        }

    }
    else
    {

        Build_Spell_List(slt_Combat, m_spellbook_spell_list);

    }




    
    Can_Cast = ST_FALSE;


    if(caster_idx <= (CASTER_IDX_BASE - 1))
    {

        if(battle_units[caster_idx].Item_Charges > 0)
        {
            // ; BUG: just because it is in an item, the spell may not
            // ; necessarily be castable (e.g. Earth Elemental on a
            // ; water tile)
            Can_Cast = ST_TRUE;
        }
        else
        {
            
            for(itr1 = 0; itr1 < m_spell_list_count; itr1++)
            {

                // ; BUG: ignores Evil Omens
                // ; the predicted amount of mana that the wizard will
                // ; be able to channel into a spell each turn?
                // ; (lower of mana+manaperturn and skill+heroskill)
                // ; in combat, the highest castable cost
                if(spell_data_table[abs(m_spellbook_spell_list[itr1])].casting_cost <= SBK_BookManaLimit)
                {

                    if(
                        !(
                            (spell_data_table[abs(m_spellbook_spell_list[itr1])].type == scc_Summoning)
                            &&
                            (CMB_combat_structure == cs_OceanTerrainType)
                            &&
                            ((_unit_type_table[spell_data_table[abs(m_spellbook_spell_list[itr1])].unit_type].Move_Flags & MV_FLYING) == 0)
                            &&
                            ((_unit_type_table[spell_data_table[abs(m_spellbook_spell_list[itr1])].unit_type].Abilities & UA_NONCORPOREAL) == 0)
                        )
                        &&
                        !(
                            (m_spellbook_spell_list[itr1] == spl_Cracks_Call)  // BUGBUG  should be abs(spell_idx)
                            &&
                            (CMB_combat_structure == cs_OceanTerrainType)
                        )
                    )
                    {
                        Can_Cast = ST_TRUE;
                    }

                }

            }

        }

    }
    else
    {
        
        for(itr1 = 0; itr1 < m_spell_list_count; itr1++)
        {

            if(Casting_Cost((caster_idx - CASTER_IDX_BASE), abs(m_spellbook_spell_list[itr1]), 1) <= SBK_BookManaLimit)
            {

                if(
                    !(
                        (spell_data_table[abs(m_spellbook_spell_list[itr1])].type == scc_Summoning)
                        &&
                        (CMB_combat_structure == cs_OceanTerrainType)
                        &&
                        ((_unit_type_table[spell_data_table[abs(m_spellbook_spell_list[itr1])].unit_type].Move_Flags & MV_FLYING) == 0)
                        &&
                        ((_unit_type_table[spell_data_table[abs(m_spellbook_spell_list[itr1])].unit_type].Abilities & UA_NONCORPOREAL) == 0)
                    )
                    &&
                    !(
                        (m_spellbook_spell_list[itr1] == spl_Cracks_Call)  // BUGBUG  should be abs(spell_idx)
                        &&
                        (CMB_combat_structure == cs_OceanTerrainType)
                    )
                )
                {
                    Can_Cast = ST_TRUE;
                }

            }

        }

    }


// SBK_BuildSpellbook__WIP()
// Total_Pages =  ((SBK_Group_3_Count + page_spell_count - 1) / page_spell_count);
// Total_Pages += ((SBK_Group_2_Count + page_spell_count - 1) / page_spell_count);
// Total_Pages += ((SBK_Group_1_Count + page_spell_count - 1) / page_spell_count);
// Total_Pages += ((SBK_Group_5_Count + page_spell_count - 1) / page_spell_count);
// Total_Pages += ((SBK_Group_4_Count + page_spell_count - 1) / page_spell_count);
// Total_Pages += ((SBK_Group_6_Count + page_spell_count - 1) / page_spell_count);

    Spellbook_Group_Counts();

total_pages =  ((SBK_Group_3_Count + 5) / NUM_SPELLS_PER_PAGE_SML);
total_pages += ((SBK_Group_2_Count + 5) / NUM_SPELLS_PER_PAGE_SML);
total_pages += ((SBK_Group_1_Count + 5) / NUM_SPELLS_PER_PAGE_SML);
total_pages += ((SBK_Group_5_Count + 5) / NUM_SPELLS_PER_PAGE_SML);
total_pages += ((SBK_Group_4_Count + 5) / NUM_SPELLS_PER_PAGE_SML);
total_pages += ((SBK_Group_6_Count + 5) / NUM_SPELLS_PER_PAGE_SML);


    m_spellbook_pages = (struct s_SPELL_BOOK_PAGE *)CMB_NearBuffer_3;

    // ¿ ~ Spellbook_Add_Page() ?
    for(itr1 = 0; (total_pages + 3) > itr1; itr1++)
    {

        m_spellbook_pages[itr1].count = 0;

        strcpy(m_spellbook_pages[itr1].title, str_empty_string__ovr117);

        for(Index_on_Page = 0; Index_on_Page < NUM_SPELLS_PER_PAGE_SML; Index_on_Page++)
        {

            m_spellbook_pages[itr1].spell[Index_on_Page] = spl_NONE;

        }

    }

    
    Spellbook_Add_Group_Pages(NUM_SPELLS_PER_PAGE_SML);

    Release_Block(_screen_seg);

    if(CMB_SpellBookPage > m_spellbook_page_count)
    {

        CMB_SpellBookPage = (m_spellbook_page_count - 2);

    }

    return Can_Cast;

}


// WZD o117p07
// drake178: OVL_ComposeBookText()
void SmlBook_Compose__WIP(struct s_SPELL_BOOK_PAGE spell_book_page, SAMB_ptr spellbook_bitmap)
{
    char spell_name[LEN_SPELL_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char temp_string[LEN_TEMP_BUFFER] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Text_Width = 0;
    int16_t Turns_To_Cast = 0;
    int16_t Cost_Limit = 0;
    int16_t casting_cost = 0;
    int16_t spell_idx = 0;
    uint8_t colors[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t itr1 = 0;  // _SI_
    int16_t itr2 = 0;  // _DI_

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

            if(SBK_BookManaLimit != 0)
            {
                Turns_To_Cast = (casting_cost / SBK_BookManaLimit);
            }
            else
            {
                Turns_To_Cast = 40;
            }

            if(Turns_To_Cast > 40)
            {
                Turns_To_Cast = 40;
            }

            if(Turns_To_Cast < 1)
            {
                Turns_To_Cast = 1;

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
                    if(spell_data_table[abs(spell_idx)].type != 11)  /* scc_Crafting_Spell */
                    {
                        Text_Width = Get_String_Width(cnst_Instant_Cast);
                        Clear_Bitmap_Region(9, (7 + (itr1 * 22)), (12 + Text_Width), (12 + (itr1 * 22)), spellbook_bitmap);
                        Print_To_Bitmap(9, (7 + (itr1 * 22)), cnst_Instant_Cast, spellbook_bitmap);
                    }
                    else
                    {
                        Text_Width = Get_String_Width(cnst_Special_Cost);
                        Clear_Bitmap_Region(9, (7 + (itr1 * 22)), (12 + Text_Width), (12 + (itr1 * 22)), spellbook_bitmap);
                        Print_To_Bitmap(9, (7 + (itr1 * 22)), cnst_Special_Cost, spellbook_bitmap);
                    }
                }

                Clear_Bitmap_Region(0, (7 + (itr1 * 22)), 8, (12 + (itr1 * 22)), spellbook_bitmap);

                Clipped_Copy_Bitmap(1, (7 + (itr1 * 22)), spellbook_bitmap, spellbook_symbols_bitm[spell_data_table[abs(spell_idx)].magic_realm]);
            }
            else  /* (Turns_To_Cast >= 1) */
            {
                if(Turns_To_Cast >= 20)
                {
                    Clear_Bitmap_Region(0, (7 + (itr1 * 22)), 121, (12 + (itr1 * 22)), spellbook_bitmap);
                    Clear_Bitmap_Region(0, (13 + (itr1 * 22)), (3 + ((Turns_To_Cast - 20) * 6)), (19 + (itr1 * 22)), spellbook_bitmap);
                }
                else
                {
                    Clear_Bitmap_Region(0, (7 + (itr1 * 22)), (3 + (Turns_To_Cast * 6)), (12 + (itr1 * 22)), spellbook_bitmap);
                }

                for(itr2 = 0; itr2 < Turns_To_Cast; itr2++)
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

            strcpy(spell_name, spell_data_table[abs(spell_idx)].name);

            Text_Width = Get_String_Width(spell_name);

            Clear_Bitmap_Region(0, (itr1 * 22), (3 + Text_Width), (5 + (itr1 * 22)), spellbook_bitmap);

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
                (casting_cost != 0) ||
                (spell_data_table[abs(spell_idx)].type != 11)  /* scc_Crafting_Spell */
            )
            {
                itoa(casting_cost, temp_string, 10);
            }
            else
            {
                strcpy(temp_string, cnst_QuestionMark5);
            }

            strcat(temp_string, cnst_Space_MP_2);

            Text_Width = Get_String_Width(temp_string);

            Clear_Bitmap_Region((118 - Text_Width), (itr1 * 22), 121, (5 + (itr1 * 22)), spellbook_bitmap);

            Print_Right_To_Bitmap(121, (itr1 * 22), temp_string, spellbook_bitmap);

        }
        else
        {
            Clear_Bitmap_Region(0, (itr1 * 22), 121, (17 + (itr1 * 22)), spellbook_bitmap);
        }

    }

}


// WZD o117p08
// drake178: CMB_ComposeBookText()
/*
; composes the image of the passed spellbook page into
; the specified work area based on caster attributes,
; assuming combat, but omitting the page title, if any
*/
/*

*/
void CmbBook_Compose__WIP(struct s_SPELL_BOOK_PAGE spell_book_page, SAMB_ptr spellbook_bitmap, int16_t caster_idx)
{

    char spell_name[LEN_SPELL_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char temp_string[LEN_TEMP_BUFFER] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Max_Spendable = 0;
    int16_t Icon_Count = 0;
    int16_t casting_cost = 0;
    int16_t spell_idx = 0;
    uint8_t colors[6] = { 0, 0, 0, 0, 0, 0 };

//     int16_t Text_Width = 0;
//     int16_t Turns_To_Cast = 0;
//     int16_t Cost_Limit = 0;
//     int16_t casting_cost = 0;
//     int16_t spell_idx = 0;
    
    int16_t itr1 = 0;  // _SI_
    int16_t itr2 = 0;  // _DI_

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
                    if(casting_cost > SBK_BookManaLimit)
                    {
                        Icon_Count = 0;
                    }
                    else
                    {
                        Icon_Count = (SBK_BookManaLimit / casting_cost);
                    }

                    if (battle_units[caster_idx].Item_Charges > 0)
                    {
                        if (_ITEMS[_players[HUMAN_PLAYER_IDX].Heroes[_UNITS[battle_units[caster_idx].unit_idx].Hero_Slot].Items[0]].embed_spell_idx == spell_idx)
                        {
                            Icon_Count = battle_units[caster_idx].Item_Charges;
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
                        (OVL_Action_Type == 1)
                    )
                )
                {
                    Icon_Count = 0;
                }

                if(
                    (spell_data_table[abs(spell_idx)].type == scc_Summoning)
                    &&
                    (CMB_combat_structure == cs_OceanTerrainType)
                    &&
                    ((_unit_type_table[spell_data_table[abs(spell_idx)].unit_type].Move_Flags & MV_FLYING) == 0)
                    &&
                    ((_unit_type_table[spell_data_table[abs(spell_idx)].unit_type].Abilities & UA_NONCORPOREAL) == 0)
                )
                {
                    Icon_Count = 0;
                }

                if(
                    (spell_idx == spl_Cracks_Call)  // BUGBUG  should be abs(spell_idx)
                    &&
                    (CMB_combat_structure == cs_OceanTerrainType)
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
                        // ; decoded image headers appended to LBX_Sandbox_Segment
                        Clipped_Copy_Bitmap((1 + (itr2 * 6)), (7 + (itr1 * 22)), spellbook_bitmap, spellbook_symbols_bitm[spell_data_table[abs(spell_idx)].magic_realm]);
                    }
                }
                colors[0] = 184;
                for(itr2 = 1; itr2 < 5; itr2++)
                {
                    colors[itr2] = 187;
                }
            }

            Set_Font_Colors_15(1, &colors[0]);

            _fstrcpy(spell_name, spell_data_table[abs(spell_idx)].name);

            Max_Spendable = Get_String_Width(spell_name);

            Clear_Bitmap_Region(0, (itr1 * 22), (3 + Max_Spendable), (5 + (itr1 * 22)), spellbook_bitmap);

            Print_To_Bitmap(0, (itr1 * 22), spell_name, spellbook_bitmap);

            if(casting_cost == 0)
            {
                strcpy(temp_string, cnst_QuestionMark5);
            }
            else
            {
                if(casting_cost == 22222)
                {
                    strcpy(temp_string, cnst_Item);
                }
                else
                {
                    itoa(casting_cost, temp_string, 10);
                    strcat(temp_string, cnst_Space_MP_2);
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
// drake178: OVL_DrawSpellbook()
void SmlBook_Draw__WIP(int16_t x, int16_t y)
{
    SAMB_ptr spellbook_bitmap = 0;
    uint8_t colors[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t itr = 0;  // _SI_

    Mark_Block(_screen_seg);

    spellbook_bitmap = Allocate_Next_Block(_screen_seg, 1050);  // 1050 PR  16800 B

    // {Nature, Sorcery, Chaos, Life, Death, Arcane}
    for(itr = 0; itr < NUM_MAGIC_REALMS; itr++)
    {
        spellbook_symbols_bitm[itr] = Allocate_Next_Block(_screen_seg, 4);  // 4 PR  64 B
        Draw_Picture_To_Bitmap(_spellbook_small_symbols[itr], spellbook_symbols_bitm[itr]);
    }

    Cycle_Palette_Color__STUB(198, 41, 33, 29, 48, 43, 43, 3);

    FLIC_Draw(x, y, _spellbook_small_seg);

    if(SBK_Dogears == 1)
    {

        if(SBK_OpenPage != 0)
        {
            FLIC_Draw((x + 13), (y + 4), _spellbook_small_left_corner_seg);
        }
        
        if(
            (SBK_OpenPage != m_spellbook_page_count)
            &&
            ((m_spellbook_page_count - 2) > SBK_OpenPage)
        )
        {
            FLIC_Draw((x + 258), (y + 4), _spellbook_small_right_corner_seg);
        }
    }

    SmlBook_Compose__WIP(m_spellbook_pages[SBK_OpenPage], spellbook_bitmap);

    Draw_Picture((x + 16), (y + 21), spellbook_bitmap);

    colors[0] = 53;

    for(itr = 1; itr < 5; itr++)
    {
        colors[itr] = 46;
    }

    Set_Font_Colors_15(4, &colors[0]);

    Print_Centered((x + 70), (y + 6), m_spellbook_pages[SBK_OpenPage].title);

    Draw_Picture_To_Bitmap(_spellbook_small_text, spellbook_bitmap);

    SmlBook_Compose__WIP(m_spellbook_pages[(SBK_OpenPage + 1)], spellbook_bitmap);

    Draw_Picture((x + 148), (y + 21), spellbook_bitmap);

    colors[0] = 53;

    for(itr = 1; itr < 5; itr++)
    {
        colors[itr] = 46;
    }

    Set_Font_Colors_15(4, &colors[0]);

    Print_Centered((x + 208), (y + 6), m_spellbook_pages[(SBK_OpenPage + 1)].title);

    Release_Block(_screen_seg);

}


// WZD o117p10
// drake178: CMB_DrawSpellbook()
/*
; draws the combat spellbook into the current draw
; frame using the passed parameters and the related
; global variables
*/
/*

*/
void CmbBook_Draw__WIP(int16_t x, int16_t y, int16_t caster_idx)
{
    uint8_t colors[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t itr = 0;  // _SI_

    // DNE  Mark_Block(_screen_seg);

    // DNE  spellbook_bitmap = Allocate_Next_Block(_screen_seg, 1050);  // 1050 PR  16800 B

    FLIC_Draw(x, y, _spellbook_small_seg);

    if(SBK_Dogears == 1)
    {

        if(SBK_OpenPage != 0)
        {
            FLIC_Draw((x + 13), (y + 4), _spellbook_small_left_corner_seg);
        }
        
        if(
            (SBK_OpenPage != m_spellbook_page_count)
            &&
            ((m_spellbook_page_count - 2) > SBK_OpenPage)
        )
        {
            FLIC_Draw((x + 258), (y + 4), _spellbook_small_right_corner_seg);
        }
    }

    // SmlBook_Compose__WIP(m_spellbook_pages[SBK_OpenPage], spellbook_bitmap);
    // spellbook_bitmap  allocated in SmlBook_Draw__WIP(), passed to SmlBook_Compose__WIP()
    CmbBook_Compose__WIP(m_spellbook_pages[SBK_OpenPage], IMG_SBK_Anims, caster_idx);

    Draw_Picture((x + 16), (y + 21), IMG_SBK_Anims);

    colors[0] = 53;

    for(itr = 1; itr < 5; itr++)
    {
        colors[itr] = 46;
    }

    Set_Font_Colors_15(4, &colors[0]);

    Print_Centered((x + 70), (y + 6), m_spellbook_pages[SBK_OpenPage].title);

    Draw_Picture_To_Bitmap(_spellbook_small_text, IMG_SBK_Anims);

    // SmlBook_Compose__WIP(m_spellbook_pages[(SBK_OpenPage + 1)], spellbook_bitmap);
    CmbBook_Compose__WIP(m_spellbook_pages[(SBK_OpenPage + 1)], IMG_SBK_Anims, caster_idx);

    Draw_Picture((x + 148), (y + 21), IMG_SBK_Anims);

    colors[0] = 53;

    for(itr = 1; itr < 5; itr++)
    {
        colors[itr] = 46;
    }

    Set_Font_Colors_15(4, &colors[0]);

    Print_Centered((x + 208), (y + 6), m_spellbook_pages[(SBK_OpenPage + 1)].title);

    // DNE  Release_Block(_screen_seg);

}


// WZD o117p11
// drake178: SBK_PageFlip_Small()
// BOOK.LBX  "PTURNSML"
void SmlBook_PageTurn__WIP(int turn_type, int combat_flag, int player_idx)
{
    int16_t Stage_Text_Widths[4] = { 0, 0, 0, 0};
    int16_t Book_Image_Top = 0;
    int16_t Book_Image_Left = 0;
    uint8_t colors[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t Color_Loop_Var = 0;
    int16_t PageTurn_Stage = 0;
    SAMB_ptr Base_Page_IMG = 0;
    SAMB_ptr page_turn_bitmap = 0;  // _SI_
    int16_t itr = 0;  // DNE in Dasm

    Book_Image_Left = 16;
    Book_Image_Top = 10;

    Stage_Text_Widths[3] = 85;
    Stage_Text_Widths[0] = 85;
    Stage_Text_Widths[2] = 55;
    Stage_Text_Widths[1] = 55;

    Deactivate_Auto_Function();

    // TODO  Open_File_Animation__STUB(book_lbx_file, 0);

    if(combat_flag == ST_FALSE)
    {
        Allocate_Reduced_Map();
    }

    Mark_Block(_screen_seg);

    Base_Page_IMG = Allocate_Next_Block(_screen_seg, 1040);

    page_turn_bitmap = Allocate_Next_Block(_screen_seg, 1400);

    for(itr = 0; itr < NUM_MAGIC_REALMS; itr++)
    {
        spellbook_symbols_bitm[itr] = Allocate_Next_Block(_screen_seg, 4);
        Draw_Picture_To_Bitmap(_spellbook_small_symbols[itr], spellbook_symbols_bitm[itr]);
    }

    Cycle_Palette_Color__STUB(198, 39, 31, 25, 50, 46, 47, 3);

    for(PageTurn_Stage = 0; PageTurn_Stage < 4; PageTurn_Stage++)
    {

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

    page_spell_counter = 0;

    strcpy(m_spellbook_pages[m_spellbook_page_count].title, title);

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
                m_spellbook_pages[m_spellbook_page_count].count = page_spell_count;

                page_spell_counter = 1;

                m_spellbook_page_count++;

                if((m_spellbook_page_count % 2) == 1)  /* ¿ odd pages ? ¿ {0,1} or {1,2} ? */
                {

                    if(magic_set.spell_book_ordering != ST_TRUE)
                    {
                        if(spell_data_table[m_spellbook_pages[(m_spellbook_page_count - 1)].spell[0]].magic_realm != group_idx)
                        {
                            strcpy(m_spellbook_pages[m_spellbook_page_count].title, title);
                        }
                    }
                    else
                    {
                        if(spell_data_table[m_spellbook_pages[(m_spellbook_page_count - 1)].spell[0]].spell_book_category != group_idx)
                        {
                            strcpy(m_spellbook_pages[m_spellbook_page_count].title, title);
                        }
                    }

                }
                else
                {
                    strcpy(m_spellbook_pages[m_spellbook_page_count].title, title);
                }
                
            }

            group_spell_count--;

            // ~ NOT Apprentice, Choose Research, or Learn Spell  AKA Big Book; therefore, Spells button or Combat  AKA Small Book
            if(page_spell_count != NUM_SPELLS_PER_PAGE_BIG)
            {
                if(m_spellbook_spell_list[itr_spellbook_spell_list] == _players[HUMAN_PLAYER_IDX].casting_spell_idx)
                {
                    SBK_OpenPage = page_spell_counter;
                }
            }

            m_spellbook_pages[m_spellbook_page_count].spell[(page_spell_counter - 1)] = m_spellbook_spell_list[itr_spellbook_spell_list];

        }

        itr_spellbook_spell_list++;

    }

    m_spellbook_pages[m_spellbook_page_count].count = page_spell_counter;

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
// drake178: SBK_PageFlip_Big()
/*
; flips the page in the apprentice spellbook based on
; the passed direction:
;     0 - backwards
;     1 - forward
;   666 - forward directly to the research candidates
; draws and displays the animation, then sets the
; redraw function to the apprentice before returning
*/
/*

*/
void BigBook__PageTurn__WIP(int16_t direction)
{
    int16_t Stage_Text_Widths[4] = { 0, 0, 0, 0 };
    int16_t PageTurn_Stage = 0;
    int16_t Left_Page = 0;
    int16_t Page_Index = 0;
    SAMB_ptr IDK_seg = 0;  // _SI_

    Stage_Text_Widths[0] = 90;
    Stage_Text_Widths[1] = 60;
    Stage_Text_Widths[2] = 60;
    Stage_Text_Widths[3] = 90;

    Deactivate_Auto_Function();

    Open_File_Animation__HACK(book_lbx_file__ovr118, 1);

    Mark_Block(_screen_seg);

    IDK_seg = Allocate_Next_Block(_screen_seg, 1800);

    // SCROLL.LBX, 007  BCORNERS    big left corner
    // SCROLL.LBX, 008  BCORNERS    big right corner
    _spellbook_big_left_corner_seg = LBX_Reload_Next(scroll_lbx_file__ovr118__1, 7, _screen_seg);
    _spellbook_big_right_corner_seg = LBX_Reload_Next(scroll_lbx_file__ovr118__1, 8, _screen_seg);

    for(PageTurn_Stage = 0; PageTurn_Stage < 4; PageTurn_Stage++)
    {

    }

    Assign_Auto_Function(BigBook_Draw, 2);

    Release_Block(_screen_seg);

}


// WZD o118p02
// drake178: SBK_ApprenticeBook()
/*
; draws the apprentice-style spellbook, including all
; text and dog-ears if allowed and applicable
;
; requires the BIGBOOK fullscreen image to be saved in
; the third VGA frame (starting at $A800)
*/
/*

*/
void BigBook_Draw(void)
{

    SAMB_ptr IDK_seg = 0;  // _SI_

    // ; (0-7), also combat casting cursor anim stage (0-4)
    SBK_NewSpellAnim_Stg = ((SBK_NewSpellAnim_Stg + 1) % 8);

    // void Cycle_Palette_Color__STUB(int16_t color_num, int16_t red_min, int16_t green_min, int16_t blue_min, int16_t red_max, int16_t green_max, int16_t blue_max, int16_t step_value)
    Cycle_Palette_Color__STUB(198, 29, 41, 63, 44, 56, 63, 2);  // {0x1D, 0x29, 0x3F}, {0x2C, 0x38, 0x3F}

    Mark_Block(_screen_seg);

    IDK_seg = Allocate_Next_Block(_screen_seg, 1800);  // 1800 PR, 28800

    // SCROLL.LBX, 007  BCORNERS    big left corner
    // SCROLL.LBX, 008  BCORNERS    big right corner
    _spellbook_big_left_corner_seg = LBX_Reload_Next(scroll_lbx_file__ovr118__1, 7, _screen_seg);
    _spellbook_big_right_corner_seg = LBX_Reload_Next(scroll_lbx_file__ovr118__1, 8, _screen_seg);

    Copy_Back_To_Off();

    if(SBK_Dogears > 1)
    {
        
        if(SBK_OpenPage != 0)
        {
            FLIC_Draw(15, 9, _spellbook_big_left_corner_seg);
        }

        if(
            (SBK_OpenPage != m_spellbook_page_count)
            &&
            ( (m_spellbook_page_count - 2) > SBK_OpenPage)
        )
        {
            FLIC_Draw(289, 9, _spellbook_big_right_corner_seg);
        }

    }

    BigBook_Compose__WIP(SBK_OpenPage, IDK_seg, 1);

    Draw_Picture_Windowed(25, -20, IDK_seg);

    BigBook_Compose__WIP((SBK_OpenPage + 1), IDK_seg, 0);

    Draw_Picture_Windowed(173, -20, IDK_seg);

    if(SBK_Dogears == 0)
    {

        Set_Font_Style_Outline_Heavy(5, 9, 0, 0);

        Set_Outline_Color(1);

        Print(43, 183, cnst_ChooseResearch);
    }

    Release_Block(_screen_seg);

}


// WZD o118p03
// drake178: GAME_NewSpellAnim()
/*
; draws a frame of the new spell animation into the
; current draw frame
*/
/*

*/
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
// drake178: VGA_DrawBlackScreen()
// ¿ MoO2  Cls() || Cls_On ?
void Fill_Black_Draw(void)
{

    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_BLACK);

}


// WZD o118p05
// drake178: GAME_LoadNewSpell()
/*
; loads the wizard lab image into the third VGA frame,
; then loads the components necessary for the spell
; learning animation into the LBX_Sandbox_Segment
*/
/*

*/
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
// drake178: GAME_LearnSpellAnim()
/*
; displays the spell learning animations and, if new
; research is enabled, flips the apprentice book to the
; pages with the research candidates afterwards
*/
/*

*/
void Learn_Spell_Animation(int16_t spell_idx, int16_t research_flag)
{
    char Spell_Description[LEN_SPELL_DESCRIPTION] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t Blink_Color_Array[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char Spell_Name[LEN_SPELL_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    SAMB_ptr learn_spell_notify_seg = 0;
    int16_t Have_Candidates = 0;
    int16_t Interrupted = 0;
    char Conversion_String[LEN_TEMP_BUFFER] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Minus1_Hotkey_Ctrl_Index = 0;
    int16_t Spell_Found = 0;
    int16_t Spell_on_Side = 0;
    int16_t casting_cost = 0;
    int16_t itr = 0;
    int16_t spellbook_page = 0;  // _SI_
    int16_t itr_spellbook_page_spell_count = 0;  // _DI_

    SBK_Spell_Index = spell_idx;

    if(magic_set.spell_animations != ST_TRUE)
    {

        Reset_First_Block(_screen_seg);

        Allocate_Reduced_Map();

        // SPELLSCR.LBX, 060  "SUMGREEN"    ""
        learn_spell_notify_seg = LBX_Reload_Next(spellscr_lbx_file__ovr118, 60, _screen_seg);

        Assign_Auto_Function(Fill_Black_Draw, 2);

        Set_Page_Off();

        Main_Screen_Draw();

        strcpy(GUI_NearMsgString, cnst_NewSpell_Msg);  // "You have mastered the spell of "

        _fstrcpy(Spell_Name, spell_data_table[spell_idx].name);

        strcat(GUI_NearMsgString, Spell_Name);

        Notify2(0, 40, 2, GUI_NearMsgString, 0, learn_spell_notify_seg, 0, 9, 0, 0, 0, 1, 0);

        Deactivate_Auto_Function();

    }
    else
    {

        if(magic_set.background_music == ST_TRUE)
        {
            // DOMSDOS  Stop_All_Sounds__STUB();

            SND_Spell_Music = LBX_Reload(music_lbx_file__ovr118, MUSIC_New_Spell, SND_Music_Segment);
            SND_Spell_Music_size = lbxload_entry_length;

            // DOMSDOS  Play_Sound__WIP(SND_Spell_Music);
            sdl2_Play_Sound__WIP(SND_Spell_Music, SND_Spell_Music_size);

        }

        Fade_Out();

        Set_Mouse_List(1, mouse_list_none);

        Learn_Spell_Animation_Load();

        Set_Page_Off();

        Learn_Spell_Animation_Draw();

        _page_flip_effect = pfe_TogglePagesFadeIn;

        PageFlip_FX();

        Clear_Fields();

        spellbook_page = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, str_empty_string__ovr118[0], ST_UNDEFINED);

        Assign_Auto_Function(Learn_Spell_Animation_Draw, 2);

        for(itr = 0; ((itr < 60) && (Get_Input() == 0)); itr++)
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

        SBK_BuildSpellbook__WIP(slt_Library, 4);

        // SCROLL.LBX, 006  "BIGBOOK"   ""
        _spellbook_big_seg = LBX_Reload(scroll_lbx_file__ovr118__2, 6, _screen_seg);

        Set_Page_Off();

        FLIC_Draw(0, 0, _spellbook_big_seg);

        Copy_Off_To_Back();

        Set_Page_On();

        Spell_Found = ST_FALSE;

        // ¿ BUG:  reuses spellbook_page, which is holding Add_Hidden_Field() ?
        for(spellbook_page = 0; (((m_spellbook_page_count - 2) > spellbook_page) && (Spell_Found == ST_FALSE)); spellbook_page++)
        {

            for(itr_spellbook_page_spell_count = 0; ((m_spellbook_pages[spellbook_page].count > itr_spellbook_page_spell_count) && (Spell_Found == ST_FALSE)); itr_spellbook_page_spell_count++)
            {

                if(abs(m_spellbook_pages[spellbook_page].spell[itr_spellbook_page_spell_count]) == spell_idx)
                {

                    if((spellbook_page % 2) == 0)
                    {

                        SBK_OpenPage = spellbook_page;
                        Spell_on_Side = 0;
                        SBK_Dogears = 0;

                    }
                    else
                    {

                        Spell_on_Side = 1;
                        SBK_Dogears = 1;
                        SBK_OpenPage = (spellbook_page - 1);

                    }

                    m_spell_list_count = itr_spellbook_page_spell_count;

                    Spell_Found = ST_TRUE;

                }

            }

        }

        // ; conflicting condition (the only caller of this
        // ; function sets the spell "Known" before the call)
        // ; - will never jump
        if(Spell_Found == ST_FALSE)
        {

            // ; BUG: would mark the first spells of wrong realms, if
            // ; this branch ever triggered
            _players[HUMAN_PLAYER_IDX].spells_list[((((spell_idx - 2) / NUM_SPELLS_PER_MAGIC_REALM) * NUM_SPELLS_PER_MAGIC_REALM) + ((spell_idx - 1) % NUM_SPELLS_PER_MAGIC_REALM))] = 1;  /* S_Knowable */

            SBK_BuildSpellbook__WIP(slt_Library, 4);

            for(spellbook_page = 0; (((m_spellbook_page_count - 1) > spellbook_page) && (Spell_Found == ST_FALSE)); spellbook_page++)
            {

                for(itr_spellbook_page_spell_count = 0; ((m_spellbook_pages[spellbook_page].count > itr_spellbook_page_spell_count) && (Spell_Found == ST_FALSE)); itr_spellbook_page_spell_count++)
                {

                    if(abs(m_spellbook_pages[spellbook_page].spell[itr_spellbook_page_spell_count]) == spell_idx)
                    {

                        if((spellbook_page % 2) == 0)
                        {

                            SBK_OpenPage = spellbook_page;
                            Spell_on_Side = 0;
                            SBK_Dogears = 0;

                        }
                        else
                        {

                            Spell_on_Side = 1;
                            SBK_Dogears = 1;
                            SBK_OpenPage = (spellbook_page - 1);

                        }

                        m_spell_list_count = itr_spellbook_page_spell_count;

                        Spell_Found = ST_TRUE;

                    }

                }

            }

        }

        Reset_First_Block(_screen_seg);

        for(spellbook_page = 0; spellbook_page < NUM_SPELLBOOK_DESCRIPTIONS; spellbook_page++)
        {

            // SBK_Descriptions[itr] = SA_MK_FP0(Allocate_Next_Block(_screen_seg, 12));
            SBK_Descriptions[spellbook_page] = (char *)Allocate_Next_Block(_screen_seg, 12);

        }

        SBK_LoadSpellDescs__WIP(SBK_OpenPage);

        Mark_Block(_screen_seg);

        IMG_SBK_PageText = Allocate_Next_Block(_screen_seg, 1800);  // 1800 PR, 28800 B

        IMG_SBK_Anims = Allocate_Next_Block(_screen_seg, 325);  // 325 PR, 5200 B

        // SPECFX.LBX, 049  "NEWSPELL"  ""
        spell_animation_seg = LBX_Reload_Next(specfx_lbx_file__ovr118, 49, _screen_seg);

        _fstrcpy(Spell_Description, SBK_Descriptions[((Spell_on_Side * 4) + itr_spellbook_page_spell_count)]);

        Create_Picture(129, 37, IMG_SBK_Anims);

        casting_cost = Casting_Cost(HUMAN_PLAYER_IDX, SBK_Spell_Index, ST_FALSE);

        for(itr = 0; itr < 16; itr++)
        {
            Blink_Color_Array[itr] = 198;
        }

        Set_Font_Colors_15(7, &Blink_Color_Array[0]);

        _fstrcpy(Spell_Name, spell_data_table[SBK_Spell_Index].name);

        Print_To_Bitmap(0, 0, Spell_Name, IMG_SBK_Anims);

        Set_Font_Colors_15(6, &Blink_Color_Array[0]);

        Print_To_Bitmap(0, 11, cnst_SP_Cost, IMG_SBK_Anims);

        itoa(casting_cost, Conversion_String, 10);

        Print_Right_To_Bitmap(42, 11, Conversion_String, IMG_SBK_Anims);

        Set_Font_LF(0);

        Set_Font_Spacing(1);

        Print_Paragraph_To_Bitmap(0, 17, 128, Spell_Description, 0, IMG_SBK_Anims);

        Clear_Fields();

        SBK_NewSpellAnim_Stg = 0;

        Minus1_Hotkey_Ctrl_Index = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_UNDEFINED, ST_UNDEFINED);

        Interrupted = 0;

        Assign_Auto_Function(SBK_DrawSpellDecode__STUB, 2);

        for(SBK_NewSpellAnim_Stg = 0; ((SBK_NewSpellAnim_Stg < 45) && (Interrupted == ST_FALSE)); SBK_NewSpellAnim_Stg++)
        {

            Interrupted = Get_Input();

            if(SBK_NewSpellAnim_Stg < 15)
            {

                Cycle_Palette_Color__STUB(198, 18, 14, 9, 38, 38, 53, 2);

            }

            Set_Page_Off();

            SBK_DrawSpellDecode__STUB();

            PageFlip_FX();

            if(SBK_NewSpellAnim_Stg == 0)
            {
                _page_flip_effect = pfe_None;
            }

        }


}



}


// WZD o118p07
// drake178: SBK_DrawSpellDecode()
/*
; draws the spellbook part of the animation of learning
; a new spell using the related global variables
*/
/*

*/
void SBK_DrawSpellDecode__STUB(void)
{
    // struct s_SPELL_DECODE Spell_Decode_Lefts = { 0, 0, 0, 0, 0, 0 };
    // struct s_SPELL_DECODE Spell_Decode_Rights = { 0, 0, 0, 0, 0, 0 };
// IDGI
// var_44= byte ptr -44h
// var_38= byte ptr -38h
// var_2C= byte ptr -2Ch
// var_18= word ptr -18h
// var_C= word ptr -0Ch
// -0044 var_44 db 12 dup(?)
// -0038 var_38 db 12 dup(?)
// -002C var_2C db 20 dup(?)
// -0018 var_18 s_SPELL_DECODE ?
// -000C var_C s_SPELL_DECODE ?
uint8_t var_44[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
uint8_t var_38[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
uint8_t var_2C[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
struct s_SPELL_DECODE var_18 = { 0, 0, 0, 0, 0, 0 };
struct s_SPELL_DECODE var_C = { 0, 0, 0, 0, 0, 0 };

// lea     ax, [bp+Spell_Decode_Rights]
// push    ss
// push    ax                              ; Dest_Struct
// mov     ax, offset TBL_SpellDecode_Rs
// push    ds
// push    ax                              ; src
// mov     cx, 12
// call    F_SCOPY@

// lea     ax, [bp+Spell_Decode_Lefts]
// push    ss
// push    ax                              ; Dest_Struct
// mov     ax, offset TBL_SpellDecode_Ls
// push    ds
// push    ax                              ; src
// mov     cx, 12
// call    F_SCOPY@

    // memcpy(Spell_Decode_Rights, TBL_SpellDecode_Rs);
    // memcpy(Spell_Decode_Lefts, TBL_SpellDecode_Ls);
    
    // Spell_Decode_Rights = TBL_SpellDecode_Rs;
    // Spell_Decode_Lefts = TBL_SpellDecode_Ls;
    
    var_C = TBL_SpellDecode_Rs;
    var_18 = TBL_SpellDecode_Ls;

    Copy_Back_To_Off();

    if(SBK_Dogears == 1)
    {

        BigBook_Compose__WIP(SBK_OpenPage, IMG_SBK_PageText, ST_TRUE);

        Draw_Picture_Windowed(25, -20, IMG_SBK_PageText);

    }
    else
    {

        BigBook_Compose__WIP((SBK_OpenPage + 1), IMG_SBK_PageText, ST_FALSE);

        Draw_Picture_Windowed(173, -20, IMG_SBK_PageText);

    }

    if(SBK_Dogears == 0)
    {
        
        BigBook_Compose__WIP(SBK_OpenPage, IMG_SBK_PageText, ST_TRUE);

    }
    else
    {

        BigBook_Compose__WIP((SBK_OpenPage + 1), IMG_SBK_PageText, ST_FALSE);

    }                          

    if(SBK_NewSpellAnim_Stg < 22)
    {

        Clear_Bitmap_Region(0, (48 + (GUI_Multipurpose_Int * 37)), 129, (82 + (GUI_Multipurpose_Int * 37)), IMG_SBK_PageText);

    }
    else if(SBK_NewSpellAnim_Stg < 28)
    {

        Clear_Bitmap_Region(0, (48 + (GUI_Multipurpose_Int * 37)), var_38[SBK_NewSpellAnim_Stg], (82 + (GUI_Multipurpose_Int * 37)), IMG_SBK_PageText);

        Clear_Bitmap_Region(0, (48 + (GUI_Multipurpose_Int * 37)), var_38[SBK_NewSpellAnim_Stg], (82 + (GUI_Multipurpose_Int * 37)), IMG_SBK_PageText);

        Clear_Bitmap_Region(var_44[SBK_NewSpellAnim_Stg], (48 + (GUI_Multipurpose_Int * 37)), 129, (82 + (GUI_Multipurpose_Int * 37)), IMG_SBK_PageText);

        Clear_Bitmap_Region(var_38[SBK_NewSpellAnim_Stg], 0, var_44[SBK_NewSpellAnim_Stg], 37, IMG_SBK_Anims);

    }

    if(SBK_Dogears == 0)
    {

        Draw_Picture_Windowed(25, -20, IMG_SBK_PageText);

        if(SBK_NewSpellAnim_Stg < 27)
        {

            Draw_Picture(25, (28 + (GUI_Multipurpose_Int * 37)), IMG_SBK_Anims);
            
        }

        if(
            (SBK_NewSpellAnim_Stg > 15)
            &&
            (SBK_NewSpellAnim_Stg < 29)
        )
        {

            Set_Animation_Frame(spell_animation_seg, (SBK_NewSpellAnim_Stg - 16));

            FLIC_Draw(20, (21 + (GUI_Multipurpose_Int * 37)), spell_animation_seg);
            
        }

    }
    else
    {

        Draw_Picture_Windowed(173, -20, IMG_SBK_PageText);

        if(SBK_NewSpellAnim_Stg < 27)
        {

            Draw_Picture(173, (28 + (GUI_Multipurpose_Int * 37)), IMG_SBK_Anims);

        }

        if(
            (SBK_NewSpellAnim_Stg > 15)
            &&
            (SBK_NewSpellAnim_Stg < 29)
        )
        {

            Set_Animation_Frame(spell_animation_seg, (SBK_NewSpellAnim_Stg - 16));

            FLIC_Draw(168, (24 + (GUI_Multipurpose_Int * 37)), spell_animation_seg);

        }

    }

}


// WZD o118p08
// drake178: SBK_LoadSpellDescs()
/*
; loads spell descriptions for the 4-spells-per-page
; book type, first filling indexes 0-7 using the global
; open page variable, then the rest using the passed
; parameter
*/
/*
    only for BIGBOOK

    loads the spell descriptions
    for each spell, on each page
    hard-coded to 4 spells per page
    ...something like the active page and the other page...

¿ why iter over SBK_OpenPage and page ?
¿ code in both loops is exactly the same ?
¿ treats array of 16 descriptions as 4 pages of 4 ?

*/
void SBK_LoadSpellDescs__WIP(int16_t page)
{
    char buffer[LEN_SPELL_DESCRIPTION] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t itr = 0;  // _SI_
    int16_t count = 0;  // _DI_

    for(itr = 0; itr < NUM_SPELLBOOK_DESCRIPTIONS; itr++)
    {
        // TODO  String_Copy_Far(SBK_Descriptions[itr], str_empty_string__ovr118);
        strcpy(SBK_Descriptions[itr], str_empty_string__ovr118);
    }

    for(itr = SBK_OpenPage; (SBK_OpenPage + 1) >= itr; itr++)
    {

        if(
            (itr < 0)
            ||
            (itr >= m_spellbook_page_count)
        )
        {

            for(count = 0; count < NUM_SPELLS_PER_PAGE_BIG; count++)
            {

                // TODO  String_Copy_Far(SBK_Descriptions[((itr - SBK_OpenPage) + count)], str_empty_string__ovr118);
                strcpy(SBK_Descriptions[(((itr - SBK_OpenPage) * 4) + count)], str_empty_string__ovr118);

            }

        }
        else
        {

            for(count = 0; count < NUM_SPELLS_PER_PAGE_BIG; count++)
            {

                if(m_spellbook_pages[itr].count > count)
                {

                    LBX_Load_Data_Static(desc_lbx_file__ovr118, 0, (SAMB_ptr)buffer, abs(m_spellbook_pages[itr].spell[count]), 1, 110);

                    // TODO  String_Copy_Far(SBK_Descriptions[((itr - SBK_OpenPage) + count)], buffer);
                    strcpy(SBK_Descriptions[(((itr - SBK_OpenPage) * 4) + count)], buffer);

                }
                else
                {

                    // TODO  String_Copy_Far(SBK_Descriptions[((itr - SBK_OpenPage) + count)], str_empty_string__ovr118);
                    strcpy(SBK_Descriptions[(((itr - SBK_OpenPage) * 4) + count)], str_empty_string__ovr118);

                }

            }

        }

    }

    for(itr = page; (page + 1) >= itr; itr++)
    {

        if(
            (itr < 0)
            &&
            (itr >= m_spellbook_page_count)
        )
        {

            for(count = 0; count < NUM_SPELLS_PER_PAGE_BIG; count++)
            {

                // TODO  String_Copy_Far(SBK_Descriptions[((itr - page) + count)], str_empty_string__ovr118);
                strcpy(SBK_Descriptions[((((itr - page) + 2) * 4) + count)], str_empty_string__ovr118);

            }

        }
        else
        {

            for(count = 0; count < NUM_SPELLS_PER_PAGE_BIG; count++)
            {

                if(m_spellbook_pages[itr].count > count)
                {

                    LBX_Load_Data_Static(desc_lbx_file__ovr118, 0, (SAMB_ptr)buffer, abs(m_spellbook_pages[itr].spell[count]), 1, 110);

                    // TODO  String_Copy_Far(SBK_Descriptions[((itr - page) + count)], buffer);
                    strcpy(SBK_Descriptions[((((itr - page) + 2) * 4) + count)], buffer);

                }
                else
                {

                    // TODO  String_Copy_Far(SBK_Descriptions[((itr - page) + count)], str_empty_string__ovr118);
                    strcpy(SBK_Descriptions[((((itr - page) + 2) * 4) + count)], str_empty_string__ovr118);

                }

            }

        }

    }

}


// WZD o118p09
// drake178: sub_9DDBC()
void Apprentice_Screen__WIP(void)
{
    int16_t dogear_left_field = 0;
    int16_t dogear_right_field = 0;
    int16_t full_screen_ESC_field = 0;
    int16_t input_field_idx = 0;
    int16_t itr = 0;  // _SI_
    int16_t leave_screen = 0;  // _DI_

    SBK_BuildSpellbook__WIP(slt_Library, 4);  // spell_list_type, page_spell_count

    SBK_OpenPage = SBK_Candidate_Page;

    // ; apprentice: 0 - choose research; 2 - apprentice book
    // ; new spell anim: 0 - spell on left; 1 - spell on right
    SBK_Dogears = 2;

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
        // SBK_Descriptions[itr] = SA_MK_FP0(Allocate_Next_Block(_screen_seg, 9));
        SBK_Descriptions[itr] = (char *)Allocate_Next_Block(_screen_seg, 9);
    }

    SBK_LoadSpellDescs__WIP(SBK_OpenPage);

    Assign_Auto_Function(BigBook_Draw, 2);

    Clear_Fields();

    dogear_left_field = Add_Hidden_Field(15, 7, 30, 22, str_empty_string__ovr118[0], ST_UNDEFINED);

    dogear_right_field = Add_Hidden_Field(290, 7, 305, 22, str_empty_string__ovr118[0], ST_UNDEFINED);

    full_screen_ESC_field = Add_Hidden_Field(SCREEN_XMIN, 25, SCREEN_XMAX, SCREEN_YMAX, str_hotkey_ESC__ovr118[0], ST_UNDEFINED);

    leave_screen = ST_FALSE;

    Set_Help_List((char *)&_help_entries[0], 10);

    while(leave_screen == ST_FALSE)
    {

        input_field_idx = Get_Input();

        for(itr = 0; itr < 4; itr++)
        {

            if(
                (m_spellbook_pages[SBK_OpenPage].count > itr)
                &&
                (m_spellbook_pages[(SBK_OpenPage + itr)].spell[itr] > 0)
            )
            {
                if(abs(m_spellbook_pages->spell[(SBK_OpenPage + itr)]) == spl_Spell_Of_Return)
                {
                    _help_entries[(2 + itr)].help_idx = HLP_SPELL_OF_RETURN;
                }
                else
                {
                    _help_entries[(2 + itr)].help_idx = abs(m_spellbook_pages->spell[((SBK_OpenPage + 0) + itr)]);
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
                (m_spellbook_pages[(SBK_OpenPage + 1)].count > itr)
                &&
                (m_spellbook_pages[((SBK_OpenPage + 1) + itr)].spell[itr] > 0)
            )
            {
                if(abs(m_spellbook_pages->spell[((SBK_OpenPage + 1) + itr)]) == 214)  /* Spell_Of_Return */
                {
                    _help_entries[(6 + itr)].help_idx = HLP_SPELL_OF_RETURN;
                }
                else
                {
                    _help_entries[(6 + itr)].help_idx = abs(m_spellbook_pages->spell[((SBK_OpenPage + 1) + itr)]);
                }
            }
            else
            {
                _help_entries[(6 + itr)].help_idx = ST_UNDEFINED;
            }

        }

        if(input_field_idx == full_screen_ESC_field)
        {
            leave_screen = ST_TRUE;
        }

        if(input_field_idx == dogear_right_field)
        {

            if((m_spellbook_page_count - 2) > SBK_OpenPage)
            {

                SBK_LoadSpellDescs__WIP((SBK_OpenPage + 2));

                BigBook__PageTurn__WIP(1);

                SBK_OpenPage += 2;

                for(itr = 0; itr < 8; itr++)
                {
                    // TODO  String_Copy_Far(SBK_Descriptions[(0 + itr)], SBK_Descriptions[(8 + itr)]);
                    strcpy(SBK_Descriptions[(0 + itr)], SBK_Descriptions[(8 + itr)]);
                }

                Set_Page_Off();
                BigBook_Draw();
                PageFlip_FX();

            }

        }


        if(input_field_idx == dogear_left_field)
        {

            if(SBK_OpenPage > 1)
            {

                SBK_LoadSpellDescs__WIP((SBK_OpenPage - 2));

                BigBook__PageTurn__WIP(0);

                SBK_OpenPage -= 2;

                for(itr = 0; itr < 8; itr++)
                {
                    // TODO  String_Copy_Far(SBK_Descriptions[(0 + itr)], SBK_Descriptions[(8 + itr)]);
                    strcpy(SBK_Descriptions[(0 + itr)], SBK_Descriptions[(8 + itr)]);
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





    SBK_Candidate_Page = SBK_OpenPage;

    Deactivate_Help_List();
    Deactivate_Auto_Function();

}


// WZD o118p10
// drake178: SBK_ComposeApprPage()
/*
// ; draws all of the text associated with the selected
// ; spell book page into the passed LBX allocation as a
// ; decoded (ready to overlay) LBX image
*/
/*

*/
void BigBook_Compose__WIP(int16_t page, SAMB_ptr pict_seg, int16_t flag)
{
    char spell_name[LEN_SPELL_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char spell_description[LEN_SPELL_DESCRIPTION] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t colors[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t description_batch = 0;
    int32_t research_cost = 0;
    int16_t turns_left = 0;
    int16_t casting_cost = 0;
    int16_t itr_spell_book_descriptions_count = 0;
    int16_t itr = 0;  // _SI_

    description_batch = (page - SBK_OpenPage);

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
                // // colors[itr_spell_book_descriptions_count] = 198;
                // colors[itr_spell_book_descriptions_count] = 186;
                colors[itr_spell_book_descriptions_count] = 168;
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

        // TODO  String_Copy_Far(spell_name, spell_data_table[abs(m_spellbook_pages[page].spell[itr])].name);
        strcpy(spell_name, spell_data_table[abs(m_spellbook_pages[page].spell[itr])].name);

        Print_To_Bitmap(0, (48 + (37 * itr)), spell_name, pict_seg);

        if(m_spellbook_pages[page].spell[itr] == _players[HUMAN_PLAYER_IDX].researching_spell_idx)
        {
            for(itr_spell_book_descriptions_count = 0; itr_spell_book_descriptions_count < 16; itr_spell_book_descriptions_count++)
            {
                // // colors[itr_spell_book_descriptions_count] = 198;
                // colors[0] = 184;
                // colors[1] = 185;
                colors[itr_spell_book_descriptions_count] = 170;
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

        if(page < SBK_Candidate_Page)
        {

            casting_cost = Casting_Cost(HUMAN_PLAYER_IDX, abs(m_spellbook_pages[page].spell[itr]), ST_FALSE);

            if(SBK_BookManaLimit == 0)
            {
                turns_left = 999;
            }
            else
            {
                turns_left = (((casting_cost + SBK_BookManaLimit) - 1) / SBK_BookManaLimit);
            }

            strcpy(spell_description, cnst_Catsing_Cost);

            itoa(casting_cost, spell_name, 10);

            strcat(spell_description, spell_name);

            strcat(spell_description, cnst_SpaceOpenBrace4);

            itoa(turns_left, spell_name, 10);
            strcat(spell_description, cnst_SpaceOpenBrace4);

            strcat(spell_description, spell_name);

            if(turns_left == 1)
            {
                strcat(spell_description, cnst_RT1turn);
            }
            else
            {
                strcat(spell_description, cnst_RTmoreturns);
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

            // ¿ ; BUG: using an arbitrary spell can yield an incorrect research income (e.g. conjurer is not based on realm)
            if(SBK_Research_Incomes[spell_data_table[abs(m_spellbook_pages[page].spell[itr])].magic_realm] == 0)
            {
                turns_left = 999;
            }
            else
            {
                turns_left = (((research_cost + SBK_Research_Incomes[spell_data_table[abs(m_spellbook_pages[page].spell[itr])].magic_realm]) - 1) / SBK_Research_Incomes[spell_data_table[abs(m_spellbook_pages[page].spell[itr])].magic_realm]);
            }

            strcpy(spell_description, cnst_Research_Cost);

            ltoa(research_cost, spell_name, 10);

            strcat(spell_description, spell_name);

            strcat(spell_description, cnst_SpaceOpenBrace4);

            itoa(turns_left, spell_name, 10);

            strcat(spell_description, spell_name);

            if(turns_left == 1)
            {
                strcat(spell_description, cnst_RT1turn);
            }
            else
            {
                strcat(spell_description, cnst_RTmoreturns);
            }

        }

        Print_To_Bitmap(0, (59 + (37 * itr)), spell_description, pict_seg);

        if(abs(m_spellbook_pages[page].spell[itr]) > 0)
        {

            Set_Font_LF(0);

            Set_Font_Spacing(1);

            // TODO  String_Copy_Far(spell_description, SBK_Descriptions[(description_batch + itr)]);
            strcpy(spell_description, SBK_Descriptions[((description_batch * 4) + itr)]);

            Print_Paragraph_To_Bitmap(0, (65 + (37 * itr)), 128, spell_description, 0, pict_seg);  // Align-Left

        }

    }  /* END: for(itr = 0; m_spellbook_pages[page].count > itr; itr++) */

    if(
        (m_spellbook_pages[page].count <= 0)
        &&
        (page < SBK_Candidate_Page)
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

    if(abs((SBK_OpenPage - page)) != 2)
    {
        if(page != SBK_OpenPage)
        {
            if(
                spell_data_table[abs(m_spellbook_pages[page].spell[0])].spell_book_category
                ==
                spell_data_table[abs(m_spellbook_pages[(page - 1)].spell[0])].spell_book_category
            )
            {
                if(page < SBK_Candidate_Page)
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
// drake178: GUI_Research_Dialog()
// AKA SBK_Research_Dialog()
// ~ MoO2  Module: TECH  Tech_Select_()
/*
; displays the "Choose a spell to research" dialog
; using the big apprentice spellbook, and prevents the
; player from progressing until a candidate has been
; picked for research, which it then sets in the wizard
; record before returning
*/
/*

*/
void Spell_Research_Select(void)
{
    int16_t Spell_GUI_Controls[NUM_RESEARCH_SPELLS] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Top_Y = 0;
    int16_t Left_X = 0;
    int16_t input_field_idx = 0;
    int16_t itr = 0;  // _SI_
    int16_t candidate_count = 0;  // _DI_
    int16_t leave_screen = 0;  // _DI_

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

    SBK_BuildSpellbook__WIP(slt_Library, 4);

    Set_Mouse_List(1, mouse_list_default);

    SBK_Dogears = 0;

    Reset_First_Block(_screen_seg);

    for(itr = 0; itr < NUM_SPELLBOOK_DESCRIPTIONS; itr++)
    {

        // SBK_Descriptions[itr] = SA_MK_FP0(Allocate_Next_Block(_screen_seg, 9));
        SBK_Descriptions[itr] = (char *)Allocate_Next_Block(_screen_seg, 9);  // 9 PR, 144 B

    }

    SBK_OpenPage = SBK_Candidate_Page;

    SBK_LoadSpellDescs__WIP(SBK_OpenPage);

    Assign_Auto_Function(BigBook_Draw, 2);

    Clear_Fields();

    for(itr = 0; itr < NUM_RESEARCH_SPELLS; itr++)
    {

        Left_X = (25 + ((itr / 4) * 147));

        Top_Y = (25 + ((itr % 4) * 37));

        Spell_GUI_Controls[itr] = Add_Hidden_Field(Left_X, Top_Y, (Left_X + 110), (Top_Y + 33), str_empty_string__ovr118[0], ST_UNDEFINED);

    }

    _page_flip_effect = 3;

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
                (m_spellbook_pages[(SBK_OpenPage + 0)].count > itr)
                &&
                (m_spellbook_pages[(SBK_OpenPage + 0)].spell[itr] > 0)
            )
            {

                if(abs(m_spellbook_pages[(SBK_OpenPage + 0)].spell[itr]) == spl_Spell_Of_Return)
                {

                    _help_entries[(2 + itr)].help_idx = HLP_SPELL_OF_RETURN;

                }
                else
                {

                    _help_entries[(2 + itr)].help_idx = abs(m_spellbook_pages[(SBK_OpenPage + 0)].spell[itr]);

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
                (m_spellbook_pages[(SBK_OpenPage + 1)].count > itr)
                &&
                (m_spellbook_pages[(SBK_OpenPage + 1)].spell[itr] > 0)
            )
            {

                if(abs(m_spellbook_pages[(SBK_OpenPage + 1)].spell[itr]) == spl_Spell_Of_Return)
                {

                    _help_entries[(6 + itr)].help_idx = HLP_SPELL_OF_RETURN;

                }
                else
                {

                    _help_entries[(6 + itr)].help_idx = abs(m_spellbook_pages[(SBK_OpenPage + 1)].spell[itr]);

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

                if(_players[HUMAN_PLAYER_IDX].research_spells[itr] > 0)
                {

                    _players[HUMAN_PLAYER_IDX].researching_spell_idx = _players[HUMAN_PLAYER_IDX].research_spells[itr];

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
// GAME_CheckResearch()
