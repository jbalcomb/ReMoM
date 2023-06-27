
¿ ~ Movement Map - game vs. engine ?



Elsewhere, ...
     in debugging the 'right-click movement map - scroll map'
     the debug log should show
     ¿ ... Toggle_Pages() | Draw_Fields() |-> Draw_Field() ... ?
          field_num == _main_map_grid_field == 8
          input_field_num == -8
          up_down_flag == 1 (down)
          calculation of screen x,y field x,y grid x,y
          updated value in p_fields[8].Param3,Param4
     ¿ ... mouse button right; ponter x,y in field xmin,ymin,xmax,ymax; ... ?


Main_Screen_Add_Fields()
     "Yay - Movement Map Grid Field"
     _main_map_grid_field
     if a stack is active, Add_Unit_Window_Fields() will add 1 to 9 field idxs

"DBG_movement_map_grid_field_idx" comes from Scan_Field()

So, ...
     hit Main_Screen_Add_Fields(), get field idx
     hit Scan_Field(), match bounds, get matched field_num
     ...
     Where?  see coords of move map x,y change
     


When does it clear the mouse button status?
     ...
          Mouse_Button_Handler() sets mouse_enabled == ST_TRUE
          Mouse_Movement_Handler() sets mouse_enabled == ST_TRUE
          MD_GetButtonStatus() doesn't set/unset anything
               repeatedly called, but just hits the INT
     ...
     Nothing? That's what down_mouse_button is for?
          Push_Field_Down() sets it
          GUI_TextEditDialog() sets it, when it creates the dialog
     









_map_x, _map_y
_map_plane
_prev_world_x, _prev_world_y

_map_x and _map_y are PbR in the call to Main_Screen_Draw_Do_Draw()
¿ _prev_world_x and _prev_world_y are passed as the target x,y ?


IDK_CheckSet_MapDisplay_XY()
on `Right-Click - Movement Map - Scroll Map`, rebounds _prev_world_x,y and does something with _map_x
So, "previous" wouldn't seem to really make much sense, would it?
So, back to ~"active"?





_main_map_grid_x
_main_map_grid_y

_main_map_grid_x,y needs to change at some point, does it not?
     Does it just so happen to be different on each run-through?
          No, because the input fields are define outside the screen-loop.
     Actually, are these just always the grid coordinates, relative to the 12x10 grid?
          They get Passed-by-Reference to Add_Grid_Fields() as xpos,ypos...
               ...which then does what with them?
                    they get assigned to Param3 and Param4
                    but, the look of it is just like any other assignment of a value
                    which means they are not being dereferenced?
                    which means it is assign the actual memomry address of the pointer?
                    so, somewhere else, we would expect to see a reference to Param3,4 for a ft_Grid Input Field?
                         What does the ref look like in IDA?
                    
                    Scan_Input()
                         if(p_fields[].type == ft_Grid && down_mouse_button == ST_UNDEFINED)
                              p_fields[].Param3 = ((Pointer_X() - p_fields[].x1) % Param1) ... cwd,idiv ... 
                    
                    Where is Scan_Input() called? ...in Interpret_Mouse_Input()?
                         N/A
                         Elsewhere, somehow?

seg036:2E79 Scan_Input mov     bx, [es:bx+s_Field.Param3]      ; Button:FullStoreFlag  (4:Align, 6:MinSlide, A:CBlock, C:Col@)
seg036:902C Draw_Field mov     bx, [es:bx+s_Field.Param3]      ; Button:FullStoreFlag  (4:Align, 6:MinSlide, A:CBlock, C:Col@)
seg036:90B9 Draw_Field mov     bx, [es:bx+s_Field.Param3]      ; Button:FullStoreFlag  (4:Align, 6:MinSlide, A:CBlock, C:Col@)
seg036:9107 Draw_Field mov     bx, [es:bx+s_Field.Param3]      ; Button:FullStoreFlag  (4:Align, 6:MinSlide, A:CBlock, C:Col@)
seg036:91B7 Draw_Field mov     bx, [es:bx+s_Field.Param3]      ; Button:FullStoreFlag  (4:Align, 6:MinSlide, A:CBlock, C:Col@)
seg036:91CD Draw_Field mov     bx, [es:bx+s_Field.Param3]      ; Button:FullStoreFlag  (4:Align, 6:MinSlide, A:CBlock, C:Col@)

