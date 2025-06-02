
...most of this should be in the spellbook docs...

Elsewhere, Player_Research_Spells()



Apprentice Screen

Big Spell Book

...the apprentice button...your complete spell library...

SEEALSO: Spell Research








## m_spellbook_pages
AKA SBK_BookPages
¿ any reason this shouldn't just be named m_spellbook / m_spell_book ? ¿ or, m_spell_library ?


## SBK_OpenPage
XREFS:  (117)




### Spell Book Page - Title
...in the code
¿ Spellbook_Add_Page() ?
    strcpy(m_spellbook_pages[m_spellbook_page_count].title, title);

~ 58,10


```
jbalcomb@HMS-PinkThad3 MINGW64 /c/STU/devel/ReMoM/src (develop)
$ grep -i title *
Spellbook.C:        strcpy(m_spellbook_pages[itr1].title, "");
Spellbook.C:    Print_Centered((x + 70), (y + 6), m_spellbook_pages[SBK_OpenPage].title);
Spellbook.C:    Print_Centered((x + 208), (y + 6), m_spellbook_pages[(SBK_OpenPage + 1)].title);
Spellbook.C:void Spellbook_Add_Page(int16_t Spell_Count, int16_t Grp_Index, char * title, int16_t Page_Size)
Spellbook.C:    strcpy(m_spellbook_pages[m_spellbook_page_count].title, title);
Spellbook.C:                            strcpy(m_spellbook_pages[m_spellbook_page_count].title, title);
Spellbook.C:                            strcpy(m_spellbook_pages[m_spellbook_page_count].title, title);
Spellbook.C:                    strcpy(m_spellbook_pages[m_spellbook_page_count].title, title);
Spellbook.C:            Print_Centered_To_Bitmap(66, 30, m_spellbook_pages[page].title, pict_seg);
Spellbook.C:            Print_Centered_To_Bitmap(58, 30, m_spellbook_pages[page].title, pict_seg);
Spellbook.H:    /* 00 */   char title[16];
Spellbook.H:void Spellbook_Add_Page(int16_t Spell_Count, int16_t Grp_Index, char * Title, int16_t Page_Size);
```

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

depends on Spellbook_Group_Counts()
    wrong count, wrong title



### Spell Book Page - Spell Description

all/only big book?

SBK_LoadSpellDescs__WIP(page)

XREF:
    GAME_LearnSpellAnim+32E
    GAME_LearnSpellAnim+5B7
    GAME_LearnSpellAnim+61A
    Apprentice_Screen__WIP+BC
    Apprentice_Screen__WIP+2E4
    Apprentice_Screen__WIP+349
    SBK_Research_Dialog__STUB+C3
    NX_j_SBK_LoadSpellDescs()

Apprentice_Screen__WIP()
    SBK_LoadSpellDescs__WIP(SBK_OpenPage)
    ...
    if(input_field_idx == dogear_right_field)
        SBK_LoadSpellDescs__WIP((SBK_OpenPage + 2))
    if(input_field_idx == dogear_rleft_field)
        SBK_LoadSpellDescs__WIP((SBK_OpenPage - 2))


empties all 16 strings

¿ same code for iter over SBK_OpenPage and iter over page parameter ?
Yes.
Why?
...IDG...but, during the left page-turn, SBK_OpenPage is 2 and page is 0
...so, dupes the descriptions into 0,2 and 1,3?
...upon return to Apprentice_Screen(), copies {8,...,15} into {0,...7} i.e., 2 into 0 and 3 into 1

for(itr = SBK_OpenPage; (SBK_OpenPage + 1) >= itr; itr++)
for(itr = page; (page + 1) >= itr; itr++)
if((itr < 0) || (itr >= m_spellbook_page_count))
    strcpy(SBK_Descriptions[((itr - SBK_OpenPage) + count)], str_empty_string__ovr118)
else
    LBX_Load_Data_Static(desc_lbx_file__ovr118, 0, (SAMB_ptr)buffer, abs(m_spellbook_pages[itr].spell[count]), 1, 110);
    strcpy(SBK_Descriptions[((itr - page) + count)], buffer);

¿ math on SBK_Descriptions{} looks like it treats the 16 descriptions as 4 pages of 4 ?

