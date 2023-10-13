


¿ relation with document on map_x, map_y, Map_p, etc. ?

_units is count of all units
_UNITS is array of struct s_UNIT for each unit

_unit_stack
_unit_stack_count

What it is?
How it is used?
Where it is?
What touches it?


_unit

_active_world_x
_active_world_y

Why did/do I have these (also) named as OVL_Map_CenterX, OVL_Map_CenterY?



Loaded_Game_Update()
    |->
GAME_Overland_Init()
    sets _prev_world_x, _prev_world_y to 0
    sets _map_x, _map_y to 0
    sets _unit to 0  ¿ ST_NONE ?
    sets _active_world_x, _active_world_y to _FORTRESSES[0].world_x, _FORTRESSES[0].world_y
    sets _map_plane to _FORTRESSES[0].world_plane

    calls Center_Map () with &_map_x, &_map_y, _FORTRESSES[0].world_x, _FORTRESSES[0].world_y, _map_plane
        ¿ this changes _map_x, _map_y ?
    calls WIZ_NextIdleStack() with _human_player_idx, &_map_x, &_map_y, &_map_plane
        ¿ this changes _unit, _active_world_x, _active_world_y ?


Main_Screen()
    Right-Click Movement Map Grid
        (entity_idx != ST_UNDEFINED)
        (entity_idx < 1000)
        (_UNITS[entity_idx].owner_idx == _human_player_idx)
            _unit = entity_idx;
            unit_idx = _unit;
            Unit_X = _UNITS[unit_idx].world_x;
            Unit_Y = _UNITS[unit_idx].world_y;
            // _active_world_x = _UNITS[unit_idx].world_x;
            // _active_world_y = _UNITS[unit_idx].world_y;
            OVL_Map_CenterX = _UNITS[unit_idx].world_x;
            OVL_Map_CenterY = _UNITS[unit_idx].world_y;





WIZ_NextUnit

Distance_From_Center = Delta_XY_With_Wrap(_active_world_x, _active_world_y, _UNITS[unit_idx].world_x, _UNITS[unit_idx].world_y, WORLD_WIDTH);

dseg:BD82 00 00                                           _active_world_y dw 0                    ; DATA XREF: GAME_Overland_Init+127w ...
dseg:BD84 00 00                                           _active_world_x dw 0                    ; DATA XREF: GAME_Overland_Init+11Bw ...
dseg:BD86 00 00                                           _map_plane dw 0                         ; DATA XREF: GAME_Overland_Init+13Fw ...
dseg:BD88 00 00                                           _map_y dw 0                             ; DATA XREF: GAME_Overland_Init:loc_45E5Dw ...
dseg:BD8A 00 00                                           _map_x dw 0                             ; DATA XREF: GAME_Overland_Init:loc_45E57w ...
dseg:BD8C 00 00                                           _human_player_idx dw 0                  ; DATA XREF: Loaded_Game_Update_WZD+Aw ...
dseg:BD8C                                                                                         ; index of the human player (special rules apply)
dseg:BD8E 00 00                                           _unit dw 0                              ; DATA XREF: Save_SAVE_GAM:loc_45727t ...



