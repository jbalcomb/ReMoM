MoM-Spells-Casting-Combat.md


CMB_SelectCaster()  ==>  Combat_Cast_Spell_With_Caster()
Combat_Cast_Spell__WIP()   ==> Combat_Cast_Spell__WIP()
GAME_LoadSpellAnim()  ==>  Spell_Animation_Load_Graphics__WIP()
CMB_ShowSpellbook__WIP()  ==>  Combat_Spellbook_Screen()
CMB_RedrawSpellbook__WIP()  ==>  Combat_Spellbook_Screen_Draw()
CMB_DrawFullScreen__WIP()  ==>  Tactical_Combat_Draw()
CMB_CheckSpellErrors()  ==>  Do_Legal_Spell_Check__WIP()
G_CMB_SpellEffect()  ==> G_CMB_SpellEffect__WIP()  ==>  Cast_Spell_On_Battle_Unit()



CMB_SpellcastMessage()
    manual message box of "[player name] has cast [spell name]", "The [unit type name] [has|have] cast [spell name].", etc.


Combat_Cast_Spell__WIP()
    USA_SUMMON_DEMON_1 or USA_SUMMON_DEMON_2
    ...
        BU_SummonDemon__SEGRAX()
AI_BU_ProcessAction__WIP()
    case BUA_SummonDemon:
        BU_SummonDemon__SEGRAX(battle_unit_idx);




## Cast_Spell_On_Battle_Unit()



Combat_Cast_Spell__WIP()
    if(Target != 999)
        Cast_Spell_On_Battle_Unit(spell_idx, Target, caster_idx, Target_X, Target_Y, IDK_mana, ST_TRUE, ST_NULL, ST_NULL);
'Target' is battle_unit_idx
'IDK_mana' is casting cost (as in TSCC)

'Anims' is always ST_TRUE  (used by group and individual spell functions)
no idea what's up with the two unused parameters

### IDK_mana
    Combat_Cast_Spell__WIP()
        if(spell_idx != spl_NONE)
            if( (spell_data_table[spell_idx].type < scc_Infusable_Spell) || (Spell_Like_Ability != ST_FALSE) )
                if(caster_idx > CASTER_IDX_BASE)
                    IDK_mana = Casting_Cost(player_idx, spell_idx, 1);
                else
                    IDK_mana = spell_data_table[spell_idx].casting_cost;
            else
                if( (player_idx == HUMAN_PLAYER_IDX) && (_auto_combat_flag == ST_FALSE) )
                    IDK_mana = Combat_Spellbook_Mana_Adder_Screen(spell_idx, Selected_Spell, caster_idx);
        if(spell_idx > spl_NONE)
            if(Target != 999)
                Cast_Spell_On_Battle_Unit(spell_idx, Target, caster_idx, Target_X, Target_Y, IDK_mana, ST_TRUE, ST_NULL, ST_NULL);






'(Summon) Phantom Warriors'
Tactical_Combat__WIP()
    Combat_Cast_Spell__WIP()
        Combat_Spell_Target_Screen__WIP()
        Cast_Spell_On_Battle_Unit()
            Create_Unit__WIP()
            UNIT_SummonToBattle__SEGRAX()
            BU_CombatSummon__SEGRAX()
                BU_CreateImage__SEGRAX()

Combat Spell Target Type  4  cstt_   AKA CTT_Tile_NoUnitA
Combat_Spell_Target_Screen__WIP()
    switch(spell_data_table[spell_idx].type)
        case scc_Summoning:
            if(_combat_attacker_player == HUMAN_PLAYER_IDX)
                CMB_TargetingType = CTT_Tile_NoUnitA;
...
Cast_Spell_On_Battle_Unit()
    switch(spell_data_table[spell_idx].type)
        case scc_Summoning:
            Figure_Count = Create_Unit__WIP(spell_data_table[spell_idx].unit_type, player_idx, 0, 0, 9, 2000);
            UNIT_SummonToBattle__SEGRAX(player_idx, (_units - 1), target_cgx, target_cgy);
            BU_CombatSummon__SEGRAX((_combat_total_unit_count - 1), target_cgx, target_cgy, spell_idx, caster_idx);
