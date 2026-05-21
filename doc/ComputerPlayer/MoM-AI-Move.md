
AIMOVE
ovr158

How does the AI decide what the Computer Player should do with its units?

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
    |-> AI_Evaluation_Map()
    |-> AI_Evaluate_Continents()
    |-> AI_Set_Unit_Orders()
        |-> AI_Disband_To_Balance_Budget()
        |-> AI_Shift_Off_Home_Plane()
        |-> AI_Move_Out_Boats()
        |-> AI_Find_Opportunity_City_Target()
            Per Plane:
                Per Landmass:
                |-> AI_Build_Stacks_Find_Targets_Order_Moves()
                |-> AI_GarrBuilderPush__WIP()
                |-> AI_Survey_Excess_Units()
                |-> AI_Do_Meld()
                |-> AI_Do_Settle()
                |-> AI_Do_Purify()
                |-> AI_Do_RoadBuild()
                |-> AI_Build_Target_List()
                |-> AI_ProcessRoamers__WIP()
                ...depends on landmass type...
                |-> AI_PullForMainWar__WIP()
                |-> G_AI_HomeRallyFill__WIP()
                |-> G_AI_RallyFill__WIP()
                    |-> AI_Reevaluate_Continent()
                |-> AI_FillGarrisons__WIP()
            ...Ocean (non-landmass)...
            |-> AI_ProcessOcean__WIP()
            |-> G_AI_ProcessTransports__WIP()

Definitely Done-Done:
[ ] Next_Turn_Proc()
[ ] |-> Next_Turn_Calc()
[ ]     |-> AI_Next_Turn()
...
[x]         |-> AI_Evaluate_Hostility()
[x]         |-> AI_Evaluate_Magic_Power_Strategy()  ... AI_Update_Magic_Power()
[x]         |-> Player_Hostile_Opponents()
[x]         |-> AI_Player_Calculate_Target_Values()
[x]         |-> AI_Landmass_Values_And_Strengths()
[x]         |-> AI_Choose_War_Landmass()            ¿ uses g_ai_evaluation_map from previous turn ?
...
...j_Cast_Spell_Overland__WIP(); EMMDATAH_Map(); Allocate_AI_Data(); Player_Hostile_Opponents(); AI_Player_Calculate_Target_Values(); j_AI_Spell_Select();
...EMMDATAH_Map(); AI_Update_Magic_Power(); AI_Sanity_Check_Overland_Enchantments(); AI_Update_Gold_And_Mana_Reserves(); AI_Update_Gold_Income_And_Food_Income();
...
[]          |-> Player_All_Colony_Autobuild(player_idx)
[x]         |-> AI_Evaluation_Map(player_idx)               populates g_ai_evaluation_map
[x]         |-> AI_Evaluate_Continents(player_idx)      uses g_ai_evaluation_map, just populated in AI_Evaluation_Map()
                ...sibling function... [x] OON XREF: AI_Set_Unit_Orders() |-> G_AI_RallyFill__WIP() |-> AI_Reevaluate_Continent()
[]          |-> AI_Set_Unit_Orders(player_idx)
[]              |-> AI_Disband_To_Balance_Budget()
[]              |-> AI_Shift_Off_Home_Plane()
[]              |-> AI_Move_Out_Boats()
[]              |-> AI_Find_Opportunity_City_Target()
[]              |-> AI_Build_Stacks_Find_Targets_Order_Moves()
[]              |-> AI_GarrBuilderPush__WIP()
[]              |-> AI_Survey_Excess_Units()
[]              |-> AI_Do_Meld()
[]              |-> AI_Do_Settle()
[]              |-> AI_Do_Purify()
[]              |-> AI_Do_RoadBuild()
[x]             |-> AI_Build_Target_List()
[x]                 |-> AI_Add_Target()
[]              |-> AI_ProcessRoamers__WIP()
[ ]                 |-> AI_AssignStackTarget__WIP()
[]              |-> AI_PullForMainWar__WIP()
[]              |-> G_AI_HomeRallyFill__WIP()
[]              |-> G_AI_RallyFill__WIP()
[x]                 |-> AI_Reevaluate_Continent()
[]              |-> AI_FillGarrisons__WIP()
[]              |-> AI_ProcessOcean__WIP()
[]              |-> G_AI_ProcessTransports__WIP()

