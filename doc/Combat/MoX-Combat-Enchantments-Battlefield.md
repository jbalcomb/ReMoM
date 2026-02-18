


Battlefield Enchantments
Prayer
    combat_enchantment_index = spell_data_table[spell_idx].ce_idx;
    combat_enchantments[combat_enchantment_index] = ST_TRUE;

#define PRAYER_ATTKR 12
#define PRAYER_DFNDR 13
#define HIGH_PRAYER_ATTKR 14
#define HIGH_PRAYER_DFNDR 15
PRAYER_ATTKR|PRAYER_DFNDR
HIGH_PRAYER_ATTKR|HIGH_PRAYER_DFNDR
PRAYER_ATTKR|PRAYER_DFNDR|HIGH_PRAYER_ATTKR|HIGH_PRAYER_DFNDR

Calc_Battlefield_Bonuses()



CMB_PrepareTurn__WIP()
CMB_CE_Refresh__WIP()
CMB_LoadResources__WIP()
Strategic_Combat_Allocate()
AITP_CombatSpell__STUB()
Cast_Spell_On_Battle_Unit()
Combat_Cast_Spell__WIP()
Do_Legal_Spell_Check__WIP()
Apply_Mana_Leak()
AI_MoveBattleUnits__WIP()
BU_Apply_Battlefield_Effects__WIP()
BU_ProcessAttack__WIP()
Calc_Battlefield_Bonuses()
Combat_Structure()
Cast_Disenchant()
Cast_Dispel_Magic()
AI_EvaluateCmbtSpell()
CMB_LoadResources__WIP()
Strategic_Combat_Allocate()
AITP_CombatSpell__STUB()
Do_Legal_Spell_Check__WIP()
Cast_Disenchant()