...Print Spell Description(s)...


Apprentice_Screen()

        if(input_field_idx == dogear_left_field)
        {

            if(SBK_OpenPage > 1)
            {

                SBK_LoadSpellDescs__WIP((SBK_OpenPage - 2));

                BigBook_PageTurn__WIP(0);

                SBK_OpenPage -= 2;

                for(itr = 0; itr < 8; itr++)
                {
                    // TODO  String_Copy_Far(SBK_Descriptions[(0 + itr)], SBK_Descriptions[(8 + itr)]);
                    strcpy(SBK_Descriptions[(0 + itr)], SBK_Descriptions[(8 + itr)]);
                }

                Set_Page_Off();
                Spellbook_Big_Draw();
                PageFlip_FX();

            }

        }






### SBK_Descriptions

all/only big book?

 * GAME_LearnSpellAnim  
 * SBK_Research_Dialog__STUB()  
 * Apprentice_Screen__WIP()  


// WZD dseg:C928
/*
¿ 2 pages, 8 per page ?
*/
char * SBK_Descriptions[16] = { 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB };

XREF:
    GAME_LearnSpellAnim+31E      mov     [word ptr SBK_Descriptions@+bx], ax; 9 or 12 LBX_Alloc_Next paragraphs in the    
    GAME_LearnSpellAnim+384      push    SBK_Descriptions@[bx-4+2]       ; src_sgmt                                       
    GAME_LearnSpellAnim+388      push    SBK_Descriptions@[bx-4]         ; src_ofst                                       
    GAME_LearnSpellAnim+5E5      push    [word ptr SBK_Descriptions@+bx] ; dst_ofst                                       
    GAME_LearnSpellAnim+649      push    [word ptr SBK_Descriptions@+bx] ; dst_ofst                                       
    SBK_LoadSpellDescs__WIP+1D   push    [word ptr SBK_Descriptions@+bx] ; dst_ofst                                       
    SBK_LoadSpellDescs__WIP+BA   push    [word ptr SBK_Descriptions@+bx] ; dst_ofst                                       
    SBK_LoadSpellDescs__WIP+F0   push    [word ptr SBK_Descriptions@+bx] ; dst_ofst                                       
    SBK_LoadSpellDescs__WIP+19B  push    [word ptr SBK_Descriptions@+bx] ; dst_ofst                                       
    SBK_LoadSpellDescs__WIP+1D3  push    [word ptr SBK_Descriptions@+bx] ; dst_ofst                                       
    Apprentice_Screen__WIP+AD    mov     [word ptr SBK_Descriptions@+bx], ax; 9 or 12 LBX_Alloc_Next paragraphs in the    
    Apprentice_Screen__WIP+312   push    [word ptr SBK_Descriptions@+bx] ; dst_ofst                                       
    Apprentice_Screen__WIP+376   push    [word ptr SBK_Descriptions@+bx] ; dst_ofst                                       
    BigBook_Compose__WIP+4F2 push    [word ptr SBK_Descriptions@+bx] ; src_ofst                                       
    SBK_Research_Dialog__STUB+AE mov     [word ptr SBK_Descriptions@+bx], ax; 9 or 12 LBX_Alloc_Next paragraphs in the    
    GAME_LearnSpellAnim+31A      mov     [word ptr (SBK_Descriptions@+2)+bx], dx; 9 or 12 LBX_Alloc_Next paragraphs in the
    GAME_LearnSpellAnim+5E1      push    [word ptr (SBK_Descriptions@+2)+bx]; dst_sgmt                                    
    GAME_LearnSpellAnim+645      push    [word ptr (SBK_Descriptions@+2)+bx]; dst_sgmt                                    
    SBK_LoadSpellDescs__WIP+19   push    [word ptr (SBK_Descriptions@+2)+bx]; dst_sgmt                                    
    SBK_LoadSpellDescs__WIP+B6   push    [word ptr (SBK_Descriptions@+2)+bx]; dst_sgmt                                    
    SBK_LoadSpellDescs__WIP+EC   push    [word ptr (SBK_Descriptions@+2)+bx]; dst_sgmt                                    
    SBK_LoadSpellDescs__WIP+197  push    [word ptr (SBK_Descriptions@+2)+bx]; dst_sgmt                                    
    SBK_LoadSpellDescs__WIP+1CF  push    [word ptr (SBK_Descriptions@+2)+bx]; dst_sgmt                                    
    Apprentice_Screen__WIP+A9    mov     [word ptr (SBK_Descriptions@+2)+bx], dx; 9 or 12 LBX_Alloc_Next paragraphs in the
    Apprentice_Screen__WIP+30E   push    [word ptr (SBK_Descriptions@+2)+bx]; dst_sgmt                                    
    Apprentice_Screen__WIP+372   push    [word ptr (SBK_Descriptions@+2)+bx]; dst_sgmt                                    
    BigBook_Compose__WIP+4EE push    [word ptr (SBK_Descriptions@+2)+bx]; src_sgmt                                    
    SBK_Research_Dialog__STUB+AA mov     [word ptr (SBK_Descriptions@+2)+bx], dx; 9 or 12 LBX_Alloc_Next paragraphs in the