Up   p Interpret_Mouse_Input+BF1 call    near ptr Draw_Field             ; draws the selected control into the current draw
Up   p Draw_Fields+65            call    near ptr Draw_Field             ; draws the selected control into the current draw
Down p Push_Field_Down+5C        call    near ptr Draw_Field             ; draws the selected control into the current draw
Down p Push_Field_Down+124       call    near ptr Draw_Field             ; draws the selected control into the current draw
Down p Push_Field_Down+133       call    near ptr Draw_Field             ; draws the selected control into the current draw
Down p Push_Field_Down+142       call    near ptr Draw_Field             ; draws the selected control into the current draw
Down p Push_Field_Down+1A6       call    near ptr Draw_Field             ; draws the selected control into the current draw

¿ |-> Interpret_Mouse_Input() |-> Draw_Field() ?
...
...
...
    down_mouse_button != ST_UNDEFINED
    p_fields[down_mouse_button].type == ft_SlideBar
    p_fields[down_mouse_button].type == ft_MultiButton
    p_fields[down_mouse_button].type == ft_StringList
    p_fields[down_mouse_button].type == ft_ContinuousStringInput

¿ |-> Draw_Fields() |-> Draw_Field() ? <-| Toggle_Pages()



                              

cmp     [es:bx+s_Field.type], ft_Grid
26 83 7F 08 0C

seg036:0C8F Interpret_Mouse_Input cmp     [es:bx+s_Field.type], ft_Grid
seg036:0CA4 Interpret_Mouse_Input cmp     [es:bx+s_Field.type], ft_Grid
seg036:1E06 GUI_GetNextControl    cmp     [es:bx+s_Field.type], ft_Grid
seg036:1EA4 GUI_GetNextControl    cmp     [es:bx+s_Field.type], ft_Grid
seg036:206F GUI_GetNextControl    cmp     [es:bx+s_Field.type], ft_Grid
seg036:22C1 GUI_GetNextControl    cmp     [es:bx+s_Field.type], ft_Grid
seg036:248F GUI_GetNextControl    cmp     [es:bx+s_Field.type], ft_Grid
seg036:26E6 GUI_GetNextControl    cmp     [es:bx+s_Field.type], ft_Grid
seg036:2947 GUI_GetNextControl    cmp     [es:bx+s_Field.type], ft_Grid
seg036:2E17 Scan_Input            cmp     [es:bx+s_Field.type], ft_Grid
seg036:2E36 Scan_Input            cmp     [es:bx+s_Field.type], ft_Grid
seg036:3170 GUI_ProcessDirKey     cmp     [es:bx+s_Field.type], ft_Grid
seg036:6CF6 Draw_Fields           cmp     [es:bx+s_Field.type], ft_Grid

mov     bx, [es:bx+s_Field.Param3]
26 8B 5F 1B
NONE / DNE

mov     bx, [es:bx+s_Field.Param3]
26 8B 5F 1C

