/*
    WIZARDS.EXE
        ovr117
*/

#include "MoM.H"
#include "MainScr.H"   /* Allocate_Reduced_Map(); */
#include "SBookScr.H"  /* Spellbook_Screen_Draw(); */
// TODO  #include ...  /* CMB_RedrawSpellbook(); */



// WZD dseg:5D1C                                                 BEGIN:  ovr117 - Strings

// WZD dseg:5D1C
// char empty_cstring__ovr117[] = "";
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



// WZD dseg:5DC0                                                 BEGIN:  ovr118 - Strings

// WZD dseg:5DC0 35 00 2D 00 25 00 10 00 06 00 06 00             TBL_SpellDecode_Rs dw 35h, 2Dh, 25h, 10h, 6, 6
// WZD dseg:5DCC 40 00 47 00 4F 00 65 00 6F 00 6F 00             TBL_SpellDecode_Ls dw 40h, 47h, 4Fh, 65h, 6Fh, 6Fh

// WZD dseg:5DD8
char cnst_BookAnim_File2[] = "book";
// WZD dseg:5DDC
// char empty_cstring__ovr118[] = "";
// WZD dseg:5DDD
char cnst_SCROLL_File[] = "scroll";
// WZD dseg:5DE4
char cnst_ChooseResearch[] = "Choose a new spell to research";
// WZD dseg:5E03
char cnst_WIZLAB_File2[] = "Wizlab";
// WZD dseg:5E0A
char cnst_MUSIC_FileA[] = "music";
// WZD dseg:5E10
char cnst_SCROLL_File2[] = "Scroll";
// WZD dseg:5E17
char cnst_SPECFX_File[] = "SPECFX";
// WZD dseg:5E1E
char cnst_SP_Cost[] = "SP Cost :";
// WZD dseg:5E28
char cnst_SPELLSCR_File2[] = "Spellscr";
// WZD dseg:5E31
char cnst_NewSpell_Msg[] = "You have mastered the spell of ";
// WZD dseg:5E51
char cnst_DESC_File[] = "Desc";
// WZD dseg:5E56
char cnst_HLPENTRY_File3[] = "hlpentry";
// WZD dseg:5E5F
char cnst_HOTKEY_Esc19[] = "\x1B";
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

// WZD dseg:5E81
// WZD dseg:5E90 00 00                                           dw 0

// WZD dseg:5E90                                                 END:  ovr118 - Strings



// WZD dseg:9448                                                 ¿ BEGIN: Spellbook (!Screen) - Uninitialized Data ?
// WZD dseg:9448                                                 BEGIN:  ovr118

// WZD dseg:9448
int16_t SBK_Candidate_Page;

// WZD dseg:944A
int16_t SBK_SomePageSaveVar;

// WZD dseg:944C 00 00                                           CMB_SpellBookPage dw 0                  ; DATA XREF: G_WLD_StaticAssetRfrsh+61w ...
// WZD dseg:944E 00 00                                           dw 0
// WZD dseg:9450 00 00                                           dw 0

// WZD dseg:9452
SAMB_ptr _spellbook_small_text;

// WZD dseg:9454
// _spellbook_small_symbols SPBK_ICONS <0>
SAMB_ptr _spellbook_small_symbols[5];

// WZD dseg:9460 00 00                                           IMG_SPL_BigBook@ dw 0                   ; DATA XREF: GAME_LearnSpellAnim+16Dw ...
// WZD dseg:9460                                                                                         ; LBXE_LoadReplace into the LBX_Sandbox_Segment
// WZD dseg:9462 00 00                                           dw 0
// WZD dseg:9464 00 00                                           IMG_SPL_BigRDogear@ dw 0                ; DATA XREF: SBK_PageFlip_Big+79w ...
// WZD dseg:9466 00 00                                           IMG_SPL_BigLDogear@ dw 0                ; DATA XREF: SBK_PageFlip_Big+62w ...

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
struct s_SPELL_BOOK_PAGE * SBK_BookPages;

// WZD dseg:C8FC 00 00                                           SBK_NewSpellAnim_Stg dw 0               ; DATA XREF: CMB_SetTargetCursor+24Dr ...
// WZD dseg:C8FC                                                                                         ; (0-7), also combat casting cursor anim stage (0-4)

