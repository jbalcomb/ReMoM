



¿ Conclusions ?

    x,y of movement map
    x,y of the map and x,y of *target*

    ¿ game code tracks curr/prev to decide if it needs to call out ?
    ¿ engine code tracks curr/prev to decide if it needs to draw ?


¿ Use-Case ?

    Right-Click - Move Map
        Mouse X,Y => Grid Field X,Y => World X,Y  ¿ mx,my; gx,gy; wx,wy ?




Program-Flow
Draw
Main Screen
Component

Main Screen
Background
Movement Map
Reduced Map
Game Buttons
Next Turn Button
Unit Actions Buttons
Unit Window
    Unit Figures
Upkeep Summary Window





¿ top-most ?
Main_Screen_Draw() |-> Main_Screen_Draw_Do_Draw()
    Reset_Window()
    Set_Page_Off()

push    [_human_player_idx]             ; Player_Index
push    [_prev_world_y]                 ; YPos
push    [_prev_world_x]                 ; XPos
push    [_map_plane]                    ; Plane
mov     ax, offset _map_y
push    ax                              ; MapY@
mov     ax, offset _map_x
push    ax                              ; MapX@
Main_Screen_Draw_Do_Draw()
    









main() |-> Screen_Control() |-> Main_Screen() |-> Main_Screen_Draw()



#### Main_Screen_Draw()
    Reset_Window()
    Set_Page_Off()
    Main_Screen_Draw_Do_Draw(_curr_world_x, _curr_world_y, _world_plane, _prev_world_x, _prev_world_y, _human_player_idx);

#### Main_Screen_Draw_Do_Draw(_curr_world_x, _curr_world_y, _world_plane, _prev_world_x, _prev_world_y, _human_player_idx);

BackGround:
    FLIC_Draw(0, 0, main_background);

##### Movement Map Window #####
main() |-> Screen_Control() |-> Main_Screen() |-> Main_Screen_Draw() |-> Main_Screen_Draw_Do_Draw() |-> Draw_Maps() |-> Draw_Map_Window()
    Draw_Maps(0, 20, 12, 10, map_x, map_y, map_plane, x_pos, y_pos, player_idx);

Reduced Map Window:
    Minimap_Set_Dims(58, 30);
?    Draw_Maps(0, 20, 12, 10, map_x, map_y, map_plane, x_pos, y_pos, player_idx);
    Draw_Minimap_Window(251, 21, 58, 30);

Status Window:
    Main_Screen_Draw_Status_Window();

Upkeep Summary Window:
    if(_unit_stack_count == 0)
        Main_Screen_Draw_Summary_Window();

Next Turn Button:
    if(_unit_stack_count == 0)
        Main_Screen_Draw_Next_Turn_Button();

Unit Window:
    _unit_stack_count > 0
        Main_Screen_Draw_Unit_Window()

Unit Action Buttons:
    _unit_stack_count > 0
        Main_Screen_Draw_Unit_Action_Buttons();

Unit Action Locked Buttons:
    _unit_stack_count !> 0
    Main_Screen_Draw_Unit_Action_Locked_Buttons()

Unit Moves:  (?"movement bar"?)
    OVL_DrawStackMoves()
        _unit_stack_count > 0

no XREFs j_OVL_DrawStackMoves()
OON XREF Main_Screen_Draw_Do_Draw()
? calling OVL_DrawStackMoves(), not j_OVL_DrawStackMoves(), means it is a module-local call ?
? no XREFs to j_OVL_DrawStackMoves() means it is a module-onlly/private function ?

##### Active Unit Stack #####
main() |-> Screen_Control() |-> Main_Screen() |-> Main_Screen_Draw() |-> Main_Screen_Draw_Do_Draw() |-> Draw_Maps() |-> Draw_Active_Unit_Stack()




