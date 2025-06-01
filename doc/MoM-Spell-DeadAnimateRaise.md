


'Animate Dead'  195 spl_Animate_Dead    Combat-Only     SCC: 5
'Raise Dead'    137 spl_Raise_Dead      Combat-Only     SCC: 5

CMB_RaiseDead                                  ovr131
CMB_AnimateDead                                ovr131

Animate Dead:
    Death. Creature Enchantment. Casting Cost: 50 mana. Very Rare.
    Raises a normal friendly or enemy unit from the dead during
    battle to fight on your side. The animated creature is undead,
    remains after battle and costs an additional 50% more than its
    normal upkeep per turn to maintain (standard units have no
    upkeep, but fantastic ones do; see Summoned Creatures).

Raise Dead:
    Life. Instant Combat. Casting Cost: 35 mana. Uncommon.
    Breathes life back into a normal unit that has died during the
    current battle. The unit reappears in a shaky state of health (half its
    normal number of figures and without any enchantments that may
    have previously been cast on the unit).



Tactical_Combat__WIP()
    if(input_field_idx == spell_button_field)
        Combat_Cast_Spell__WIP()
            Combat_Spellbook_Build__WIP()
            CMB_ComposeBookBG__WIP()
            Combat_Spellbook_Screen()
            Do_Legal_Spell_Check__WIP()
            Combat_Spell_Target_Screen__WIP()
            if(Target != 999)
                G_CMB_SpellEffect__WIP()
            


Comat Spell Target Type
    Combat_Spell_Target_Screen__WIP()
        switch(spell_data_table[spell_idx].type)
            case scc_Special_Spell:
                if(
                    (spell_idx == spl_Raise_Dead)
                    ||
                    (spell_idx == spl_Animate_Dead)
                )
                {
                    if(_combat_attacker_player == HUMAN_PLAYER_IDX)
                    {
                        CMB_TargetingType = CTT_Tile_NoUnitA;
                    }
                    else
                    {
                        CMB_TargetingType = CTT_Tile_NoUnitD;
                    }
                }


    // ovr112:0A6D
    // @@Target_And_Effect:
    if(spell_idx > spl_NONE)
        if(
            (spell_data_table[spell_idx].type == scc_Battlefield_Spell)
            ||
            (spell_data_table[spell_idx].type == scc_Counter_Spell)
            ||
            (spell_data_table[spell_idx].type == scc_Disenchant_Spell)
            ||
            (spell_data_table[spell_idx].type == scc_City_Enchantment)
            ||
            (spell_idx == spl_Animate_Dead)
        )
        {
            Target = 99;
            Target_X = 0;
            Target_Y = 0;




SBookScr.C
Spell_Animation_Load_Graphics__WIP()
    spell_idx == spl_Raise_Dead
    spell_idx == spl_Animate_Dead
        spell_animation_seg = LBX_Reload_Next(cmbtfx_lbx_file__ovr134__1of2, 22, _screen_seg);
        return_value = ST_FALSE;



## Cast_Raise_Dead()

Combat_Cast_Spell__WIP()
    Do_Legal_Spell_Check__WIP()
        if(spell_idx == spl_Raise_Dead)

G_CMB_SpellEffect__WIP()
    switch(spell_data_table[spell_idx].type)
        case scc_Special_Spell:
            if(spell_idx == spl_Raise_Dead)
                Cast_Raise_Dead(player_idx, caster_idx, target_cgx, target_cgy)

XREF:
    j_Cast_Raise_Dead()
        G_CMB_SpellEffect__WIP()



## Cast_Animate_Dead()

Combat_Cast_Spell__WIP()
    Do_Legal_Spell_Check__WIP()
        if(spell_idx == spl_Animate_Dead)

G_CMB_SpellEffect__WIP()
    switch(spell_data_table[spell_idx].type)
        case scc_Special_Spell:
            if(spell_idx == spl_Animate_Dead)
                Cast_Animate_Dead(player_idx, caster_idx)

XREF:
    j_Cast_Animate_Dead()
        G_CMB_SpellEffect__WIP()
