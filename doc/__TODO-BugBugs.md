


11 book Invulnerable Guarian Spirit strategy
//link12342



Prayer/Luck attacker to-hit -1 bug
NOTE(JimBalcomb,20260218): the bug is exactly the effect of Invisibility

Prayer
    Manual (Spellbook) Page 27
        Calls upon the forces of good to bless all friendly units with an
        increase of one (+10%) in their abilities to hit (melee, thrown, and
        ranged attacks), their defense (each shield gets a +10% ability to
        block a hit) and their resistance.
    OSG Page 81 +1 cross, +1 To Hit, +1 To Defend

Luck
    Manual  Page 83
    Adds one to all of unit’s rolls: attack, defense and resistance
    (i.e., a plus 10% modifier to hit with each sword, a plus 10% chance to block with each shield, and plus 10% modifier for all resistance checks).

OSG  Page 286  (PDF Page 291)
    **** These spells decrease the To Hit ability of enemy units in direct melee combat with the enchanted unit.
         Note that in addition to increasing the target units' abilities To Hit, High Prayer, Prayer, and Luck are like
         Invisibility in that they also decrease enemy units' To Hit abilities by one.

ReMoM
...has to be at or after BU_ProcessAttack__WIP()

    ...grep for USA_LUCKY, BU_LUCKY, HSA_LUCKY, HERO_LUCKY  VSC  Ctrl-Shft-F  USA_LUCKY|BU_LUCKY|HSA_LUCKY|HERO_LUCKY
    Combat.c
        BU_Init_Battle_Unit()
            if(BU_LUCKY())                                  #define BU_LUCKY()      ( ( battle_unit->Attribs_1 & USA_LUCKY     ) != 0 )
                battle_unit->tohit += 1;
                battle_unit->toblock += 1;
                battle_unit->resist += 1;
                battle_unit->Gold_Resist += 1;
        BU_Init_Hero_Unit()
            if(HERO_LUCKY(hero_owner_idx,hero_type))        #define HERO_LUCKY(_player_idx_,_hero_type_)          ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_LUCKY         ) != 0)
                battle_unit->tohit += 1;
                battle_unit->toblock += 1;
                battle_unit->resist += 1;
                battle_unit->Gold_Resist += 1;
        FWIW, the bug is not in the code that handles Invisibility...
        BU_ProcessAttack__WIP()
            Line 21098
                if(
                    (
                        ((defender_enchantments & UE_INVISIBILITY) != 0)
                        ||
                        ((battle_units[defender_battle_unit_idx].Abilities & UA_INVISIBILITY) != 0)
                    )
                    &&
                    ((battle_units[attacker_battle_unit_idx].Attribs_1 & USA_IMMUNITY_ILLUSION) == 0)
                )
                {
                    attack_tohit -= 1;
                }
    UnitView.c
        HSA_LUCKY ... cnst_Lucky ... "Lucky"

    Prayer Spell
        Combat.c
            Line 11746
            Cast_Spell_On_Battle_Unit()
            Line 12010
            case scc_Battlefield_Spell:     // 10
            // Combat Battlefield Enchantment  (NOT Combat Battlefield Instant)
            combat_enchantment_index = spell_data_table[spell_idx].ce_idx;
            combat_enchantments[combat_enchantment_index] = ST_TRUE;
            REINIT_BATTLEUNIT();


