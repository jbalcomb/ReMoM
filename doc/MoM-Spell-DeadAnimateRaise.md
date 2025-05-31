


'Animate Dead'  195 spl_Animate_Dead    Combat-Only     SCC: 5
'Raise Dead'    137 spl_Raise_Dead      Combat-Only     SCC: 5

CMB_RaiseDead                                  ovr131
CMB_AnimateDead                                ovr131

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




SBookScr.C
Spell_Animation_Load_Graphics__WIP()
    spell_idx == spl_Raise_Dead
    spell_idx == spl_Animate_Dead
        spell_animation_seg = LBX_Reload_Next(cmbtfx_lbx_file__ovr134__1of2, 22, _screen_seg);
        return_value = ST_FALSE;