seg036:2E79 Scan_Input mov     bx, [es:bx+s_Field.Param3]      ; Button:FullStoreFlag  (4:Align, 6:MinSlide, A:CBlock, C:Col@)
seg036:902C Draw_Field mov     bx, [es:bx+s_Field.Param3]      ; Button:FullStoreFlag  (4:Align, 6:MinSlide, A:CBlock, C:Col@)
seg036:90B9 Draw_Field mov     bx, [es:bx+s_Field.Param3]      ; Button:FullStoreFlag  (4:Align, 6:MinSlide, A:CBlock, C:Col@)
seg036:9107 Draw_Field mov     bx, [es:bx+s_Field.Param3]      ; Button:FullStoreFlag  (4:Align, 6:MinSlide, A:CBlock, C:Col@)
seg036:91B7 Draw_Field mov     bx, [es:bx+s_Field.Param3]      ; Button:FullStoreFlag  (4:Align, 6:MinSlide, A:CBlock, C:Col@)
seg036:91CD Draw_Field mov     bx, [es:bx+s_Field.Param3]      ; Button:FullStoreFlag  (4:Align, 6:MinSlide, A:CBlock, C:Col@)



Elsewhere, ...
     XREF's to ft_Grid
          Interpret_Mouse_Input()
               seg036:0C8F
               seg036:0CA4

26 8B 47 08                                     mov     ax, [es:bx+s_Field.type]

26 83 7F 08 06                                  cmp     [es:bx+s_Field.type], ft_Slidebar
26 83 7F 08 0B                                  cmp     [es:bx+s_Field.type], ft_ContinuousStringInput
            ^^ field type
^^^^^^^^^^^ cmp [es:bx+s_Field.type]


26 8B 5F 1A                                     mov     bx, [es:bx+s_Field.Param2]      ; State@        (4/B:BG_Color, 6:StSeg, 8:Len, C:CellH)
         ^^ structure member offset
^^^^^^^^

26 39 7F 20                                     cmp     [es:bx+s_Field.Param5], di

26 8B 5F 1A                                     mov     bx, [es:bx+s_Field.Param2]      ; State@        (4/B:BG_Color, 6:StSeg, 8:Len, C:CellH)
C7 07 01 00                                     mov     [word ptr bx], 1








GAME_Overland_Init()
     CRP_OVL_MapWindowX = 0;
     CRP_OVL_MapWindowY = 0;
     _prev_world_x = 0;
     _prev_world_y = 0;
     _map_x = 0;
     _map_y = 0;
     _active_world_x = _FORTRESSES[0].world_x
     _active_world_y = _FORTRESSES[0].world_y
     _map_plane = _FORTRESSES[0].world_plane

     Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane)

     WIZ_NextIdleStack(_human_player_idx, _map_x@, _map_y@, _map_plane@)

WIZ_NextIdleStack() |-> OVL_StackSelect() |-> OVL_BringIntoView()

OVL_BringIntoView()
     updates Map_X@,Map_Y@
     sets _prev_world_x,y
     sets CRP_OVL_MapWindowX,Y = _prev_world_x,y * 20,18 MAP_SQUARE_W,H

     So, ...
          the call to Set_Entities_On_Map_Window() means that map_x,y,p represent where the view is right now
          the call to OVL_BringIntoView() through WIZ_NextIdleStack() means it needs to know where the view is and where the unit is to decide if the view needs to move
               needs unit_x,y, map_plane, player id to build a stack  (finds unit by x,y; matches unit plane to map plane)
          







