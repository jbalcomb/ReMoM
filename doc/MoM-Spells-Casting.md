


Overland Casting
Combat Casting

...accumulate mana/skill towards current casting

Clicking on a Spell in the Spellbook
vs.
Casting Cost is Paid





Eh?  ... "Spell Casting Category" (SCC)?



WTF w/ `999-Spells.md`



Data Points?
Usages?

Which spells use the Overland Spell Target Picker Screen?
...same as those that call the target type coordinates function?

Which spells have their own screen?
    Spell Of Mastery

Which spells are handled in-line?
    Time Stop

Which spells are in which Spell Casting Category (SCC)?
...Spell Book Category

Which spells are extra special?
    Floating Island     
    Animate Dead
    Raise Undead
    Earth Lore          hard-code checks and logic changes

Which spells are treated like unit abilities?
    ¿ per Combat_Cast_Spell();  'Doom Bolt', 'Fire Bolt', 'Web', 'Healing', 'Summon Demon' ?

Which spells are added to items?


Which LBX has the spell's graphics?
Which LBX has the spell's sound effects?
Which LBX has the spell's music?


Apply_City_Changes()
    Apply_Spell_Consecration(itr_cities);
    // SPELLY  CTY_StreamOfLife(itr_cities);
    // SPELLY  CTY_ChaosRift(itr_cities);
    // SPELLY  CTY_GaiasBlessing(itr_cities);
    // SPELLY  CTY_NightshadeDispel(itr_cities);
"...negative enchantments..."
    Apply_Spell_Consecration()
        _CITIES[city_idx].enchantments[CHAOS_RIFT] = 0;
        _CITIES[city_idx].enchantments[EVIL_PRESENCE] = 0;
        _CITIES[city_idx].enchantments[CURSED_LANDS] = 0;
        _CITIES[city_idx].enchantments[FAMINE] = 0;
        _CITIES[city_idx].enchantments[PESTILENCE] = 0;
        _CITIES[city_idx].enchantments[CHAOS_RIFT] = 0;



SBookScr.C
// WZD o134p10
int16_t Spell_Animation_Load_Graphics__WIP(int16_t spell_idx)

spell_idx == spl_Floating_Island
    spell_animation_seg = LBX_Reload_Next(specfx_lbx_file__ovr134__2of3, spell_data_table[spell_idx].magic_realm, _screen_seg);
    return_value = ST_TRUE;
spell_idx == spl_Raise_Dead
spell_idx == spl_Animate_Dead
    spell_animation_seg = LBX_Reload_Next(cmbtfx_lbx_file__ovr134__1of2, 22, _screen_seg);
    return_value = ST_FALSE;







## Cast Spell - Overland

Next_Turn_Proc()
    |-> Cast_Spell_Overland__WIP()

