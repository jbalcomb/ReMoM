
Next Turn - Buildings

'Next Turn Proc'
'Next Turn Calc'


## g_bldg_msg_ctr

XREF:
    City_Cancel_Production+1C                        cmp     [g_bldg_msg_ctr], 20
    City_Cancel_Production+23                        mov     al, [g_bldg_msg_ctr]
    City_Cancel_Production+34                        mov     al, [g_bldg_msg_ctr]
    City_Cancel_Production+56                        mov     al, [g_bldg_msg_ctr]
    City_Cancel_Production+5B                        mov     [g_bldg_msg_ctr], al
    Next_Turn_Proc:loc_52B5B                         cmp     [g_bldg_msg_ctr], 0 
    Next_Turn_Proc:loc_52D8A                         mov     al, [g_bldg_msg_ctr]
    Next_Turn_Proc+292                               mov     [g_bldg_msg_ctr], 0 
    City_Delete_Building_Complete_Messages:loc_A229A mov     al, [g_bldg_msg_ctr]
    City_Delete_Building_Complete_Messages+63        mov     al, [g_bldg_msg_ctr]
    City_Delete_Building_Complete_Messages+68        mov     [g_bldg_msg_ctr], al
    City_Delete_Building_Complete_Messages:loc_A22AB mov     al, [g_bldg_msg_ctr]
    End_Of_Combat__WIP:loc_A5C0E                     mov     al, [g_bldg_msg_ctr]
    End_Of_Combat__WIP+C76                           mov     al, [g_bldg_msg_ctr]
    End_Of_Combat__WIP+C7B                           mov     [g_bldg_msg_ctr], al
    End_Of_Combat__WIP:loc_A5C1F                     mov     al, [g_bldg_msg_ctr]
    CTY_ProdProgress+2C8                             cmp     [g_bldg_msg_ctr], 20
    CTY_ProdProgress+2CF                             mov     al, [g_bldg_msg_ctr]
    CTY_ProdProgress+2E0                             mov     al, [g_bldg_msg_ctr]
    CTY_ProdProgress+300                             inc     [g_bldg_msg_ctr]    


### Next_Turn_Proc()
if(g_bldg_msg_ctr > 0)
    for(itr_msg = 0; itr_msg < g_bldg_msg_ctr; itr_msg++)
...
g_bldg_msg_ctr = 0;




### City_Cancel_Production()
if(_CITIES[city_idx].owner_idx == _human_player_idx)
    if(g_bldg_msg_ctr < 20)
        MSG_Building_Complete[g_bldg_msg_ctr].city_idx = city_idx;
        MSG_Building_Complete[g_bldg_msg_ctr].bldg_type_idx = -(_CITIES[city_idx].construction);  // DEDU  negative means?  ("...can no longer produce...")
        g_bldg_msg_ctr++;


City_Delete_Building_Complete_Messages()

End_Of_Combat__WIP()

CTY_ProdProgress()