## m_spellbook_spell_list
AKA SBK_SpellList

***ALWAYS*** index using itr_spellbook_spell_list

```c
// WZD dseg:C910
// drake178: 0Dh LBX_Alloc_Space paragraphs (first or next); NOT BIG ENOUGH TO HOLD MORE THAN 104 SPELLS!
SAMB_ptr m_spellbook_spell_list;
```

SBK_CountSpellGroups__WIP()
    uses GUI_Multipurpose_Int to iter over m_spellbook_spell_list[] to index spell_data_table[]
    switches on realm or category to incr counts
    6 groups
    (don't see any reason it wouldn't/couldn't/shouldn't be an array of counts)
    (¿ don't see anything indexing it like an array, though ?)
    (cases don't line up with indices)
book order: arcane, chaos, life, nature, sorcery, ..., Research Spells
¿ death ?
¿ sbr_ ?



Spellbook_Add_Page() increments m_spellbook_page_count

## m_spellbook_page_count
AKA SBK_BookPageCount














Apprentice_Research_List()
WIZ_GetResearchList__STUB()






WIZ_ResearchSort__STUB()



## Spellbook_Big_Draw()
~ Big Spellbook Draw
AKA SBK_ApprenticeBook__WIP()



## "Choose a new spell to research"

Spellbook_Big_Draw(2, 4)  
    ...  
    if(SBK_Dogears == 0) { ... }  






##### Naming Things Is Hard

PTURNSML  ... page turn small ...  ... small as in small spellbook ...  
PTURNBIG  ... page turn big ...    ... big as in big spellbook ...  
complete spell library  
spell library  
runic script  
Spells...apprentice’s spellbook...divided into categories  
research spells are those that you can learn  
spells...currently researching and casting appear in glowing letters  



BOOK.LBX  

PTURNSML  
PTURNBIG  


SCROLL.LBX

BIGBOOK     ""
BCORNERS    "big left corner"
BCORNERS    "big right corner"


Page 12  (PDF Page 17)

Every wizard in Master of Magic has a spell library which represents those spells that the wizard can research and learn.  
This library is composed of spell books (called ranks).  
Each spell rank contains cryptic information on spells of a specific type of magic (life, death, chaos, nature or sorcery).  
The more spell ranks a wizard has in a particular area, the more spells he or she can research in that area.  
Also, a wizard can learn spells of greater rarity if he or she has more spell ranks in the relevant area.  


Page 21  (PDF Page 26)

In the bottom right corner of the main movement screen is the next turn button.  
Clicking on this button means you are done with this turn and takes you to the next turn in the game.  
This updates all of your economic, diplomatic and research data by one turn and allows your units to start a new turn of movement.  


Page 36  (PDF Page 41)

APPRENTICE
Clicking on the apprentice button takes you to your complete spell library.  
All the spells that you have already researched are legible in this book.  
All of the spells that you will eventually be able to research appear in a runic script.  
Spells in the apprentice’s spellbook are divided into categories:  
    summoning spells, special spells, city spells, enchantments, unit spells, combat spells and research spells.  
The research spells are those that you can learn at this time.  
The spells that you are currently researching and casting appear in glowing letters.  
To turn the pages of the apprentice’s spellbook, click on the dog ears in the upper corners of the pages.  
Click on the bookmark to exit.  
