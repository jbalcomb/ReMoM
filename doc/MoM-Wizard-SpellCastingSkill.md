


triangle number distribution

slightly better very early returns than powers of 2 distribution




Points of Impact (PoI):
'Magic Screen'
"Casting Skill:   SCS(+HSCS)



how your available magic power is divided between researching, reserves and spell casting skill

Spell casting skill is the amount of mana per turn that you can channel into casting overland spells.  
It is also the amount of mana you can cast during one battle.  
Fledgling wizards start with a spell casting skill of five unless they are also Archmages (see Special Abilities),  
  in which case they start with a spell casting skill of fifteen.  
Note that if a wizard has a hero with spell casting skill residing in his or her enchanted fortress,  
  half of that hero’s spell casting skill contributes to the wizard’s overland casting ability (see Casting Overland).
A hero’s contribution to this skill is indicated by a second number in parentheses following the spell casting skill.  
This second number is the total current spell casting skill of the wizard, taking any heroes’ contributions into account.










## Player_Base_Casting_Skill() vs. Player_Nominal_Skill()






All_Players_Apply_Spell_Casting()
    _players[itr_players].Skill_Left += Player_Hero_Casting_Skill(itr_players);
    _players[itr_players].Nominal_Skill = Player_Base_Casting_Skill(itr_players);



Cast_Spell_Overland_Do()
        instant_mana = _players[player_idx].mana_reserve;
        instant_mana = _players[player_idx].Skill_Left;


NEXTTURN.C:// drake178: Calc_Nominal_Skill()
NEXTTURN.C:// AKA Player_Nominal_Skill()

INITGAME.C:            _players[itr_players].Nominal_Skill = Player_Nominal_Skill(itr_players);
NEXTTURN.C:        _players[itr1].Nominal_Skill = Player_Base_Casting_Skill(itr1);
NEXTTURN.C:            _players[itr_players].Nominal_Skill = Player_Base_Casting_Skill(itr_players);
Spellbook.C:    _players[HUMAN_PLAYER_IDX].Nominal_Skill = Player_Base_Casting_Skill(HUMAN_PLAYER_IDX);

NEXTTURN.C:        // ¿ vs. Calc_Nominal_Skill() ?