Up   w GAME_Overland_Init+FD                 mov     [_map_y], 0                               
Up   o GAME_Overland_Init+163                mov     ax, offset _map_y                         
Up   r GAME_Overland_Init+181                push    [_map_y]                        ; TopY    
Up   o GAME_Overland_Init:loc_45EFA          mov     ax, offset _map_y                         
Up   r City_Screen+76C                       mov     ax, [_map_y]                              
Up   r City_Screen+EAE                       mov     ax, [_map_y]                              
Up   r sub_499B1+16                          push    [_map_y]                        ; TopY    
Up   r Main_Screen+80                        push    [_map_y]                        ; TopY    
Up   o Main_Screen+511                       mov     ax, offset _map_y                         
Up   r Main_Screen+553                       push    [_map_y]                        ; TopY    
Up   r Main_Screen+68C                       push    [_map_y]                        ; TopY    
Up   o Main_Screen+6C5                       mov     ax, offset _map_y                         
Up   o Main_Screen+6DD                       mov     ax, offset _map_y                         
Up   r Main_Screen+706                       push    [_map_y]                        ; TopY    
Up   o Main_Screen+753                       mov     ax, offset _map_y                         
Up   o Main_Screen+76B                       mov     ax, offset _map_y                         
Up   o Main_Screen+795                       mov     ax, offset _map_y                         
Up   o Main_Screen+7AD                       mov     ax, offset _map_y                         
Up   o Main_Screen+7FD                       mov     ax, offset _map_y                         
Up   o Main_Screen+815                       mov     ax, offset _map_y                         
Up   o Main_Screen+865                       mov     ax, offset _map_y                         
Up   o Main_Screen+87D                       mov     ax, offset _map_y                         
Up   r Main_Screen+8A9                       push    [_map_y]                        ; TopY    
Up   o Main_Screen+8D7                       mov     ax, offset _map_y                         
Up   o Main_Screen+8EF                       mov     ax, offset _map_y                         
Up   r Main_Screen+918                       push    [_map_y]                        ; TopY    
Up   o Main_Screen+93A                       mov     ax, offset _map_y                         
Up   r Main_Screen+99F                       push    [_map_y]                        ; TopY    
Up   r Main_Screen+9EB                       push    [_map_y]                        ; TopY    
Up   r Main_Screen+A78                       push    [_map_y]                        ; TopY    
Up   r Main_Screen:loc_507D6                 mov     ax, [_map_y]                              
Up   o Main_Screen+C9A                       mov     ax, offset _map_y                         
Up   r Main_Screen+CC4                       mov     ax, [_map_y]                              
Up   o Main_Screen+CE7                       mov     ax, offset _map_y                         
Up   r Main_Screen+D54                       cmp     ax, [_map_y]                              
Up   r Main_Screen:loc_508BD                 mov     ax, [_map_y]                              
Up   r Main_Screen+DCE                       sub     ax, [_map_y]                              
Up   o Main_Screen+DE5                       mov     ax, offset _map_y                         
Up   r Main_Screen+E28                       mov     ax, [_map_y]                              
Up   o Main_Screen+E84                       mov     ax, offset _map_y                         
Up   o Main_Screen+EDC                       mov     ax, offset _map_y                         
Up   o Main_Screen+FD6                       mov     ax, offset _map_y                         
Up   r Main_Screen+1010                      push    [_map_y]                        ; TopY    
Up   r Main_Screen+1222                      mov     ax, [_map_y]                              
Up   w Main_Screen+1261                      mov     [_map_y], ax                              
Up   o Main_Screen+1270                      mov     ax, offset _map_y                         
Up   r MainScreen_Add_Fields+C5              mov     ax, [_map_y]                              
Up   r MainScreen_Add_Fields+105             mov     ax, [_map_y]                              
     o Main_Screen_Draw+1D                   mov     ax, offset _map_y                         
