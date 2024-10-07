
Next Turn - MoX/MoM

'Next Turn Proc'
'Next Turn Calc'



Remove *killed* Units
Remove/Replaced Buildings
Heal Units
Count Volcanoes
Grand Vizier

¿ processes required due to changes in `_units` ?

¿ processes required due to changes in terrain ?



20241007
Where is the update to casting_cost_remaining?




## Next_Turn_Proc()
    Delete_Dead_Units();
    All_Units_In_Towers();
    Set_Unit_Draw_Priority();
    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
    Reset_Map_Draw();
    Next_Turn_Calc();
    GFX_Swap_Cities();
    Delete_Dead_Units();
    All_Units_In_Towers();
    Chancellor_Screen();  /* "End of Turn Summary" */
    if g_bldg_msg_ctr > 0
        o62p01_empty_function(_human_player_idx);
    current_screen = scr_Main_Screen;
    g_bldg_msg_ctr = 0;
    Update_Units_MvsSts();
    o62p01_empty_function(_human_player_idx);
    o59p10_empty_function();
    Cast_Spell_Overland__WIP(_human_player_idx);
    all_units_moved = ST_FALSE;
    WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);
    Reset_Draw_Active_Stack();


Delete_Dead_Units();  All_Units_In_Towers();
Set_Unit_Draw_Priority();  Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);  Reset_Map_Draw();
*** Next_Turn_Calc(); ***
*** GFX_Swap_Cities(); ***
Delete_Dead_Units();  All_Units_In_Towers();
*** Chancellor_Screen();  /* "End of Turn Summary" */ ***
if g_bldg_msg_ctr > 0  g_bldg_msg_ctr = 0;
current_screen = scr_Main_Screen;
*** Update_Units_MvsSts(); ***
*** Cast_Spell_Overland__WIP(_human_player_idx); ***
all_units_moved = ST_FALSE;  WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);  Reset_Draw_Active_Stack();


## Next_Turn_Calc()

Set_Random_Seed(RNG_AI_Turn_Seed);
Set_Mouse_List(1, mouse_list_hourglass);




All_Players_Apply_Spell_Casting();
Delete_Dead_Units();
Set_Unit_Draw_Priority();
Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
All_City_Removed_Buildings();
Do_All_Units_XP_Check();
Heal_All_Units();
Record_History();
Increment_Background_Music();
_turn++;
...cap gold, mana 30000...
OVL_EnableIncmBlink();
Do_Autosave();
All_City_Calculations();
RNG_AI_Turn_Seed = Get_Random_Seed();
Set_Mouse_List(1, mouse_list_default);







## City Production

`Human Player (HP)`
    Next_Turn_Calc()
        |-> Apply_Colony_Changes()
            |-> City_Apply_Production()
                |-> Player_Colony_Autobuild_HP()

`Computer Player (HP)`
    Next_Turn_Calc()
        |-> AI_Overland_Turn()
            |-> All_Player_Colony_Autobuild()
                |-> Player_Colony_Autobuild_CP()

`Neutral Player (NP)`
    Next_Turn_Calc()
        |-> AI_Overland_Turn()
            |-> All_Player_Colony_Autobuild()
                |-> Player_Colony_Autobuild_NP()

### Apply_City_Changes()
AKA CTY_ProgressTurn()
    handles graduating or destroying 'Outpost'
    applies population growth
    handles grow or shrink message
    City_Apply_Production()
    applies CONSECRATION, STREAM_OF_LIFE, CHAOS_RIFT, GAIAS_BLESSING, NIGHTSHADE
    TILE_CountVolcanoes()

### City_Apply_Production()
AKA CTY_ProdProgress()
    (only) handles the completion of a Unit or Building
    if Prod_Accu >= product_cost
        _CITIES[city_idx].bldg_status[_CITIES[city_idx].construction] = bs_Built;
            but, also, maybe _CITIES[city_idx].bldg_status[bldg_data_table[_CITIES[city_idx].construction].replace_bldg] = bs_Replaced;
    if bt_Oracle, TILE_ExploreRadius__WIP()