`Main_Screen_Draw_Do_Draw()`
main() |-> Screen_Control() |-> Main_Screen() |-> Main_Screen_Draw() |-> Main_Screen_Draw_Do_Draw()

    Main_Screen_Draw_Do_Draw(&_map_x, &_map_y, _map_plane, _prev_world_x, _prev_world_y, _human_player_idx);
        ...
        |-> Draw_Maps(0, 20, 12, 10, map_x, map_y, map_plane, x_pos, y_pos, player_idx);

What gets used where, how, why?

API Boundary?
    the call here to Draw_Maps() is the source of screen x,y
    would this not be like the moment the game code would tell the engine code what it wants?
    also, what about the PbR of map x,y?

¿ curr. / prev. ?
    in Draw_Maps(), if ~prev != curr, map moved
    if map moved, bounds checks and handle wrap around
    directly updates _map_x,y  (which is what got passed in and is going to get passed back?)
    call others, with maybe updated local copy of passed in _map_x,y
    set map_x,y for pass back 
    So, ...
        Draw_Maps() has access to _map_x,y, but not _prev_world_x,y?
        because _map_x,y is game data, but _prev_world_x,y is (just) movement map data?




    _map_x
    _map_y
    _map_plane
    _prev_world_x
    _prev_world_y
    _human_player_idx

ush    [bp+player_idx]                 ; Player_Index
push    [bp+YPos]                       ; YPos
push    [bp+XPos]                       ; XPos
push    [bp+map_plane]                  ; Plane
push    [bp+map_y]                      ; MapY@
push    [bp+map_x]                      ; MapX@
mov     ax, 10                          ; movement map height
push    ax                              ; VTiles
mov     ax, 12                          ; movement map width
push    ax                              ; HTiles
mov     ax, 20                          ; screen y
push    ax                              ; Top
xor     ax, ax                          ; screen x
push    ax                              ; Left
Draw_Maps




`Draw_Maps()`
main() |-> Screen_Control() |-> Main_Screen() |-> Main_Screen_Draw() |-> Main_Screen_Draw_Do_Draw() |-> Draw_Maps()

    Draw_Maps(0, 20, 12, 10, map_x, map_y, map_plane, x_pos, y_pos, player_idx);




push    [bp+map_plane]                  ; Plane
push    _DI_cur_map_ypos                ; YPos
push    _SI_cur_map_xpos                ; XPos
push    [bp+map_height]                 ; VTiles
push    [bp+map_width]                  ; HTiles
push    [bp+screen_y]                   ; Top
push    [bp+screen_x] 
Draw_Map_Window()
    screen_x    passed in
    screen_y    passed in
    map_width   passed in
    map_height  passed in
    ..._SI_cur_map_xpos...pbr map_x@...
    ..._DI_cur_map_ypos...pbr map_y@...

    map_moved_flag
    shift_right_flag
    half_swap_flag

    map_moved_flag
        l_map_x = map_x
        l_map_y = map_y
        map_moved = ST_FALSE
        if xpos != l_map_x
            map_moved = ST_TRUE
            ....
        if ypos != l_map_y
            map_moved = ST_TRUE
            ....

    shift_right_flag
    half_swap_flag
        if xpos != l_map_x
            map_moved = ST_TRUE
            shift_right_flag = ST_FALSE

            if xpos < l_map_x
                shift_right_flag = ST_FALSE
            else
                shift_right_flag = ST_TRUE
    
            if(xpos > 50 && l_map_x < 20)
                shift_right_flag = ST_FALSE
                half_swap_flag = ST_TRUE

            if(l_map_x > 50 && xpos < 20)
                shift_right_flag = ST_TRUE
                half_swap_flag = ST_TRUE

        So, ...
            shift_right_flag = ST_FALSE ... if (xpos <  l_map_x) || (xpos    > 50 && l_map_x < 20)
            shift_right_flag = ST_TRUE  ... if (xpos >= l_map_x) || (l_map_x > 50 && xpos    < 20)