## Call graph
```
AI_Set_Unit_Orders(player_idx)
├── EMM_Map_CONTXXX__WIP                     [setup]
├── AI_Disband_To_Balance_Budget             [global pre-pass]
├── AI_Shift_Off_Home_Plane                  [global pre-pass]
├── AI_Move_Out_Boats                        [global pre-pass]
├── AI_Find_Opportunity_City_Target          [global pre-pass; OGBUG: wp=0 stale]
└── for wp in [Arcanus, Myrror]:
    └── for landmass_idx in [1..NUM_LANDMASSES):
        ├── AI_Build_Stacks_Find_Targets_Order_Moves
        ├── AI_GarrBuilderPush__WIP
        ├── AI_Survey_Excess_Units
        ├── AI_Do_Meld
        ├── AI_Do_Settle
        ├── AI_Do_Purify
        ├── AI_Do_RoadBuild
        ├── AI_Build_Target_List
        ├── AI_ProcessRoamers__WIP
        ├── AI_PullForMainWar__WIP           [gate: NOT lmt_Contested basically]
        ├── G_AI_HomeRallyFill__WIP          [gate: lmt_Own / lmt_Abandon+]
        ├── G_AI_RallyFill__WIP              [always]
        │   └── AI_Reevaluate_Continent     [5% roll when stage is full]
        └── AI_FillGarrisons__WIP            [gate: lmt_Own / lmt_Contested / lmt_Abandon+]
    ├── AI_ProcessOcean__WIP                 [per-plane post-pass]
    └── G_AI_ProcessTransports__WIP          [per-plane post-pass]
└── EMM_Map_DataH                            [cleanup]
```

[]          |-> EMMDATAH_Map();
[]          |-> AI_Kill_Excess_Settlers_And_Engineers(player_idx)

[x] c:/python314/python.exe .\rasm_named_gemini.py AI_SetEnemyStrMaps__WIP
[x] c:/python314/python.exe .\rasm_named_gemini.py AI_SetUnitOrders__WIP
[] c:/python314/python.exe .\rasm_named_gemini.py AI_Kill_Excess_Settlers_And_Engineers

[] c:/python314/python.exe .\rasm_named_gemini.py WIZ_NextIdleStack

...
    |-> AI_Evaluation_Map()             ovr162
    |-> AI_Evaluate_Continents()      ovr162



cp_landmass_wx_array = &_ai_continents.plane[wp].player[player_idx].wx_array[0];
cp_landmass_wy_array = &_ai_continents.plane[wp].player[player_idx].wy_array[0];
cp_landmass_type_array = &_ai_continents.plane[wp].player[player_idx].type_array[0];

_ai_continents__0
_ai_continents__1
    AI_Reevaluate_Continent()
    AI_Evaluate_Continents()
    AI_Set_Unit_Orders+A7              add     dx, offset _ai_continents__0    ; _bdata_start                     
    AI_ProcessOcean__WIP+4A            mov     al, [byte ptr _ai_continents__0.Player_0.wx_array+bx]; _bdata_start
    AI_ProcessOcean__WIP+13A           mov     al, [byte ptr _ai_continents__0.Player_0.wx_array+bx]; _bdata_start
    AI_ProcessOcean__WIP+175           mov     al, [byte ptr _ai_continents__0.Player_0.wx_array+bx]; _bdata_start
    AI_ProcessRoamers__WIP+2EA         mov     [byte ptr _ai_continents__0.Player_0.wx_array+bx], al; _bdata_start
    G_AI_HomeRallyFill__WIP+1F         mov     al, [byte ptr _ai_continents__0.Player_0.wx_array+bx]; _bdata_start
    AI_PullForMainWar__WIP+154         mov     al, [byte ptr _ai_continents__0.Player_0.wx_array+bx]; _bdata_start

## _ai_landmass_war_targets[]
    Allocate_Data_Space()
    AI_ProcessOcean__WIP()
    G_AI_ProcessTransports__WIP()
    AI_ProcessRoamers__WIP()
    AI_PullForMainWar__WIP()
    AI_Reevaluate_Continent()
    AI_Evaluate_Continents()
    AI_Choose_War_Landmass

set in 1 function, at 2 sites
    if (Value_Sum == 0) {
        _ai_landmass_war_targets[wp][player_idx] = 0;
    } else {
        _ai_landmass_war_targets[wp][player_idx] = Get_Weighted_Choice(&Continent_Values_2[0], NUM_LANDMASSES);
    }



ai_human_hostility
    AI_Set_Unit_Orders()

g_ai_minattackstack
    AI_Set_Unit_Orders()
    G_AI_RallyFill__WIP()

