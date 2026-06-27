#AI_Target_Pickers_AITP.md



Why AITP_Transmute under AI_Select_Spell_Group_City_Enchantment()?
Why AITP_Transmute under AI_Select_Spell_Group_City_Enchantment() and Get_Map_Square_Target_For_Spell()?



enum e_OVERLAND_SPELL_TARGET_TYPE
enum e_COMBAT_SPELL_TARGET_TYPE
Get_Map_Square_Target_For_Spell()
Pick_Target_For_City_Enchantment__WIP()



1.  AI decides if it should cast a spell and which spell to cast
    AI_Spell_Select() |-> AI_Select_Spell_Group() |-> AI_Select_Spell_Group_Summon(), etc.
2.  AI decides what the target should be
    AITP_Disjunction(), etc.

Cast_Spell_Overland()
    switch(spell_data_table[spell_idx].type)
        case scc_Special_Spell:  switch(spell_idx) case spl_Spell_Binding: |-> Cast_Spell_Binding() |-> AITP_Disjunction()
        case scc_Disjunctions:   |-> AITP_Disjunction()

Cast_Spell_Overland()
    switch(spell_data_table[spell_idx].type)

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
            |-> AI_Spell_Select()
                |-> AI_Compute_Spells_Info()
                |-> AI_Select_Spell_Group()
                |-> AI_Select_Spell_Group_City_Enchantment()
                    |-> AITP_Wall_Of_Stone()
                    |-> AITP_Transmute()
                    |-> AITP_Change_Terrain()
                    |-> AITP_Move_Fortress()
                    |-> AITP_Earth_Gate()
                    |-> AITP_Gaias_Blessing()
                    |-> AITP_Flying_Fortress()
                    |-> AITP_WallofFire()
                    |-> AITP_HeavenlyLight()
                    |-> AITP_StreamofLife()
                    |-> AITP_Inspirations()
                    |-> AITP_Prosperity()
                    |-> AITP_AstralGate()
                    |-> AITP_DarkRituals()
                    |-> AITP_CloudofShadow()
                    |-> AITP_SpellWard__STUB()
                    |-> AITP_Consecration()
                    |-> AITP_WallofDarkness()
                    |-> AITP_AltarofBattle()
