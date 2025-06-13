



Blur
Not in the manual, but is in the OSG
¿ added in v1.2 ?
https://masterofmagic.fandom.com/wiki/Blur#:~:text=Blur%20is%20an%20Uncommon%20Combat,by%20enemy%20units%20will%20miss.






spl_Warp_Wood
spl_Healing
spl_Creature_Binding
spl_Disrupt
    handled in-line in Cast_Spell_On_Battle_Unit()
spl_Recall_Hero
spl_Word_Of_Recall


Spell Researching
Spell Learning
Spell Casting



spell_data_table[spell_idx].spell_book_category
0   "Special Spells"
1   "Summoning"
2   "Enchantment"
3   "City Spells"
4   "Unit Spells"
5   "Combat Spells"



ovr136
ovr137
ovr138

Spell Data
Spell Book
Spell Casting
Spell Cast Animation
Spell Researching

Spell Cast Animation - Global Enchantment
Global Enchantment Animation

Spell Cast Animation - Summoning
Creature Summon Animation



Spells that take a "Target Wizard"
...or target wizard spell

Cruel Unminding:
Death. Instant. Casting Cost: 150 mana. Very Rare.
Permanently destroys one to 10% of a target wizard’s total spell casting skill points!

Drain Power:
Death. Instant. Casting Cost: 50 mana. Uncommon.
Drains from 50 to 150 points of magic power from a target
wizard’s mana reserve.

Spell Binding:
Sorcery. Instant. Casting Cost: 1000 mana;
Upkeep: As For Target Enchantment. Very Rare.
Steals a global enchantment from another wizard. The stolen
overland spell becomes the property of the casting wizard in terms
of maintenance and control.

Spell Blast:
Sorcery. Instant. Casting Cost: 50+ mana. Uncommon.
Attempts, through the direct manipulation of magic power, to
counter an enemy wizard’s spell as it is being cast. When spell blast
takes effect, a wizard must expend an amount of his or her own
magic power (from mana reserves) equal to the amount already put
into the enemy wizard’s spell to blast that spell out of existence. If
the wizard does not have enough mana, spell blast fails.

Subversion:
Death. Instant. Casting Cost: 100 mana. Uncommon.
Creates a state of universal contempt toward the target enemy
wizard, causing his or her diplomatic relations with all other wizards
to suffer severely.

TABLE OF SPECIAL SPELLS
The spells included in the following table do not fall under any convenient classification scheme.





ovr128  
ovr129  
ovr130  
ovr131  
ovr132  
ovr133  
ovr134  
ovr135  
ovr136  
ovr137  
ovr138  



...
...
...
click on a spell in the spellbook from the main screen
"Instant"

...
...
...
complete the casting of a spell in the next turn procedure

...
...
...





e.g.,
spell_data_table[spell_idx].type == sdt_Summon
spell_idx != spl_Floating_Island
... |-> IDK_SummonAnim()



ovr060:044E 83 BF 18 9F 00                                  cmp     [_players.casting_cost_remaining+bx], 0
ovr060:045F 83 BF 1C 9F 00                                  cmp     [_players.casting_spell_idx+bx], 0

ovr135:09F3 8B BF 1C 9F                                     mov     _DI_spell_idx, [_players.casting_spell_idx+bx]

ovr135:2880 C7 87 18 9F 00 00                               mov     [_players.casting_cost_remaining+bx], 0
ovr135:2890 C7 87 1A 9F 00 00                               mov     [_players.casting_cost_original+bx], 0
ovr135:28A0 C7 87 1C 9F 00 00                               mov     [_players.casting_spell_idx+bx], 0


ovr134:08E0 89 BF 1C 9F                                     mov     [_players.casting_spell_idx+bx], _DI_spell_idx ; already asigned in Spellbook_Screen()


ovr134:08ED 8B 87 1E 9F                                     mov     ax, [_players.Skill_Left+bx]

ovr140:1C5B 29 87 18 9F                                     sub     [_players.casting_cost_remaining+bx], ax

ovr140:1C30 29 BF 18 9F                                     sub     [_players.casting_cost_remaining+bx], di


C7 87   mov
18 9F   casting_cost_remaining
83 BF   cmp
18 9F   casting_cost_remaining
29 87   sub ... ax
18 9F   casting_cost_remaining
29 BF   sub ... di
18 9F   casting_cost_remaining

8B 87   mov
1E 9F   Skill_Left