_unit
XREF:
Save_SAVE_GAM:loc_45727                mov     ax, offset _unit.Current_Unit                                                      
Load_SAVE_GAM+1BE                      mov     ax, offset _unit.Current_Unit                                                      
GAME_Overland_Init+10D                 mov     [_unit], 0                                                                         
sub_49E50+7                            cmp     [_unit], 0FFFFh                                                                    
Main_Screen+F71                        mov     [_unit], ax                                                                        
Main_Screen+F74                        mov     ax, [_unit]                                                                        
Main_Screen_Draw_Unit_Window:loc_517E8 cmp     [_unit], e_ST_UNDEFINED                                                            
IDK_DoMoveStack_s5336C+33              mov     [_unit], ax                                                                        
IDK_DoMoveStack_s5336C+78              mov     ax, [_unit]                                                                        
WIZ_NextIdleStack:loc_5388C            mov     ax, [_unit]                                                                        
WIZ_NextIdleStack+AB                   mov     ax, [_unit]                                                                        
WIZ_NextIdleStack:loc_538FA            mov     ax, [_unit]                                                                        
WIZ_NextIdleStack+DA                   mov     ax, [_unit]                                                                        
WIZ_NextIdleStack+F5                   mov     ax, [_unit]                                                                        
WIZ_NextIdleStack+10B                  mov     ax, [_unit]                                                                        
WIZ_NextIdleStack+122                  mov     ax, [_unit]                                                                        
WIZ_NextIdleStack+136                  mov     ax, [_unit]                                                                        
WIZ_NextIdleStack+15D                  push    [_unit]                         ; Unit_Index                                       
WIZ_NextIdleStack+185                  mov     ax, [_unit]                                                                        
WIZ_NextUnit+1A6                       mov     [_unit], ax                                                                        
WIZ_NextUnit+1AE                       mov     ax, [_unit]                                                                        
WIZ_NextUnit+1C2                       mov     ax, [_unit]                                                                        
WIZ_NextUnit+23D                       mov     [_unit], ax                                                                        
WIZ_NextUnit+240                       mov     ax, [_unit]                                                                        
WIZ_NextUnit+254                       mov     ax, [_unit]                                                                        
Select_Unit_Stack:loc_54EB0            mov     ax, [_unit]                                                                        
Select_Unit_Stack+8D                   mov     ax, [_unit]                                                                        
Select_Unit_Stack+A1                   mov     ax, [_unit]                                                                        
Sort_Unit_Stack+149                    mov     [_unit], ax                     ; _unit = _unit_stack[0].unit_idx                  
Sort_Unit_Stack+167                    mov     [_unit], ax                                                                        
ArmyList_Screen:loc_58859              mov     ax, [_unit]                                                                        
ArmyList_Screen+28C                    mov     ax, [_unit]                                                                        
ArmyList_Screen+34F                    mov     [_unit], ax                     ; _unit = IDK_UNIT_w42BB0[itr_army_list_item_count]
ArmyList_Screen+352                    mov     ax, [_unit]                                                                        
ArmyList_Screen+367                    mov     ax, [_unit]                                                                        
ArmyList_Screen+37A                    mov     ax, [_unit]                                                                        
ArmyList_Screen:loc_58991              mov     ax, [_unit]                                                                        
ArmyList_Screen+3C5                    mov     ax, [_unit]                                                                        
ArmyList_Screen+549                    mov     [_unit], ax                                                                        
ArmyList_Screen+54C                    mov     ax, [_unit]                                                                        
ArmyList_Screen+561                    mov     ax, [_unit]                                                                        
ArmyList_Screen+574                    mov     ax, [_unit]                                                                        



_active_world_x, _active_world_y
XREF:
Down w GAME_Overland_Init+11B mov     [_active_world_x], ax                   
Down w Main_Screen+FB4        mov     [_active_world_x], ax                   
Down r WIZ_NextUnit+DB        push    [_active_world_x]               ; x1    
Down w WIZ_NextUnit+1BF       mov     [_active_world_x], ax                   
Down w WIZ_NextUnit+251       mov     [_active_world_x], ax                   
Down w EarthGateTeleport+2D4  mov     [_active_world_x], ax                   
Down w ArmyList_Screen+3C2    mov     [_active_world_x], ax                   
Down w Outpost_Screen+2FE     mov     [_active_world_x], ax                   
Down r Outpost_Screen+31B     push    [_active_world_x]               ; Unit_X
Down w sub_B4250+15C          mov     [_active_world_x], ax                   
Down w sub_B4471+11D          mov     [_active_world_x], ax                   
Down w sub_B4E00+21E          mov     [_active_world_x], ax                   
Down w G_OVL_Cast+407         mov     [_active_world_x], ax                   
Down w EVNT_RandomOffers+2FA  mov     [_active_world_x], ax                   


