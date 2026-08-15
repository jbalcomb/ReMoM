Combat-Spell_Cast.md

SEEALSO:  Combat-Combat_Spellbook_Screen.md
SEEALSO:  Combat-Combat_Spell_Target_Screen.md
SEEALSO:  Combat-Spell_Cast_AI_Select.md
SEEALSO:  Combat-Spell_Cast_AI_Target.md
SEEALSO:  Combat-Cast_Spell_On_Battle_Unit.md
SEEALSO:  Combat-Combat_Spell_Dispel.md
SEEALSO:  Combat-Summon_Demon.md
SEEALSO:  Combat-Combat_Cast_Spell.md

Combat_Cast_Spell_With_Caster()
Combat_Cast_Spell()
Combat_Cast_Spell_Error()
Do_Legal_Spell_Check__WIP()
Combat_Casting_Cost_Multiplier()

### Indistinctly connected to 'Combat Spell Cast'
Combat_Cast_Spell_Message() (o113p14)
...nine callers are all spell animation functions in Spells131.c and Spells133.c — Animate_Lightning_Bolt, Wall_Rise, Battle_Unit_Summon_Animation, etc.
...probably not worth a 'Combat Spell Animation' session

Combat_Cast_Spell_With_Caster()
    |-> Combat_Cast_Spell()

Combat_Cast_Spell()
    |-> Summon_Demon()
    |-> Combat_Spellbook_Build()
    |-> Combat_Compose_Spellbook_Background()
    |-> Combat_Spellbook_Screen()
    |-> Do_Legal_Spell_Check__WIP()
    |-> Combat_Compose_Background()
    |-> Combat_Screen_Draw()
    |-> AI_SetCombatRealms()
    |-> AI_SelectCmbtSpell()
    |-> Combat_Spellbook_Mana_Adder_Screen()
    |-> Combat_Spell_Dispel_Attempt()
    |-> Combat_Spell_Counter_Message()
    |-> Combat_Casting_Cost_Multiplier()
    |-> Update_Combat_Enchantments_Icon_And_Help()
    |-> Combat_Spell_Target_Screen()
    |-> AITP_Combat_Spell()
    |-> Cast_Spell_On_Battle_Unit()

NOT:
    'Combat Screen'
        Combat_Compose_Background()
        Combat_Screen_Draw()
        Update_Combat_Enchantments_Icon_And_Help()
    'Combat Spellbook'
        Combat_Spellbook_Build()
    'Combat Spellbook Screen'
        Combat_Spellbook_Screen()
        Combat_Compose_Spellbook_Background()
        Combat_Spellbook_Mana_Adder_Screen()
    'Combat Spell Target Screen'                                    Combat-Combat_Spell_Target_Screen.md
        Combat_Spell_Target_Screen()
    'Combat Spell Cast - AI - Select'                               Combat-Spell_Cast_AI_Select.md
        AI_SetCombatRealms()
        AI_SelectCmbtSpell()
    'Combat Spell Cast - AI - Target'                               Combat-Spell_Cast_AI_Target.md
        AITP_Combat_Spell()
    'Cast Spell On Unit'                                            Combat-Cast_Spell_On_Battle_Unit.md
        Cast_Spell_On_Battle_Unit()
    'Combat Spell Dispell'                                          Combat-Combat_Spell_Dispel.md
        Combat_Spell_Dispel_Attempt()
        Combat_Spell_Counter_Message()

---
