
Combat Battlefield Instant
Combat Battlefield Enchantment



NOTE: 'Counter Magic' uses count, rather than {F,T}

'Counter Magic' is in its own SCC, but takes the same code-path as the battlefield spells

Seems mostly just 'Combat Enchantment', but ...
...includes 'Counter Magic'
...includes 'Flame Strike'

all call Combat_Spell_Animation__WIP()
excludes 
    spl_Flame_Strike
    spl_Holy_Word
    spl_Death_Spell
    spl_Call_Chaos
    spl_Mass_Healing
from setting combat_enchantments[]





Tactical_Combat__WIP()
    Combat_Cast_Spell__WIP()
        if(spell_idx == spl_NONE)
            Can_Cast = Combat_Spellbook_Build__WIP(caster_idx);
            CMB_ComposeBookBG__WIP();
            spell_idx = Combat_Spellbook_Screen(caster_idx, &Selected_Spell);
        // Combat_Spell_Target_Screen__WIP()
        ...must be Target == 99?
        if(Target != 999)
            Cast_Spell_On_Battle_Unit(spell_idx, Target, caster_idx, Target_X, Target_Y, IDK_mana, ST_TRUE, ST_NULL, ST_NULL);
                case scc_Battlefield_Spell:     // 10
                case scc_Combat_Counter_Magic:  // 21
                    Combat_Spell_Animation__WIP()

#### 'Cast Flame Strike'

Cast_Spell_On_Battle_Unit()
Combat_Spell_Animation__WIP()

Spell_Animation_Load_Graphics()
    case spl_Flame_Strike:
        // CMBTFX.LBX, 033  "FIREPILL"  "Fire Storm"
        spell_animation_seg = LBX_Reload_Next(cmbtfx_lbx_file__ovr134__1of2, 33, _screen_seg);
    return_value = 0
spell_animation_seg
    width:   28
    height:  34
frame_count: 16
...
if(Anim_Size == 0)  /* Flame Strike */
    screen_x -= 14;
    screen_y -= 21;

CMB_ConvSpellAttack__WIP__SEGRAX()
¿ hard-coded to rat_Nat_Bolt as the attack type ?
...can't be "Nat" as in "Nature", though, because also takes magic realm?
...
test    [bp+Attack_Flags], Att_AREAFLAG
attack count = figure count
...adds Att_DMGLIMIT ... but, what's Att_DMGLIMIT?
    ...means the damage max is hits per figure?
        ...because each figure takes that damage amount

Effective_Defense == 8
calc?

