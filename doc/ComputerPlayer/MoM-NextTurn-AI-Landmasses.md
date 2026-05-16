
SEEALSO: MoM-AI-Continents.md
SEEALSO: MoM-NextTurn-AI.md



probably just 'AI Turn', not realy 'Next Turn'



_ai_landmass_strength_ratios[][]





## _ai_landmass_strength_ratios[][]

indexed by plane and landmass

allocated in Allocate_AI_Data()

populated in AI_Landmass_Values_And_Strengths()
 * @brief Aggregates per-landmass military strength and strategic value totals for one AI player.
 * Using the city, garrison, and stack valuation data produced by @c AI_Player_Calculate_Target_Values(), ...


Usage:
AI_Set_Unit_Orders(itr_players)
    |-> AI_Balance_Upkeep(itr_players)
            if(_ai_landmass_strength_ratios[Unit_Plane][Unit_Landmass] == 0)
                Unit_Value *= 2;

XREF:
    AI_Continent_Eval+4C5      mov     bx, [_ai_landmass_strength_ratios+bx]        ; 120 LBX_NearAlloc_Next bytes
    AI_Balance_Upkeep__WIP+116 mov     bx, [_ai_landmass_strength_ratios+bx]        ; 120 LBX_NearAlloc_Next bytes
    AI_Balance_Upkeep__WIP+29A mov     bx, [_ai_landmass_strength_ratios+bx]        ; 120 LBX_NearAlloc_Next bytes
    sub_F6CAB+6D               mov     bx, [_ai_landmass_strength_ratios+bx]        ; 120 LBX_NearAlloc_Next bytes
    Allocate_AI_Data+D6        mov     [_ai_landmass_strength_ratios], ax           ; 120 LBX_NearAlloc_Next bytes
    Allocate_AI_Data+DC        mov     [_ai_landmass_strength_ratios+2], ax         ; 120 LBX_NearAlloc_Next bytes