jbalcomb@Velociraptor MINGW64 /c/STU/devel/ReMoM/src (current)
$ grep -I Nominal_Skill *
AIDUDES.C:    int16_t Nominal_Skill = 0;
AIDUDES.C:    Nominal_Skill = Player_Base_Casting_Skill(player_idx);
AIDUDES.C:                ((Mana_Total * 4) < Nominal_Skill)
Combat.C:        _players[_combat_attacker_player].Cmbt_Skill_Left = _players[_combat_attacker_player].Nominal_Skill;
Combat.C:            _players[_combat_defender_player].Cmbt_Skill_Left = _players[_combat_defender_player].Nominal_Skill;
Combat.C:        if(_players[_combat_attacker_player].Nominal_Skill < attacker_mana_multiplied)
Combat.C:            attacker_mana_multiplied = _players[_combat_attacker_player].Nominal_Skill;
Combat.C:        attacker_third_nominal_skill = (_players[_combat_attacker_player].Nominal_Skill / 3);
Combat.C:        if(_players[_combat_defender_player].Nominal_Skill < defender_mana_multiplied)
Combat.C:            defender_mana_multiplied = _players[_combat_defender_player].Nominal_Skill;
Combat.C:        defender_third_nominal_skill = (_players[_combat_defender_player].Nominal_Skill / 3);
INITGAME.C:        _players[itr_players].Nominal_Skill = 0;
INITGAME.C:        while(_players[itr_players].Nominal_Skill < spell_skill)
INITGAME.C:            _players[itr_players].Nominal_Skill = Player_Nominal_Skill(itr_players);
INITGAME.C:int16_t Player_Nominal_Skill(int16_t player_idx)
INITGAME.H:int16_t Player_Nominal_Skill(int16_t player_idx);
LoadScr.C:    _players[HUMAN_PLAYER_IDX].Nominal_Skill = Player_Base_Casting_Skill(HUMAN_PLAYER_IDX);
grep: MOX: Is a directory
MagicScr.C:            if((WIZ_ManaPerTurn + _players[HUMAN_PLAYER_IDX].mana_reserve) > _players[HUMAN_PLAYER_IDX].Nominal_Skill)
MagicScr.C:                SBK_BookManaLimit = _players[HUMAN_PLAYER_IDX].Nominal_Skill;
MainScr.C:                turns_til_return = _players[HUMAN_PLAYER_IDX].casting_cost_remaining / _players[HUMAN_PLAYER_IDX].Nominal_Skill;
NEXTTURN.C:// drake178: Calc_Nominal_Skill()
NEXTTURN.C:// AKA Player_Nominal_Skill()
NEXTTURN.C:        _players[itr1].Nominal_Skill = Player_Base_Casting_Skill(itr1);
NEXTTURN.C:        _players[itr1].Skill_Left = _players[itr1].Nominal_Skill;
NEXTTURN.C:        // ¿ vs. Calc_Nominal_Skill() ?
NEXTTURN.C:        _players[itr_players].Nominal_Skill
NEXTTURN.C:            _players[itr_players].Nominal_Skill = Player_Base_Casting_Skill(itr_players);
NEXTTURN.C:                    _players[itr_players].casting_cost_remaining -= _players[itr_players].Nominal_Skill;
NEXTTURN.C:                    _players[itr_players].mana_reserve -= _players[itr_players].Nominal_Skill;
NEXTTURN.C:                    if(_players[itr_players].Nominal_Skill > _players[itr_players].mana_reserve)
NEXTTURN.C:                        magic_units = _players[itr_players].Nominal_Skill;
NEXTTURN.C:                _players[itr_players].Skill_Left += _players[itr_players].Nominal_Skill;
NEXTTURN.C:                _players[itr_players].Skill_Left = _players[itr_players].Nominal_Skill;
grep: STU: Is a directory
Spellbook.C:    _players[HUMAN_PLAYER_IDX].Nominal_Skill = Player_Base_Casting_Skill(HUMAN_PLAYER_IDX);
Spellbook.C:    if((WIZ_ManaPerTurn + _players[HUMAN_PLAYER_IDX].mana_reserve) > (_players[HUMAN_PLAYER_IDX].Nominal_Skill + hero_casting_skill))
Spellbook.C:        SBK_BookManaLimit = (_players[HUMAN_PLAYER_IDX].Nominal_Skill + hero_casting_skill);






https://masterofmagic.fandom.com/wiki/Spell_Casting_Skill#:~:text=Spell%20Casting%20Skill%20is%20the,apply%20only%20to%20overland%20spellcasting.
Power and Skill Point Summary
The program stores a wizard's lifetime, cumulative investment of  Power Points in skill. It derives the Spell Casting Skill from this lump sum.
Using your displayed Spell Casting Skill in the magic screen, you can estimate your "lifetime total," using the equation below. The game is doing it the other way around, plugging the "lifetime total" into the quadratic rearrangement (second below) to obtain your nominal Spell Casting Skill.
Total  Power invested as a function of current skill  	
{\displaystyle Power=(Skill-1)^{2}+Skill}
Spell Casting Skill as a function of invested  Power	
{\displaystyle Skill={\frac {{\sqrt {4(Power)-3}}+1}{2}}}
Each Spellbook rank taken at the start contributes an initial endowment to the "lifetime" total, as if enough power points had already been invested to advance the wizard +2 Spell Casting Skill Points.
Turn-by-turn  Power Point allocations to Skill from your power base are added onto the lifetime total.
The Archmage Retort adds +10 points to Spell Casting Skill which are not incorporated in the above formulas in any way; therefore, the bonus does not slow down your advancement. To the turn-by-turn  Power spending, the retort quietly adds 50% more than is displayed, rounded down to the nearest whole integer.
For instance, let us say a wizard starts up with 8 spellbooks, in any combination. The game knows she should begin with 16 Spell Casting Skill, and therefore gives her an initial endowment of
(16 − 1)² + 16 =  241 already added towards improving skill.
If she commits  7 towards skill, this lifetime total rises to  248. As an Archmage, the only differences will be that this total rises to  251 instead, and that her nominal casting skill is going to be 10 points higher at all times.
Note: In the case where you start with no spellbooks and have no power invested, the game just declares the Spell Casting Skill to be zero (rather than attempting to take the square root of a negative number in its formula!). Getting the first skill point costs exactly  1.

