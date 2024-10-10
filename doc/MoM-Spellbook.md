
Spell Book
Spellbook

¿¿¿
Spells button
Apprentice
Combat
Choose Research
Learn Spell
???



¿ anywhere it might be worth knowing what is specific to big or small or just agnostic ?

'Big Book'      Apprentice Screen, Choose Research, Learn Spell
'Small Book'    Spells button/Overland, Combat

"BIGBOOK"
"SMLBOOK"



Spellbook_Build__WIP()
Build_Spell_List()
Build_Spell_List()
Spellbook_Group_Counts()
Spellbook_Add_Group_Pages()
Spellbook_Add_Page()
OVL_ComposeBookText()
SBK_LoadSpellDescs__WIP()
BigBook_PageTurn__WIP()
Spellbook_Big_Draw()
Spellbook_Big_Draw()
BigBook_Compose__WIP()



Apprentice_Screen__WIP()
    |-> Spellbook_Build__WIP(2, 4)
        |-> Build_Spell_List()
        |-> Spellbook_Group_Counts()
        |-> Spellbook_Add_Group_Pages()
        |-> Spellbook_Add_Page()
            ...
            strcpy(m_spellbook_pages[m_spellbook_page_count].title, title);
            ...
    ...
    |-> SBK_LoadSpellDescs__WIP()
    |-> BigBook_PageTurn__WIP()
    |-> Spellbook_Big_Draw()
    ...
    Spellbook_Big_Draw()
        |-> BigBook_Compose__WIP()

¿ anywhere it might be worth knowing what is specific to big or small or just agnostic ?





## Spellbook_Build__WIP()

; Book_Type corresponds to one of the following:
;   0 - overland spellbook
;   1 - combat spellbook
;   2 - Apprentice spellbook

XREF:
    j_SBK_BuildSpellbook__WIP()
        GAME_LearnSpellAnim()
        Apprentice_Screen__WIP()
        Spell_Research_Select__STUB()
        Spellbook_Screen()

Apprentice_Screen__WIP()
    |-> Spellbook_Build__WIP(2, 4)

Spell_Research_Select__STUB()
    |-> Spellbook_Build__WIP(2, 4)

Spellbook_Screen()
    |-> Spellbook_Build__WIP(0, 6)

GAME_LearnSpellAnim()
    |-> Spellbook_Build__WIP(2, 4)

G_CMB_CastSpell(Caster_ID)
    |-> CMB_BuildSpellbook(Caster_ID)
        ...
        |-> SBK_AddRealm()
        ...
        |-> Build_Spell_List()
        ...
        |-> Spellbook_Group_Counts()
        ...
        |-> Spellbook_Add_Group_Pages(6)
        ...no Spellbook_Add_Page() ?

allocates m_spellbook_spell_list
allocates SBK_Research_Incomes
clears group counts, spell list count, and spellbook page count


SBK_Research_Incomes[itr_magic_realms] = ((research_points * research_bonus_percentage) / 100);


...
Build_Spell_List()
...

...
Spellbook_Group_Counts()
...


## SBK_CountSpellGroups__WIP()
if(magic_set.spell_book_ordering == ST_TRUE)
    SBK_Group_2_Count++;  // "Special Spells"
    SBK_Group_1_Count++;  // "Summoning"
    SBK_Group_5_Count++;  // "Enchantment"
    SBK_Group_3_Count++;  // "City Spells"
    SBK_Group_4_Count++;  // "Unit Spells"
    SBK_Group_6_Count++;  // "Combat Spells"
else
    SBK_Group_4_Count++;  // "Nature"
    SBK_Group_6_Count++;  // "Sorcery"
    SBK_Group_2_Count++;  // "Chaos"
    SBK_Group_5_Count++;  // "Life"
    SBK_Group_3_Count++;  // "Death"
    SBK_Group_1_Count++;  // "Arcane"

test game data