...
UNIT_SummonToBattle__SEGRAX()
    BU_UnitLoadToBattle__SEGRAX()
        Load_Battle_Unit()
        Battle_Unit_Slot_Open()
        Combat_Figure_Load()


¿¿¿



???




BU_CreateImage__SEGRAX()

~ USELESS_Combat_Figure_Load_Compose()
    FIGUREX_OFFSET
    FIGUREX_MAP
    FIGUREX_POINTER
    FLIC_Set_CurrentFrame(ptr_figure_pointer_seg[figure_set_idx], frame_num);
    Draw_Picture_To_Bitmap(ptr_figure_pointer_seg[figure_set_idx], GfxBuf_2400B);
    Combat_Figure_Banner_Color(player_idx);
    Combat_Unit_Enchantment_Outline_Draw(enchantment_magic_realm);

IMG_CMB_FX_Figure = Allocate_Next_Block(World_Data, 124);  // 124 PR, 1984 B
















Code-Path to Human Player Casts Spell
...Summoning
Combat_Cast_Spell__WIP()
    ...
    ...
    ...
    ovr112:0A6D
    @@Target_And_Effect:
        j_CMB_TargetSpell()  || j_AITP_CombatSpell__STUB()
        j_Cast_Spell_On_Battle_Unit()











## ¿ Caster ID ?
battle_unit_idx OR (20 + player_idx)
NOTE: maximum battle_unit_idx is 17
there are hard-coded checks for caster_id = { ¿ 20, 21, 22, 23, 24, ... ? }



#### Combat_Cast_Spell__WIP() vs. Combat_Cast_Spell_With_Caster()
Tactical_Combat__WIP()
    |-> Combat_Cast_Spell__WIP()
    ||
    |-> Combat_Cast_Spell_With_Caster()
        |-> Combat_Cast_Spell__WIP()



BU_SummonDemon__STUB()



BU_HasSpellAbility__WIP()

Combat_Cast_Spell_With_Caster()
Combat_Cast_Spell__WIP()
Combat_Cast_Spell_Error()

Combat_Spellbook_Build__STUB()
CMB_ComposeBookBG()
CMB_ShowSpellbook()

CMB_SpellSlider()

CMB_CheckSpellErrors()

Casting_Cost()
Casting_Cost_Reduction()

WIZ_DispelAttempt()

CMB_CounterMessage()

CMB_TargetSpell()
AITP_CombatSpell__STUB()

Cast_Spell_On_Battle_Unit()

BU_SummonDemon()



COMBAT.C
¿ ovr103 ?
    cnst_CmbCastError_2 db 'You are unable to throw spells at this time.',0
    cnst_CmbCastError_1 db 'You may only cast once per turn.',0
    cnst_Cancel_2 db 'Cancel',0             ; DATA XREF: Combat_Cast_Spell_With_Caster+49o
    cnst_CasterSelectMsg db 'Who Will Cast',0

; shows the combat spell caster selection dialog and,
; if a valid caster is chosen, proceeds directly to
; calling and resolving the CMB_CastSpell function
Combat_Cast_Spell_With_Caster()
XREF:
    j_Combat_Cast_Spell_With_Caster()
        Tactical_Combat__WIP()


CMB_WizCastAvailable {F,T}

CMB_CastSpell()

## Combat_Cast_Spell__WIP()
XREF:
    j_Combat_Cast_Spell__WIP()
        Tactical_Combat__WIP+DF0
        AI_CMB_PlayTurn__WIP+24
        Combat_Cast_Spell_With_Caster+93
        Combat_Cast_Spell_With_Caster+BD
        Combat_Cast_Spell_With_Caster+DB
        AI_BU_ProcessAction__WIP+8AB




## Combat_Cast_Spell_With_Caster()

