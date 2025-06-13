


Resistance Strength = crosses

each indicates a +10 percent in this ability
Resist: is the unit's resistance or crosses.

Resistance is shown by crosses and indicates
the chance that the unit ( or figure, when
applicable) bas to defend itself against spelJs
and certain special attack types, such as gaze
attacks (see Chapter 20).

Page 292  (PDF Page 297)

Table 20.7 Resisting Special Attacks and Spells

The chance that a unit or a figure, depending on the spell or attack, successfully resists the spell or attack depends on three things:  

    1. The unit's resistance (i.e., the number of crosses it has)  
    2. The sum of any acting Resistance Modifiers from unit skills, spells, or artifacts  
    3. Any modifier to the saving throw  

The saving throw modifiers basically alter the number of crosses a unit or figure has to resist a spell or attack.  
If the spell or attack acts at a -2 save, for example, then the defender must resist it with two fewer crosses than it can normally use to resist spells and attacks.  
Each cross the unit has is worth a 10 percent chance to resist a spell or attack.  
Thus, a unit with six crosses would have a base 60 percent chance when resisting.  
The total chance that a unit has to resist something is given by:  
Chance to Resist = 10% x (Number of Crosses + Sum of Resistance Modifiers + Saving Throw Modifier)  



saving roll
saving throw
penalty
bonus



## Combat_Resistance_Check()
// WZD o122p03

What's done will the value it returns?
    ...how many times it failed to resist?
    ...how much it failed to resist?
        ...as a percentage?

XREF:
    BU_ProcessAttack__WIP()
    BU_CauseFear__NOOP()
    j_Combat_Resistance_Check()
        CMB_PrepareTurn__WIP()
        Cast_Spell_On_Battle_Unit()
        IDK_Cast_BlackWind()
        Cast_DeathWish()
        IDK_Spell_GreatUnsummoning()
        WIZ_HolyWord()
        WIZ_DeathSpell()
        WIZ_Wrack()
        BU_LifeDrain__WIP()
        WIZ_CallChaos()
        Do_All_Units_XP_Check()



## Combat_Effective_Resistance
// WZD o122p04
echantments
spell magic realm
Hero Charmed  +30 "...never fail resistance rolls..."
UE_ELEMENTALARMOR
UE_RESISTELEMENTS
UE_BLESS
UE_RESISTMAGIC

XREF:
    Combat_Resistance_Check()
    j_Combat_Effective_Resistance()
        AITP_CombatSpell__STUB()
        AITP_WarpCreature()
        UU15_AITP_Disintegrate()
        Cast_Spell_On_Battle_Unit()
        WIZ_CallChaos()
        AI_EvaluateCmbtSpell()