Summoning:      17
Special:        22  4 unknown
City:            3
Enchantment:     7
Unit:           17
Combat:          6
Research:        4

21 pages



### SBK_Group_¿?_Count vs. Magic Realm vs. Spell Book Category

spell book category order in OG-MoM is "Summoning", "Special Spells", "City Spells", "Enchantment", "Unit Spells", "Combat Spells"

in-mem
// WZD dseg:C916
int16_t SBK_Group_6_Count;
int16_t SBK_Group_5_Count;
int16_t SBK_Group_4_Count;
int16_t SBK_Group_3_Count;
int16_t SBK_Group_2_Count;
int16_t SBK_Group_1_Count;

SBK_CountSpellGroups__WIP() does the counts by magic realm in order of SBR

Spellbook_Add_Group_Pages() does either/both in order of how they are in the spellbook (of course?)

Spellbook_Build__WIP()
    Total_Pages =  ((SBK_Group_3_Count + page_spell_count - 1) / page_spell_count);
    Total_Pages += ((SBK_Group_2_Count + page_spell_count - 1) / page_spell_count);
    Total_Pages += ((SBK_Group_1_Count + page_spell_count - 1) / page_spell_count);
    Total_Pages += ((SBK_Group_5_Count + page_spell_count - 1) / page_spell_count);
    Total_Pages += ((SBK_Group_4_Count + page_spell_count - 1) / page_spell_count);
    Total_Pages += ((SBK_Group_6_Count + page_spell_count - 1) / page_spell_count);

// WZD dseg:5D4D
char cnst_SpellGroup1[] = "Summoning";
char cnst_SpellGroup2[] = "Special Spells";
char cnst_SpellGroup3[] = "City Spells";
char cnst_SpellGroup4[] = "Enchantment";
char cnst_SpellGroup5[] = "Unit Spells";
char cnst_SpellGroup6[] = "Combat Spells";

// WZD dseg:5D98
char cnst_SpellRealm1[] = "Arcane";
char cnst_SpellRealm2[] = "Chaos";
char cnst_SpellRealm3[] = "Death";
char cnst_SpellRealm4[] = "Life";
char cnst_SpellRealm5[] = "Nature";
char cnst_SpellRealm6[] = "Sorcery";



## Spellbook_Add_Group_Pages()
AKA SBK_Fill()

    Spellbook_Add_Page(SBK_Group_1_Count, 1, cnst_SpellGroup1, Page_Size);  // "Summoning"
    Spellbook_Add_Page(SBK_Group_2_Count, 0, cnst_SpellGroup2, Page_Size);  // "Special Spells"
    Spellbook_Add_Page(SBK_Group_3_Count, 3, cnst_SpellGroup3, Page_Size);  // "City Spells"
    Spellbook_Add_Page(SBK_Group_5_Count, 2, cnst_SpellGroup5, Page_Size);  // "Enchantment"
    Spellbook_Add_Page(SBK_Group_4_Count, 4, cnst_SpellGroup4, Page_Size);  // "Unit Spells"
    Spellbook_Add_Page(SBK_Group_6_Count, 5, cnst_SpellGroup6, Page_Size);  // "Combat Spells"
    OR
    Spellbook_Add_Page(SBK_Group_1_Count, sbr_Arcane,  cnst_SpellRealm1, page_spell_count);  // "Arcane"
    Spellbook_Add_Page(SBK_Group_2_Count, sbr_Chaos,   cnst_SpellRealm2, page_spell_count);  // "Chaos"
    Spellbook_Add_Page(SBK_Group_3_Count, sbr_Death,   cnst_SpellRealm3, page_spell_count);  // "Death"
    Spellbook_Add_Page(SBK_Group_5_Count, sbr_Life,    cnst_SpellRealm5, page_spell_count);  // "Life"
    Spellbook_Add_Page(SBK_Group_4_Count, sbr_Nature,  cnst_SpellRealm4, page_spell_count);  // "Nature"
    Spellbook_Add_Page(SBK_Group_6_Count, sbr_Sorcery, cnst_SpellRealm6, page_spell_count);  // "Sorcery"





