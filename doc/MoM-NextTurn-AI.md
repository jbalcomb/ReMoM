


AI_CullTheWeak()    ==>  AI_Kill_Lame_Units()
AI_Overland_Turn()  ==>  AI_Next_Turn__WIP()





Main_Screen()
...current_screen = scr_NextTurn;
Screen_Control()
    |-> Next_Turn_Proc()
        |-> Next_Turn_Calc()
            |-> AI_Kill_Lame_Units()
                |-> AI_Calculate_Average_Unit_Cost()
            |-> AI_Next_Turn__WIP()
                |-> Allocate_AI_Data()

MoO2
Mean_Ship_Strength_()
Make_Scrap_Ships_Dead_()
Kill_Ships_()
Compute_Empire_Building_Needs_()




¿ AI *decisions* ?

New City?
New Building?
New Unit?
New Spell?

Attack Eenemy?

Personality
Objective





## AI_Calculate_Average_Unit_Cost()

does not include Settler; Magic Spirit, Guardian Spirit; Engineer, Trireme, Galley, Catapult, Warship;

Does the maths not look odd?
Yes'm. Cause 4-byte value (long) and Long Division (LDIV).

XREF:
    NX_j_AI_Calculate_Average_Unit_Cost()
    AI_Kill_Lame_Units()



## AI_Kill_Lame_Units()

...as of turn 100...

if((_turn % 25) != 0)
    ...non-human, non-neutral...flag all units in cities...
else
    ...recalculate average unit cost...
    ...non-human, non-neutral...flag all units in cities...
    if(_unit_type_table[_UNITS[troops[itr_troops]].type].cost < (_players[_CITIES[itr_cities].owner_idx].average_unit_cost / 2))
        Kill_Unit(troops[itr_troops], 0);
...every 25 turns...
...maybe, removes 1 unit from each city...
...oldest unit that has a cost less than half of the player's average military unit cost...

¿ BUGBUG should have the same exclusion for non-military units ?



## AI_Next_Turn__WIP()

...sanity check all unit coordinates...make insane units be dead... (owner == -1)

EMM_Map_DataH()
Allocate_AI_Data()

// process a planning phase for each AI in turn,
// evaluating the situation, casting a spell, setting
// production and strategy, and giving orders to all
// available units



## Allocate_AI_Data()

    AI_Own_City_Values       = Near_Allocate_First(200);
    AI_Enemy_City_Values     = Near_Allocate_Next(200);
    AI_Own_Garr_Strs         = Near_Allocate_Next(200);
    AI_NME_Garrison_Strs     = Near_Allocate_Next(200);
    AI_Own_Stacks            = Near_Allocate_Next(800);
    AI_Enemy_Stacks          = Near_Allocate_Next(1440);
    AI_OVL_Spell_Cats        = Near_Allocate_Next(92);
    CRP_AI_OVL_SpellList     = Near_Allocate_Next(50);
    AI_Arc_Cont_Own_Str      = Near_Allocate_Next(120);
    AI_Myr_Cont_Own_Str      = Near_Allocate_Next(120);
    CRP_AI_Arc_Cont_Nme_Str  = Near_Allocate_Next(120);
    CRP_AI_Myr_Cont_Nme_Str  = Near_Allocate_Next(120);
    AI_Arc_Cont_Own_Val      = Near_Allocate_Next(120);
    AI_Myr_Cont_Own_Val      = Near_Allocate_Next(120);
    AI_Arc_Cont_Nme_Val      = Near_Allocate_Next(120);
    AI_Myr_Cont_Nme_Val      = Near_Allocate_Next(120);
    AI_Arc_ContBalances      = CRP_AI_Arc_Cont_Nme_Str;
    AI_Myr_ContBalances      = CRP_AI_Myr_Cont_Nme_Str;











https://masterofmagic.fandom.com/wiki/AI_to_AI_Diplomacy#End_of_turn_process

