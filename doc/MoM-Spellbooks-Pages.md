


1 or 2 pages
4, 6, or spells per page

Small Spellbook uses 6 spells per page, for both Overland and Combat
...only/both for the casting of a spell


## Creating Fields
Spellbook_Screen()
Small Spellbook (Overland)
x_start = 16;
y_start = 12;
for(itr_spellbook_page_fields = 0; itr_spellbook_page_fields < 6; itr_spellbook_page_fields++)
    spellbook_pages[(itr_spellbook_page_fields + 0)] = Add_Hidden_Field((x_start + 16), (y_start + (itr_spellbook_page_fields * 22) + 17), (x_start + 137), (y_start + (itr_spellbook_page_fields * 22) + 34), 0, ST_UNDEFINED);
for(itr_spellbook_page_fields = 0; itr_spellbook_page_fields < 6; itr_spellbook_page_fields++)
    spellbook_pages[(itr_spellbook_page_fields + 6)] = Add_Hidden_Field((x_start + 148), (y_start + (itr_spellbook_page_fields * 22) + 17), (x_start + 268), (y_start + (itr_spellbook_page_fields * 22) + 34), 0, ST_UNDEFINED);



## Matching Input
Spellbook_Screen()
Small Spellbook (Overland)
BEGIN:  Left-Click Spellbook Page Spell Fields
for(itr_spellbook_page_fields = 0; itr_spellbook_page_fields < 12; itr_spellbook_page_fields++)
    if(spellbook_pages[itr_spellbook_page_fields] == input_field_idx)





...set the page to the one with the currently casting spell on it, and set the casting flag...
m_spellbook_page_count
m_spellbook_pages[].count
m_spellbook_pages[].spell[] == _players[].casting_spell_idx
if((itr_spellbook_page_count % 2) != 1)
    SBK_OpenPage = itr_spellbook_page_count;
else
    SBK_OpenPage = (itr_spellbook_page_count - 1);
player_is_casting = ST_TRUE;
if(player_is_casting == ST_FALSE)
    SBK_OpenPage = SBK_SomePageSaveVar;