## Spellbook_Add_Page()
AKA SBK_AddPages()


void Spellbook_Add_Page(int16_t Spell_Count, int16_t group_idx, char * title, int16_t Page_Size)

...
    |-> Spellbook_Build__WIP(2, 4)
        |-> Build_Spell_List()
        |-> Spellbook_Group_Counts()
        |-> Spellbook_Add_Group_Pages(Page_Size)
            |-> Spellbook_Add_Page(SBK_Group_1_Count, 1, cnst_SpellGroup1, Page_Size);
            ...
        |-> Spellbook_Add_Page()

XREF:
    Spellbook_Build__WIP()
    Spellbook_Add_Group_Pages()
    NX_j_SBK_AddPages__WIP()

call from Spellbook_Add_Group_Pages() does 6 categories or magic realms
subsequent call from Spellbook_Build__WIP() is just for research pages
    hence the code for the group of no-group


Spellbook_Build__WIP()
        BEGIN: "Research Spells"
        Spellbook_Add_Page((4 - itr2), ST_UNDEFINED, cnst_Rsrch_Page1, Page_Size);
        Spellbook_Add_Page((4 - itr2), ST_UNDEFINED, cnst_Rsrch_Page2, Page_Size);

Spell_Count is count of spells in the *group*
Page_Size is count of spells to fit on a page
¿ code that manages more spells in group than fit on page ?






## Build_Spell_List()

~ "OVER" vs. "CMB"

XREF:
    NX_j_Build_Spell_List()
    Spellbook_Build__WIP()
    CMB_BuildSpellbook+12E      call    near ptr Build_Spell_List     ; adds the list of the human player's spells into the
    CMB_BuildSpellbook+2C4      call    near ptr Build_Spell_List     ; adds the list of the human player's spells into the

Build_Spell_List(Book_Type, m_spellbook_spell_list);

Spell_Research_Select__STUB()
    |-> Spellbook_Build__WIP(2, 4)
        |-> Build_Spell_List(Book_Type, m_spellbook_spell_list)

Spellbook_Screen()
    |-> Spellbook_Build__WIP(0, 6)
        |-> Build_Spell_List(Book_Type, m_spellbook_spell_list)

Apprentice_Screen__WIP()
    |-> Spellbook_Build__WIP(2, 4)
        |-> Build_Spell_List(Book_Type, m_spellbook_spell_list)

GAME_LearnSpellAnim()
    |-> Spellbook_Build__WIP(2, 4)
        |-> Build_Spell_List(Book_Type, m_spellbook_spell_list)

GAME_LearnSpellAnim()
    |-> Spellbook_Build__WIP(2, 4)
        |-> Build_Spell_List(Book_Type, m_spellbook_spell_list)




### Spellbook Categories

SBK_Group_3_Count   ; City Spells or Death
SBK_Group_2_Count   ; Special Spells or Chaos
SBK_Group_1_Count   ; Summoning Spells or Arcane
SBK_Group_5_Count   ; Enchantments or Life
SBK_Group_4_Count   ; Unit Spells or Nature
SBK_Group_6_Count   ; Combat Spells or Sorcery
¿ Research ?

