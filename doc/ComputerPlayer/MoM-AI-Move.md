
AIMOVE
ovr158



How does the AI decide what the Computer Player should do with its units?

Per-turn driver (per AI player)
Where AI_Set_Unit_Orders and AI_MoveUnits fit in AI_Next_Turn:
The order is strict: Orders writes Status + dst, then Move reads Status + dst. No interleaving. (See AIDUDES.c:241/284/285/327 for the call sites.)
AI_Next_Turn()
    |-> AI_Set_Unit_Orders()
    |-> AI_MoveUnits()

...roughly...
|-> AI_Evaluate_Hostility(player_idx)
|-> Player_Hostile_Opponents(player_idx)
|-> AI_Player_Calculate_Target_Values(player_idx)
|-> AI_Landmass_Strength_Ratios(player_idx)
|-> AI_Choose_War_Landmass(player_idx)
_players[].peace_duration[] -= 1
|-> AI_Evaluation_Map(player_idx)
|-> AI_Evaluate_Continents(player_idx)
|-> AI_Set_Unit_Orders(player_idx)                      <- this does ALL of the actual order setting
|-> EMMDATAH_Map()
|-> AI_Kill_Excess_Settlers_And_Engineers(player_idx)



NOTE: "Garrison" gets mixed up later/at some point, when it comes Site, not just City, and even Site vs. Fortress City

City Defense Garrion
Node Defense Garrison
Attack Enemy City
Attack Enemy Stack
Merge Own Stack
Build Road
Settle Outpost
Purify Corruption
Meld Node





MoO2
Move_All_AI_()
    |-> Move_AI_()
    |-> Move_NPCs_()

Move_AI_()
    |-> Get_Colonizable_Planets_(&_colonizable[0], &_n_colonizable)
    _nonplayer_here[star_idx] = monster ship's owner_idx
    ...
    |-> Assign_New_Colony_Ship_Destinations_()
    ...

Gather_Player_Ships_At_Star_()





Next_Turn_Proc() |-> Next_Turn_Calc() |-> AI_Next_Turn()
Per Computer Player  {1, ..., _num_players{2,3,4,5}}
AI_Next_Turn()
    |-> AI_Evaluation_Map()                     ...populates g_ai_evaluation_map
    |-> AI_Evaluate_Continents()
    |-> AI_Set_Unit_Orders()
        |-> AI_Disband_To_Balance_Budget()
        |-> AI_Shift_Off_Home_Plane()           ...uses struct s_AI_STACK_DATA * _ai_all_own_stacks
        |-> AI_Move_Out_Boats()                 ...uses struct s_AI_STACK_DATA * _ai_all_own_stacks
            |-> AI_Stack_Set_Boats_Goto()       ...uses struct s_AI_STACK_DATA * _ai_all_own_stacks
        |-> AI_Find_Opportunity_City_Target()   ...uses struct s_AI_STACK_DATA * _ai_all_own_stacks
            Per Plane:
                Per Landmass:
                |-> AI_Stacks_Init_Build_Target_Order   ...populates _ai_own_stack_count, etc.
                    |-> AI_Stacks_Target_Nearest_Hostile_Stack()
                    |-> AI_Stacks_Order_Attack_Target_Or_Goto_Destination()
                |-> AI_Stacks_Move_Out_NonMilitary_Garrisoned()
                |-> AI_Stacks_Survey_Expedition_Forces()
                    |-> AI_Stacks_Survey_Expedition_Forces_Stack()
                |-> AI_Stacks_Do_Meld()
                |-> AI_Stacks_Do_Settle()
                |-> AI_Do_Purify()
                |-> AI_Do_RoadBuild()
                |-> AI_Build_Target_List()      ...populates _ai_targets_count, etc.
                    |-> AI_Add_Target()
                |-> AI_Stacks_Roamers_Target_Or_Deploy()
                    |-> AI_Stacks_Assign_Target()
                ...depends on landmass type...
                |-> AI_Stacks_Order_To_War_Landmass()       ...uses _ai_own_stack_count, etc.
                |-> AI_Stacks_Relocate_Roamers()            ...uses _ai_own_stack_count, etc.
                    |-> AI_Stacks_Setup_Ferry()             OON XREF
                        |-> AI_Stacks_Ferry_Add_Location()  ...uses _ai_ferry_count, etc.
                            |-> AI_Stacks_Order_Attack_Target_Or_Goto_Destination()
                        |-> AI_Stacks_Order_Ferry()         ...uses _ai_own_stack_count, etc.
                |-> AI_Stacks_Stage_Expedition_Forces()
                    |-> AI_Reevaluate_Continent()
                    |-> AI_Stacks_Order_Attack_Target_Or_Goto_Destination()
                |-> AI_Stacks_Garrison_Sites()              ...uses _ai_own_stack_count, etc.
                    |-> AI_Stacks_Order_Attack_Target_Or_Goto_Destination()
            ...Ocean (non-landmass)...
            |-> AI_Stacks_Wartime_Ocean_Movement_And_Cleanup()                      ...kinda uses _ai_own_stack
            |-> AI_Stacks_Ocean_Landmass_Orders()

