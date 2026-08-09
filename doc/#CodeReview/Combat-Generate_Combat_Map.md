Combat-Generate_Combat_Map.md

SEEALSO:  Combat-Combat_Screen_Map_Draw.md
SEEALSO:  Combat-Combat_Figure_Compose.md
SEEALSO:  Combat-Generate_Combat_Map.md
SEEALSO:  Combat-End_Of_Combat.md
SEEALSO:  C:\STU\devel\ReMoM\doc\Combat\MoM-CombatScreen-Grid.md

CMB_TreeGen()       ==>  
CMB_RockGen()       ==>  
CMB_RoadGen()       ==>  
CMB_RiverGen()      ==>  
CMB_MergeDirt()     ==>  
CMB_RemoveRough()   ==>  
CMB_TileGen__WIP()  ==>  

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\Generate_Combat_Map.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\Patch_Terrain_Group.asm
CMB_RiverGen
CMB_RoadGen
CMB_RemoveRough
CMB_MergeDirt
CMB_TileGen__WIP
Set_Movement_Cost_Maps
CMB_TreeGen
CMB_RockGen

Combat_Screen()
    |-> Allocate_Combat_Base_Blocks()
    |-> Combat_Screen_Load_Resources()
    |-> Cache_Graphics_Combat()
    |-> Build_Battlefield()
        |-> Load_Combat_Terrain_Pictures()
        |-> Generate_Combat_Map()
            |-> Patch_Terrain_Group()

        |-> Combat_Compose_Background()

---