// WZD dseg:C8FE
// drake178: IMG_SBK_BookRealms@ SPBK_ICONS
SAMB_ptr spellbook_symbols_bitm[6];

// WZD dseg:C90A
// drake178:  the predicted amount of mana that the wizard will
// drake178:  be able to channel into a spell each turn?
// drake178:  (lower of mana+manaperturn and skill+heroskill)
// drake178:  in combat, the highest castable cost
int16_t SBK_BookManaLimit;

// WZD dseg:C90C
// drake178: NearAlloc_First!
SAMB_ptr SBK_Research_Incomes;

// WZD dseg:C90E
int16_t WIZ_ManaPerTurn;

// WZD dseg:C910
// drake178: 0Dh LBX_Alloc_Space paragraphs (first or next); NOT BIG ENOUGH TO HOLD MORE THAN 104 SPELLS!
SAMB_ptr SBK_SpellList;

// WZD dseg:C914
int16_t SBK_BookPageCount;

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
int16_t GUI_Multipurpose_Int;

// WZD dseg:C928 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+SBK_Descriptions@ dd 0                                    ; 0
// WZD dseg:C928 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; DATA XREF: GAME_LearnSpellAnim+31Ew ...
// WZD dseg:C928 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+dd 0                                    ; 1 ; 9 or 12 LBX_Alloc_Next paragraphs in the
// WZD dseg:C928 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 dd 0                                    ; 2 ; LBX_Sandbox_Segment (after a flush)
// WZD dseg:C928                                                 dd 0                                    ; 3
// WZD dseg:C928                                                 dd 0                                    ; 4
// WZD dseg:C928                                                 dd 0                                    ; 5
// WZD dseg:C928                                                 dd 0                                    ; 6
// WZD dseg:C928                                                 dd 0                                    ; 7
// WZD dseg:C928                                                 dd 0                                    ; 8
// WZD dseg:C928                                                 dd 0                                    ; 9
// WZD dseg:C928                                                 dd 0                                    ; 10
// WZD dseg:C928                                                 dd 0                                    ; 11
// WZD dseg:C928                                                 dd 0                                    ; 12
// WZD dseg:C928                                                 dd 0                                    ; 13
// WZD dseg:C928                                                 dd 0                                    ; 14
// WZD dseg:C928                                                 dd 0                                    ; 15
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
void SBK_AddHumanPlayer(int16_t Book_Type, SAMB_ptr SpList_Ptr);

// WZD o117p02
// drake178: SBK_AddRealm()
// SBK_AddRealm()

// WZD o117p03
// drake178: SBK_CountSpellGroups()
void SBK_CountSpellGroups(void);

// WZD o117p04
// drake178: SBK_BuildSpellbook()
// PUBLIC  void SBK_BuildSpellbook(int16_t Book_Type, int16_t Page_Size);

// WZD o117p05
// drake178: sub_9A9E8()
// sub_9A9E8()

// WZD o117p06
// drake178: CMB_BuildSpellbook()
// CMB_BuildSpellbook()

// WZD o117p07
// drake178: OVL_ComposeBookText()
void OVL_ComposeBookText(struct s_SPELL_BOOK_PAGE Page, SAMB_ptr spellbook_bitmap);

// WZD o117p08
// drake178: CMB_ComposeBookText()
// CMB_ComposeBookText()

// WZD o117p09
// drake178: OVL_DrawSpellbook()
void OVL_DrawSpellbook(int16_t x, int16_t y);

// WZD o117p10
// drake178: CMB_DrawSpellbook()
// CMB_DrawSpellbook()

// WZD o117p11
// drake178: SBK_PageFlip_Small()
// BOOK.LBX  "PTURNSML"
void SBK_PageTurn_Small(int turn_type, int combat_flag, int player_idx);

// WZD o117p12
// drake178: SBK_AddPages()
void SBK_AddPages(int16_t Spell_Count, int16_t Grp_Index, char * Title, int16_t Page_Size);

// WZD o117p13
// drake178: SBK_Fill()
void SBK_Fill(int16_t Page_Size);



