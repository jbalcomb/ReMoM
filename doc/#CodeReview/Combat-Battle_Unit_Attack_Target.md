Combat-Battle_Unit_Attack_Target.md

BU_AttackTarget__WIP()  ==>  Battle_Unit_Attack_Target()

Battle_Unit_Attack__WIP
    |-> Battle_Unit_Moves2

OON XREF:  Battle_Unit_Attack__WIP() |-> BU_AttackTarget__WIP() |-> BU_ProcessAttack__WIP()

Combat_Screen__WIP()
    |-> Battle_Unit_Action()
        |-> Battle_Unit_Attack()
            |-> BU_MeleeWallCheck
            |-> BU_AttackTarget__WIP
                |-> Check_Attack_Ranged
                |-> BU_ProcessAttack__WIP
                |-> Check_Wall_Of_Fire_Attack
                |-> BU_CauseFear__NOOP
        |-> Move_Battle_Unit

---
