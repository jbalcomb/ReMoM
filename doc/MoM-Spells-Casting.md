


Eh?  ... "Spell Casting Category" (SCC)?



WTF w/ `999-Spells.md`

Next_Turn_Proc()
    |-> Cast_Spell_Overland__WIP()


IDA Group Colors
    sdt_Summoning           ( 0)  #24 reddish-brown
    ...
    sdt_Global_Enchantment  ( 9)  #13 ~ blue, greyish/greenish
    ...
    sdt_Crafting_Spell      (11)  #17 brown
    ...
    Banish_Spell            (23)



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