### g_bldg_msg_ctr

Next_Turn_Proc() iters over g_bldg_msg_ctr()
    if(MSG_Building_Complete[itr].city_idx != ST_UNDEFINED_DB)
        

XREF:
    City_Apply_Production+2C8                             cmp     [g_bldg_msg_ctr], 20
    City_Apply_Production+2CF                             mov     al, [g_bldg_msg_ctr]
    City_Apply_Production+2E0                             mov     al, [g_bldg_msg_ctr]
    City_Apply_Production+300                             inc     [g_bldg_msg_ctr]    
    City_Cancel_Production+1C                        cmp     [g_bldg_msg_ctr], 20
    City_Cancel_Production+23                        mov     al, [g_bldg_msg_ctr]
    City_Cancel_Production+34                        mov     al, [g_bldg_msg_ctr]
    City_Cancel_Production+56                        mov     al, [g_bldg_msg_ctr]
    City_Cancel_Production+5B                        mov     [g_bldg_msg_ctr], al
    City_Delete_Building_Complete_Messages+63        mov     al, [g_bldg_msg_ctr]
    City_Delete_Building_Complete_Messages+68        mov     [g_bldg_msg_ctr], al
    City_Delete_Building_Complete_Messages:loc_A229A mov     al, [g_bldg_msg_ctr]
    City_Delete_Building_Complete_Messages:loc_A22AB mov     al, [g_bldg_msg_ctr]
    End_Of_Combat__WIP+C76                           mov     al, [g_bldg_msg_ctr]
    End_Of_Combat__WIP+C7B                           mov     [g_bldg_msg_ctr], al
    End_Of_Combat__WIP:loc_A5C0E                     mov     al, [g_bldg_msg_ctr]
    End_Of_Combat__WIP:loc_A5C1F                     mov     al, [g_bldg_msg_ctr]
    Next_Turn_Proc+292                               mov     [g_bldg_msg_ctr], 0 
    Next_Turn_Proc:loc_52B5B                         cmp     [g_bldg_msg_ctr], 0 
    Next_Turn_Proc:loc_52D8A                         mov     al, [g_bldg_msg_ctr]






### `GAME_InTowerRefresh()`
; clears and resets the in-tower flag for all units
; based on their current circumstances





## Gold, Mana, Food

### Income - Gold, Mana, Food

### Upkeep - Gold, Mana, Food











move these to the IDA documentation...
Jump to address
ovr120:1133
Nay "000506FE", for "Main_Screen+B9E"
Nay "00043ABE", for "Main_Screen+B9E"
Yay "Main_Screen+B9E"






### Reset Movement Points

// WZD o60p06
// ¿ AI version ?
void Update_Units_MvsSts(void)

Next_Turn_Proc()
    Next_Turn_Proc+299
    |-> Update_Units_MvsSts()



## Unit Status Changes:

In Main_Screen(), 'Right-Click - Movement Map - Select Stack'
    Select_Unit_Stack(_human_player_idx, &_map_x, &_map_y, _map_plane, selected_unit_x, selected_unit_y)
            Build_Unit_Stack(_human_player_idx, _map_plane, selected_unit_x, selected_unit_y)
                checks HMoves & Finished to set active to ST_TRUE
                    and if status is 'Wait', changes status to 'No Orders'

Update_Units_MvsSts()
    iterates over ALL _units
        sets Finished to ST_FALSE, by default
        sets Finished to S_TRUE, if 'Patrol', 'Build Road', 'Casting'
        sets Status  to 'No Orders', if 'Wait' or 'Done






## Next Turn vs. Init/New/Load


WZD ovr101
WZD o101p01  [1 of 1]
GAME_NextHumanStack()
XREF:
j_GAME_NextHumanStack()
j_GAME_NextHumanStack()
XREF:
Loaded_Game_Update_WZD+6E call    j_GAME_NextHumanStack           ; a wrapper for WIZ_NextIdleStack hard-coded for the human-player
Loaded_Game_Update+4D     call    j_GAME_NextHumanStack           ; a wrapper for WIZ_NextIdleStack hard-coded for the human-player