Down r Main_Screen_Reset+25                  push    [_map_y]                        ; TopY    
Down o IDK_UnitMoves_and_PlanarTravel+151    mov     ax, offset _map_y                         
Down o IDK_UnitMoves_and_PlanarTravel+199    mov     ax, offset _map_y                         
Down o sub_51A1A+46                          mov     ax, offset _map_y                         
Down r sub_51A1A+73                          push    [_map_y]                        ; TopY    
Down r sub_51AA0+64                          push    [_map_y]                        ; TopY    
Down r sub_52B09+1A                          push    [_map_y]                        ; TopY    
Down o sub_52B09+237                         mov     ax, offset _map_y                         
Down o sub_52B09+2E1                         mov     ax, offset _map_y                         
Down o EarthGateTeleport+2F1                 mov     ax, offset _map_y                         
Down r MainScr_Prepare_Reduced_Map+19        mov     ax, [_map_y]                              
Down r OVL_DrawPath+29                       push    [_map_y]                        ; Map_TopY
Down o Cities_Screen+373                     mov     ax, offset _map_y                         
Down o Cities_Screen+3DD                     mov     ax, offset _map_y                         
Down w Draw_Maps+120                         mov     [_map_y], _DI_cur_map_ypos                
Down r OVL_DrawMapSection+9A                 sub     ax, [_map_y]                              
Down r Road_Build_Screen+196                 push    [_map_y]                        ; TopY    
Down r Road_Build_Screen+2AC                 mov     ax, [_map_y]                              
Down r Road_Build_Screen+2DB                 mov     ax, [_map_y]                              
Down w Road_Build_Screen+31A                 mov     [_map_y], ax                              
Down o Road_Build_Screen+329                 mov     ax, offset _map_y                         
Down r IDK_RoadBuild_s5BC57+36               mov     ax, [_map_y]                              
Down r IDK_RoadBuild_s5BC57+76               mov     ax, [_map_y]                              
Down r sub_5BF7F+7F                          sub     ax, [_map_y]                              
Down r sub_5BF7F:loc_5C066                   mov     ax, [_map_y]                              
Down r IDK_Spell_Casting_Screen+9D           push    [_map_y]                        ; TopY    
Down r IDK_Spell_Casting_Screen+185          mov     ax, [_map_y]                              
Down r IDK_Spell_Casting_Screen+513          mov     ax, [_map_y]                              
Down r sub_5CDB2+35                          mov     ax, [_map_y]                              
Down r sub_5CDB2+75                          mov     ax, [_map_y]                              
Down o IDK_Spell_Casting_Screen_Draw+22      mov     ax, offset _map_y                         
Down r sub_5D15B:loc_5D1B4                   mov     ax, [_map_y]                              
Down r sub_5D20A+13                          push    [_map_y]                                  
Down r sub_5D20A:loc_5D338                   mov     ax, [_map_y]                              
Down r sub_5D20A+391                         push    [_map_y]                        ; TopY    
Down o USW_Display+542                       mov     ax, offset _map_y                         
Down o USW_Display+76A                       mov     ax, offset _map_y                         
Down w IDK_Cartographer_Screen+12D           mov     [_map_y], ax                              
Down r IDK_Cartographer_Screen+163           push    [_map_y]                        ; TopY    
Down o Outpost_Screen+323                    mov     ax, offset _map_y                         
Down r EVNT_ShowMessage:loc_6C579            push    [_map_y]                        ; TopY    
Down r Surveyor_Screen+91                    push    [_map_y]                        ; TopY    
Down r Surveyor_Screen+161                   mov     ax, [_map_y]                              
Down w Surveyor_Screen+1A0                   mov     [_map_y], ax                              
Down o Surveyor_Screen:loc_7A56F             mov     ax, offset _map_y                         
Down r IDK_SurveyorScreenAddFields_s7A5EF+1D mov     ax, [_map_y]                              
Down r IDK_SurveyorScreenAddFields_s7A5EF+5D mov     ax, [_map_y]                              
Down o IDK_SurveyorDraw_s7A6A9+1D            mov     ax, offset _map_y                         
Down r IDK_Surveyor_s7A761+79                add     ax, [_map_y]                              
Down w OVL_MoveUnitStack+E9                  mov     [_map_y], ax                              
Down w OVL_MoveUnitStack+4FE                 mov     [_map_y], ax                              
Down r OVL_ResolveConflict+76C               push    [_map_y]                        ; TopY    
Down r UU_IDK_Main_Screen_Draw+C             mov     ax, [_map_y]                              
Down w UU_IDK_Main_Screen_Draw+37            mov     [_map_y], ax                              
Down w UU_IDK_Main_Screen_Draw+A0            mov     [_map_y], ax                              
Down r AI_MoveUnits:loc_81E01                mov     ax, [_map_y]                              
Down w AI_MoveUnits+AB                       mov     [_map_y], ax                              
Down r AI_MoveUnits+B4                       mov     ax, [_map_y]                              
Down r AI_UNIT_Move+A9                       mov     ax, [_map_y]                              
Down w AI_UNIT_Move+B8                       mov     [_map_y], ax                              
Down o GAME_NextHumanStack+2E                mov     ax, offset _map_y                         
Down r sub_9EC10+1F6                         push    [_map_y]                        ; TopY    
Down r sA9AC5_Events_Anim_Scroll+28          push    [_map_y]                        ; TopY    
Down r sAFA06_Anim_EarthLore:loc_AFB1B       mov     di, [_map_y]                              
Down r sAFA06_Anim_EarthLore:loc_AFB47       mov     ax, [_map_y]                              
Down r sub_AFCA8+E                           mov     ax, [_map_y]                              
Down r sub_AFCA8+286                         push    [_map_y]                        ; TopY    
Down r sub_AFCA8+2AA                         push    [_map_y]                        ; Map_TopY
Down r sub_AFCA8+4F3                         mov     ax, [_map_y]                              
Down w sub_AFCA8+50C                         mov     [_map_y], ax                              
Down r sub_AFCA8+519                         push    [_map_y]                        ; Tile_Y  
Down o sub_AFCA8+521                         mov     ax, offset _map_y                         
Down r sub_AFCA8+535                         push    [_map_y]                        ; TopY    
Down r sub_B0C07+2B                          mov     ax, [_map_y]                              
Down r sub_B0C07+3BB                         push    [_map_y]                        ; Map_TopY
Down w sub_B0C07+4E3                         mov     [_map_y], ax                              
Down r sB1280_Anim_EnchantRoad:loc_B1294     mov     ax, [_map_y]                              
Down w sB1280_Anim_EnchantRoad+1FE           mov     [_map_y], ax                              
Down r sub_B517B+1C                          push    [_map_y]                        ; Map_TopY
Down r sub_B517B+85                          push    [_map_y]                        ; TopY    
Down r sub_B517B+107                         push    [_map_y]                        ; TopY    
Down r sub_B529D+11                          mov     ax, [_map_y]                              
Down r sub_B529D+1E8                         push    [_map_y]                        ; Map_TopY
Down r sub_B529D+322                         push    [_map_y]                        ; TopY    
Down r sub_B529D+3D3                         push    [_map_y]                        ; TopY    
Down r sub_B529D+5A8                         push    [_map_y]                        ; Map_TopY
Down r sub_B529D+5D1                         push    [_map_y]                        ; TopY    
Down w sub_B529D+66E                         mov     [_map_y], ax                              
Down r sub_BA1AF+45                          mov     ax, [_map_y]                              
Down r sub_BA1AF+78                          push    [_map_y]                        ; Map_TopY
Down w sub_BA1AF+100                         mov     [_map_y], ax                              
Down r sub_BF048+14E                         push    [_map_y]                        ; Map_TopY
Down r OVL_DrawUnits+29                      sub     ax, [_map_y]                              



