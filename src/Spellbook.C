/*
    WIZARDS.EXE
        ovr128
        ovr117
        ovr130
        ovr132
*/

#include "MoM.H"
#include "MainScr.H"   /* Allocate_Reduced_Map(); */
#include "SBookScr.H"  /* Spellbook_Screen_Draw(); */
// TODO  #include ...  /* CMB_RedrawSpellbook(); */



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

// WZD dseg:5DC0 35 00 2D 00 25 00 10 00 06 00 06 00             TBL_SpellDecode_Rs dw 35h, 2Dh, 25h, 10h, 6, 6
// WZD dseg:5DCC 40 00 47 00 4F 00 65 00 6F 00 6F 00             TBL_SpellDecode_Ls dw 40h, 47h, 4Fh, 65h, 6Fh, 6Fh

// WZD dseg:5DD8
char cnst_BookAnim_File2[] = "book";

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



// WZD dseg:68A0                                                 ¿ BEGIN:  ovr132 - Strings ?

// WZD dseg:68A0
char message_lbx_file__ovr132[] = "message";
// WZD dseg:68A8
char names_lbx_file__ovr132[] = "NAMES";
// WZD dseg:68AE
char cnst_PlaneShift_2[] = "Plane Shift";
// WZD dseg:68BA
char soundfx_lbx_file__ovr132[] = "soundfx";

// WZD dseg:68BA                                                 ¿ END:  ovr132 - Strings ?




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

// WZD dseg:946C 00 00                                           word_3FF0C dw 0                         ; DATA XREF: sub_BECD9+23w ...

// WZD dseg:946E
SAMB_ptr _spellbook_small_seg;

// WZD dseg:946E                                                 END:  ovr118



// WZD dseg:C8FA                                                 ¿ BEGIN: Spellbook (!Screen) - Uninitialized Data ?
// WZD dseg:C8FA                                                 BEGIN:  ovr117

// WZD dseg:C8FA
struct s_SPELL_BOOK_PAGE * m_spellbook_pages;

// WZD dseg:C8FC
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
/*

    count of spell_idx in m_spellbook_spell_list[]
        Spellbook_Group_Counts()
    set in Build_Spell_List()

*/
int16_t GUI_Multipurpose_Int;  /* NOTE(JimBalcomb,20240922): presently, now, only used in Combat.C */
int16_t m_spell_list_count;  // DNE in Dasm;  uses GUI_Multipurpose_Int

// WZD dseg:C928
/*
¿ 2 pages, 8 per page ?
*/
char * SBK_Descriptions[NUM_SPELLBOOK_DESCRIPTIONS];