`Draw_Map_Window()`
main() |-> Screen_Control() |-> Main_Screen() |-> Main_Screen_Draw() |-> Main_Screen_Draw_Do_Draw() |-> Draw_Maps() |-> Draw_Map_Window()

? same code used to draw the "City Map Window" ?

Draw_Map_Window()

Draw_Map_Terrain(screen_x, screen_y, map_width, map_height, map_x, map_y, map_p)
...already knows...world size, world width, map square pixel width, map square pixel heigh  ...?terrain_tile_base?
gets passed screen x,y - to pass to Draw_Picture()
map/world view x,y,w,h, and plane

...bottom-up...
to draw a terrain picture in a movement map square
src, dst
screen x, screen y to start drawing
width and height of picture
which terrain picture to draw

so, start from x,y of the movement map
iterate w,h
index into world_maps to get terrain type  ~ (world plane * world size) + (world y * world width) + (world x)
... world map value / terrain type is the index into TERRAIN.LBX, Entry 1 `terrain_lbx_001`
...
...there's logic around not drawing map squares that are not explored
...there's logic around a flag for not drawing terrain that is not animated

point terrain_pict_seg to terrain_lbx_000, in EMM  (EPFB + offset in emm_terrain_lbx from terrain_001_1)

¿ the logic for dealing with the map of the world is equivalent to dealing with drawing an image somewhere on the screen ?
```c
itr_y = y;
while(itr_y < y + h)
{
    itr_x = x;
    while(itr_x < x + w)
    {

    }
}
```



¿ there is a boundary here of some sort, given that it uses that passed parameters as-is - no PbR ?
¿ presumably, it is *fully parameterized* to draw a map whever you might like to do so ?
¿ ...terrain, minerals, specials, roads, cities, towers, lairs, units, nodes... ? ¿ then, exploration ? ¿ then, incr anims ?
¿ at the least, I can box up the boundary of curr. vs. prv. x,y ?
¿ are these variables/parameters specific to the "draw_map" ?
¿ how about where these parameters come from, and how they get there ?

Draw_Map_Window()
    sets _draw_full_main_map and map_draw_sustain, for its own purpose, for the next time around
        ¿ ~ if current x,y != previous x,y ?
            full draw = TRUE, sustain = 1
            else
            if --sustain < 0
                full draw = FALSE
        So, ...
            just making sure we do a full draw atleast one time around?
            only a performance concern?
    current x,y here is the x,y passed in ~ the target/requested x,y of the movement map
    also, uses these to set _prev_map_draw_x,y
        ¿ previous x,y are only used for determining the state of full draw / sustain ?
        so, the purpose of Reset_Map_Draw() is to *force* a full draw?
            when do we decide to call it? when we change something that impacts what should be drawn on the movement map?
            Sure looks like. Just went through the calls from Main_Screen() - looks like everything that changes the map, but doesn't go to a different screen
        Do I want a flag/an assert for when Reset_Map_Draw() has/has not been call, but curr != prev?
    

_prev_map_draw_x
XREF
    Draw_Map_Window+B                cmp     _SI_curr_map_draw_x, [_prev_map_draw_x]
    Draw_Map_Window:loc_D9633        mov     [_prev_map_draw_x], _SI_curr_map_draw_x
    OVL_MapStateRestore+6            mov     [_prev_map_draw_x], ax                 
    OVL_MapStateSave+15              mov     [_prev_map_draw_x], 0FFFFh             
    OVL_MapStateSave:loc_D95C4       mov     ax, [_prev_map_draw_x]                 
    Reset_Map_Draw:loc_D95B3 mov     [_prev_map_draw_x], e_ST_UNDEFINED     

Reset_Map_Draw  o150p01
XREF
    ...72!!
    ...Main Screen, of course, also City Screen (¿~City Map Window?), but also ¿ *screens* that overlay on the main movement map ?


