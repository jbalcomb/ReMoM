
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

