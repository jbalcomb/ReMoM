


Eh?  ... "Spell Casting Category" (SCC)?



WTF w/ `999-Spells.md`

Next_Turn_Proc()
    |-> Cast_Spell_Overland__WIP()


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
