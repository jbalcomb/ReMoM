


Move_Units()
    |-> Combat()
        |-> Strategic_Combat()
    |-> Lair_Combat()
        |-> Lair_Combat_Do()
            |-> Combat()
                |-> Strategic_Combat()



### Strategic_Combat()

¿ blocks / branches ?

- Combat Structure  {city, sorcery, nature, chaos, ocean}
- Load Battle Units
- Battlefield Effects
- Boat Riders
- Status 6 Battle Units
- Health / Threat
- Mana Multiplier  
    if(_combat_attacker_player < _num_players) { ... } else { attacker_mana_multiplied = 0; }  
    if(_combat_defender_player < _num_players) { ... } else { defender_mana_multiplied = 0; }  
    Combat_Casting_Cost_Multiplier()  
    ...
    3 vars
    Combat_Casting_Cost_Multiplier(), Nominal Casting Skill, Combat Condition, Spellranks per Realm
- ¿ halve values for Neutral Player ?
- Combat Turns Loop  
  - ranged threat, health, mana multiplied  
  - att: var_64, var_5C, var_60; var_54?  
  - def: var_66, var_5E, var_62; var_56?  



...ranged threat...
    IDK_health_attacker += var_60;
    IDK_health_defender += var_62;

    IDK_health_attacker -= var_3A;
    IDK_health_defender -= var_36;
...melee threat...
IDK_health_attacker -= var_3A;
IDK_health_defender -= var_36;


## Bottom-Up
    determine winner
        if(IDK_melee_threat_attacker > 10) { winner_player_idx = _combat_attacker_player; }
        if(IDK_melee_threat_defender > 10) { winner_player_idx = _combat_defender_player; }

¿ IDK_melee_threat_attacker ?
¿ "10" as in "100%" ?



var_44 = ((100 * IDK_health_attacker) / IDK_health_attacker__2);
var_44 = ((100 * IDK_health_defender) / IDK_health_defender__2);
var_42 = ((var_44 * IDK_damage_attacker) / 100);
var_42 = ((var_44 * IDK_damage_defender) / 100);
var_40 = IDK_damage_attacker;            
var_40 = IDK_damage_defender;

¿ var_44, var_42, var_40 ?

var_44 = ((100 * IDK_health_attacker) / IDK_health_attacker__2);
¿ IDK_health_attacker ?
¿ IDK_health_attacker__2 ?
¿ (IDK_health_attacker * 100) ?
¿ ((100 * IDK_health_attacker) / IDK_health_attacker__2) ?







enum enum_COMBAT_NODE_TYPE
cnt_Sorcery  = 0
cnt_Nature  = 1
cnt_Chaos  = 2

enum enum_Magic_Realm
mr_Chaos  = 0
mr_Death  = 1
mr_Life  = 2
mr_Nature  = 3
mr_Sorcery  = 4

FFFFFFFF ; enum Realm_Byte
FFFFFFFF _Nature  = 0
FFFFFFFF _Sorcery  = 1
FFFFFFFF _Chaos  = 2
FFFFFFFF _Life  = 3
FFFFFFFF _Death  = 4
FFFFFFFF _Arcane  = 5
FFFFFFFF _No_Realm  = 0FFFFFFFFh

FFFFFFFF ; enum enum_COMBAT_STRUCTURE
FFFFFFFF cs_NONE  = 0
FFFFFFFF cs_City  = 1
FFFFFFFF cs_SorceryNode  = 2
FFFFFFFF cs_NatureNode  = 3
FFFFFFFF cs_ChaosNode  = 4
FFFFFFFF cs_UNKOWN  = 5
FFFFFFFF cs_OceanTerrainType  = 6


elsewhere, Tactical Combat uses `CMB_combat_structure`
many checks for cs_Ocean - e.g., Spell - Summon







the Euclidean distance between two points in the Euclidean space
 is defined as the length of the line segment between two points. 
 As the Euclidean distance can be found by using the coordinate points and the Pythagoras theorem, it is occasionally called the Pythagorean distance