Spellbook_Screen()
    |-> Cast_Spell_Overland_Do
        if(did_select_spell == ST_TRUE)
            Cast_Spell_Overland_Do(HUMAN_PLAYER_IDX, spell_idx, spellbook_page_spell_index);

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
        Cast_Spell_Overland_Do(HUMAN_PLAYER_IDX, spell_idx, spellbook_page_spell_index);
            ...
            Cast_Spell_Overland__WIP(player_idx);
    }
}
```




spl_Time_Stop           handles in-line     (just sets g_TimeStop_PlayerNum)
spl_Natures_Awareness   calls out
spl_Holy_Arms           calls out
spl_Crusade             extra in-line handling
spl_Awareness           calls out



## City Enchantment (2)


## City Curse (3)



## Global Enchantment (9)

~ Special Global Enchantments
spl_Time_Stop
spl_Natures_Awareness
spl_Holy_Arms
spl_Crusade
spl_Awareness

WIZ_GlobalSpellAnim()
g_TimeStop_PlayerNum = (player_idx + 1);
// SPELLY  Cast_NaturesAwareness(player_idx);
// SPELLY  Cast_HolyArms(player_idx);
// SPELLY  Cast_Awareness(player_idx);

Change_Relations_For_Enchantments(player_idx, spell_idx, 1);





Next_Turn_Proc()
    if(
        (_players[_human_player_idx].casting_cost_remaining <= 0)
        &&
        (_players[_human_player_idx].casting_spell_idx > spl_NONE))
    {
        Cast_Spell_Overland__WIP(_human_player_idx);
    }



IDA Group Colors
    sdt_Summoning           ( 0)  #24 reddish-brown
    ...
    sdt_Global_Enchantment  ( 9)  #13 ~ blue, greyish/greenish
    ...
    sdt_Crafting_Spell      (11)  #17 brown
    ...
    Banish_Spell            (23)




## Cast_Spell_Overland_Do()

But, what does it *DO*?!?
    "To cast a spell, click on the name of the spell."

...in ovr134, so ~ 'Spellbook Screen - Cast Spell'
...but, also, just 'Cast Spell'
...maybe, no reason it's not Overland_Cast_Spell() |-> Overland_Cast_Spell_Do()
    ...overstates the relationship?  ...no, because it's clear how the spell to cast gets set?
(What does the 'Combat Spellbook' do?)

...checks mana reserve and spell casting skill...
    ...just to see if it will/can/does cast immediately?  "Instant"

if scc_Crafting_Spell
    if spl_Enchant_Item
        IDK_CreateArtifact__STUB();  Make_Item();
    if spl_Create_Artifact
        IDK_CreateArtifact__STUB();  Make_Item();
else
    if spl_Spell_Of_Mastery
        SoM_Started()
    else
        sets casting_cost_remaining
        sets casting_cost_original
if >= scc_Infusable_Spell
    Casting_Cost()
    SBK_SpellSlider()




XREF:
    Spellbook_Screen()
    j_Cast_Spell_Overland_Do()
        AI_Spell_Select__STUB()
```c
Spellbook_Screen()
{
    if(did_select_spell == ST_TRUE)
    {
        Cast_Spell_Overland_Do(HUMAN_PLAYER_IDX, spell_idx, spellbook_page_spell_index);
            ...
            Cast_Spell_Overland__WIP(player_idx);
    }
}
```





## AI_Spell_Select__STUB()

XREF:
    j_AI_Spell_Select__STUB()
        AI_Next_Turn__WIP()
            j_AI_Next_Turn__WIP()
                Next_Turn_Calc()



## AITP_OVL_TileSpells__STUB()
spl_Black_Wind
spl_Change_Terrain
spl_Corruption
spl_Disenchant_Area
spl_Disenchant_True
spl_Enchant_Road
spl_Fire_Storm
spl_Floating_Island
spl_Ice_Storm
spl_Natures_Cures
spl_PlaneShift
spl_Raise_Volcano
spl_Stasis
spl_Transmute

Cast_Spell_Target_Error(spell_idx)
; if debug is enabled, quits the game with an error
; stating the specified spell could not be found for
; the computer player, indicating a missing AI target
; picker function








## Cast_Spell_Overland__WIP()

BEGIN:  Suppress Magic
BEGIN:  Tranquility  (~== Suppress Chaos Magic)
BEGIN:  Life Force  (~== Suppress Death Magic)

switch(spell_data_table[spell_idx].type)




### show_message_flag

4 XREFs

if(player_idx == HUMAN_PLAYER_IDX)
{
    show_message_flag = ST_TRUE;
}
else
{
    show_message_flag = ST_FALSE;
}

Cast_Successful = j_CTY_CounterSpell(spell_idx, itr, player_idx, show_message_flag);

Cast_Successful = j_CTY_CounterSpell(spell_idx, G_Have_Targets, player_idx, show_message_flag);

; attempts to use any relevant asset in the city to
; counter the selected spell if possible
; returns 1 if the spell can be cast, or 0 if countered
CTY_CounterSpell()
...controls whether the fizzle notification is shown, manually handled per spell/counter reason
if(ShowMsg != ST_FALSE)
{
    j_Fizzle_Notification(player_idx, _CITIES[city_idx].owner_idx, spell_idx, cnst_Consecration_2);
}
