




## Variables

g_spellbook_left_page

CMB_SpellBookPage
SBK_SomePageSaveVar
g_first_research_page

G_WLD_StaticAssetRfrsh()
    SBK_SomePageSaveVar = 0
    CMB_SpellBookPage = 0
    g_first_research_page = 0
Settings_Screen()
    CMB_SpellBookPage = 0
    SBK_SomePageSaveVar = 0
    g_first_research_page = 0



## g_spellbook_left_page


## g_first_research_page

¿ start/current page for the big spellbook ?

Settings_Screen() sets CMB_SpellBookPage, SBK_SomePageSaveVar, g_first_research_page to 0
G_WLD_StaticAssetRfrsh() 
Build_Spellbook() sets g_first_research_page = m_spellbook_page_count, if page size is 4, where page size means it's the Apprentice spellbook
Apprentice_Screen__WIP() sets g_first_research_page = g_spellbook_left_page when it leaves
Spell_Research_Select() sets g_spellbook_left_page = g_first_research_page
¿ Learn_Spell_Animation() uses g_spellbook_left_page and g_first_research_page to BigBook_PageTurn() to the page that has the spell that is being learned ?

XREF:
    G_WLD_StaticAssetRfrsh+67       mov     [g_first_research_page], 0               
    Build_Spellbook+226     mov     [g_first_research_page], ax              
    BigBook_PageTurn:loc_9CA9C mov     ax, [g_first_research_page]              
    BigBook_PageTurn+1A0       mov     ax, [g_first_research_page]              
    Learn_Spell_Animation+5A8       cmp     ax, [g_first_research_page]              
    Learn_Spell_Animation+60E       cmp     ax, [g_first_research_page]              
    Learn_Spell_Animation+614       push    [g_first_research_page]            ; page
    Learn_Spell_Animation+627       mov     ax, [g_first_research_page]              
    Apprentice_Screen__WIP+1C       mov     ax, [g_first_research_page]              
    Apprentice_Screen__WIP+3B2      mov     [g_first_research_page], ax              
    BigBook_Compose__WIP:loc_9E35F  cmp     _DI_page, [g_first_research_page]        
    BigBook_Compose__WIP+557        cmp     _DI_page, [g_first_research_page]        
    BigBook_Compose__WIP+615        cmp     _DI_page, [g_first_research_page]        
    Spell_Research_Select+B8        mov     ax, [g_first_research_page]              
    Settings_Screen+344             mov     [g_first_research_page], 0               

zeroed out by G_WLD_StaticAssetRfrsh() and Settings_Screen()  

set in Apprentice_Screen__WIP() and Spellbook_Build__WIP()  
    Spellbook_Build__WIP()  
        g_first_research_page = m_spellbook_page_count  
    Apprentice_Screen__WIP()  
        g_first_research_page = g_spellbook_left_page  

used in Apprentice_Screen__WIP()  
    Spellbook_Build__WIP()  
    g_spellbook_left_page = g_first_research_page  
    ...so, 'Apprentice Screen' ***ALWAYS*** starts on the "Research Spells" page?  

used in BigBook_Compose__WIP()  
    if(page < g_first_research_page) { ... } else { ... }  
    ...does casting cost or research cost...  
    ...so, g_first_research_page is assumed to be the last page/page-max?  

Why does it set `g_first_research_page = g_spellbook_left_page` when you leave the 'Apprentice Screen'?
    What uses g_first_research_page after that?
        Spells button - spellbook
        Apprentice - spellbook
        Tactical Combat - spellbook
        Choose Research
        Learn Spell