ai_seektransport_cnt
    AI_Set_Unit_Orders()
    G_AI_RallyFill__WIP()
    ...
    G_AI_ProcessTransports__WIP()
    AI_SendToColonize__WIP()
    AI_SeekTransportFrom__WIP()




AI_Set_Unit_Orders
    |-> AI_Survey_Excess_Units()
        |-> AI_Survey_Excess_Units_In_Stack()
    |-> G_AI_RallyFill__WIP()



cleared in AI_Survey_Excess_Units()
populated in AI_Survey_Excess_Units_In_Stack()
    G_Seafaring_Count
    G_Seafaring_Lowest_Value
populated in AI_Survey_Excess_Units_In_Stack()
    G_Seafaring_Values[MAX_STACK]
    G_Seafaring_CX_IDs[MAX_STACK]
    G_Seafaring_UL_Indices[MAX_STACK]
    G_Seafaring_Unit_Indices[MAX_STACK]

cleared in AI_Survey_Excess_Units()
populated in AI_Survey_Excess_Units_In_Stack()
    cp_drafted_unit_count
    G_Pushout_Lowest_Value
populated in AI_Survey_Excess_Units_In_Stack()
    G_Pushout_Values[MAX_STACK]
    G_Pushout_CX_IDs[MAX_STACK]
    G_Pushout_UL_Indices[MAX_STACK]
used in G_AI_RallyFill__WIP()
    cp_drafted_unit_count
    G_Pushout_CX_IDs[MAX_STACK]
    G_Pushout_UL_Indices[MAX_STACK]
    G_Pushout_Unit_Indices[MAX_STACK]

## cp_staged_unit_count
    set in AI_Build_Stacks_Find_Targets_Order_Moves
    used in G_AI_RallyFill__WIP
    ...something like...
    ...if staging area already staffed
    ...if staged and routed enough to staff the staging area
    ...if drafted and staged and routed enough to staff the staging area

¿ MoO2 Move_To_Stage_Points_() ?

mov     ax, [cp_staged_unit_count]
add     ax, [cp_enroute_unit_countt]
add     ax, [cp_drafted_unit_count]
cmp     ax, [g_ai_minattackstack]       ; turn / 30 + 2, max 9
jge     short loc_EC03C



Down r G_AI_RallyFill__WIP+B                       cmp [cp_staged_unit_count], e_MAX_STACK
Down r G_AI_RallyFill__WIP:loc_EBFDD               mov ax, [cp_staged_unit_count]         
Down r G_AI_RallyFill__WIP+7C                      mov ax, [cp_staged_unit_count]         
Down w AI_Build_Stacks_Find_Targets_Order_Moves+14 mov [cp_staged_unit_count], 0          
Down w AI_Build_Stacks_Find_Targets_Order_Moves+D8 inc [cp_staged_unit_count]             

Down r G_AI_RallyFill__WIP+33                       add ax, [cp_enroute_unit_countt]
Down r G_AI_RallyFill__WIP+7F                       add ax, [cp_enroute_unit_countt]
Down r G_AI_RallyFill__WIP+92                       sub ax, [cp_enroute_unit_countt]
Down w AI_Build_Stacks_Find_Targets_Order_Moves+E   mov [cp_enroute_unit_countt], 0 
Down w AI_Build_Stacks_Find_Targets_Order_Moves+203 inc [cp_enroute_unit_countt]    
Down w AI_Build_Stacks_Find_Targets_Order_Moves+339 inc [cp_enroute_unit_countt]    

Down w AI_Set_Unit_Orders+5E mov [g_ai_minattackstack], ax       ; turn / 30 + 2, max 9  
Down r AI_Set_Unit_Orders+61 cmp [g_ai_minattackstack], e_MAX_STACK; turn / 30 + 2, max 9
Down w AI_Set_Unit_Orders+68 mov [g_ai_minattackstack], e_MAX_STACK; turn / 30 + 2, max 9
Down r G_AI_RallyFill__WIP+37   cmp ax, [g_ai_minattackstack]       ; turn / 30 + 2, max 9  
Down r G_AI_RallyFill__WIP+87   cmp ax, [g_ai_minattackstack]       ; turn / 30 + 2, max 9  


## AI_Set_Move_Or_Goto_Target()

Up   J NX_j_AI_Set_Move_Or_Goto_Target              jmp     AI_Set_Move_Or_Goto_Target         
     p G_AI_RallyFill__WIP+DD                       call    near ptr AI_Set_Move_Or_Goto_Target
