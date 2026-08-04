Combat-Battle_Unit_Attack_Target.md

BU_AttackTarget__WIP()  ==>  Battle_Unit_Attack_Target()

Battle_Unit_Attack()
    |-> Battle_Unit_Moves2()

OON XREF:  Battle_Unit_Attack() |-> Battle_Unit_Attack_Target() |-> Battle_Unit_Process_Attack()

Combat_Screen__WIP()
    |-> Battle_Unit_Action()
        |-> Battle_Unit_Attack()
            |-> BU_MeleeWallCheck()
            |-> Battle_Unit_Attack_Target()
                |-> Check_Attack_Ranged()
                |-> Battle_Unit_Process_Attack()
                |-> Check_Wall_Of_Fire_Attack()
                |-> BU_CauseFear__NOOP()
        |-> Move_Battle_Unit

---