/*
    WIZARDS.EXE  ovr118
*/

// WZD o118p01
// drake178: SBK_PageFlip_Big()
// SBK_PageFlip_Big()

// WZD o118p02
// drake178: SBK_ApprenticeBook()
// SBK_ApprenticeBook()

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
// SBK_LoadSpellDescs()

// WZD o118p09
// drake178: sub_9DDBC()
// sub_9DDBC()

// WZD o118p10
// drake178: SBK_ComposeApprPage()
// SBK_ComposeApprPage()

// WZD o118p11
// drake178: SBK_Research_Dialog()
// SBK_Research_Dialog()

// WZD o118p12
// drake178: GAME_CheckResearch()
// GAME_CheckResearch()



/*
    WIZARDS.EXE  ovr117
*/

// WZD o117p01
// drake178: SBK_AddHumanPlayer()
/*
    set spell indices in spell list
    {0, 1, 2} {overland, combat, apprentice}

*/
void SBK_AddHumanPlayer(int16_t Book_Type, SAMB_ptr SpList_Ptr)
{
    int16_t Spell_Index;
    int16_t itr_realms;  // _CX_
    int16_t itr_spells;  // _SI_
    int16_t spell_status;  // _DI_

    for(itr_realms = 0; itr_realms < 6; itr_realms++)
    {
        for(itr_spells = 0; itr_spells < 40; itr_spells++)
        {

            spell_status = 0;
            
            Spell_Index = ((itr_realms * 40) + itr_spells + 1);

            switch(Book_Type)
            {
                case 0:
                {
                    if(
                        (_players[HUMAN_PLAYER_IDX].Spells_Known[((itr_realms * 40) + itr_spells)] == 2) &&  /* S_Known */
                        (spell_data_table[Spell_Index].Eligibility > -1) &&
                        (spell_data_table[Spell_Index].Eligibility != 5)
                    )
                    {
                        spell_status = 1;
                    }
                } break;
                case 1:
                {
                    if(
                        (_players[HUMAN_PLAYER_IDX].Spells_Known[((itr_realms * 40) + itr_spells)] == 2) &&  /* S_Known */
                        (spell_data_table[Spell_Index].Eligibility != 1) &&
                        (spell_data_table[Spell_Index].Eligibility != 5)
                    )
                    {
                        spell_status = 1;
                    }
                } break;
                case 2:
                {
                    if(_players[HUMAN_PLAYER_IDX].Spells_Known[((itr_realms * 40) + itr_spells)] > 0)  /* S_Unknown */
                    {
                        if(_players[HUMAN_PLAYER_IDX].Spells_Known[((itr_realms * 40) + itr_spells)] != 2)  /* S_Known */
                        {
                            spell_status = 2;
                        }
                        else
                        {
                            spell_status = 1;
                        }
                    }
                } break;
            }

            if(spell_status > 0)
            {
                if(spell_status != 1)
                {
                    SpList_Ptr[GUI_Multipurpose_Int] = -(Spell_Index);
                }
                else
                {
                    SpList_Ptr[GUI_Multipurpose_Int] = Spell_Index;
                }

                GUI_Multipurpose_Int++;

            }
        }        
    }

}


// WZD o117p02
// drake178: SBK_AddRealm()
// SBK_AddRealm()