2-D Space
Euclidian

{x1,y1},{x2,y2}

Line
Point


Point1(x,y)
Point2{x,y}

~== Point1 - Point2

Absolute?


the length of the line segment between two points

Line
Line Segment

¿ mapped to {0,100} ?
¿ mapped to (0,10,20,30,40,50,60,70,80,90,100) ?
¿ mapped to (0-9,10-19,20-29,30-39,40-49,50-59,60-69,70-79,80-89,90-99,100) ?






`<<< ENTER COMBAT >>>`

Combat()
    |-> Strategic_Combat()
        |-> IDK_Combat_Allocate()
        |-> Combat_Structure()
        |-> Init_Battlefield_Effects()



WIZ_GetRangeModifier()

`<<< EXIT COMBAT >>>`



### Legacy Code
MoO1
Monsters
    checks if the Player Id is less than 0 or great than the number of defined players
    they used a bogus player id for monsters, because monsters weren't in the regular game data















##### Other People's Projects
Realms-Beyond
Fandom Wiki
MoM Manual
MoM Strategy Guide (OSG)


https://www.realmsbeyond.net/forums/archive/index.php?thread-7843.html
https://www.realmsbeyond.net/forums/showthread.php?tid=7843

https://www.realmsbeyond.net/forums/archive/index.php?thread-8142.html
https://www.realmsbeyond.net/forums/showthread.php?tid=8142


https://masterofmagic.fandom.com/wiki/Forum:Strategic_combat%3F



