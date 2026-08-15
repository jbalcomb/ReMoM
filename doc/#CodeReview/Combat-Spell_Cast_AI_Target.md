Combat-Spell_Cast_AI_Target.md

SEEALSO:  C:\STU\devel\ReMoM\doc\ComputerPlayer\Combat-AITP_Combat_Spell.md

AITP_Combat_Spell()

the eleven AITP_* combat targeters
under the already-reviewed dispatcher:
ovr111 p02-p07 and p09
(AITP_DarknessLight, AITP_Healing, AITP_WarpWood, AITP_WarpCreature, UU15_AITP_Disintegrate, AITP_DispelMagic, AITP_HolyWord)
ovr112 p08-p12
(AITP_EarthToMud, AITP_Disrupt, AITP_CracksCall, UU_AITP_WordofRecall, AITP_RecallHero)

Combat_Cast_Spell_With_Caster()
    |-> Combat_Cast_Spell()

Combat_Cast_Spell()
    |-> AI_SetCombatRealms()
    |-> AI_SelectCmbtSpell()
    |-> AITP_Combat_Spell()

NOT:
    'Combat Screen'
    'Combat Spellbook'
    'Combat Spellbook Screen'
    'Combat Spell Target Screen'                                    Combat-Combat_Spell_Target_Screen.md
    'Combat Spell Cast'  (non-AI; yay HP, nay CP, NP, NPC, Monster)
    'Combat Spell Cast - AI - Select'                               Combat-Spell_Cast_AI_Select.md
    'Cast Spell On Unit'                                            Combat-Cast_Spell_On_Battle_Unit.md
    'Combat Spell Dispell'                                          Combat-Combat_Spell_Dispel.md

---