Definitely Done-Done:
[ ] Next_Turn_Proc()
[ ] |-> Next_Turn_Calc()
[ ]     |-> AI_Next_Turn()
...
/*
    BEGIN:  'AI Turn - Orders Phase'
*/
[x]         |-> AI_Evaluate_Hostility()
[x]         |-> AI_Evaluate_Magic_Power_Strategy()          ... AI_Update_Magic_Power()
[x]         |-> Player_Hostile_Opponents()
[x]         |-> AI_Player_Calculate_Target_Values()
[x]         |-> AI_Landmass_Values_And_Strengths()
[x]         |-> AI_Choose_War_Landmass()                    ¿ uses g_ai_evaluation_map from previous turn ?
...
...j_Cast_Spell_Overland__WIP(); EMMDATAH_Map(); Allocate_AI_Data(); Player_Hostile_Opponents(); AI_Player_Calculate_Target_Values(); j_AI_Spell_Select();
...EMMDATAH_Map(); AI_Update_Magic_Power(); AI_Sanity_Check_Overland_Enchantments(); AI_Update_Gold_And_Mana_Reserves(); AI_Update_Gold_Income_And_Food_Income();
...
[]          |-> Player_All_Colony_Autobuild(player_idx)
[x]         |-> AI_Evaluation_Map(player_idx)               populates g_ai_evaluation_map
[x]         |-> AI_Evaluate_Continents(player_idx)          uses g_ai_evaluation_map, just populated in AI_Evaluation_Map()
                ...sibling function... [x] OON XREF: AI_Set_Unit_Orders() |-> AI_Stacks_Stage_Expedition_Forces() |-> AI_Reevaluate_Continent()
[ ]         |-> AI_Set_Unit_Orders(player_idx)
[ ]             |-> AI_Disband_To_Balance_Budget()
[ ]             |-> AI_Shift_Off_Home_Plane()
[ ]             |-> AI_Move_Out_Boats()
[ ]             |-> AI_Find_Opportunity_City_Target()
[x]             |-> AI_Stacks_Init_Build_Target_Order()
[x]                 |-> AI_Stacks_Target_Nearest_Hostile_Stack()
[x]                 |-> AI_Stacks_Order_Attack_Target_Or_Goto_Destination()
[x]             |-> AI_Stacks_Move_Out_NonMilitary_Garrisoned()
[x]             |-> AI_Stacks_Survey_Expedition_Forces()
[x]                 |-> AI_Stacks_Survey_Expedition_Forces_Stack()
[ ]             |-> AI_Stacks_Do_Meld()
[ ]             |-> AI_Stacks_Do_Settle()
[x]                 |-> AI_Stacks_Reorder_Settle_Elsewhere()
[x]                     |-> Next_Nearest_Ferry_Square()
[x]                     |-> Adjacent_Land_Square()
[x]                         |-> Square_Is_Land()
[ ]             |-> AI_Do_Purify()
[ ]             |-> AI_Do_RoadBuild()
[x]             |-> AI_Build_Target_List()
[x]                 |-> AI_Add_Target()
[x]             |-> AI_Stacks_Roamers_Target_Or_Deploy()
[x]                 |-> AI_Stacks_Assign_Target()
[x]             |-> AI_Stacks_Order_To_War_Landmass()
[x]             |-> AI_Stacks_Relocate_Roamers()
[x]                 |-> AI_Stacks_Setup_Ferry()
[x]                     |-> AI_Stacks_Ferry_Add_Location()
[x]                         |-> AI_Stacks_Order_Attack_Target_Or_Goto_Destination()
[x]                     |-> AI_Stacks_Order_Ferry()
[x]             |-> AI_Stacks_Stage_Expedition_Forces()
[x]                 |-> AI_Reevaluate_Continent()
[x]                 |-> AI_Stacks_Order_Attack_Target_Or_Goto_Destination()
[x]             |-> AI_Stacks_Garrison_Sites()
[x]                 |-> AI_Stacks_Order_Attack_Target_Or_Goto_Destination()
[x]             |-> AI_Stacks_Wartime_Ocean_Movement_And_Cleanup()    ...calls AI_Stacks_Init_Build_Target_Order(player_idx, 0, wp);
[x]                 |-> AI_Stacks_Init_Build_Target_Order()
[x]             |-> AI_Stacks_Ocean_Landmass_Orders()                       ...calls AI_Stacks_Init_Build_Target_Order(player_idx, 0, wp);
[x]                 |-> AI_Stacks_Init_Build_Target_Order()
[ ]                 |-> AI_Stacks_Do_Meld()
[x]                 |-> AI_Stacks_Order_Attack_Target_Or_Goto_Destination()
[ ]                 |-> AI_Enemy_Unit_In_Range()
[x]                 |-> AI_Stacks_Order_Ferry()
[x]                 |-> AI_Stacks_Order_Attack_Target_Or_Goto_Destination()
/*
    END:  'AI Turn - Orders Phase'
*/
