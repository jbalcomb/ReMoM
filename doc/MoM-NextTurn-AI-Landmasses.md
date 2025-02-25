


AI_ContBalances[][]





## AI_ContBalances[][]

indexed by plane and landmass

allocated in Allocate_AI_Data()

populated in ? with ?

Usage:
AI_SetUnitOrders__WIP(itr_players)
    |-> AI_Balance_Upkeep(itr_players)
            if(AI_ContBalances[Unit_Plane][Unit_Landmass] == 0)
                Unit_Value *= 2;

XREF:
    AI_Continent_Eval+4C5      mov     bx, [AI_ContBalances+bx]        ; 120 LBX_NearAlloc_Next bytes
    AI_Balance_Upkeep__WIP+116 mov     bx, [AI_ContBalances+bx]        ; 120 LBX_NearAlloc_Next bytes
    AI_Balance_Upkeep__WIP+29A mov     bx, [AI_ContBalances+bx]        ; 120 LBX_NearAlloc_Next bytes
    sub_F6CAB+6D               mov     bx, [AI_ContBalances+bx]        ; 120 LBX_NearAlloc_Next bytes
    Allocate_AI_Data+D6        mov     [AI_ContBalances], ax           ; 120 LBX_NearAlloc_Next bytes
    Allocate_AI_Data+DC        mov     [AI_ContBalances+2], ax         ; 120 LBX_NearAlloc_Next bytes