map_draw_sustain
XREF
    Draw_Map_Window+1D        mov     [map_draw_sustain], 1 
    Draw_Map_Window+29        cmp     [map_draw_sustain], 0 
    Draw_Map_Window:loc_D961A dec     [map_draw_sustain]    
    OVL_MapStateRestore+12    mov     [map_draw_sustain], ax
    OVL_MapStateSave+F        mov     ax, [map_draw_sustain]





`Main_Screen_Draw_Unit_Actions()`







Screen: 320 x 200
                           {   x,   y,   w,   h }
Background                     0,   0, 320, 200
/ < Game Buttons               0,   0, 320, ???
| < Movement Map Window      ???, ???, ???, ???
| < Reduced Map Window       ???, ???, ???, ???
| / / Status Window          ???, ???, ???, ???
| | \ Upkeep Summary Window  ???, ???, ???, ???
| \ Unit Window              
| / Next Turn Button         ???, ???, ???, ???
\ \ Unit Actions Buttons     

+-------+--------+--------+--------+-------+------+--------+
| Game  | Spells | Armies | Cities | Magic | Info | Plane  |
+-------+--------+--------+--------+-------+------+--------+
+-----------------------------------------++---------------+
|                                         ||    Reduced    |
|  Movement                               ||      Map      |
|    Map                                  ||    Window     |
|   Window                                |+---------------+
|                                         |+---------------+
|                                         ||    Status     |
|                                         ||    Window     |
|                                         |+---------------+
|                                         |+---------------+
|                                         ||    Upkeep     |
|                                         ||    Summary    |
|                                         ||    Window     |
|                                         |+---------------+
|                                         |+---------------+
|                                         ||   Next Turn   |
|                                         ||    Button     |
+-----------------------------------------++---------------+

+-------+--------+--------+--------+-------+------+--------+
| Game  | Spells | Armies | Cities | Magic | Info | Plane  |
+-------+--------+--------+--------+-------+------+--------+
+-----------------------------------------++---------------+
|                                         ||    Reduced    |
|  Movement                               ||      Map      |
|    Map                                  ||    Window     |
|   Window                                |+---------------+
|                                         |+---------------+
|                                         ||     Unit      |
|                                         ||    Window     |
|                                         ||               |
|                                         ||               |
|                                         ||               |
|                                         ||               |
|                                         ||               |
|                                         |+---------------+
|                                         |+ Movement Bar  +
|                                         |+---------------+
|                                         ||  Unit Action  |
|                                         ||    Buttons    |
+-----------------------------------------++---------------+



¿ ~ MainScr_Maps.C ?
¿ all/only Draw Main Movement Map ?
//MoM_Manual: "The main movement view is the large map on the main movement screen"..."the window"

***BEGIN: ovr150***
    *Initialized Data*
        dseg:6FF4 00 00                                           OVL_TileAnim_Stage dw 0                 ; DATA XREF: Draw_Map_Terrain+CAr ...
        dseg:6FF6 00 00                                           OVL_EnchRoad_Stage dw 0                 ; DATA XREF: OVL_StepMapAnims+30r ...
        dseg:6FF8 00 00                                           OVL_NodeSprkl_Stage dw 0                ; DATA XREF: OVL_StepMapAnims+22r ...
        dseg:6FFA 00                                              db    0
        dseg:6FFB 00                                              db    0
        dseg:6FFC 00 00                                           OVL_Anim_Stepper dw 0                   ; DATA XREF: OVL_StepMapAnims+6r ...
        dseg:6FFE FF FF                                           map_draw_prev_x dw 0FFFFh               ; DATA XREF: Reset_Map_Draw+3w ...
        dseg:7000 FF FF                                           map_draw_prev_y dw 0FFFFh               ; DATA XREF: Reset_Map_Draw+9w ...
        dseg:7002 02 00                                           map_draw_sustain dw 2                   ; DATA XREF: OVL_MapStateSave+Fr ...

    *Uninitialized Data*
        dseg:CB56 00 00                                           OVL_MapDrawY_Save dw 0                  ; DATA XREF: OVL_MapStateSave+Cw ...
        dseg:CB58 00 00                                           OVL_MapDrawX_Save dw 0                  ; DATA XREF: OVL_MapStateSave+6w ...
        dseg:CB5A 00 00                                           OVL_NewMapSust_Save dw 0                ; DATA XREF: OVL_MapStateSave+12w ...
        dseg:CB5C 00 00                                           map_draw_full dw 0                      ; DATA XREF: Draw_Map_Window:@@MapMovedw ...