If an AI wizard is banished, it does no AI to AI diplomacy. Otherwise, at end of every turn, the following process is executed by each AI wizard. For the purpose of this section, the AI wizard currently making the decision will be called the "actor" or "acting wizard".  

First, if there is an AI ally who is at war with another AI wizard, the actor joins in by declaring War on that other wizard immediately.  
In the 1.50 unofficial patch, this feature is removed: Instead the actor will consider helping allies when checking for the Need for War.  
The primary differences this creates for 1.50 are:

 * The actor respects the Peace Counter, abstaining from going to war shortly after a Peace Treaty or being successfully threatened.
 * Joining the war isn't necessarily immediate, since the chance of war on each check is less than 100%.
 * The actor may help the human player if they are an ally, or declare War on them if they are an ally's enemy.

Afterwards, towards each other AI wizard in the game with whom it has Diplomatic Contact, the acting wizard does the following:  

 1. On most turns, it will randomly decide to skip all subsequent steps towards that wizard that turn and take no action at all. Specifically, if and only if a random number between 1 and 15-(2*Difficulty Level) is exactly 1, it will continue considering further actions.
Note that doing nothing at all is poor design, because it means the AI will not check for the Need for War even though that procedure was designed to be used every turn. Instead, it'll only be called once every ~10 turns, significantly reducing the chances of AI players Breaking Treaty or Declaring War on each other. Due to bugs that cause them to Declare War on uninvolved players for no reason at all, this bug isn't noticeable. In the 1.50 unofficial patch, this chance skips to the check for the Need for War instead.  
 2. Calculate an Action Score (AS) determining the chances of subsequent diplomatic interaction. All variables are for the acting wizard toward the other wizard.  
    * Until patch 1.50, AS = Hidden Relation + Visible Relation + Personality Modifier + random(1-100) + random(4-9)*Difficulty Level + Treaty Interest + 10*Wizard's Pact + 20*Alliance  
    * After patch 1.50, AS = Hidden Relation + Visible Relation + Personality Modifier + random(1-50) + random(3-9)*Difficulty Level + Treaty Interest/4 + 10*Wizard's Pact + 20*Alliance instead.  
 3. In patch 1.50 only, jump to the Diplomatic Reaction check below if there is a War between the two wizards.  
 4. If Action Score>150 and there is no Alliance present, form an Alliance. In patch 1.50, instead check if Action Score>170.  
 5. If Action Score>150 and there is an Alliance present, form a Wizard's Pact, replacing the Alliance. In patch 1.50, instead check if Action Score>112, and there must be no Alliance present (so the Wizard's Pact will not replace an Alliance).  
 6. If Action Score > Trade Interest+Treaty Interest+80, and there is a pair of spells available for trade, trade those spells. Note that the higher Trade Interest is, the less likely it is for this formula to result in a trade. In patch 1.50, instead check if Action Score+Trade Interest>=150. However, in all game versions, there is a bug in which the spells selected are assigned to the wrong wizards, so each party only receives the spell it already had, accomplishing nothing.  
 7. At a 5% chance, execute a +5 strength Diplomatic Reaction.  
 8. If there is a War, and Peace Interest+(a totally unrelated variable containing -1 to 3)>100, form a Peace Treaty. This is most certainly a buggy formula, as it uses a variable used to select text lines during Player to AI Diplomacy, and almost never meets the condition. In patch 1.50, this is fixed to check Action Score+Peace Interest>0 instead. However, it will use the other wizard's Peace Interest variable, not the proposing party's, to simulate the mutual agreement needed to sign the treaty.  
 9. Reduce Treaty Interest, Peace Interest and Trade Interest by 30. In patch 1.50, Peace Interest is not reduced.  
10. Check the Need for War, which may result in Moving towards War by breaking a treaty or declaring war.  
11. Check if there are any units standing in a location violating the Wizard's Pact or Alliance, the same way as in AI to Player Diplomacy. This check is removed in the 1.50 unofficial patch because the AI is unable to keep their troops away from cities to avoid penalties.  
