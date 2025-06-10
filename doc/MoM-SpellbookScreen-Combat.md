



WHEREIS  add extra mana / slider
    Combat_Cast_Spell__WIP()
        |-> Combat_Spellbook_Mana_Adder_Screen()



scc_Var_Dmg_Spell       = 22,   /* COMBAT:  Fire Bolt, Fireball, Ice Bolt, Life Drain, Lightning Bolt, Psionic Blast */

before G_CMB_SpellEffect__WIP()
¿ 'Mana' is the extra mana passed in ?
...and, some things override this value...
...in G_CMB_SpellEffect__WIP, case scc_Var_Dmg_Spell: just applies the cost and damage

Combat_Cast_Spell__WIP()
    Combat_Spellbook_Build__WIP()
    CMB_ComposeBookBG__WIP()
Combat.C
    Combat_Spellbook_Screen()


CMB_LoadSpellSlider()
CMB_SliderRedraw()
Combat_Spellbook_Mana_Adder_Screen()

Combat.C
Combat_Cast_Spell__WIP()
    |-> Combat_Spellbook_Mana_Adder_Screen()

(spell_data_table[spell_idx].type >= sdt_Infusable_Spell)

player_idx == HUMAN_PLAYER_IDX
&&
_auto_combat_flag == ST_FALSE








Combat_Spellbook_Build__WIP()

CMB_ComposeBookBG__WIP()
CMB_ComposeBackgrnd__WIP()

Combat_Spellbook_Screen()


Combat_Spellbook_Add_Spells_From_Magic_Realm()

Build_Spell_List()

Spellbook_Group_Counts()
Spellbook_Add_Group_Pages(NUM_SPELLS_PER_PAGE_SML)



Spellbook Page Title

    CmbBook_Draw__WIP()
        Print_Centered((x + 70), (y + 6), m_spellbook_pages[SBK_OpenPage].title);

Spellbook Page Entries

    CmbBook_Draw__WIP()
        CmbBook_Compose__WIP(m_spellbook_pages[SBK_OpenPage], IMG_SBK_Anims, caster_idx);
        Draw_Picture((x + 16), (y + 21), IMG_SBK_Anims);
        CmbBook_Compose__WIP(m_spellbook_pages[(SBK_OpenPage + 1)], IMG_SBK_Anims, caster_idx);
        Draw_Picture((x + 148), (y + 21), IMG_SBK_Anims);