***END: ovr150***



dseg:973E 00 00                                           _minimap_grid_field dw 0                ; DATA XREF: IDK_CityScreen_AddFields+18w ...
dseg:9740 00 00                                           _main_map_grid_field dw 0               ; DATA XREF: IDK_CityScreen_AddFields+12w ...
dseg:9742 00 00                                           _minimap_grid_x dw 0                    ; DATA XREF: Main_Screen+124Fr ...
dseg:9744 00 00                                           _minimap_grid_y dw 0                    ; DATA XREF: Main_Screen+1246r ...
dseg:9746 00 00                                           CRP_OVL_MapWindowY dw 0                 ; DATA XREF: GAME_Overland_Init+E5w ...
dseg:9748 00 00                                           CRP_OVL_MapWindowX dw 0                 ; DATA XREF: GAME_Overland_Init+DFw ...
dseg:974A 00 00                                           _prev_world_y dw 0                      ; DATA XREF: GAME_Overland_Init+F1w ...
dseg:974C 00 00                                           _prev_world_x dw 0                      ; DATA XREF: GAME_Overland_Init+EBw ...
dseg:974E 00 00                                           _main_map_grid_y dw 0                   ; DATA XREF: Main_Screen+C79r ...
dseg:9750 00 00                                           _main_map_grid_x dw 0                   ; DATA XREF: Main_Screen+C87r ...

dseg:BD82 00 00                                           _active_world_y dw 0                    ; DATA XREF: GAME_Overland_Init+127w ...
dseg:BD84 00 00                                           _active_world_x dw 0                    ; DATA XREF: GAME_Overland_Init+11Bw ...
dseg:BD86 00 00                                           _map_plane dw 0                         ; DATA XREF: GAME_Overland_Init+13Fw ...
dseg:BD88 00 00                                           _map_y dw 0                             ; DATA XREF: GAME_Overland_Init+FDw ...
dseg:BD8A 00 00                                           _map_x dw 0                             ; DATA XREF: GAME_Overland_Init:loc_45E57w ...
dseg:BD8C 00 00                                           _human_player_idx dw 0                  ; DATA XREF: WZD_Startup_MainGame+Aw ...








Reset_Map_Draw()  Undef_Prev_Map_Draw_XY()  j_OVL_MapDrawRenew()
map_draw_sustain  OVL_NewMapSustain
map_draw_full
map_draw_prev_x     Map_LastDraw_X
map_draw_prev_y     Map_LastDraw_Y
_prev_world_x       G_OVL_MapDisplay_X
_prev_world_y       G_OVL_MapDisplay_Y





map_draw_sustain
XREF
    Draw_Map_Window+1D        mov     [map_draw_sustain], 1 
    Draw_Map_Window+29        cmp     [map_draw_sustain], 0 
    Draw_Map_Window:loc_D961A dec     [map_draw_sustain]    
    OVL_MapStateRestore+12    mov     [map_draw_sustain], ax
    OVL_MapStateSave+F        mov     ax, [map_draw_sustain]


Reset_Map_Draw  o150p01
XREF
    ...72!!
    ...Main Screen, of course, also City Screen (¿~City Map Window?), but also ¿ *screens* that overlay on the main movement map ?
    + XREFs to Main_Screen_Reset()
    + XREFs to Reset_Active_Stack_Draw()
    ¿ Main_Screen_Reset() calls Reset_Active_Stack_Draw(), but also calls Reset_Map_Draw() ?


