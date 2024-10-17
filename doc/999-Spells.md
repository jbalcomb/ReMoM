


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

animation counter
    GAME_MP_SpellVar_1
player index
    GAME_MP_SpellVar_3
spell index
    SBK_Spell_Index



Spell Cast Animation - Summoning
Creature Summon Animation

IDK_SummonAnim()
IDK_SummonAnim_Load()
IDK_SummonAnim_Draw()

    GAME_MP_SpellVar_3 = player_idx;

    IDK_DiploScrn_scanned_field = unit_type;

    GUI_Interaction_Done = ST_FALSE;
    var_6 = 130;
    ((SBK_SliderAnimStage < var_6) && (GUI_Interaction_Done == ST_FALSE))





Spells that tkae a "Target Wizard"

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



## Spell Researching

MoO2
Next_Turn_Calc()
    |-> Apply_All_Player_Changes_()
        |-> Check_For_Research_Breakthrough_()
Module: TECH
    Tech_Select_()

1oom
game_turn_process()
    |-> game_tech_research()
        |-> game_tech_get_new()
        |-> game_tech_finish_new()  ... ¿ same as populating the wizard's research spell list ?
        |-> ui_newtech()

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Players_Update_Magic_Power()
        |-> Players_Apply_Magic_Power()
        |-> Players_Check_Spell_Research()

Players_Apply_Magic_Power() updates _players[].research_cost_remaining.

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Players_Check_Spell_Research()
            |-> WIZ_LearnSpell__WIP()
            |-> Spell_Research_Select()
            |-> AI_Research_Picker__STUB()






## Spell Casting

Next_Turn_Proc()
    |-> Cast_Spell_Overland__WIP()

Spellbook_Screen()
    |-> WIZ_SetOverlandSpell__WIP

```c
Next_Turn_Proc()
{
    if(
        (_players[_human_player_idx].casting_cost_remaining <= 0)
        &&
        (_players[_human_player_idx].casting_spell_idx > spl_NONE))
    {
        Cast_Spell_Overland__WIP(_human_player_idx);
    }
}
```

```c
Spellbook_Screen()
{
    if(did_select_spell == ST_TRUE)
    {
        WIZ_SetOverlandSpell__WIP(HUMAN_PLAYER_IDX, spell_idx, spellbook_page_spell_index);
    }
}
```


spl_Time_Stop           handles in-line     (just sets g_TimeStop_PlayerNum)
spl_Natures_Awareness   calls out
spl_Holy_Arms           calls out
spl_Crusade             handles in-line
spl_Awareness           calls out





WIZ_GlobalSpellAnim()

