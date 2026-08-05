Combat-Init.md

¿ IDK, misg-mash of combat init code and external usage of 'battle unit', and follow-ons ?

CMB_PrepareTurn__WIP
CMB_Units_Init__WIP
Load_Battle_Unit

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr116\BU_Init_Battle_Unit.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr116\BU_Apply_Battlefield_Effects__WIP.asm

~ CMB_Units_Init__WIP()  AKA Prepare_All_Battle_Units()
OON XREF:  Prepare_Battle_Unit_Summons() |-> Prepare_Battle_Unit
...which is only used for scc_Summoning and USA 'Summon Demon'
OON XREF:  Combat_Cast_Spell() |-> Summon_Demon()

CMB_PrepareTurn__WIP
    j_BU_Init_Battle_Unit
CMB_Units_Init__WIP
    j_Load_Battle_Unit
        Load_Battle_Unit
            BU_Init_Battle_Unit

Battle_Unit_Heal
    |-> BU_Init_Battle_Unit
    |-> BU_Apply_Battlefield_Effects__WIP

---
