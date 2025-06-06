

SPELLSCR.LBX
MONSTER.LBX
SPLMASTR.C
// WZD o137p01
void IDK_SummonAnim_Load(int16_t type_or_other, int16_t player_idx)
// WZD o137p02
void IDK_SummonAnim_Draw(void)
// WZD o137p03
void IDK_SummonAnim(int16_t unit_type, int16_t magic_realm_spell_idx, int16_t player_idx)
// WZD o137p08
void OVL_LoadGlobalAnim(int16_t spell_idx, int16_t player_idx)
// WZD o137p09
void OVL_DrawGlobalAnim(void)
// WZD o137p10
void WIZ_GlobalSpellAnim(int16_t player_idx, int16_t spell_idx)

...Learn_Spell_Animation() in ovr118
...Cast_Spell_Overland__WIP() in ovr135



## City Enchantment

// WZD o137p06
void IDK_Spell_Cityscape_1(int16_t city_idx, int16_t spell_idx, int16_t player_idx)
// WZD o137p07
void IDK_Spell_Cityscape_2(int16_t city_idx, int16_t spell_idx, int16_t player_idx)

Cast_Spell_Overland__WIP()
    switch(spell_data_table[spell_idx].type)
    case scc_City_Enchantment:  //  2
    Cast_Successful = Spell_Casting_Screen__WIP(stt_Friendly_City, &G_Have_Targets, &RetY, &RetP, &var_12, &var_10, &spell_name);
    if(Cast_Successful == ST_TRUE)
    /* SPELLY */  IDK_Spell_Cityscape_1__WIP(G_Have_Targets, spell_idx, player_idx);
    /* SPELLY */  IDK_Spell_Cityscape_2__WIP(G_Have_Targets, spell_idx, player_idx);








// WZD o134p10
// drake178: GAME_LoadSpellAnim()
/*
; loads and appends the animation corresponding to the
; selected spell into the LBX_Sandbox_Segment after
; reallocating the battle figure space, assigning its
; segment pointer to the IMG_GAME_SpellAnim@ global
; returns 1 for Black Wind, Earth to Mud, and spells
; loading the generic realm effects; or 0 otherwise
; (indicates the draw location for the animation)
; CAN'T LOAD MULTIPLE ANIMATIONS ONE AFTER THE OTHER!
*/
void Spell_Animation_Load_Graphics__WIP(int16_t spell_idx)


spl_Floating_Island
spl_Raise_Dead
spl_Animate_Dead

if(spell_data_table[spell_idx].type == scc_Summoning)
    if (spell_idx == spl_Floating_Island)
        spell_animation_seg = LBX_Reload_Next(specfx_lbx_file__ovr134__2of3, spell_data_table[spell_idx].magic_realm, _screen_seg);
        return_value = ST_TRUE;
    else if(
    (spell_idx == spl_Raise_Dead)
    ||
    (spell_idx == spl_Animate_Dead)
        spell_animation_seg = LBX_Reload_Next(cmbtfx_lbx_file__ovr134__1of2, 22, _screen_seg);
        return_value = ST_FALSE;
    else
            spell_animation_seg = LBX_Reload_Next(specfx_lbx_file__ovr134__2of3, spell_data_table[spell_idx].magic_realm, _screen_seg);
            return_value = ST_TRUE;



ovr134:15B1
jt_spell_idx_load_anim
spl_Earth_to_Mud
spl_Web
spl_Cracks_Call
spl_Ice_Bolt
spl_Change_Terrain
spl_Petrify
spl_Ice_Storm
spl_Call_Lightning
spl_Dispel_Magic_True
spl_Confusion
spl_Psionic_Blast
spl_Disenchant_True
spl_Vertigo
spl_Spell_Blast
spl_DisjunctionTrue
spl_Banish
spl_Mind_Storm
spl_Stasis
spl_SpellBinding
spl_Warp_Wood
spl_Disrupt
spl_Fire_Bolt
spl_Corruption
spl_Fireball
spl_Raise_Volcano
spl_Warp_Lightning
spl_Doom_Bolt
spl_Magic_Vortex
spl_Fire_Storm
spl_Flame_Strike
spl_Disintegrate
spl_Call_The_Void
spl_Star_Fires
spl_Dispel_Evil
spl_Life_Drain
spl_Drain_Power
spl_Possession
spl_Subversion
spl_Warp_Node
spl_Black_Wind
spl_Cruel_Unminding
spl_Word_of_Death
spl_Death_Spell
spl_Dispel_Magic
spl_Disenchant_Area
spl_Disjunction