City_Screen+102B               
City_Screen+10D                
City_Screen+5E1                
City_Screen+916                
City_Screen+A5B                
City_Screen+E71                
EVNT_MercHireDialog+24         
IDK_Cartographer_Screen+173    
IDK_CityEncht_s4A3F0+45E       
IDK_DrawCityScrn_s48DAF+1A5    
IDK_RoadBuild_s5BD4D+D         
IDK_Spell_Casting_Screen+144   
IDK_Spell_Casting_Screen+421   
IDK_Spell_Casting_Screen+4AF   
IDK_Spell_Casting_Screen+574   
IDK_Spell_Casting_Screen+644   
IDK_Spell_Casting_Screen+6CA   
IDK_Spell_Casting_Screen+7BB   
IDK_Spell_Casting_Screen+840   
IDK_Spell_Casting_Screen+B2    
IDK_Spell_Casting_Screen_Draw+D
IDK_SurveyorDraw_s7A6A9+B1     

Main_Screen+72                 on entry, prep work
Main_Screen+52B                *Center Map*  post  only/also j_MainScr_Prepare_Reduced_Map(), j_Set_Mouse_List_Normal()
Main_Screen+563                *Unselect*  _unit_stack_count = 0; ...; Undef(); also, _DI_IDK_screen_changed = 1
Main_Screen+953                *Plane Button* ~ resets everything
Main_Screen+A88                ¿ Unit Window - Unit View ?  ¿ somehow keeps the whole currently drawn background, but ~ resets everything ?
Main_Screen+D00                Left-Click Movement Map Grid Field && unit_stack_count != 0 && unit_stack_hmoves < 1 ... ¿ ... ?
Main_Screen+1025               Right-Click Movement Map Grid Field - Map Square has Stack (Own) - Build & Select ~ reset everything
Main_Screen+107C               Right-Click Movement Map Grid Field - Map Square has Stack (Others) - Unit List Window ~ reset everything
Main_Screen+1155               Right-Click, entity, city, own, size 0 - Outpost Screen
Main_Screen+11CE               Right-Click, entity, city, enemy - Enemy City Screen
Main_Screen+128A               Left/Right-Click on Reduced Map

Main_Screen_Draw_Do_Draw+3     

Main_Screen_Reset+53           

OVL_DisplayMainScrn+8          
OVL_DrawMapSection+93
OVL_MosaicFlip+8              
OVL_MoveUnitStack+103         
OVL_MoveUnitStack+130         
OVL_MoveUnitStack+4E6         
OVL_MoveUnitStack+603         
OVL_MoveUnitStack+774         
Outpost_Screen+253            

Reset_Active_Stack_Draw:@@Done

Road_Build_Screen+1A6         
Road_Build_Screen+2B6         
Road_Build_Screen+343         
STK_Move:loc_7B8D2            
Surveyor_Screen+12B           
Surveyor_Screen+1C9           
Surveyor_Screen+A1            
USW_HireHero+51               
UU_IDK_Main_Screen_Draw+51    
UU_IDK_Main_Screen_Draw+83    

WIZ_NextIdleStack+1D          

sA9AC5_Events_Anim_Scroll+2C5 
sA9AC5_Events_Anim_Scroll+442 
sA9AC5_Events_Anim_Scroll+5AA 
sAFA06_Anim_EarthLore+D6      
sB1280_Anim_EnchantRoad+14C   
sub_523C6+8B                  
sub_52514:loc_525AC           
sub_52B09+2A                  
sub_AEDB1+1AC                 
sub_AEDB1+DD                  
sub_AFCA8+3B1                 
sub_B0C07+41F                 
sub_B529D+43E                 
sub_B529D+492                 
sub_B529D+61B                 
sub_B8AD0:loc_B8ADD           