CMB_PrepareTurn__WIP+2C                     les     bx, [combat_enchantments]           
CMB_PrepareTurn__WIP:loc_77218              les     bx, [combat_enchantments]           
CMB_PrepareTurn__WIP:loc_7722D              les     bx, [combat_enchantments]           
CMB_PrepareTurn__WIP:loc_77242              les     bx, [combat_enchantments]           
CMB_PrepareTurn__WIP+12D                    les     bx, [combat_enchantments]           
CMB_PrepareTurn__WIP:loc_77327              les     bx, [combat_enchantments]           
CMB_PrepareTurn__WIP:loc_773C7              les     bx, [combat_enchantments]           
CMB_PrepareTurn__WIP:loc_773EA              les     bx, [combat_enchantments]           
CMB_CE_Refresh__WIP:loc_7EB1E               les     bx, [combat_enchantments]           
CMB_CE_Refresh__WIP:loc_7EB39               les     bx, [combat_enchantments]           
CMB_LoadResources__WIP+11C                  mov     [word ptr combat_enchantments], ax  
CMB_LoadResources__WIP:loc_8500A            les     bx, [combat_enchantments]           
Strategic_Combat_Allocate+3B                mov     [word ptr combat_enchantments], ax  
Strategic_Combat_Allocate:loc_888F2         les     bx, [combat_enchantments]           
AITP_CombatSpell__STUB+DED                  add     bx, [word ptr combat_enchantments]  
Cast_Spell_On_Battle_Unit+A25               les     bx, [combat_enchantments]           
Cast_Spell_On_Battle_Unit:loc_8C6C2         les     bx, [combat_enchantments]           
Combat_Cast_Spell__WIP+58F                  les     bx, [combat_enchantments]           
Combat_Cast_Spell__WIP+5B6                  les     bx, [combat_enchantments]           
Combat_Cast_Spell__WIP:loc_8D6F4            les     bx, [combat_enchantments]           
Combat_Cast_Spell__WIP+795                  les     bx, [combat_enchantments]           
Combat_Cast_Spell__WIP+79F                  les     bx, [combat_enchantments]           
Combat_Cast_Spell__WIP+7AC                  les     bx, [combat_enchantments]           
Do_Legal_Spell_Check__WIP+527               add     bx, [word ptr combat_enchantments]  
Apply_Mana_Leak+7                           les     bx, [combat_enchantments]           
Apply_Mana_Leak+16                          les     bx, [combat_enchantments]           
Apply_Mana_Leak:loc_91AA3                   les     bx, [combat_enchantments]           
AI_MoveBattleUnits__WIP+12C                 les     bx, [combat_enchantments]           
AI_MoveBattleUnits__WIP+137                 les     bx, [combat_enchantments]           
AI_MoveBattleUnits__WIP+142                 les     bx, [combat_enchantments]           
BU_Apply_Battlefield_Effects__WIP+345       les     bx, [combat_enchantments]           
BU_Apply_Battlefield_Effects__WIP:loc_99B17 les     bx, [combat_enchantments]           
BU_Apply_Battlefield_Effects__WIP:loc_99BDF les     bx, [combat_enchantments]           
BU_Apply_Battlefield_Effects__WIP:loc_99BF8 les     bx, [combat_enchantments]           
BU_Apply_Battlefield_Effects__WIP:loc_99C5E les     bx, [combat_enchantments]           
BU_Apply_Battlefield_Effects__WIP+4AF       les     bx, [combat_enchantments]           
BU_Apply_Battlefield_Effects__WIP:loc_99D07 les     bx, [combat_enchantments]           
BU_Apply_Battlefield_Effects__WIP+559       les     bx, [combat_enchantments]           
BU_Apply_Battlefield_Effects__WIP:loc_99DA8 les     bx, [combat_enchantments]           
BU_Apply_Battlefield_Effects__WIP+5FA       les     bx, [combat_enchantments]           
BU_Apply_Battlefield_Effects__WIP:loc_99E52 les     bx, [combat_enchantments]           
BU_Apply_Battlefield_Effects__WIP+6A3       les     bx, [combat_enchantments]           
BU_Apply_Battlefield_Effects__WIP+756       les     bx, [combat_enchantments]           
BU_Apply_Battlefield_Effects__WIP:loc_99F28 les     bx, [combat_enchantments]           
BU_Apply_Battlefield_Effects__WIP:loc_99FDB les     bx, [combat_enchantments]           
BU_Apply_Battlefield_Effects__WIP:loc_99FF4 les     bx, [combat_enchantments]           
BU_Apply_Battlefield_Effects__WIP:loc_9A02A les     bx, [combat_enchantments]           
BU_Apply_Battlefield_Effects__WIP+87C       les     bx, [combat_enchantments]           
BU_Apply_Battlefield_Effects__WIP:loc_9A05A les     bx, [combat_enchantments]           
BU_Apply_Battlefield_Effects__WIP:loc_9A073 les     bx, [combat_enchantments]           
BU_ProcessAttack__WIP+865                   les     bx, [combat_enchantments]           
BU_ProcessAttack__WIP+89D                   les     bx, [combat_enchantments]           
Calc_Battlefield_Bonuses+14                 les     bx, [combat_enchantments]           
Calc_Battlefield_Bonuses+29                 les     bx, [combat_enchantments]           
Calc_Battlefield_Bonuses+46                 les     bx, [combat_enchantments]           
Calc_Battlefield_Bonuses+57                 les     bx, [combat_enchantments]           
Calc_Battlefield_Bonuses+68                 les     bx, [combat_enchantments]           
Calc_Battlefield_Bonuses:loc_A3C7E          les     bx, [combat_enchantments]           
Calc_Battlefield_Bonuses+7C                 les     bx, [combat_enchantments]           
Combat_Structure+3B                         les     bx, [combat_enchantments]           
Combat_Structure+59                         les     bx, [combat_enchantments]           
Cast_Disenchant+B5                          add     bx, [word ptr combat_enchantments]  
Cast_Disenchant+1AE                         add     bx, [word ptr combat_enchantments]  
Cast_Dispel_Magic+6B9                       les     bx, [combat_enchantments]           
Cast_Dispel_Magic:loc_B281C                 les     bx, [combat_enchantments]           
AI_EvaluateCmbtSpell+107                    les     bx, [combat_enchantments]           
AI_EvaluateCmbtSpell+112                    les     bx, [combat_enchantments]           
AI_EvaluateCmbtSpell+11D                    les     bx, [combat_enchantments]           
AI_EvaluateCmbtSpell+147                    les     bx, [combat_enchantments]           
AI_EvaluateCmbtSpell+152                    les     bx, [combat_enchantments]           
AI_EvaluateCmbtSpell+15D                    les     bx, [combat_enchantments]           
AI_EvaluateCmbtSpell:loc_C3211              les     bx, [combat_enchantments]           
CMB_LoadResources__WIP+118                  mov     [word ptr combat_enchantments+2], dx
Strategic_Combat_Allocate+37                mov     [word ptr combat_enchantments+2], dx
AITP_CombatSpell__STUB+DE9                  mov     es, [word ptr combat_enchantments+2]
Do_Legal_Spell_Check__WIP+523               mov     es, [word ptr combat_enchantments+2]
Cast_Disenchant+B1                          mov     es, [word ptr combat_enchantments+2]
Cast_Disenchant+1AA                         mov     es, [word ptr combat_enchantments+2]