// WZD o117p03
// drake178: SBK_CountSpellGroups()
void SBK_CountSpellGroups(void)
{
    int16_t itr;  // _SI_

    if(magic_set.Spell_Book_Order != ST_TRUE)
    {
        for(itr = 0; itr < GUI_Multipurpose_Int; itr++)
        {
            switch(spell_data_table[abs(SBK_SpellList[itr])].Realm)
            {
                case 0:
                {
                    SBK_Group_4_Count++;
                } break;
                case 1:
                {
                    SBK_Group_6_Count++;
                } break;
                case 2:
                {
                    SBK_Group_2_Count++;
                } break;
                case 3:
                {
                    SBK_Group_5_Count++;
                } break;
                case 4:
                {
                    SBK_Group_3_Count++;
                } break;
                case 5:
                {
                    SBK_Group_1_Count++;
                } break;
            }
        }
    }
    else
    {
        for(itr = 0; itr < GUI_Multipurpose_Int; itr++)
        {
            switch(spell_data_table[abs(SBK_SpellList[itr])].Book_Section)
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

}


// WZD o117p04
// drake178: SBK_BuildSpellbook()
void SBK_BuildSpellbook(int16_t Book_Type, int16_t Page_Size)
{
    int16_t HeroSkill;
    int16_t itr2;
    int16_t Total_Pages;
    int16_t Research_Income;
    int16_t itr1;  // _SI_
    int16_t DBG_nominal_skill;  // DNE in Dasm
    int16_t DBG_mana_ratio;  // DNE in Dasm
    int16_t DBG_power_base;  // DNE in Dasm
    int16_t DBG_mana_income;  // DNE in Dasm
    int16_t DBG_mana_upkeep;  // DNE in Dasm
    int16_t DBG_mana_per_turn;  // DNE in Dasm

    SBK_SpellList = Allocate_First_Block(_screen_seg, 13);

    SBK_Research_Incomes = Near_Allocate_First(12);

    SBK_Group_3_Count = 0;      // City Spells or Death
    SBK_Group_2_Count = 0;      // Special Spells or Chaos
    SBK_Group_1_Count = 0;      // Summoning Spells or Arcane
    SBK_Group_5_Count = 0;      // Enchantments or Life
    SBK_Group_4_Count = 0;      // Unit Spells or Nature
    SBK_Group_6_Count = 0;      // Combat Spells or Sorcery
    GUI_Multipurpose_Int = 0;   // Spell List Count, New Spell Index, ...
    SBK_BookPageCount = 0;
    
    Players_Update_Magic_Power();

    Player_Magic_Power_Distribution(&Total_Pages, &Total_Pages, &Research_Income, HUMAN_PLAYER_IDX);

    for(itr1 = 0; itr1 < 6; itr1++)
    {
        Total_Pages = Player_Spell_Research_Bonus(HUMAN_PLAYER_IDX, ((itr1 * 40) + 1));

        SBK_Research_Incomes[itr1] = ((Total_Pages * Research_Income) / 100);
    }

    // _players[HUMAN_PLAYER_IDX].Nominal_Skill = Player_Base_Casting_Skill(HUMAN_PLAYER_IDX);
    DBG_nominal_skill = Player_Base_Casting_Skill(HUMAN_PLAYER_IDX);
    _players[HUMAN_PLAYER_IDX].Nominal_Skill = DBG_nominal_skill;
    
    // WIZ_ManaPerTurn = (((_players[HUMAN_PLAYER_IDX].Mana_Pnct * _players[HUMAN_PLAYER_IDX].Power_Base) / 100) - Player_Armies_And_Enchantments_Mana_Upkeep(HUMAN_PLAYER_IDX));
    DBG_mana_ratio = _players[HUMAN_PLAYER_IDX].Mana_Pnct;
    DBG_power_base = _players[HUMAN_PLAYER_IDX].Power_Base;
    DBG_mana_upkeep = Player_Armies_And_Enchantments_Mana_Upkeep(HUMAN_PLAYER_IDX);
    WIZ_ManaPerTurn = (((DBG_mana_ratio * DBG_power_base) / 100) - DBG_mana_upkeep);

    HeroSkill = Player_Hero_Casting_Skill(HUMAN_PLAYER_IDX);

    if(
        (WIZ_ManaPerTurn + _players[HUMAN_PLAYER_IDX].mana_reserve)
        <= (_players[HUMAN_PLAYER_IDX].Nominal_Skill + HeroSkill)
    )
    {
        if((WIZ_ManaPerTurn + _players[HUMAN_PLAYER_IDX].mana_reserve) <= 0)
        {
            SBK_BookManaLimit = 0;
        }
        else
        {
            SBK_BookManaLimit = (WIZ_ManaPerTurn + _players[HUMAN_PLAYER_IDX].mana_reserve);
        }
    }
    else
    {
        SBK_BookManaLimit = (_players[HUMAN_PLAYER_IDX].Nominal_Skill + HeroSkill);
    }

    SBK_AddHumanPlayer(Book_Type, SBK_SpellList);

    SBK_CountSpellGroups();

    Total_Pages = ((SBK_Group_3_Count + Page_Size - 1) / Page_Size);
    Total_Pages += ((SBK_Group_3_Count + Page_Size - 1) / Page_Size);
    Total_Pages += ((SBK_Group_3_Count + Page_Size - 1) / Page_Size);
    Total_Pages += ((SBK_Group_3_Count + Page_Size - 1) / Page_Size);
    Total_Pages += ((SBK_Group_3_Count + Page_Size - 1) / Page_Size);
    Total_Pages += ((SBK_Group_3_Count + Page_Size - 1) / Page_Size);

    SBK_BookPages = (struct s_SPELL_BOOK_PAGE *)Near_Allocate_Next((Total_Pages + 3) * sizeof(struct s_SPELL_BOOK_PAGE));

    for(itr1 = 0; (Total_Pages + 3) > itr1; itr1++)
    {
        SBK_BookPages[itr1].Spell_Count = 0;
        strcpy(SBK_BookPages[itr1].Title, "");

        for(itr2 = 0; itr2 < Page_Size; itr2++)
        {
            SBK_BookPages[itr1].Spells[itr2] = 0;  /* ¿ NO_SPELL ? */
        }
    }

    SBK_Fill(Page_Size);

    if(Page_Size == 4)
    {
        SBK_Candidate_Page = SBK_BookPageCount;

        itr2 = 0;

        for(itr1 = 0; itr1 < 4; itr1++)
        {
            if(_players[HUMAN_PLAYER_IDX].Rsrch_Candidates[itr1] > 0)
            {
                SBK_SpellList[(itr1 - itr2)] = _players[HUMAN_PLAYER_IDX].Rsrch_Candidates[itr1];
            }
            else
            {
                itr2++;
            }
        }

        SBK_AddPages((4 - itr2), -1, cnst_Rsrch_Page1, Page_Size);

        itr2 = 0;
        for(itr1 = 0; itr1 < 4; itr1++)
        {
            if(_players[HUMAN_PLAYER_IDX].Rsrch_Candidates[(4 + itr1)] <= 0)
            {
                itr2++;
            }
            else
            {
                SBK_SpellList[(itr1 - itr2)] = _players[HUMAN_PLAYER_IDX].Rsrch_Candidates[(4 + itr1)];
            }
        }
    }

}


// WZD o117p05
// drake178: sub_9A9E8()
// sub_9A9E8()

// WZD o117p06
// drake178: CMB_BuildSpellbook()
// CMB_BuildSpellbook()

// WZD o117p07
// drake178: OVL_ComposeBookText()
void OVL_ComposeBookText(struct s_SPELL_BOOK_PAGE Page, SAMB_ptr spellbook_bitmap)
{
    char Spell_Name[20];
    char temp_string[10];
    int16_t Text_Width;
    int16_t Turns_To_Cast;
    int16_t Cost_Limit;
    int16_t casting_cost;
    int16_t spell_idx;
    uint8_t colors[6];
    int16_t itr1;  // _SI_
    int16_t itr2;  // _DI_

    Draw_Picture_To_Bitmap(_spellbook_small_text, spellbook_bitmap);

    for(itr1 = 0; itr1 < 6; itr1++)
    {
        if(Page.Spell_Count > itr1)
        {
            colors[0] = 184;
            for(itr2 = 1; itr2 < 5; itr2++)
            {
                colors[itr2] = 187;
            }

            Set_Font_Colors_15(1, &colors[0]);

            spell_idx = Page.Spells[itr1];

            if(abs(spell_idx) != _players[HUMAN_PLAYER_IDX].Spell_Cast)
            {
                casting_cost = Casting_Cost(HUMAN_PLAYER_IDX, abs(spell_idx), 0);
            }
            else
            {
                casting_cost = _players[HUMAN_PLAYER_IDX].Cast_Cost_Left;
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
                    if(spell_data_table[abs(spell_idx)].Type != 11)  /* Crafting_Spell */
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

                Clipped_Copy_Bitmap(1, (7 + (itr1 * 22)), spellbook_bitmap, spellbook_symbols_bitm[spell_data_table[abs(spell_idx)].Realm]);
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
                        Clipped_Copy_Bitmap((1 + ((itr2 % 20) * 6)), (13 + (itr1 * 22)), spellbook_bitmap, spellbook_symbols_bitm[spell_data_table[abs(spell_idx)].Realm]);
                    }
                    else
                    {
                        Clipped_Copy_Bitmap((1 + (itr2 * 6)), (7 + (itr1 * 22)), spellbook_bitmap, spellbook_symbols_bitm[spell_data_table[abs(spell_idx)].Realm]);
                    }
                }
            }

            if(abs(spell_idx) != _players[HUMAN_PLAYER_IDX].Spell_Cast)
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

            // TODO  String_Copy_Far(Spell_Name, spell_data_table[abs(spell_idx)].Name);
            strcpy(Spell_Name, spell_data_table[abs(spell_idx)].Name);

            Text_Width = Get_String_Width(Spell_Name);

            Clear_Bitmap_Region(0, (itr1 * 22), (3 + Text_Width), (5 + (itr1 * 22)), spellbook_bitmap);

            Print_To_Bitmap(0, (itr1 * 22), Spell_Name, spellbook_bitmap);

            if(abs(spell_idx) != _players[HUMAN_PLAYER_IDX].Spell_Cast)
            {
                casting_cost = Casting_Cost(HUMAN_PLAYER_IDX, abs(spell_idx), 0);
            }
            else
            {
                casting_cost = _players[HUMAN_PLAYER_IDX].Cast_Cost_Left;
            }

            if(casting_cost < 0)
            {
                casting_cost = 0;
            }

            if(
                (casting_cost != 0) ||
                (spell_data_table[abs(spell_idx)].Type != 11)  /* Crafting_Spell */
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
// CMB_ComposeBookText()


// WZD o117p09
// drake178: OVL_DrawSpellbook()
void OVL_DrawSpellbook(int16_t x, int16_t y)
{
    SAMB_ptr spellbook_bitmap;
    uint8_t colors[6];
    int16_t itr;  // _SI_

    Mark_Block(_screen_seg);

    spellbook_bitmap = Allocate_Next_Block(_screen_seg, 1050);  // 1050 PR  16800 B

    // {Nature, Sorcery, Chaos, Life, Death, Arcane}
    for(itr = 0; itr < 6; itr++)
    {
        spellbook_symbols_bitm[itr] = Allocate_Next_Block(_screen_seg, 4);  // 4 PR  64 B
        Draw_Picture_To_Bitmap(_spellbook_small_symbols[itr], spellbook_symbols_bitm[itr]);
    }

    // TODO  Cycle_Palette_Color(198, 41, 33, 29, 48, 43, 43, 3);

    FLIC_Draw(x, y, _spellbook_small_seg);

    if(SBK_Dogears == 1)
    {
        if(SBK_OpenPage != 0)
        {
            FLIC_Draw((x + 13), (y + 4), _spellbook_small_left_corner_seg);
        }
        if(
            (SBK_OpenPage != SBK_BookPageCount) &&
            ((SBK_BookPageCount - 2) > SBK_OpenPage)
        )
        {
            FLIC_Draw((x + 258), (y + 4), _spellbook_small_right_corner_seg);
        }
    }

    OVL_ComposeBookText(SBK_BookPages[SBK_OpenPage], spellbook_bitmap);
    Draw_Picture((x + 16), (y + 21), spellbook_bitmap);

    colors[0] = 53;
    for(itr = 1; itr < 5; itr++)
    {
        colors[itr] = 46;
    }
    Set_Font_Colors_15(4, &colors[0]);

    Print_Centered((x + 70), (y + 6), SBK_BookPages[SBK_OpenPage].Title);

    Draw_Picture_To_Bitmap(_spellbook_small_text, spellbook_bitmap);
    OVL_ComposeBookText(SBK_BookPages[(SBK_OpenPage + 1)], spellbook_bitmap);
    Draw_Picture((x + 148), (y + 21), spellbook_bitmap);

    colors[0] = 53;
    for(itr = 1; itr < 5; itr++)
    {
        colors[itr] = 46;
    }
    Set_Font_Colors_15(4, &colors[0]);

    Print_Centered((x + 208), (y + 6), SBK_BookPages[(SBK_OpenPage + 1)].Title);

    Release_Block(_screen_seg);

}


// WZD o117p10
// drake178: CMB_DrawSpellbook()
// CMB_DrawSpellbook()

// WZD o117p11
// drake178: SBK_PageFlip_Small()
// BOOK.LBX  "PTURNSML"
void SBK_PageTurn_Small(int turn_type, int combat_flag, int player_idx)
{
    int16_t Stage_Text_Widths[4];
    int16_t Book_Image_Top;
    int16_t Book_Image_Left;
    uint8_t colors[6];
    int16_t Color_Loop_Var;
    int16_t PageTurn_Stage;
    SAMB_ptr Base_Page_IMG;
    SAMB_ptr page_turn_bitmap;  // _SI_
    int16_t itr;  // DNE in Dasm

    Book_Image_Left = 16;
    Book_Image_Top = 10;

    Stage_Text_Widths[3] = 85;
    Stage_Text_Widths[0] = 85;
    Stage_Text_Widths[2] = 55;
    Stage_Text_Widths[1] = 55;

    // TODO  Deactivate_Auto_Function();

    // TODO  VGA_FILEH_LoadFirst(cnst_BookAnim_File, 0);

    if(combat_flag == ST_FALSE)
    {
        Allocate_Reduced_Map();
    }

    Mark_Block(_screen_seg);
    Base_Page_IMG = Allocate_Next_Block(_screen_seg, 1040);
    page_turn_bitmap = Allocate_Next_Block(_screen_seg, 1400);

    for(itr = 0; itr < 6; itr++)
    {
        spellbook_symbols_bitm[itr] = Allocate_Next_Block(_screen_seg, 4);
        Draw_Picture_To_Bitmap(_spellbook_small_symbols[itr], spellbook_symbols_bitm[itr]);
    }

    // TODO  Cycle_Palette_Color(198, 39, 31, 25, 50, 46, 47, 3);

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
        // TODO  Assign_Auto_Function(Spellbook_Screen_Draw, 2);
    }


}


// WZD o117p12
// drake178: SBK_AddPages()
void SBK_AddPages(int16_t Spell_Count, int16_t Grp_Index, char * Title, int16_t Page_Size)
{
    int16_t Group_Match;
    int16_t Spells_on_Page;
    int16_t List_Index;

    Spells_on_Page = 0;

    strcpy(SBK_BookPages[SBK_BookPageCount].Title, Title);

    List_Index = 0;

    while(Spell_Count > 0)
    {
        Group_Match = ST_FALSE;

        if(magic_set.Spell_Book_Order != ST_TRUE)
        {
            if(
                (spell_data_table[abs(SBK_SpellList[List_Index])].Realm == Grp_Index) ||
                (Grp_Index == -1)
            )
            {
                Group_Match = ST_TRUE;
            }
        }
        else
        {
            if(
                (spell_data_table[abs(SBK_SpellList[List_Index])].Book_Section == Grp_Index) ||
                (Grp_Index == -1)
            )
            {
                Group_Match = ST_TRUE;
            }
        }

        if(Group_Match == ST_TRUE)
        {
            Spells_on_Page++;

            if((Page_Size + 1) == Spells_on_Page)
            {
                SBK_BookPages[SBK_BookPageCount].Spell_Count = Page_Size;

                Spells_on_Page = 1;

                SBK_BookPageCount++;

                if((SBK_BookPageCount % 2) == 1)  /* ¿ odd pages ? ¿ {0,1} or {1,2} ? */
                {
                    if(magic_set.Spell_Book_Order != ST_TRUE)
                    {
                        if(spell_data_table[SBK_BookPages[(SBK_BookPageCount - 1)].Spells[0]].Realm != Grp_Index)
                        {
                            strcpy(SBK_BookPages[SBK_BookPageCount].Title, Title);
                        }
                    }
                    else
                    {
                        if(spell_data_table[SBK_BookPages[(SBK_BookPageCount - 1)].Spells[0]].Book_Section != Grp_Index)
                        {
                            strcpy(SBK_BookPages[SBK_BookPageCount].Title, Title);
                        }
                    }

                }
                else
                {
                    strcpy(SBK_BookPages[SBK_BookPageCount].Title, Title);
                }
                
            }

            Spell_Count--;

            if(Page_Size != 4)
            {
                if(SBK_SpellList[List_Index] == _players[HUMAN_PLAYER_IDX].Spell_Cast)
                {
                    SBK_OpenPage = Spells_on_Page;
                }
            }

            SBK_BookPages[SBK_BookPageCount].Spells[(Spells_on_Page - 1)] = SBK_SpellList[List_Index];
        }

        List_Index++;
    }

    SBK_BookPages[SBK_BookPageCount].Spell_Count = Spells_on_Page;

    SBK_BookPageCount++;
    
}

// WZD o117p13
// drake178: SBK_Fill()
void SBK_Fill(int16_t Page_Size)
{

    if(magic_set.Spell_Book_Order == ST_TRUE)
    {
        if(SBK_Group_1_Count > 0)
        {
            SBK_AddPages(SBK_Group_1_Count, 1, cnst_SpellGroup1, Page_Size);  // "Summoning"
        }
        if(SBK_Group_2_Count > 0)
        {
            SBK_AddPages(SBK_Group_2_Count, 0, cnst_SpellGroup2, Page_Size);  // "Special Spells"
        }
        if(SBK_Group_3_Count > 0)
        {
            SBK_AddPages(SBK_Group_3_Count, 3, cnst_SpellGroup3, Page_Size);  // "City Spells"
        }
        if(SBK_Group_5_Count > 0)
        {
            SBK_AddPages(SBK_Group_5_Count, 2, cnst_SpellGroup5, Page_Size);  // "Enchantment"
        }
        if(SBK_Group_4_Count > 0)
        {
            SBK_AddPages(SBK_Group_4_Count, 4, cnst_SpellGroup4, Page_Size);  // "Unit Spells"
        }
        if(SBK_Group_6_Count > 0)
        {
            SBK_AddPages(SBK_Group_6_Count, 5, cnst_SpellGroup6, Page_Size);  // "Combat Spells"
        }
    }
    else
    {
        if(SBK_Group_1_Count > 0)
        {
            SBK_AddPages(SBK_Group_1_Count, 5, cnst_SpellRealm1, Page_Size);  // "Arcane"
        }
        if(SBK_Group_2_Count > 0)
        {
            SBK_AddPages(SBK_Group_2_Count, 2, cnst_SpellRealm2, Page_Size);  // "Chaos"
        }
        if(SBK_Group_3_Count > 0)
        {
            SBK_AddPages(SBK_Group_3_Count, 4, cnst_SpellRealm3, Page_Size);  // "Death"
        }
        if(SBK_Group_5_Count > 0)
        {
            SBK_AddPages(SBK_Group_5_Count, 3, cnst_SpellRealm5, Page_Size);  // "Life"
        }
        if(SBK_Group_4_Count > 0)
        {
            SBK_AddPages(SBK_Group_4_Count, 0, cnst_SpellRealm4, Page_Size);  // "Nature"
        }
        if(SBK_Group_6_Count > 0)
        {
            SBK_AddPages(SBK_Group_6_Count, 1, cnst_SpellRealm6, Page_Size);  // "Sorcery"
        }
    }

    if((SBK_BookPageCount % 2) == 1)
    {
        SBK_BookPages[SBK_BookPageCount].Spell_Count = 0;
        SBK_BookPageCount++;
    }

}



/*
    WIZARDS.EXE  ovr118
*/

// WZD o118p01
// drake178: SBK_PageFlip_Big()
// SBK_PageFlip_Big()

// WZD o118p02
// drake178: SBK_ApprenticeBook()
// SBK_ApprenticeBook()

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
// SBK_LoadSpellDescs()

// WZD o118p09
// drake178: sub_9DDBC()
// sub_9DDBC()

// WZD o118p10
// drake178: SBK_ComposeApprPage()
// SBK_ComposeApprPage()

// WZD o118p11
// drake178: SBK_Research_Dialog()
// SBK_Research_Dialog()

// WZD o118p12
// drake178: GAME_CheckResearch()
// GAME_CheckResearch()
