Combat-Spell_Cast_AI_Select.md

SEEALSO:  Combat-Combat_Spellbook_Screen.md
SEEALSO:  Combat-Spell_Cast.md

NOTE: all six of ovr139

AI_SelectCmbtSpell()
AI_EvaluateCmbtSpell()
Effective_Battle_Unit_Strength()
AI_SetCombatRealms()
AI_UnitThreatRealms()
AI_CombatSpellList()

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
    'Combat Spell Cast - AI - Target'                               Combat-Spell_Cast_AI_Target.md
    'Cast Spell On Unit'                                            Combat-Combat_Cast_Apply_Spell_Effect.md
    'Combat Spell Dispell'                                          Combat-Combat_Spell_Dispel.md

---
