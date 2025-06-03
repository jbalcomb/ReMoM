
MoM vs. MoX

Spell Screens
Spell Books
Spell Casting
Spell Animations
Spell Effects

Big Spellbook
Small Spellbook
Combat Spellbook

Main Spells  (small)
Apprentice  (big)
Next-Turn Research New  (big)
Combat  (combat/small)

Spells Per Page
6 or 8

SEEALSO:  MoM-Spellbooks.md
    spellbook mechanics - big, small, combat, shared


¿ Meaningfulness of Code Segments ?
ovr117
    Build Spell List
    Buld Spellbook
    Sml/Cmb Compose
    Sml/Cmb Draw
ovr134
    Overland Spellbook Screen
    Spell Animation Screen
    ...
ovr135
    Cast_Spell_Overland()
    ...


### Choose, Change, Abort
SBookScr.C
Spellbook_Screen()
Small Spellbook (Overland)
Abort_Spell__YN

¿ Abort_Spell__YN vs. did_select_spell ?
if(Abort_Spell__YN == ST_TRUE)
    did_select_spell = ST_TRUE;
if(did_select_spell == ST_TRUE)
    WIZ_SetOverlandSpell__WIP(HUMAN_PLAYER_IDX, spell_idx, spellbook_page_spell_index);
else
    OVL_MosaicFlip__STUB();



e.g. "Casting Overland"
    Main Screen
    Spells Button
    Small Spellbook
MainScr.C
Left-Click Spells Button
if(input_field_idx == _spells_button)
current_screen = scr_Spellbook_Screen
MOM_SCR.C
Screen_Control()
switch(current_screen)
case scr_Spellbook_Screen:
SBookScr.C
Spellbook_Screen()
Left-Click Spellbook Page Spell Fields
...
WIZ_SetOverlandSpell__WIP()
Cast_Spell_Overland__WIP()
switch(spell_data_table[spell_idx].type)
