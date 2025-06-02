




## Variables

SBK_OpenPage

CMB_SpellBookPage
SBK_SomePageSaveVar
SBK_Candidate_Page

G_WLD_StaticAssetRfrsh()
    SBK_SomePageSaveVar = 0
    CMB_SpellBookPage = 0
    SBK_Candidate_Page = 0
Settings_Screen()
    CMB_SpellBookPage = 0
    SBK_SomePageSaveVar = 0
    SBK_Candidate_Page = 0



## SBK_OpenPage


## SBK_Candidate_Page

¿ start/current page for the big spellbook ?

Settings_Screen() sets CMB_SpellBookPage, SBK_SomePageSaveVar, SBK_Candidate_Page to 0
G_WLD_StaticAssetRfrsh() 
SBK_BuildSpellbook__WIP() sets SBK_Candidate_Page = m_spellbook_page_count, if page size is 4, where page size means it's the Apprentice spellbook
Apprentice_Screen__WIP() sets SBK_Candidate_Page = SBK_OpenPage when it leaves
Spell_Research_Select() sets SBK_OpenPage = SBK_Candidate_Page
¿ Learn_Spell_Animation() uses SBK_OpenPage and SBK_Candidate_Page to BigBook_PageTurn__WIP() to the page that has the spell that is being learned ?

XREF:
    G_WLD_StaticAssetRfrsh+67       mov     [SBK_Candidate_Page], 0               
    SBK_BuildSpellbook__WIP+226     mov     [SBK_Candidate_Page], ax              
    BigBook_PageTurn__WIP:loc_9CA9C mov     ax, [SBK_Candidate_Page]              
    BigBook_PageTurn__WIP+1A0       mov     ax, [SBK_Candidate_Page]              
    Learn_Spell_Animation+5A8       cmp     ax, [SBK_Candidate_Page]              
    Learn_Spell_Animation+60E       cmp     ax, [SBK_Candidate_Page]              
    Learn_Spell_Animation+614       push    [SBK_Candidate_Page]            ; page
    Learn_Spell_Animation+627       mov     ax, [SBK_Candidate_Page]              
    Apprentice_Screen__WIP+1C       mov     ax, [SBK_Candidate_Page]              
    Apprentice_Screen__WIP+3B2      mov     [SBK_Candidate_Page], ax              
    BigBook_Compose__WIP:loc_9E35F  cmp     _DI_page, [SBK_Candidate_Page]        
    BigBook_Compose__WIP+557        cmp     _DI_page, [SBK_Candidate_Page]        
    BigBook_Compose__WIP+615        cmp     _DI_page, [SBK_Candidate_Page]        
    Spell_Research_Select+B8        mov     ax, [SBK_Candidate_Page]              
    Settings_Screen+344             mov     [SBK_Candidate_Page], 0               

zeroed out by G_WLD_StaticAssetRfrsh() and Settings_Screen()  

set in Apprentice_Screen__WIP() and Spellbook_Build__WIP()  
    Spellbook_Build__WIP()  
        SBK_Candidate_Page = m_spellbook_page_count  
    Apprentice_Screen__WIP()  
        SBK_Candidate_Page = SBK_OpenPage  

used in Apprentice_Screen__WIP()  
    Spellbook_Build__WIP()  
    SBK_OpenPage = SBK_Candidate_Page  
    ...so, 'Apprentice Screen' ***ALWAYS*** starts on the "Research Spells" page?  

used in BigBook_Compose__WIP()  
    if(page < SBK_Candidate_Page) { ... } else { ... }  
    ...does casting cost or research cost...  
    ...so, SBK_Candidate_Page is assumed to be the last page/page-max?  

Why does it set `SBK_Candidate_Page = SBK_OpenPage` when you leave the 'Apprentice Screen'?
    What uses SBK_Candidate_Page after that?
        Spells button - spellbook
        Apprentice - spellbook
        Tactical Combat - spellbook
        Choose Research
        Learn Spell
