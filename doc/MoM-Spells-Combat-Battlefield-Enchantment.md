
¿ Combat Battlefield Enchantment ?
    Black Prayer, Blur, Call Lightning, Darkness, Entangle, High Prayer, Mana Leak, Mass Invisibility, Metal Fires, Prayer, Terror, True Light, Warp Reality, Wrack


Black Prayer, Blur, Call Lightning, Darkness, Entangle, High Prayer, Mana Leak, Mass Invisibility, Metal Fires, Prayer, Terror, True Light, Warp Reality, Wrack


...casting code...
...on-going effect code...
    'Call Lightning', Wrack, 'Magic Vortex'

e.g., 'Call Lightning'
        CMB_PrepareTurn__WIP()
            |-> j_WIZ_CallLightning()
                |-> WIZ_CallLightning()
                    |-> Animate_Lightning_Bolt()



Combat_Cast_Apply_Spell_Effect()



Tactical_Combat__WIP()
    Combat_Cast_Spell()
        if(spell_idx == spl_NONE)
            Can_Cast = Combat_Spellbook_Build(caster_idx);
            CMB_ComposeBookBG__WIP();
            spell_idx = Combat_Spellbook_Screen(caster_idx, &Selected_Spell);
        // Combat_Spell_Target_Screen__WIP()
        ...must be Target == 99?
        if(Target != 999)
            Combat_Cast_Apply_Spell_Effect(spell_idx, Target, caster_idx, Target_X, Target_Y, IDK_mana, ST_TRUE, ST_NULL, ST_NULL);
                case scc_Battlefield_Spell:     // 10
                case scc_Combat_Counter_Magic:  // 21
                    Combat_Spell_Animation()


## Combat_Cast_Apply_Spell_Effect()

Combat_Cast_Apply_Spell_Effect()
    case scc_Battlefield_Spell:     // 10
        Combat_Spell_Animation()
        &&
        combat_enchantments[ce_idx] = ST_TRUE
        &&
        for(itr = 0; itr < _combat_total_unit_count; itr++)
            if(battle_units[itr].status == bus_Active)
                REINIT_BATTLEUNIT();

## Combat_Spell_Animation()

spell_idx != {spl_Flame_Strike, spl_Holy_Word, spl_Death_Spell, spl_Call_Chaos, spl_Mass_Healing }
(anims_on != ST_FALSE)
switch(spell_data_table[spell_idx].magic_realm)
...red = ;  green = ;  blue = ;
for(itr = 0; itr < 20; itr++)
    Set_Palette_Changes(0, -1);
    VGA_ShadeScreen__STUB((itr / 2), red, green, blue);
for(itr = 20; itr > -1; itr--)
    Set_Palette_Changes(0, -1);
    VGA_ShadeScreen__STUB((itr / 2), red, green, blue);



Combat_Spell_Animation(target_cgx, target_cgy, spell_idx, player_idx, anims_on, caster_idx);

target_cgx, target_cgy

spell_idx

player_idx

anims_on
    USELESS

caster_idx


Black Prayer, Blur, Call Lightning, Darkness, Death Spell, Entangle, High Prayer, Mana Leak, Mass Invisibility, Metal Fires, Prayer, Terror, True Light, Warp Reality, Wrack

Black Prayer
Blur
Call Lightning
Darkness
Entangle
High Prayer
Mana Leak
Mass Invisibility
Metal Fires
Prayer
Terror
True Light
Warp Reality
Wrack