Down p AI_FillGarrisons__WIP+76A                    call    near ptr AI_Set_Move_Or_Goto_Target
Down p G_AI_ProcessTransports__WIP+646              call    near ptr AI_Set_Move_Or_Goto_Target
Down p G_AI_ProcessTransports__WIP+75B              call    near ptr AI_Set_Move_Or_Goto_Target
Down p G_AI_ProcessTransports__WIP+915              call    near ptr AI_Set_Move_Or_Goto_Target
Down p G_AI_ProcessTransports__WIP+9FE              call    near ptr AI_Set_Move_Or_Goto_Target
Down p AI_ProcessRoamers__WIP:loc_ED7E1             call    near ptr AI_Set_Move_Or_Goto_Target
Down p AI_PullForMainWar__WIP+15F                   call    near ptr AI_Set_Move_Or_Goto_Target
Down p G_AI_RallyOrFerry__WIP+E1                    call    near ptr AI_Set_Move_Or_Goto_Target
Down p G_AI_RallyOrFerry__WIP+341                   call    near ptr AI_Set_Move_Or_Goto_Target
Down p AI_Build_Stacks_Find_Targets_Order_Moves+3FD call    near ptr AI_Set_Move_Or_Goto_Target
Down p AI_GarrBuilderPush__WIP+C9                   call    near ptr AI_Set_Move_Or_Goto_Target
Down p AI_GarrBuilderPush__WIP+1C0                  call    near ptr AI_Set_Move_Or_Goto_Target
Down p AI_Do_Meld+29E                               call    near ptr AI_Set_Move_Or_Goto_Target
Down p AI_Do_Settle+1DF                             call    near ptr AI_Set_Move_Or_Goto_Target
Down p AI_Do_Settle+39B                             call    near ptr AI_Set_Move_Or_Goto_Target
Down p AI_Do_Purify+1E4                             call    near ptr AI_Set_Move_Or_Goto_Target
Down p AI_Do_RoadBuild+1D7                          call    near ptr AI_Set_Move_Or_Goto_Target
Down p AI_SendToColonize__WIP+E4                    call    near ptr AI_Set_Move_Or_Goto_Target
Down p AI_SendToColonize__WIP+1A3                   call    near ptr AI_Set_Move_Or_Goto_Target
Down p AI_SendToColonize__WIP+298                   call    near ptr AI_Set_Move_Or_Goto_Target





// WZD dseg:D3EC                                                 BEGIN:  ovr158 - Uninitialized Data

// WZD dseg:D3EC
uint8_t * cp_landmass_type_array;
// WZD dseg:D3EE
uint8_t * cp_landmass_wy_array;
// WZD dseg:D3F0
uint8_t * cp_landmass_wx_array;

// WZD dseg:D3F2
int16_t G_Seafaring_Lowest_Value;
// WZD dseg:D3F4
int16_t G_Pushout_Lowest_Value;
// WZD dseg:D3F6
/*
cleared in AI_Survey_Excess_Units()
populated in AI_Survey_Excess_Units_In_Stack()
*/
int16_t G_Seafaring_Count;
// WZD dseg:D3F8
int16_t cp_drafted_unit_count;
// WZD dseg:D3FA
int16_t G_Seafaring_Values[MAX_STACK];
// WZD dseg:D40C
int16_t G_Pushout_Values[MAX_STACK];
// WZD dseg:D41E
int16_t G_Seafaring_CX_IDs[MAX_STACK];
// WZD dseg:D430
int16_t G_Pushout_CX_IDs[MAX_STACK];
// WZD dseg:D442
int16_t G_Seafaring_UL_Indices[MAX_STACK];
// WZD dseg:D454
int16_t G_Pushout_UL_Indices[MAX_STACK];
// WZD dseg:D466
int16_t G_Seafaring_Unit_Indices[MAX_STACK];

// WZD dseg:D478
/*
AI_Set_Unit_Orders
    |-> AI_Survey_Excess_Units()
        |-> AI_Survey_Excess_Units_In_Stack()
    |-> G_AI_RallyFill__WIP()

*/
int16_t G_Pushout_Unit_Indices[MAX_STACK];

// WZD dseg:D48A
int16_t cp_staged_unit_count;
// WZD dseg:D48C
int16_t cp_enroute_unit_countt;

// WZD dseg:D48E
int16_t g_ai_minattackstack;

// WZD dseg:D490
int16_t UU_AI_TargetingVar;

// WZD dseg:D492 00 00                                           dw 0

// WZD dseg:D492                                                 END:  ovr158 - Uninitialized Data