Up   w GAME_Overland_Init+EB                 mov     [_prev_world_x], 0                                                
     w City_Screen+769                       mov     [_prev_world_x], ax                                               
Down w City_Screen+EAB                       mov     [_prev_world_x], ax                                               
Down r IDK_DrawCityScrn_s48DAF+1C1           push    [_prev_world_x]                 ; XPos                            
Down w Main_Screen+11F5                      add     [_prev_world_x], ax             ; grid x - (map width / 2) = map x
Down w Main_Screen+124C                      mov     [_prev_world_x], ax                                               
Down r Main_Screen+1258                      mov     ax, [_prev_world_x]                                               
Down r Main_Screen+126C                      push    [_prev_world_x]                 ; X_Pos                           
Down r MainScreen_Add_Fields+BF              cmp     ax, [_prev_world_x]                                               
Down r MainScreen_Add_Fields+FF              cmp     ax, [_prev_world_x]                                               
Down r Main_Screen_Draw+15                   push    [_prev_world_x]                 ; XPos                            
Down w OVL_BringIntoView+C0                  mov     [_prev_world_x], ax                                               
Down r OVL_BringIntoView+C8                  mov     ax, [_prev_world_x]                                               
Down w OVL_TileOffScrnEdge+7D                mov     [_prev_world_x], ax                                               
Down r OVL_TileOffScrnEdge+85                mov     ax, [_prev_world_x]                                               
Down r IDK_CheckSet_MapDisplay_XY:loc_59DB4  cmp     [_prev_world_x], 60                                               
Down r IDK_CheckSet_MapDisplay_XY+1B         mov     ax, [_prev_world_x]                                               
Down w IDK_CheckSet_MapDisplay_XY+21         mov     [_prev_world_x], ax                                               
Down r IDK_CheckSet_MapDisplay_XY:loc_59DC4  cmp     [_prev_world_x], 0                                                
Down r IDK_CheckSet_MapDisplay_XY+2B         mov     ax, [_prev_world_x]                                               
Down w IDK_CheckSet_MapDisplay_XY+31         mov     [_prev_world_x], ax                                               
Down r IDK_CheckSet_MapDisplay_XY+48         cmp     [_prev_world_x], 49                                               
Down w Center_Map+80                         mov     [_prev_world_x], ax                                               
Down r Center_Map+88                         mov     ax, [_prev_world_x]                                               
Down w IDK_CityScreen_Map_s5A97D+62          mov     [_prev_world_x], ax                                               
Down r IDK_CityScreen_Map_s5A97D+6A          mov     ax, [_prev_world_x]                                               
Down w Road_Build_Screen+305                 mov     [_prev_world_x], ax                                               
Down r Road_Build_Screen+311                 mov     ax, [_prev_world_x]                                               
Down r Road_Build_Screen+325                 push    [_prev_world_x]                 ; X_Pos                           
Down w Road_Build_Screen+357                 add     [_prev_world_x], ax                                               
Down r IDK_RoadBuild_s5BC57+30               cmp     ax, [_prev_world_x]                                               
Down r IDK_RoadBuild_s5BC57+70               cmp     ax, [_prev_world_x]                                               
Down r IDK_RoadBuild_s5BD4D+1A               push    [_prev_world_x]                 ; XPos                            
Down w IDK_Spell_Casting_Screen+53D          mov     [_prev_world_x], ax                                               
Down r IDK_Spell_Casting_Screen+556          push    [_prev_world_x]                 ; X_Pos                           
Down w IDK_Spell_Casting_Screen+7DC          add     [_prev_world_x], ax                                               
Down r sub_5CDB2+2F                          cmp     ax, [_prev_world_x]                                               
Down r sub_5CDB2+6F                          cmp     ax, [_prev_world_x]                                               
Down r IDK_Spell_Casting_Screen_Draw+1A      push    [_prev_world_x]                 ; XPos                            
Down w Surveyor_Screen+18B                   mov     [_prev_world_x], ax                                               
Down r Surveyor_Screen+197                   mov     ax, [_prev_world_x]                                               
Down r Surveyor_Screen:loc_7A56B             push    [_prev_world_x]                 ; X_Pos                           
Down w Surveyor_Screen+1DD                   add     [_prev_world_x], ax                                               
Down r IDK_SurveyorScreenAddFields_s7A5EF+17 cmp     ax, [_prev_world_x]                                               
Down r IDK_SurveyorScreenAddFields_s7A5EF+57 cmp     ax, [_prev_world_x]                                               
Down r IDK_SurveyorDraw_s7A6A9+15            push    [_prev_world_x]                 ; XPos                            
Down w AI_MoveUnits+B1                       mov     [_prev_world_x], ax                                               