// WZD dseg:C968 00 00                                           IMG_GAME_Lab_Wizard@ dw 0               ; DATA XREF: GAME_NewSpellAnim+1Cr ...
// WZD dseg:C96A 00 00                                           IMG_GAME_LabFmiliar@ dw 0               ; DATA XREF: GAME_NewSpellAnim+3Ar ...
// WZD dseg:C96C 00 00                                           IMG_GAME_MagicColmn@ dw 0               ; DATA XREF: GAME_NewSpellAnim+4Er ...
// WZD dseg:C96E 00 00                                           IMG_GAME_Lab_Podium@ dw 0               ; DATA XREF: GAME_NewSpellAnim+8r ...
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
// SBK_AddRealm()

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
// CMB_BuildSpellbook()

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
                    if(spell_data_table[abs(spell_idx)].type != 11)  /* sdt_Crafting_Spell */
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
                (spell_data_table[abs(spell_idx)].type != 11)  /* sdt_Crafting_Spell */
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
void CmbBook_Compose__STUB(void)
{

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
void CmbBook_Draw__STUB(void)
{





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
        // TODO  Assign_Auto_Function(CMB_RedrawSpellbook, 2);
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

    // TODO  Open_File_Animation__STUB(cnst_BookAnim_File2, 1);

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
// GAME_NewSpellAnim()

// WZD o118p04
// drake178: VGA_DrawBlackScreen()
// VGA_DrawBlackScreen()

// WZD o118p05
// drake178: GAME_LoadNewSpell()
// GAME_LoadNewSpell()

// WZD o118p06
// drake178: GAME_LearnSpellAnim()
// GAME_LearnSpellAnim()

// WZD o118p07
// drake178: SBK_DrawSpellDecode()
// SBK_DrawSpellDecode()

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

    // SCROLL.LBX, 006      BIGBOOK
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
                (m_spellbook_pages[(SBK_OpenPage + itr)].spell > 0)
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
                (m_spellbook_pages[((SBK_OpenPage + 1) + itr)].spell > 0)
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
                research_cost = _players[HUMAN_PLAYER_IDX].Research_Left;
            }
            else if(abs(m_spellbook_pages[page].spell[itr]) == spl_Spell_Of_Mastery)
            {
                research_cost = _players[HUMAN_PLAYER_IDX].SoM_RC;
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
// drake178: SBK_Research_Dialog()
// SBK_Research_Dialog()

// WZD o118p12
// drake178: GAME_CheckResearch()
// GAME_CheckResearch()



/*
    WIZARDS.EXE  ovr128
*/

// WZD o128p01
/*

; fills any empty slots in the research candidate list
; if possible, and ensures that if the wizard has any
; candidates available, they are researching something
; by calling AI_Research_Picker or GUI_Research_Dialog
; if that is not the case
; returns the player's research candidate count

*/
int16_t WIZ_RefreshResearch__STUB(int16_t player_idx)
{
    int16_t * Possible_Candidate_List;
    int16_t Ignore_SoM;
    int16_t Candidate_Index_In_Realm;
    int16_t Candidate_Realm;
    int16_t Candidate_Count;
    int16_t List_Index;
    int16_t Max_Candidates;
    int16_t itr; // _SI_


    Max_Candidates = 8;
    // DELETEME  GUI_Multipurpose_Int = 0;
    m_spell_list_count = 0;
    Candidate_Count = 0;
    Ignore_SoM = 0;

    Possible_Candidate_List = (int16_t *)Near_Allocate_First(200);

    WIZ_GetResearchList__STUB(player_idx, Possible_Candidate_List);


    // [byte ptr bx-24062]
    // [(_players.spells_list+0D4h)+bx]
    // D4h  212d  5 * 40 + 12
    // _players.spells_list+Spell_Of_Mastery-1)
    // DEDU  ¿ macro for spell status would know to xlat 'Spell Number' to 'spell_idx' ?
    if(_players[player_idx].spells_list[((sbr_Arcane * 40) + 12)] = 2)
    {
        Ignore_SoM = ST_TRUE;
    }
    else
    {
        for(itr = 0; itr < Max_Candidates; itr++)
        {
            if(_players[player_idx].research_spells[itr] > 0)
            {
                Candidate_Count++;
                if(_players[player_idx].research_spells[itr] == 213)  /* Spell_Of_Mastery */
                {
                    Ignore_SoM = ST_TRUE;
                }
            }
        }
    }


    // DELETEME  while((Candidate_Count < Max_Candidates) && (GUI_Multipurpose_Int > 0))
    while((Candidate_Count < Max_Candidates) && (m_spell_list_count > 0))
    {
        // DELETEME  List_Index = (Random(GUI_Multipurpose_Int) - 1);
        List_Index = (Random(m_spell_list_count) - 1);

        Candidate_Count++;

        for(itr = 0; itr < Max_Candidates; itr++)
        {
            if(_players[player_idx].research_spells[itr] == 0)
            {
                _players[player_idx].research_spells[itr] = Possible_Candidate_List[List_Index];
                Candidate_Index_In_Realm = ((Possible_Candidate_List[List_Index] - 1) % 40);
                Candidate_Realm = ((Possible_Candidate_List[List_Index] - 1) / 40);
                _players[player_idx].spells_list[((Candidate_Realm * 40) + Candidate_Index_In_Realm)] = 3;  /* S_Researchable */
                WIZ_GetResearchList__STUB(player_idx, &Possible_Candidate_List[0]);
            }
        }
    }


    Candidate_Count = 0;

    for(itr = 0; itr < Max_Candidates; itr++)
    {
        if(_players[player_idx].research_spells[itr] > 0)
        {
            Candidate_Count++;
        }
    }


    if(
        (Candidate_Count < Max_Candidates)
        &&
        /* DELETEME  (GUI_Multipurpose_Int == 0) */
        (m_spell_list_count == 0)
        &&
        (Ignore_SoM == ST_FALSE)
    )
    {
        if(_players[player_idx].research_spells[itr] == 0)
        {
            _players[player_idx].research_spells[itr] = spl_Spell_Of_Mastery;
        }
    }


    WIZ_ResearchSort__STUB(player_idx, Candidate_Count);


    if(
        (_players[player_idx].researching_spell_idx == 0)
        &&
        (Candidate_Count > 0)
        &&
        (_turn > 1)
    )
    {
        if(player_idx == HUMAN_PLAYER_IDX)
        {
            // ; displays the "Choose a spell to research" dialog
            // ; using the big apprentice spellbook, and prevents the
            // ; player from progressing until a candidate has been
            // ; picked for research, which it then sets in the wizard
            // ; record before returning
            // TODO  SBK_Research_Dialog();
        }
        else
        {
            // ; selects the spell to research from the wizard's list
            // ; of candidates using a weighted random roll,
            // ; prioritizing combat spells in research groups from
            // ; which the player does not yet have a known spell
            // ;
            // ; contains multipe BUGs that prevent research-related
            // ; profile traits from properly affecting the outcome
            // TODO  AI_Research_Picker(player_idx);
        }

    }



    return Candidate_Count;
}


// WZD o128p02
// drake178: WIZ_GetResearchList()
/*

; compiles a list of the lowest rarity spells from each realm available to the specified player

*/
void WIZ_GetResearchList__STUB(int16_t player_idx, int16_t research_list[])
{
    int16_t Max_Rarity;
    int16_t itr;  // _SI_
    int16_t flag;  // _DI_
    int16_t itr_spells;  // _CX_

    // DELETEME  GUI_Multipurpose_Int = 0;  // ; Spell List Count, New Spell Index, ...
    m_spell_list_count = 0;

    for(itr = 0; itr < 4; itr++)
    {

        flag = ST_FALSE;

        for(itr_spells = 0; itr_spells < 40; itr_spells++)
        {

            if(flag == ST_FALSE)
            {
                Max_Rarity = (itr_spells / 10);
            }

        }

    }

    // drake178: add Arcane spells up to Disenchant Area, Awareness, or Summon Champion (the lowest available group)
    flag = ST_FALSE;

    // itr_spells, Disenchant_Area - Magic_Spirit
    //     spl_Spell_Of_Mastery    = 213,
    //     spl_Magic_Spirit        = 201,
    for(itr_spells = 0; itr_spells < 12; itr_spells++)
    {

        // itr_spells, Disenchant_Area - Magic_Spirit
        //     spl_Disenchant_Area     = 204,
        //     spl_Magic_Spirit        = 201,
        // itr_spells, Awareness - Magic_Spirit
        //     spl_Awareness           = 209,
        //     spl_Magic_Spirit        = 201,
        if(
            (flag == ST_TRUE)
            &&
            (
                (itr_spells == 3)
                ||
                (itr_spells == 8)
            )
        )
        {
            return;
        }

        // ~==
        // (player_idx * sizeof(struct s_WIZARD))
        // + itr_spells
        // cmp     (_players.spells_list+Magic_Spirit-1)[bx], S_Knowable
        // ...
        // if(_players[player_idx].spells_list[(201 - 1 + itr_spells)] == 1)
        // ovr128:02EE 80 BF F6 A1 01                                  cmp     [byte ptr bx-5E0Ah], 1
        // cmp     [(_players.spells_list+0C8h)+bx], 1
        // ...
        if(_players[player_idx].spells_list[(200 + itr_spells)] == 1)
        {

            // DELETEME  research_list[(GUI_Multipurpose_Int)] = (201 + itr_spells);
            research_list[(m_spell_list_count)] = (201 + itr_spells);

            // DELETEME  GUI_Multipurpose_Int++;
            m_spell_list_count++;

            flag = ST_TRUE;

        }

    }

}


// WZD o128p03
int16_t WIZ_RollSpellReward(int16_t player_idx, int16_t rarity)
{
    int16_t Invalid_Realms[NUM_MAGIC_TYPES] = { 0, 0, 0, 0, 0 };
    int16_t Max_Rarity = 0;
    int16_t Spell_Found = 0;
    int16_t Random_Result = 0;
    int16_t Valid_Realm_Count = 0;
    int16_t In_Realm_Index = 0;
    int16_t Added_Rarity = 0;
    int16_t itr;  // _SI_
    int16_t magic_realm = 0;  // _DI_

    Valid_Realm_Count = 0;
    Spell_Found = 0;

    // ¿ BUG: the arcane realm is evaluated by the presence of the Alchemy or Warlord retorts instead of being marked as always available ?
    // DNM, because no item powers have mt_Arcane? which wouldn't make sense anyway?
    for(itr = 0; itr < NUM_MAGIC_TYPES; itr++)
    {
        // ¿ ; conflicting condition - will never jump ?
        if(
            (itr == NUM_MAGIC_TYPES)
            ||
            (_players[player_idx].spellranks[itr] == 0)
        )
        {
            Valid_Realm_Count++;
            Invalid_Realms[itr] = ST_FALSE;
        }
        else
        {
            Invalid_Realms[itr] = ST_TRUE;
        }
    }

    while((Valid_Realm_Count > 0) && (Spell_Found == ST_FALSE))
    {
        Random_Result = Random(Valid_Realm_Count);  // {0,1,2,3,4,5,6}

        itr = 0;
        magic_realm = 0;
        while(Random_Result != itr)
        {
            if(Invalid_Realms[magic_realm] == ST_FALSE)
            {
                itr++;
            }
            magic_realm++;
        }

        magic_realm--;

        if(magic_realm == 5)  /* _Arcane ... enum Realm_Byte*/
        {
            Max_Rarity = 3;
        }
        else if(_players[player_idx].spellranks[magic_realm] == 1)
        {
            Max_Rarity = 2;
        }
        else if(_players[player_idx].spellranks[magic_realm] == 2)
        {
            Max_Rarity = 3;
        }
        else
        {
            Max_Rarity = 4;
        }

        for(Added_Rarity = 0; ((Added_Rarity < 4) && (Spell_Found == ST_FALSE)); Added_Rarity++)
        {
            Random_Result = (Random(10) - 1);

            for(itr = 0; itr < 10; itr++)
            {
                if(Spell_Found == ST_FALSE)
                {
                    if(magic_realm == 5)  /* _Arcane */
                    {
                        In_Realm_Index = (Random(11) - 1);
                    }
                    else
                    {
                        // TODO(JimBalcomb,20240531): no way this is right - run in through the Struggle-Mode debugger
                        In_Realm_Index = (((Random_Result + itr) % 10) + ((((rarity + Added_Rarity) - 1) / Max_Rarity) * 10));
                    }

                    if(_players[player_idx].spells_list[((magic_realm * 40) + In_Realm_Index)] == 2)  /* S_Known ... enum Research_Status */
                    {
                        Spell_Found = ST_TRUE;
                        In_Realm_Index = (((magic_realm * 40) + In_Realm_Index) + 1);
                    }

                }
            }

        }

        Valid_Realm_Count--;
    }

    if(Spell_Found == ST_FALSE)
    {
        In_Realm_Index = 0;
    }

    // @@Done:
    return In_Realm_Index;
}

// WZD o128p04
// WIZ_GetSpellValue()

// WZD o128p05
// WIZ_SpellTradeList()

// WZD o128p06
// sub_AC19E()


// WZD o128p07
// drake178: WIZ_GE_Diplomacy()
/*
; initiates a diplomatic reaction towards the specified
; player for the selected global spell from every other
; wizard
;
; WARNING: the per-turn divisors make the action values
; very small or even zero
*/
/*
spl_Spell_Of_Mastery    - 50  10
spl_Eternal_Night       - 12  12
spl_Evil_Omens          - 20  12
spl_Zombie_Mastery      - 14  12
spl_Aura_of_Majesty     + 10   0
spl_Wind_Mastery        -  4  12
spl_Suppress_Magic      - 25  12
spl_Time_Stop              0  12
spl_Nature_Awareness       0  12
spl_Natures_Wrath       - 20  12
spl_Herb_Mastery           0  12
spl_Chaos_Surge         - 10  12
spl_Doom_Mastery        -  8  12
spl_Great_Wasting       - 20  12
spl_Meteor_Storm        - 15  12
spl_Armageddon          - 25  12
spl_Tranquility         - 20  12
spl_Life_Force          - 20  12
spl_Crusade             - 10  12
spl_Just_Cause             0  12
spl_Holy_Arms           -  5  12

*/
/*
    updates diplomatic relations for all other wizards, with the caster of the overland enchantment, based on which spell is being cast

XREF:
    WIZ_SetOverlandSpell__WIP()
    Cast_Spell_Overland__WIP()
    WIZ_ProcessGlobals()


*/
void Global_Enchantment_Change_Relations(int16_t player_idx, int16_t spell_idx, int16_t divisor)
{
    int16_t itr_players = 0;  // _DI_

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {

        if(itr_players != player_idx)
        {

            if(spell_idx == spl_Spell_Of_Mastery)
            {
                Change_Relations((-50 / divisor), player_idx, itr_players, 10, 0, spell_idx);
            }

            if(spell_idx == spl_Eternal_Night)
            {
                Change_Relations((-12 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(
                (spell_idx == spl_Evil_Omens)
                &&
                (
                    (_players[itr_players].spellranks[sbr_Life] > 0)
                    ||
                    (_players[itr_players].spellranks[sbr_Nature] > 0)
                )
            )
            {
                Change_Relations((-20 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(spell_idx == spl_Zombie_Mastery)
            {
                Change_Relations((-14 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(spell_idx == spl_Aura_of_Majesty)
            {
                Change_Relations((10 / divisor), player_idx, itr_players, 0, 0, spell_idx);
            }

            if(spell_idx == spl_Wind_Mastery)
            {
                Change_Relations((-4 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(spell_idx == spl_Suppress_Magic)
            {
                Change_Relations((-4 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(spell_idx == spl_Time_Stop)
            {
                Change_Relations((0 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(spell_idx == spl_Nature_Awareness)
            {
                Change_Relations((0 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(
                (spell_idx == spl_Natures_Wrath)
                &&
                (
                    (_players[itr_players].spellranks[sbr_Chaos] > 0)
                    ||
                    (_players[itr_players].spellranks[sbr_Death] > 0)
                )
            )
            {
                Change_Relations((-20 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(spell_idx == spl_Herb_Mastery)
            {
                Change_Relations((0 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(
                (spell_idx == spl_Chaos_Surge)
                &&
                (_players[itr_players].spellranks[sbr_Chaos] == 0)
            )
            {
                Change_Relations((-10 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(spell_idx == spl_Doom_Mastery)
            {
                Change_Relations((-8 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(spell_idx == spl_Great_Wasting)
            {
                Change_Relations((-20 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(spell_idx == spl_Meteor_Storm)
            {
                Change_Relations((-15 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(spell_idx == spl_Armageddon)
            {
                Change_Relations((-25 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(
                (spell_idx == spl_Tranquility)
                &&
                (_players[itr_players].spellranks[sbr_Chaos] > 0)
            )
            {
                Change_Relations((-20 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

            if(
                (spell_idx == spl_Life_Force)
                &&
                (_players[itr_players].spellranks[sbr_Death] > 0)
            )
            {
                Change_Relations((-20 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }


            if(spell_idx == spl_Crusade)
            {
                Change_Relations((-10 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }


            if(spell_idx == spl_Just_Cause)
            {
                Change_Relations((0 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }


            if(spell_idx == spl_Holy_Arms)
            {
                Change_Relations((-5 / divisor), player_idx, itr_players, 12, 0, spell_idx);
            }

        }

    }

}


// WZD o128p08
/*

; sorts the wizard's research candidates in ascending
; order by estimated research time or, if the research
; income is 0, by research cost
;
; contains a BUG that results in some retorts not
; being applied properly, especially Conjurer

*/
void WIZ_ResearchSort__STUB(int16_t player_idx, int16_t count)
{





}

// WZD o128p09
/*
awards the selected wizard a spellbook of the
specified realm, adding knowable spells as necessary,
and ensuring that they have research candidates if
possible

BUG: contains an outdated rarity per book table
BUG: if called with 11 books and missing very rare
 spells, the book the book is converted to a
 different realm instead
BUG: if called with 12 books and missing spells,
 memory corruption occurs and the effect is undefined

00000000 struc PerBook_Spells ; (sizeof=0x8, standard type)
00000000 Common dw ?
00000002 Uncommon dw ?
00000004 Rare dw ?
00000006 Very_Rare dw ?
00000008 ends PerBook_Spells

*/
void WIZ_AddSpellbook__WIP(int16_t player_idx, int16_t magic_realm)
{
    int16_t realm_spells[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Book_Spell_Table[10][4] = {
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    };
    int16_t random_realm_spell = 0;
    int16_t Existing_Books = 0;
    int16_t In_Rarity_Index = 0;
    int16_t realm_spell_count = 0;  // _SI_
    int16_t itr = 0;  // _DI_

    Book_Spell_Table[0][0] = 3;
    Book_Spell_Table[0][1] = 1;
    Book_Spell_Table[0][2] = 0;
    Book_Spell_Table[0][3] = 0;
    Book_Spell_Table[1][0] = 2;
    Book_Spell_Table[1][1] = 1;
    Book_Spell_Table[1][2] = 1;
    Book_Spell_Table[1][3] = 1;  // ¿ ; BUG: should be 0 ?
    Book_Spell_Table[2][0] = 1;
    Book_Spell_Table[2][1] = 1;
    Book_Spell_Table[2][2] = 1;
    Book_Spell_Table[2][3] = 1;
    Book_Spell_Table[3][0] = 1;
    Book_Spell_Table[3][1] = 1;
    Book_Spell_Table[3][2] = 1;
    Book_Spell_Table[3][3] = 1;
    Book_Spell_Table[4][0] = 1;
    Book_Spell_Table[4][1] = 1;
    Book_Spell_Table[4][2] = 1;
    Book_Spell_Table[4][3] = 1;
    Book_Spell_Table[5][0] = 1;
    Book_Spell_Table[5][1] = 1;
    Book_Spell_Table[5][2] = 1;
    Book_Spell_Table[5][3] = 1;
    Book_Spell_Table[6][0] = 1;
    Book_Spell_Table[6][1] = 2;
    Book_Spell_Table[6][2] = 1;
    Book_Spell_Table[6][3] = 1;
    Book_Spell_Table[7][0] = 0;
    Book_Spell_Table[7][1] = 2;
    Book_Spell_Table[7][2] = 1;
    Book_Spell_Table[7][3] = 1;
    Book_Spell_Table[8][0] = 0;
    Book_Spell_Table[8][1] = 0;
    Book_Spell_Table[8][2] = 2;
    Book_Spell_Table[8][3] = 1;
    Book_Spell_Table[9][0] = 0;
    Book_Spell_Table[9][1] = 0;
    Book_Spell_Table[9][2] = 2;  // ¿ ; BUG: should be 1 ?
    Book_Spell_Table[9][3] = 1;  // ¿ ; BUG: should be 3 ?

    Existing_Books = _players[player_idx].spellranks[magic_realm];

    // ¿ ; BUG: values greater than 10 corrupt memory ?
    if(Existing_Books != 10)
    {
        for(itr = 0; itr < 4; itr++)
        {
            realm_spell_count = 0;
            
            for(In_Rarity_Index = 0; In_Rarity_Index < 10; In_Rarity_Index++)
            {
                // DEDU  sizeof() 40 and 10 for spells magic realm and rarity
                if(_players[player_idx].spells_list[((magic_realm * 40) + (itr * 10) + In_Rarity_Index)] == 0);  /* S_Unknown */
                {
                    realm_spells[realm_spell_count] = ((itr * 10) + In_Rarity_Index);
                    realm_spell_count++;
                }
            }

            while(
                (realm_spell_count > 0)
                &&
                (Book_Spell_Table[Existing_Books][itr] > 0)
            )
            {
                random_realm_spell = (Random(realm_spell_count) - 1);

                _players[player_idx].spells_list[((magic_realm * 40) + realm_spells[random_realm_spell])] = 1;  /* S_Knowable */

                Clear_Structure(random_realm_spell, (uint8_t *)&realm_spells[0], sizeof(realm_spells[0]), realm_spell_count);

                realm_spell_count--;

                Book_Spell_Table[Existing_Books][itr]--;
            }
        }
    }

    _players[player_idx].spellranks[magic_realm]++;

    // TODO  WIZ_RefreshResearch(player_idx);

}


// WZD o128p10
// sub_ACACC()

// WZD o128p11
// WIZ_ConquestSpells()




/*
    WIZARDS.EXE  ovr130
*/


// WZD o130p01
/*

XREF:
    j_UNIT_SetEquipSlots()
        WIZ_HireHero()

e.g.,
WIZ_HireHero()
    |-> Hero_Slot_Types(unit_type_idx, _players[player_idx].Heroes[hero_slot_idx].Item_Slots)

Page 28  (PDF Page 33)
Note that the weapons and armor a hero can be equipped with depend on
the hero’s type; some heroes are mages, others are warriors, etc.
*/
void Hero_Slot_Types(int16_t unit_type_idx, int16_t item_slots[])
{

    if(_unit_type_table[unit_type_idx].hero_type == ht_IDK2)
    {
        item_slots[0] = ist_SwordStaff_Slot;
        item_slots[1] = ist_Armor_Slot;
        item_slots[2] = ist_Misc_Slot;
    }
    else if(_unit_type_table[unit_type_idx].hero_type == ht_Mage)
    {
        item_slots[0] = ist_Staff_Slot;
        item_slots[1] = ist_Misc_Slot;
        item_slots[2] = ist_Misc_Slot;
    }
    else if(_unit_type_table[unit_type_idx].hero_type == ht_IDK1)
    {
        item_slots[0] = ist_Bow_Slot;
        item_slots[1] = ist_Armor_Slot;
        item_slots[2] = ist_Misc_Slot;
    }
    else  /* ht_Warrior */
    {
        item_slots[0] = ist_Sword_Slot;
        item_slots[1] = ist_Armor_Slot;
        item_slots[2] = ist_Misc_Slot;
    }

}


// WZD o130p02
// sub_AF331()

// WZD o130p03
// sub_AF56D()

// WZD o130p04
// sub_AF7C1()

// WZD o130p05
// sub_AF9AA()

// WZD o130p06
// sAFA06_Anim_EarthLore()

// WZD o130p07
// sub_AFB7F()

// WZD o130p08
// sub_AFCA8()

// WZD o130p09
// sub_B01F7()

// WZD o130p10
// sub_B0692()

// WZD o130p11
// sub_B0C07()

// WZD o130p12
// sub_B1108()

// WZD o130p13
// sB1280_Anim_EnchantRoad()

// WZD o130p14
// sub_B148C()

// WZD o130p15
// sub_B1843()

// WZD o130p16
// CTY_Consecration()

// WZD o130p17
// sub_B1A01()

// WZD o130p18
// sub_B1ABE()



/*
    WIZARDS.EXE  ovr132
*/

// WZD o132p01
// sub_B4250()

// WZD o132p02
// sub_B4471()

// WZD o132p03
// CTY_ChaosRift()

// WZD o132p04
// WIZ_MeteorStorm()

// WZD o132p05
// CTY_StreamOfLife()

// WZD o132p06
// ¿ MoO2  Module: OFFICER  Set_Officer_To_Player_()
// Hire_Officer_() |-> Set_Officer_To_Player_()  ...Hire_Officer_() handles the officer cost
/*

    assumes (_units - 1)

unit_type_idx is passed over all the wzay from Hire_Hero_Popup()
so, not quite 'Generate Random Hero'


XREF:
    sub_B4250()
    sub_B4471()
    sub_B4E00()
    j_WIZ_HireHero()
        Hire_Hero_Popup()
        AI_Accept_Hero()
e.g.,
    Hire_Hero_Popup()
        |-> j_WIZ_HireHero(HUMAN_PLAYER_IDX, unit_type_idx, hero_slot_idx, 0)

    void AI_Accept_Hero(int16_t player_idx, int16_t hero_slot_idx, int16_t unit_type_idx)
        Hero_Hired = WIZ_HireHero(player_idx, unit_type_idx, hero_slot_idx, 0);

"Hire" as in "Summon"
*/
int16_t WIZ_HireHero(int16_t player_idx, int16_t unit_type_idx, int16_t hero_slot_idx, int16_t saved_flag)
{
    int16_t itr;

    Create_Unit__WIP(unit_type_idx, player_idx, FORTX(), FORTY(), FORTP(), -1);

    _UNITS[(_units - 1)].Finished = 0;

    _UNITS[(_units - 1)].moves2 = _UNITS[(_units - 1)].moves2_max;

    _UNITS[(_units - 1)].Hero_Slot = hero_slot_idx;

    _players[player_idx].Heroes[hero_slot_idx].unit_idx = (_units - 1);

    Hero_Slot_Types(unit_type_idx, _players[player_idx].Heroes[hero_slot_idx].Item_Slots);

    for(itr = 0; itr < NUM_HERO_ITEM_SLOTS; itr++)
    {
        _players[HUMAN_PLAYER_IDX].Heroes[hero_slot_idx].Items[itr] = ST_UNDEFINED;
    }

    if(saved_flag == ST_TRUE)
    {
        if(player_idx == HUMAN_PLAYER_IDX)
        {
            strcpy(_players[player_idx].Heroes[hero_slot_idx].name, hero_names_table[unit_type_idx].name);
            _UNITS[(_units - 1)].XP = hero_names_table[unit_type_idx].experience_points;
            _UNITS[(_units - 1)].Level = Calc_Unit_Level((_units - 1));
        }
        else
        {
            LBX_Load_Data_Static(names_lbx_file__ovr132, 0, (SAMB_ptr)_players[player_idx].Heroes[hero_slot_idx].name, ((player_idx * 35) + unit_type_idx), 1, 13);
            _UNITS[(_units - 1)].Level = abs(_HEROES2[player_idx]->heroes[unit_type_idx].Level);
            _UNITS[(_units - 1)].XP = TBL_Experience[_UNITS[(_units - 1)].Level];
        }
    }
    else
    {
        LBX_Load_Data_Static(names_lbx_file__ovr132, 0, (SAMB_ptr)_players[player_idx].Heroes[hero_slot_idx].name, ((player_idx * 35) + unit_type_idx), 1, 13);
        SETMAX(_HEROES2[player_idx]->heroes[unit_type_idx].Level, HL_GRANDLORD);
    }

    _UNITS[(_units - 1)].Level = _HEROES2[player_idx]->heroes[unit_type_idx].Level;
    _UNITS[(_units - 1)].XP = TBL_Experience[_HEROES2[player_idx]->heroes[unit_type_idx].Level];

    // BUG  Did this used to do something different? What tests it?
    // may be is/was success status as in cast the spell
    // NOTE(JimBalcomb,202409090905): AI_Accept_Hero() tests this
    return ST_TRUE;
}


// WZD o132p07
// sub_B4E00()

// WZD o132p08
// sub_B50AE()

// WZD o132p09
// sub_B517B()

// WZD o132p10
// IDK_SplCst_sB529D()

// WZD o132p11
// WIZ_GreatWasting()

// WZD o132p12
// CTY_GaiasBlessing()

// WZD o132p13
// WIZ_Armageddon()

// WZD o132p14
// sub_B5D8E()

// WZD o132p15
// CTY_NightshadeDispel()

// WZD o132p16
// sub_B609C()

// WZD o132p17
// sub_B62F7()

// WZD o132p18
// sub_B6505()