https://masterofmagic.fandom.com/wiki/Spell_Casting_Skill#articleComments
I like Serena
10/4/2015
Changing Spell Skill to Spell Casting Skill.
Before I make that change, I'm checking if the word "Spell Skill" has any meaning in Master of Magic. So I've scanned the relevant manuals and found:

Game Manual p.86, applying to items:
    Spell Skill which can raise the hero’s spell casting skill by the specified amount.
Game Manual Table L, applying to items:
    Note that all increments for adding the above bonuses to artifacts are in increments of +1 except for Spell Skill which is added in +5 increments and Spell Save which is in -1 increments.
Game Manual:
    too many references to count to "Spell casting skill". Note that the bare "Casting skill" never occurs.
Game binaries:
    2 references, both referring to items.
Game binaries:
    about 80 references,
    almost always as "Spell casting skill", but also as "Base casting skill". Only in 1 case it's "Casting skill", and in 2 more cases it's "Caster's skill".
Strategy Guild pp. 30-31, 56-57, 322-323, 206-207:
    Inconsistent refererences to "Spell-casting skill", which is sometimes shortened to "Spell Skill" or referred to as "Skill level".

So it seems to me we can go forward an make the change, but to "Spell casting skill" instead of just "Casting skill".
The phrase "Spell Skill" should only be retained where it refers to an item bonus.

A Fandom user
10/4/2015
Oh you made huge efforts. There's also Special:WhatLinksHere. About 70 pages link to Spell Skill. I think Casting Skill is less ambiguous than Spell Skill, but I don't think the term has to be used everywhere (to clarify, I am not the guest who added the note at the beginning of this article; I am the guest under the Power conversation, and I don't think anything has to be changed; that said, it's sometimes hard to distinguish between casting skill and mana reserve because "Mana" is often used in both cases).
I like Serena
10/4/2015
Thanks. I'll check that link. Hmm... it seems I have another ~100 pages to work through...
Well, I like consistency. It turns out that the game and its manual are very consistent in its use of "Spell casting skill", so I felt we should do nothing less here.
(It is somewhat confusing to see two different people named "A Wikia contributor" though. Any chance that can be alleviated? It should take less of an effort. ;))
And as I see it now, both Magic Power and Spell Casting Skill do refer to mana. I consider "mana" the unit.  
Magic Power determines the amount of mana that will partly be transferred to the Magic Reserve (or Mana Pool).  
When we cast a spell, it requires a certain amount of spell skill points to cast it, and additionally it requires an amount of mana points from the Magic Reserve, which is typically 2-3 times higher, depending on distance.
(Edited by I like Serena)
I like Serena
9/30/2015
The whole wording around mana and skill is pretty ambiguous.
Buildings like the shrine provide some kind of raw Mana that can then be spent on Mana, Skill, or Research.
When raw Mana is spent on Mana it will land in the Magic Reserve, which is commonly referred to as the Mana pool, increasing the ambiguity. That is, mana could refer to what is produced by the shrine, what you set as percentage to spend on mana, or to the mana that you have in reserve.
When raw mana is spent on Skill, it lands in some kind of Skill pool, that in time results through a complicated formula in Casting Skill (which in this article is currently named Spell Skill).
For a building like the Alchemist's Guild, I currently don't know if it's paid for from raw mana or from the mana pool. Same for the upkeep of phantastic creatures.
Can we come up with some consistent naming scheme?
(Edited by I like Serena)
A Fandom user
10/1/2015
Surveyor says Power for Mithril Ores etc. . The race descriptions also say Power/population. But yeah the building descriptions only say Mana. Myrran pick also says Mana.
(Edited by WikiaBot)
A Fandom user
10/1/2015
Finally, Meld Node says Power, Historian says Magic Power.
(Edited by WikiaBot)
I like Serena
10/4/2015
Okay. Okay. Magic Power it is, or Power for short.
Recapping, I think we have:
    0 (MP) -> Spell Skill Pool (MP) -> Spell Casting Skill (SP/turn or SP/battle) -> Spell Cost (SP)
    0 (MP) ->  0 or  0 (MP) -> Spell Cost (SP)
    0 (MP) ->  0 (RP) -> Known spells (RP)
(Edited by I like Serena)