all_units_moved
    controls both displaying the next turn button and adding the next turn button input field
set by WIZ_NextIdleStack()
based on return value from WIZ_NextUnit









MoX.H
scr_NextTurn = 110,  /* ¿ drake187: NextTurnProc ? ¿ MoO2: Do_Next_Turn_() ? */





C7 06 90 99 0A 00                               mov     [current_screen], scr_Next_Turn_Proc

Main_Screen+B9E                      mov     [current_screen], scr_Next_Turn_Proc                   
Main_Screen+B6E                      mov     [current_screen], scr_Next_Turn_Proc                   
Main_Screen+12A8                     mov     [current_screen], scr_Next_Turn_Proc                   



WZD dseg:C050 00 00                                           _next_turn_button dw 0                  ; DATA XREF: Main_Screen:loc_50610r ...

Main_Screen:@@Check_Input_NextTurnButton cmp     _SI_InputIndex, [_next_turn_button]
Main_Screen+B17                          cmp     _SI_InputIndex, [_next_turn_button]
MainScreen_Add_Fields+28                 mov     [_next_turn_button], -1000         
MainScreen_Add_Fields+B9                 mov     [_next_turn_button], ax            



Main_Screen+B6        mov     [bp+IDK_EoT_flag], ST_FALSE
Main_Screen:loc_50631 cmp     [bp+IDK_EoT_flag], ST_FALSE
Main_Screen+B1D       cmp     [bp+IDK_EoT_flag], ST_FALSE
Main_Screen+B74       mov     [bp+IDK_EoT_flag], ST_FALSE
Main_Screen:loc_506DB mov     [bp+IDK_EoT_flag], ST_TRUE
Main_Screen:loc_506E2 mov     [bp+IDK_EoT_flag], ST_TRUE
Main_Screen+BA4       mov     [bp+IDK_EoT_flag], ST_FALSE




¿ for showing 'Next Turn' button vs. logic for doing next turn proc when EoT wait if off ?

// WZD o063p02
void Main_Screen_Draw_Do_Draw(int16_t * map_x, int16_t * map_y, int16_t map_plane, int16_t x_pos, int16_t y_pos, int16_t player_idx)
    if(_unit_stack_count > 0)
    else
        Main_Screen_Draw_Summary_Window();
        Main_Screen_Draw_Unit_Action_Locked_Buttons();
        Main_Screen_Draw_Next_Turn_Button();
// WZD o064p05
void Main_Screen_Draw_Next_Turn_Button(void)
    if(all_units_moved == ST_TRUE)

## Program-Path:

main() |-> Screen_Control() |-> Main_Screen()

ovr057:0AB0                                                 @@Check_Input_NextTurnButton:           ; CODE XREF: Main_Screen+A9Fj ...
ovr057:0AB0 3B 36 50 C0                                     cmp     _SI_InputIndex, [_next_turn_button]
ovr057:0AB4 74 3B                                           jz      short @@PrepNextTurn

@@Check_Input_NextTurnButton
// IDA: Main_Screen+AB0


left-click next_turn_button
or 'Time Stop'
or magic_set.end_of_turn_wait

Any_Units_Not_Busy()



Main_Screen+B9E
mov     [current_screen], scr_Next_Turn_Proc

    Main_Screen+B17
        cmp     _SI_InputIndex, [_next_turn_button]




Main_Screen+B6E

Main_Screen+12A8





## Next_Turn_Proc()
j_IDK_EoT_s52B09()
    |-> IDK_EoT_s52B09()
Screen_Control()
    |-> j_IDK_EoT_s52B09







? _Next_Turn_Proc (0Ah 10d) ?


GameState_10:                           ; case 0xA
call    j_IDK_EoT_s52B09

jmp     short GameState_11_13           ; default