https://masterofmagic.fandom.com/wiki/Lucky
Lucky
Lucky is the name of two identical abilities in Master of Magic. One is a Unit Ability, while the other is a Hero Ability which might randomly appear, but they have the same exact effect.
A unit or Hero possessing the Lucky ability receives a bonus of exactly  +10% To Hit,  +10% To Block, and  +1 Resistance. Any Melee Attack on the Lucky unit is also carried out at  -10% (a separate and distinct effect from the unit's own bonus To Block). Lucky therefore enhances the unit's chance to succeed in various die rolls it might be required to perform, slightly increasing both damage output and survivability.
Only 6 Normal Units possess the Lucky ability by default, all belonging to the Halfling Race. Any Hero that has at least one Random Ability pick may also occasionally acquire Lucky (25 Heroes in all).
The Lucky ability does not improve with Experience, but its percentage-based bonuses mean that improving the unit's attack and defense ratings (whether through Experience or by other means) also improves the total benefit from this ability.
There is no known way of adding Lucky to a unit that did not start the game with this ability.
Description
Authorities on cosmic laws suggest that the race is neither to the swift, nor the battle to the strong, but that sometimes all it takes is just pure luck. In Master of Magic, the Lucky ability could be a mark of divine favor or a naturally-developed gift for making the most of circumstances.
Halflings are inherently lucky creatures. In a straight reenactment of fantasy tradition, they can in fact be led by a wizard to steer the course of a war. From time to time, lucky heroes may also petition to join a wizard's cause who, like the Halflings, also seem to have a strange accord with the wheels of destiny, one that drives their abilities beyond their normal limits.
Effect
The Unit Ability and Hero Ability called "Lucky" work in exactly the same way. These abilities tip various random rolls in favor of their owner during combat.

Per the Fandom Wiki, this is where the BUGBUG lies:
    "Any Melee Attack on the Lucky unit is also carried out at  -10% (a separate and distinct effect from the unit's own bonus To Block)."

...
...
...


3 Comments
A Fandom user
5/17/2013
I'd be happy, if you add the opponents to Hit malus to the Wiki. It's truth is backed up by 3 sides:
1. My research in combat situation #1.
2. Patch 1.40n mentions to have corrected this bug: Lucky and Prayer bonus was incorrectly making all incoming attacks less efficient (-1 to hit), on top of its correct effect.
3. The strategy guide points it out hidden at the bottom of page 286: Note, that in addition to increasing the target unit's abilities To Hit, High Prayer, Prayer, and Luck are like Invisibility in that they also decrease enemy units' To Hit alilities by one.

Spearman D92-R
·
5/17/2013
I think you are most qualified to begin making the edits having done this research.  This is not to be dismissive but to emphasize another point, that there are scads of text that may require some proofreading in light your progress on the topic, namely:


Lucky

Invisibility

Invisibility (Spell)

Mass Invisibility

Illusion

Blur

Warp Reality

Prayer

High Prayer

Defense

Halflings (?)

Special Damage and child categories...

Phantom Warriors

Phantom Beast

Aerie the Illusionist

Eldritch Weapon

Melee Damage

Can you think of some more?

(Edited by Spearman D92-R)
A Fandom user
·
5/17/2013
I can't. My English is bad. I am bad in formulating things. And I didn't want to contribute to this Wiki. I just change some wrong number etc, when I see them.

Most linked pages seems to be correct* and are unrelated to the -10% Hit penalty by Lucky, Prayer and High Prayer (additional to +10% Block). On the other hand, every page, where some Halfling unit is listed should be changed. A further sentence similar to this one from Invisibility should be enough: Finally, whenever an Invisible unit is attacked, the attacker suffers a [1] -10% To Hit penalty, reducing overall damage output.


Here some words to Invisibility and Blur page

1. Invisibility: You should mention that Invisibility though enchanted Amulets is bugged. You get the Invisibilty bonus (-10% to Hit, ranged units can't attack), but the computer AI will still know where you are, so moving towards you and throwing spells on you.

2. Blur: Sometimes its effect is listed correct, sometimes not. It just blocks 10 % incoming damage, but doesn't give a to Hit penalty. Note, a to Hit penalty is stronger. Whenever you speak from 10% to Hit, you mean 10 percentage points, which is at least as much as 10 percent.

User avatar
MysticX2
·
5/17/2013
I just wanted to say thanks for all your help!

A Fandom user
·
5/5/2013
OK now I did further research with high shielded heroes.

combat situation 4: Great Drake (Chaos Node) vs Morgana the Witch (Lucky)
Dragon has 32 swords, +3 to Hit. Morgana has 20 shields and 12 hitpoints. If Morgana died, I assumed 13 damage were done to her. Here again the damage you would expect when certain modifiers are active:

-1 to Hit: 32*0.5 - 20*0.3 = 16 - 6 = 10 damage in average
-1 to Hit AND +1 to Block: 32*0.5 - 20*0.4 = 16 - 8 = 8 damage in average

I did 100 tests. Morgana died 19 times and 2 times no damage was dealt. In those tries I experienced an average of 7.82 damage. Looks like Block modifiers would exist after all.

combat situation 5: Great Drake (Chaos Node) vs Taki the War Monk
Dragon has 32 swords, +3 to Hit. Taki has 32 shields and 12 hitpoints. If Taki died, I assumed 13 damage were dealt. Here again the damage you would expect when certain modifiers are active:

-1 to Hit: 32*0.5 - 32*0.3 = 32*0.2 = 6.4 damage in average
-1 to Hit AND +1 to Block: 32*0.5 - 32*0.4 = 32 * 0.1 = 3.2 damage in average

I did 100 tries with Taki on Invisibility. He received 6.7 damage in average, whereas 8 times he died and 2 times he stayed unhurt. Looks like Invisibility reduces enemy's to Hit by 1, but don't add to Block.

I did 100 tries with Prayer cast. If Taki stayed unhurt, I used -1 damage in the calculation. He received 3.58 damage in average, wheras 1 time he died and 24 times he stayed unhurt. Looks like Prayer add +1 ro Block additionaly by weakening enemy's to Hit by 1.

Summary: Now I am pretty sure, Prayer and Lucky work as follows: Additionally to the +1 resistance and +1 to Hit Bonus to your unit, your unit also gets a defensive bonus by

1. reducing enemy's to Hit by 1 (i.e. enemy's swords have a 10*(2+toHitBonus) % chance to work)
2. increasing your to Block by 1 (i.e. shields have a 40 % chance to work)

With other words the defensive effect of Lucky and Prayer is huge. It also seems that spells are effected by the To Hit Malus. For example Psionic Blast cast on a Halfling does less damage than against any other unit. Psionic Blast should ignore armor/shields.

View more replies
User avatar
Spearman D92-R
·
5/5/2013
Yeah at the moment Psionic Blast appears to behave like a generic point blank ranged attack (in the mold of fire bolt and ice bolt).  It should be an illusionary attack, but it isn't.

A Fandom user
·
5/6/2013
Yes, to-Hit effects are cumulative, but each attack/sword has at least a 10 % chance to hit (and 90 % at most AFAIK), i.e. the final to-Hit modifier lies between -2 and +7. I did a test with Blur, too.

combat situation 6: Phantom Warriors (Sorcery Node) vs. Brax the Dwarf

Prayer and Blur are active. I did 200 tries. Phantom Warriors did 5.53 damage in average.

expected -1 to-Hit damage: 6*5*0.2 = 6 damage

expected -2 to-Hit damage: 6*5*0.1 = 3 damage

As seen in combat situation 1, Prayer gives you 1 to-Hit (also 1 to-Block, but that doesn't matter agianst Phantom Warriors). So Blur doesn't give you another one. I assume, Blur works the way like described in the manual: 10% of incoming garbage is canceled before shields are applied. In the example above, you whould expect 5.4 damage then. That's close to the experienced 5.53 damage. Note, that -1 to-Hit has always a stronger effect. For example, if attacker has no to-Hit Bonus, then -1 to-Hit decreases chance to Hit from 30 % to 20 %. That's like 33.3% of incoming garbage is canceled. So Prayer is way stronger spell than Blur.

Mind Stormy
·
5/6/2013
Are you sure about the "90% at most?"  (This seems easy to test with a superhero.)

A Fandom user
·
5/7/2013
The statement "90% at most" was wrong. I leveled Shalla the Amazon to demi-god, thus +7 to-Hit via her Blademaster ability. She had 22 Thrown Weapon and I attacked Phantom Beasts in Sorcery Nodes. The Phantom Beasts never did any damage, so the Thrown weapon ability must always have killed them.

A Fandom user
·
4/30/2013
Can somebody tell me, where you have the following information from?  Is it guessed or have you access to the MoM code? Each roll results in a number between 1 and 100. For each roll that comes up equal to or lower than the unit's [1] To Block score, the unit averts [2] 1 point of incoming Damage". 

I am not sure, if the To Block bonus really exists or if it is in reality a To Hit Malus for your opponent. Let's take the following scenario: Phantom Warriors vs Halfmen: Theoretically each Phantom Warrior figure should inflict 0.9 damage, as the Halfmen's shield will be ignored. But in reality it's less.

View more replies
User avatar
MysticX2
·
5/1/2013
I think you may be referring to the explanations on the Physical Damage page. 

Are you saying it is like that(?): Phantom Warrior figure #1 tries to kill some Halflings. Simoultanously, Halfling figure #1 tries to kill some Phantom Warriors. If some Phantom Warrior/Halfling figures survived, Halfling & Phantom Warrior figure #2 try to deal damage. And so on. 

In a way I think that is what I was saying, although it is probably better explained on the Physical Damage page and/or the Melee Damage page.  I think Spearman D92-R can probably explain things better, so I will leave this to the two of you. :D

A Fandom user
·
5/1/2013
The strategy guide also condradicts itself a bit with the following statement, which can be found at the bottom of page 286: Note, that in addition to increasing the target unit's abilities To Hit, High Prayer, Prayer, and Luck are like Invisibility in that they also decrease enemy units' To Hit alilities by one.

I don't assume the strategy guide wants to imply that Lucky and Prayer both increase own's To Block by 1 and decrease enemy's to Hit at the same time. But I cannot exclude this. One time I was fighting with some Halfmen elite swordsman against an (elite?) Airship. Both wizards used the spell Prayer and the mentioned units engaged roughly 100 times to melee attack before the Airship was finally defeated. If you assume, that Prayer increases your To Hit by 1 and decreases enemy to Hit by 1, then the 2 Prayers should have neglected each other, which was definitely not the case.

User avatar
MysticX2
·
5/4/2013
There have been a few times when Headrock noted discrepancies in the strategy guide, so finding another wouldn't surprise me.

As for 2 Prayer spells being active, I don't think they cancel each other, but would simply be like having opposing Iron Skin spells...they both gain the benefits of the spell. 

You may be right about the Lucky trait, but I don't know how that was tested.  I know that I get some surprising results when battling creatures with an Illusion attack, but I haven't recorded any information regarding that.  Did you check out either of the pages I linked above?

A Fandom user
·
5/5/2013
With canceling each other I meant the to Hit modifier, not the resistance. Assuming Prayer gives you +1 to Hit and gives a -1 to Hit penalty to your opponent, then 2 Prayers active would be like no Prayer active at all (except resistance).

I did some research myself:

Combat situation 1: Phantom Beast vs Brax/Morgana
Phantom Beast has 20 swords and +1 to Hit and ignores the hero's armor. With the following modifiers you would expect that average damage:

No modif.: 20*0.4 = 8
-1 to Hit: 20*0.3 = 6
-2 to Hit: 20*0.2 = 4
-3 to Hit: 20*0.1 = 2

I did 200 tests each with the following spell modifications. Here the average damage per strike:

Prayer only: 5.995
Prayer + Lucky: 3.910
Prayer + Lucky + Invisibility: 1.885

The numbers encourage the theory, that Lucky and Prayer are reducing enemy's to Hit by 1 each. As armor is ignored, no statement can be made over to Block modifier.

combat situation 2: Great Drake (Chaos Node) vs. Brax the Dwarf
Great Drake has 32 swords and +3 to hit. Brax has 6 shields and 19 Hitpoints. Brax attacks Great Drake, so the dragon's Fire Breath doesn't matter. With the following modifiers you would expect that average damage:

No modif.: 32*0.6 - 6*0.3 = 19.2 - 1.8 = 17.4
+1 to Bl.: 32*0.6 - 6*0.4 = 19.2 - 2.4 = 16.8
-1 to Hit: 32*0.5 - 6*0.3 = 16.0 - 1.8 = 14.2
-2 to Hit: 32*0.4 - 6*0.3 = 12.8 - 1.8 = 11.0

I did 100 tests with Brax enchanted with Prayer and/or Invisibilty, and wrote down the damage dealt to Brax. If Brax died, I assumed 20 damage were done to him. Here are the results:

Prayer only: 14.24 damage in average (6 deaths)
Invis. only: 14.01 damage in average (10 deaths)
Prayer + Invis.: 10.45 damage in average (0 deaths)

The damage with Prayer only is very close to the -1 to Hit to enemy effect. The difference of Invisibility only is close to the number for -1 to Hit (just 0.2 difference). The difference between expected -2 to Hit damage and Prayer + Invisisibility is bigger than 0.5. In my opinion this gap can't be explained from not enough tests made (I did 100 tests).

The gap is even bigger when you combine Lucky, Prayer, Invisibility all together:

combat situation 3: Great Drake (Chaos Node) vs. Morgana (Lucky)
Lucky Morgana has 8 shields and 11 Hitpoints. I let the Great Drake attack Morgana or Morgana would have needed to waste all her mana beforehand. Nevertheless, the dragon's Fire Breath shouldn't matter, because the 50 shields via Magic Immunity should absorb the 32 strength Fire Breath damage with at least 99 % probability (Lucky, Prayer and Invisibility cancel the dragon's +3 to Hit). If Morgana died, I assumed 12 damage were done to her.

And here it comes: 5.6 damage in average (3 deaths, 5 times no damage at all) With -3 to Hit, you would expect 7.2 damage ( 32*0.3 - 8*0.3 = 9.6 - 2.4 = 7.2 ).

Summary: I am pretty sure Lucky and Prayer decrease your enemy's to Hit by 1. The combined effects of those spells against the Great Dragon were not clear. The effect was stronger than expected. But I don't assume they also increase your to Block, because I have strong doubts that to Block was implemented in the game. To exclude this, you would have to fight with a well armored hero against the Great Drake. Perhaps I will do this in future.



https://masterofmagic.fandom.com/wiki/Prayer

Prayer is an Uncommon Life magic Combat Enchantment. For  30 it may be cast during combat to provide a lasting effect, whereby all friendly creatures receive a +10% bonus to the success chance of any Attack Roll, Defense Roll, or Resistance check they make. As a result, they become more deadly, and harder to damage, hinder, or incapacitate.

Effects
Prayer raises the morale of all friendly units on the battlefield, encouraging them to perform better in almost every respect. As a result, units will strike more truly with their attacks, dodge incoming damage more deftly, and become more resilient in the face of curses and other maledictions.

To Hit Bonus
While Prayer is in effect, each and every friendly unit on the battlefield receives a  +10% To Hit bonus. This means that every die of their Attack Rolls is more likely to cause a point of "raw"  damage, thus increasing their overall average Conventional Damage output. The bonus applies equally to all attacks, with the exception of those that deliver  spell damage. That is, it will affect  Melee- and Counter Attacks;  /  /  Ranged Attacks;  Thrown- and  Breath Attacks; and even the hidden Conventional component of Gaze Attacks. On the other hand, it does not affect Ability Immolation Immolation, or any spells, spell-like abilities, or Spell Charges used by the unit.

To Block Bonus
While Prayer is in effect, all friendly units on the battlefield get a +10% chance of success for each die of every Defense Roll they make to reduce incoming Conventional Damage. The spell does not increase the unit's Defense score itself - instead, it raises the chance that each  Shield will manage to block a point of "raw"  damage. Therefore, while the unit can still potentially block the same amount of  damage, it is likely to block more on average. Naturally, this does not apply against attacks which do not allow a Defense Roll in the first place, but the benefit is otherwise proportional to the unit's  Defense attribute.

Resistance Bonus
While Prayer is in effect, each and every friendly unit on the battlefield receives a Resistance bonus of  +1. In other words, their chance to shrug off malicious effects is increased by +10%. As a result, they will be slightly harder to affect with Unit Curses, and will stand up better against Special Damage attacks like poison or petrification.

Enemy Melee To Hit Malus
While Prayer is in effect, each and every enemy unit on the battlefield receives a  -10% To Hit malus to their Melee Attacks. That is, every time they perform such an attack against a unit affected by Prayer, including during Counter Attacks, the success chance of each die of their Attack Roll is reduced by  10%. While this does not affect their potential  damage, it does decrease their average "raw" Melee Damage output by an amount proportionate to their Melee Strength. Other Attack Types are not affected.

It should be noted that this effect is removed from the game starting with the unofficial Insecticide patch. Examination of the game code has led multiple patch-makers to the conclusion that this is actually not an intentional benefit. It is most likely left over from an earlier stage of the game development, where Melee Attacks were handled differently from other Conventional Damage types (hence why the effect only triggers for Melee Damage).
