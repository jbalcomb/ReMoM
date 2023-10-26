


move these to the IDA documentation...
Jump to address
ovr120:1133
Nay "000506FE", for "Main_Screen+B9E"
Nay "00043ABE", for "Main_Screen+B9E"
Yay "Main_Screen+B9E"








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







0
? _Next_Turn_Proc (0Ah 10d) ?


GameState_10:                           ; case 0xA
call    j_IDK_EoT_s52B09

jmp     short GameState_11_13           ; default

