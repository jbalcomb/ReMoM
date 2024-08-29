
MoM - "Grand Vizier"

SEEALSO:  MoM-AI-Building.md


Player_Colony_Autobuild_HP()
// WZD o157p05
// MoO2  Module: AIBUILD  Player_Colony_Autobuild_()  |->  Assign_Colony_New_Building_()


MoM
    Next_Turn_Calc()
        |-> Apply_Colony_Changes()
            |-> City_Apply_Production()
                |-> Player_Colony_Autobuild_HP()
CTY_OutpostGrowth()

MoO2
    Next_Turn_Calc_()
        |-> Apply_All_Colony_Changes_()
            |-> Apply_Colony_Changes_()
                |-> Apply_Colony_Pop_Growth_()
                |-> Apply_Production_()



Next_Turn_Calc()

```c
if(grand_vizier == ST_TRUE)
{
    for(itr_cities = 0; itr_cities < _cities; itr_cities++>)
    {

    }
}
```

Something like 'auto build', ...
But, ...
'Human Player' (HP) vs. 'Computer Player' (CP) vs. 'Neutral Player' (NP)

Per `MoO2`, ...
    Module: AIBUILD
        Player_Colony_Autobuild_()
            |->  Assign_Colony_New_Building_()

Apply_Colony_Changes_()
    |-> Apply_Production_()
        |-> Player_Colony_Autobuild_()
            |->  Assign_Colony_New_Building_()

Assign_Buildings_()
    |-> Assign_Colony_Building_()
        |->  Assign_Colony_New_Building_()

Colony_AI_Given_Tax_()
    |-> Assign_Colony_Building_()
        |->  Assign_Colony_New_Building_()

Next_Turn_Proc_()
    |-> All_Colony_AI_()
        |-> Colony_AI_()
            |-> Assign_Buildings_()
                |-> Assign_Colony_Building_()


¿ data (0 bytes) _colony_auto_building Address: 02:00195EAC ?
XREF:
    Add_To_Queue_+25A           mov dl, byte ptr _colony_auto_building
    Evaluate_Input__COLBLDG+9B  mov dl, byte ptr _colony_auto_building
    Evaluate_Input__COLBLDG+534 mov bl, byte ptr _colony_auto_building
    Evaluate_Input__COLBLDG+5A4 mov dl, byte ptr _colony_auto_building
    Add_Fields___COLBLDG+D1     push offset _colony_auto_building     
    Build_Queue_Popup_+187      mov _colony_auto_building, ax         

...IDK...
...something like, (s_COLONY.IDK_auto_build != ST_FALSE && s_COLONY.product_idx >= -3 && s_COLONY.product_idx <= -1)
...Player_Colony_Autobuild_()

Player_Colony_Autobuild_()
    |-> Colony_Calculation_()
    |-> Assign_Colony_New_Building_()


_CITIES[].construction == bt_GRANDVIZIER  ~== s_COLONY.IDK_auto_build == ST_TRUE



MoO2
Module: COLBLDG
    data (0 bytes) _colony_auto_building
    Address: 02:00195EAC


## grand_vizier
    Save_SAVE_GAM+36C            mov     ax, offset grand_vizier   
    Load_SAVE_GAM+383            mov     ax, offset grand_vizier   
    Advisor_Screen+1F2           cmp     [grand_vizier], e_ST_TRUE 
    GrandVizier_Window+3         cmp     [grand_vizier], e_ST_FALSE
    GrandVizier_Window+19        mov     [grand_vizier], e_ST_TRUE 
    GrandVizier_Window:loc_66BE1 mov     [grand_vizier], e_ST_FALSE
    Next_Turn_Calc+95            cmp     [grand_vizier], e_ST_TRUE 
    City_Apply_Production:loc_C42A4   cmp     [grand_vizier], e_ST_TRUE 
    City_Apply_Production+453         cmp     [grand_vizier], e_ST_TRUE 
    City_Apply_Production+507         cmp     [grand_vizier], e_ST_FALSE
    City_Apply_Production+577         cmp     [grand_vizier], e_ST_TRUE 



## ai_transport_count
used in 'auto build', for both HP and CP
affects the weighted chance of selecting a unit with carrying capacity
every 30 turns for HP, every 40 turns for CP
max 10 for HP, max 8 for CP

XREF:
    AI_OVL_Eval_Assets+1262   mov     [ai_transport_count], 0 
    AI_OVL_Eval_Assets+12A2   inc     [ai_transport_count]    
    AI_OVL_PickSummon+107     cmp     [ai_transport_count], 10
    AI_OVL_PickSummon+117     cmp     ax, [ai_transport_count]
    AI_CTY_SetProduction+688  cmp     ax, [ai_transport_count]
    AI_CTY_SetProduction+68E  cmp     [ai_transport_count], 8 
    AI_CTY_SetProduction+6AB  sub     ax, [ai_transport_count]
    CTY_GrandVizier__STUB+B3  mov     [ai_transport_count], 0 
    CTY_GrandVizier__STUB+1AD inc     [ai_transport_count]    
    CTY_GrandVizier__STUB+5E7 cmp     ax, [ai_transport_count]
    CTY_GrandVizier__STUB+5ED cmp     [ai_transport_count], 10
    CTY_GrandVizier__STUB+60A sub     ax, [ai_transport_count]