https://www.realmsbeyond.net/forums/showthread.php?tid=7843
20151020
Tiltowait
How does strategic combat work, exactly?
...
20151021
Seravy
1. Units are converted to their battle stats.
2. Units owned by a wizard with ID below 0 or above 5, they are all set as status=6, disintegrated. As these wizard IDs are invalid, this step probably does absolutely nothing.
3. Original stores away (30-unit defense) into a variable (var_AE).
Insecticide stores 1 into the variable instead....but if unit has 50 or higher defense (that's not even possible???) then it stores (102-unit cost/10) instead.
4. Calls the procedure "AI_Unitvalue_Healthdefense" which assigns a rating to the unit based on remaining health and defense. Adds up this value for all units together and stores it.
5. If unit has a ranged attack, it calls the "AI_Unitvalue_RangedAttack" and adds up the total then stores it.
6. Calls "AI_Unitvalue_Attack" to rate attack power like the above two, and stores it into another total for the army.
7. A long complicated process follows where the wizard's mana and skill is measured and the amount they can afford to spend on the battle is spent and stored in other variables.
8. Some complicated parts follow with lots of percentage calculations to decide the outcome
9. Units in the loser's army die
10. Units are selected by using var_AE as a priority array, and damage is being applied to them on the winner side I assume. If the unit dies from damage, another one gets selected.
11. Combat_End is called and then it's over.

A point of interest :

point 3 seems to be broken in insecticide. Don't see why he would want to assign a non-1 value only to units of 50 or higher defense.

Edit : I think it does 1 damage to the winning army for each point of difference between two variables, one of which being the total health of the winner's army. The other is a percentage of the same value, so it's making the army lose a percentage of their total health based on the outcome.

About the Mana spent
-Amount seems to be equal to 10% of the total available mana, but no more than the casting skill.
-I think it spends skill/3 mana in one go so there might be 3 "turns" of the combat.
-Influence of mana spent on the outcome of battle is applied through three different variables (probably a modifier for each of attack, ranged, and defense) multiplied by the amount of skill spent per use.
-The amount of these is influenced by the spellbooks the wizard has and the battle location (nodes). Some spellbooks seem to add more to these values than others, it does not always add up to 100%.
-If less than 10 mana would be used, it isn't, and the whole thing gets skipped. This implies you need 100 mana to have any effect on the battle through spellcasting because only 10% of the mana is spent.






https://www.realmsbeyond.net/forums/archive/index.php?thread-8142.html
20160420
Seravy
Strategic combat, further investigations

1. The spell power influence of books of a realm a node can counter are completely ignored for the attacker.
   In other words, it is assumed that all such spells will always get countered.
2. Nodes have no effect on defender spell power at all. In other words it is assumed that nodes never counter for the defender.
3. If neither the attacker, nor the defender is the human player, all non-neutral AI players receive a +50% stats boost to Total army Attack, Defense, Ranged and all spell power ratings.
   In other words, all AI players have a huge advantage against neutrals. (50% might not sound like a lot but it's applied to everything so the effect multiplies)
4. Spell power is divided to 3 categories. One is directly added to defense. One is directly added to attack. One is a percentage bonus to the attack sum of the army.
5. Casting skill/Mana is spent in chucks of 1/3 of the total available. So it'll only influence the first 3 turns of combat.
6. During the first 3 turns of combat, only ranged attack power is used in calculations.
7. The weights of realms contributing to each spell power :
   Nature = 50 Attack, 10 Percentage Attack, 25 Defense.
   Sorcery = 75 Attack, 25 Defense
   Chaos = 100 Attack, 30 Percentage Attack
   Life = 15 Percentage Attack, 40 Defense
   Death = 75 Attack, 15 Defense.
   These numbers are per book.
8. Actual Spell Power[attack] is sum of the above for all books/30
   Actual Spell Power[percentage] is sum of the above for all books/20
   Actual Spell Power[defense] is sum of the above for all books/20
9. For each 5 mana spent, Spell Power[attack] is added to the army total attack rating, for the turn it was spent only.
   For each 5 mana spent, Spell Power[percentage]*attack rating of army is added to the army total attack rating, for the turn it was spent only.
   For each 5 mana spent, Spell Power[defense] is added to the army total defense rating, and this extra amount will carry over to future turns.

For example, a skill of 50 and 10 Chaos books means (33*16)/5=105 attack power per turn for the cost of 16 MP per turn. For reference, a unit of 6 melee and 6 figures is roughly 720 attack power, so this is roughly as good as 1/6th of the attack power of a single halberdier. Unless my calculations are wrong somewhere, the effect of magic is very poor in this department, which, being the highest part of most realms, is alarming.
The same books and skill would mean (15*16)/5 = 48% attack boost for the entire army each turn from the percentage multiplier which is far more reasonable.
Assuming the same in Life books, (20*16)/5= 64 defense power is added to the army total. For reference, a unit of 2 health, 6 figures and 4 shields is 384 defensive power.

10. Damage is subtracted from the attack power to simulate how killed units don't get to fight in later rounds. Nice, something is done correctly at least! Damage done in the ranged phase is subtracted from both the melee and ranged attacks. Damage is also subtracted from "defense" which contains/works as health.

Conclusions :
3 is just unfair. The AI already had a huge advantage in these battles (neutrals don't have spells).
  Furthermore, the bonus is applied AFTER the health is saved for end of combat damage distribution for survivors.
  This means the extra 50% health all non-neutral sides gain is extra army health that, if damaged, will not translate to actual damage on units.
  Meaning in AI vs AI combat, a significant portion of the damage is automatically prevented on the winner.
5+6 means spell power will only ever affect the ranged phase of the combat (unless both parties have 0 ranged). Meaning melee armies benefit somewhat less from the "percentage attack" part of spell power.
7 is just stupid. Spell power should be increased by researched combat spells, not books. As is, a wizard with no spells known but 10 books is as powerful in battle as one who has all the spells.





Base game's formula is...similar, 3*(defense-2)*total health if defense>=3.

Damage dealt is Rnd(10)*Attack rating/100 per turn. This amount is subtracted from enemy defense rating, then an equal percentage is lost from both attack powers to simulate the lost units/figures. 
This goes on until either side reaches 0 defensive rating, the other is the winner.