Spellbook_Add_Group_Pages(Page_Size)
    Spellbook_Add_Page(SBK_Group_1_Count, 1, cnst_SpellGroup1, Page_Size);  // "Summoning"
    Spellbook_Add_Page(SBK_Group_2_Count, 0, cnst_SpellGroup2, Page_Size);  // "Special Spells"
    Spellbook_Add_Page(SBK_Group_3_Count, 3, cnst_SpellGroup3, Page_Size);  // "City Spells"
    Spellbook_Add_Page(SBK_Group_5_Count, 2, cnst_SpellGroup5, Page_Size);  // "Enchantment"
    Spellbook_Add_Page(SBK_Group_4_Count, 4, cnst_SpellGroup4, Page_Size);  // "Unit Spells"
    Spellbook_Add_Page(SBK_Group_6_Count, 5, cnst_SpellGroup6, Page_Size);  // "Combat Spells"
    OR
    Spellbook_Add_Page(SBK_Group_1_Count, 5, cnst_SpellRealm1, Page_Size);  // "Arcane"
    Spellbook_Add_Page(SBK_Group_2_Count, 2, cnst_SpellRealm2, Page_Size);  // "Chaos"
    Spellbook_Add_Page(SBK_Group_3_Count, 4, cnst_SpellRealm3, Page_Size);  // "Death"
    Spellbook_Add_Page(SBK_Group_5_Count, 3, cnst_SpellRealm5, Page_Size);  // "Life"
    Spellbook_Add_Page(SBK_Group_4_Count, 0, cnst_SpellRealm4, Page_Size);  // "Nature"
    Spellbook_Add_Page(SBK_Group_6_Count, 1, cnst_SpellRealm6, Page_Size);  // "Sorcery"
void Spellbook_Add_Page(int16_t Spell_Count, int16_t Grp_Index, char * title, int16_t Page_Size)





OVL_ComposeBookText()
    |-> LBX_DrawTextLine()
        |-> LBX_DrawStyledString()
            |-> LBX_DrawString()
                |-> LBX_DrawGlyph()

1oom:
    |-> lbxfont_print_str_normal_do()
        |-> lbxfont_print_str_do()
            |-> lbxfont_print_char_ret_x()
                |-> lbxfont_plotchar()
                    |-> lbxfont_plotchar_1x()

MoO2: Module: fonts

    |-> Print_To_Bitmap()
        |-> Print_Display_To_Bitmap()
            |-> Print_String_To_Bitmap()
                |-> Print_Character_To_Bitmap()
                    |-> Print_Clipped_Letter_To_Bitmap()




LBX_DrawGlyph()  ~== lbxfont_plotchar() && lbxfont_plotchar_1x()


LBX_DrawJustified()  ~==  lbxfont_print_str_normal_do_w0()


print_xpos, print_ypos  ~== lbxfont_temp_x, lbxfont_temp_y


Print_Display_To_Bitmap()  vs. Print_Display()

    outline_style checks
        != 2
        != 1 && != 3
        == 3 || == 5
        > 3
        == 5

## Print_Display_To_Bitmap()
    no draw_alias_flag

## Print_Display()

















##### Naming Things Is Hard




BOOK.LBX  

"PTURNSML"  ""  
"PTURNBIG"  ""  


SCROLL.LBX  

"BIGBOOK"       ""  
"BCORNERS"      "big left corner"  
"BCORNERS"      "big right corner"  


SPECIAL2.LBX  

034 "SMLBOOK"   ""  
035 "BCORNERS"  ""  
036 "BCORNERS"  ""  
037 "BOOKSYMB"  ""  
038 "BOOKSYMB"  ""  
039 "BOOKSYMB"  ""  
040 "BOOKSYMB"  ""  
041 "BOOKSYMB"  ""  
042 "BOOKSYMB"  ""  
043 "SMALTEXT"  ""  


Page 25  (PDF Page 30)

Spells





Clicking on the spells button takes you to your spellbook.
Only those spells that you know and that can be cast overland
(i.e., noncombat spells such as city enchantments, unit enchantments, summoning spells, etc.)
are visible.
At the beginning of the game, the only spell in your spellbook is likely to be magic spirit, a common arcane summoning spell described in the Spellbook supplement.
The casting cost of a spell is shown to the right of the spell’s name in your spellbook.


Dog ear: click here to turn page
Casting cost
Bookmark: click here to close book
Length of time to cast
Name of spell
Type of spell effect
