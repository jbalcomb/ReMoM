

Page 28  (PDF Page 33)
"Note that the weapons and armor a hero can be equipped with depend on
the hero’s type; some heroes are mages, others are warriors, etc."


Page 80  (PDF Page 85)
The following table defines all of the special abilities available to units.
Abilities available only to heroes are indicated by an asterisk (*).
There are "super" variations of all hero special abilities that give an additional 50% bonus.

*Arcane Power. . . . . . Increases magic ranged attack strength by one per experience level of the hero.





Init_Heroes()


switch(Random(14) - 1)
case  0:  /* Leadership */
case  1:  /* Legendary */
case  2:  /* Armsmaster */
case  3:  /* Blademaster */
case  4:  /* Might */
case  5:  /* Constitution */
case  6:  /* Arcane Power */
case  7:  /* Prayermaster */
case  8:  /* Casting Skill */
case  9:  /* Noble */
case 10:  /* Charm */
case 11:  /* Lucky */
case 12:  /* Agility */
case 13:  /* Sage */


...something with which pick-count is tested for each skill...
Lucky doesn't test the pick counts
Casting Skill tests mage_picks
Sage tests mage_picks

NOTE: Arcane Power is complicated and buggy

UNITTYPE.H
    /* this group is used for assigning 'Arcane Power' - "magic ranged attack" */
    rat_Lightning    =  30,
    rat_Fireball     =  31,
    rat_Sorcery      =  32,
    rat_Deathbolt    =  33,
    rat_Icebolt      =  34,
    rat_Pr_Shaman    =  35,
    rat_Drow         =  36,
    rat_Sprite       =  37,
    rat_Nat_Bolt     =  38,
