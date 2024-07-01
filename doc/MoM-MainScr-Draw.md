
UnitDraw_WorkArea
    UNIT_Draw_UE_Outline+2F8          
    UNIT_Draw_UE_Outline+31B          
    Draw_Unit_StatFig+63              
    Draw_Unit_StatFig+CC              
    Draw_Unit_StatFig:loc_56286       
    Draw_Unit_StatFig+163             
    Draw_Unit_StatFig+19F             
    Draw_Unit_StatFig+1AE             
    Draw_Unit_StatFig+1EA             
    Draw_Unit_StatFig:@@Do_DrawPicture
    Draw_Unit_StatFig+207             
    UNIT_Draw+B2                      
    UNIT_Draw+DE                      
    UNIT_Draw+106                     
    UNIT_Draw+130                     
    UNIT_Draw:loc_753BF               
    UNIT_Draw+15B                     
    Allocate_Data_Space+3A4           



... Window (GP,MP)
(PS. MoO2 *gold* = "BC")

Draw Unit Window
Draw *Movement Bar*

if(_unit_stack_count > 0)
    Main_Screen_Draw_Unit_Window(_unit_window_start_x, _unit_window_start_y)
    Main_Screen_Draw_Unit_Action_Buttons()  o57p074

Unit_Window_Draw_Unit_Attributes(x1,y1,unit_idx)



Cycle_Unit_Enchantment_Animation()
unit_weapon_type_animation_count++;
if(unit_weapon_type_animation_count > 3)
    unit_weapon_type_animation_count = 0;



    flag: {0,1,2,3}
        0, 1 - status text, partol grayscaling, and unit enchantment outlines
        2 - no status text
        3 - no status text or patrol grayscaling
        4+: none of the above
Draw_Unit_StatFig()
    FLIC_Set_CurrentFrame(_unit_type_table[unit_type_idx].pict_seg, 0);
    FLIC_Set_CurrentFrame(_unit_type_table[unit_type_idx].pict_seg, 1);
    Draw_Picture_To_Bitmap(_unit_type_table[unit_type_idx].pict_seg, UnitDraw_WorkArea);
    Replace_Color(UnitDraw_WorkArea, itr_color_remap + 214, COL_Banners[ ((banner_idx * 5) + itr_color_remap) ]);
    if(flag == 0)
    {

    }
    else
    {

    }
    ...
        j_UNIT_HasInvisibility() ==/!= ST_FALSE/ST_TRUE
        Replace_Color_All()
        Outline_Bitmap_Pixels_()
        _UNIT[].Status  ==/!= US_Patrol
        unit_owner_idx ==/!= _human_player_idx
        LBX_IMG_Grayscale()
        UNIT_Draw_UE_Outline()
    ...
    Draw_Picture()  // MoO2  Draw(x,y,_ship_bitmap)
    So, checked flag for 0 or 1, though to no effect...
    Then, checked flag for 0,1,2,3? ... 

    Invisible is NOT only for current/human player, just Unit Has Invisibility, and flag == {0,1,2,3}
    Gray-Scaled is only for current/human player, and flag != 3
    Status Mark is only for current/human player, and flag == {0,1}


Draw_Unit_StatFig()
XREF:
    j_Draw_Unit_StatFig()
    Draw_Unit_Picture()

j_Draw_Unit_StatFig()
XREF:
    IDK_DrawEnemyCityWindow_s4A90D()
    IDK_ArmiesScreen_s58CD2()
    Outpost_Screen_Draw()

Draw_Unit_Picture()
XREF:
    j_Draw_Unit_Picture()

j_Draw_Unit_Picture()
XREF:
    sub_49E50()
    Main_Screen_Draw_Unit_Window()
    Draw_Active_Unit_Stack()
    Draw_Unit_List_Window_Pup()
    OVL_MoveUnitStack()
    Draw_Map_Units()


IDK_DrawEnemyCityWindow_s4A90D()
    calls j_Draw_Unit_StatFig() with flag = 1
IDK_ArmiesScreen_s58CD2()
    calls j_Draw_Unit_StatFig() with flag = 1
Outpost_Screen_Draw()
    calls j_Draw_Unit_StatFig() with flag = 1

Draw_Unit_Picture()
    calls Draw_Unit_StatFig() with the flag passed through

sub_49E50()
    calls j_Draw_Unit_Picture() with flag = 1

Main_Screen_Draw_Unit_Window()
    calls j_Draw_Unit_Picture() with flag = _unit_stack[].active
Draw_Active_Unit_Stack()
    calls j_Draw_Unit_Picture() with flag = 2
Draw_Unit_List_Window_Pup()
    calls j_Draw_Unit_Picture() with flag = (unit_owner_idx = current/human_player_idx ? 1 : 3)  (turns off gray-scale for Enemy Unit List Window)
OVL_MoveUnitStack()
    calls j_Draw_Unit_Picture() with flag = 2
Draw_Map_Units()
    calls j_Draw_Unit_Picture() with flag = 2


Armies Screen Unit Figures includes invisible, gray-scaled, and status mark
"G" for Go-To, "B" for Build-Road, "P" for Purify-Corruption

Does it just not for the Enemy Unit List Window and Enemy City Screen?
(is there an enemy outpost screen?)





Draw Maps
* Draw Movement Map
    * Main Screen
    * City Screen
* Draw Reduced Map
    * Main Screen
    * Cities Screen
    * Armies Screen
~ Draw Cartographer Screen

Draw Main Screen Movement Map
    Units
    Cities
    Terrain
    Terrain Specials
    ¿ Towers of Wizardy, Monster Lairs ?
    Nodes
    Roads
    Unexplored Area
    ¿ Corruption ?



Draw Units
world square is explored
world square is scouted
300 byte bit-field for scouted
updated in Next_Turn_Proc() and Move_Stack()
if Explored, then Scouted
Per City - Scounting Range
Per Unit - Scouting Range
-Scouting Ability
-Patrolling Status (+1 scouting ability)
-Scouting Range
--1 for walking units
--2 for flying units
Special Unit Abilities - Scouting
"Increases the range a unit can see overland by the ability's level. Flying units normal have a scouting levell of two, others have a level of one."
Standard and Race-Specific Units
Creature    Special Abilities
Rangers     Scouting (Range 2)
Building Types
Building Type   Effects
City Walls      Detect enemy units up to 3 squares away
Oracle          Detects enemu units up to 4 squares away
Master of Magic Spellbook
Spell Name:                     Nature's Eye
Necessary Magic Realm:          Nature
Category of Effect:             City Enchantment
Casting Cost:                   75 mana
Upkeep Cost:                    1 mana/turn
Degree of Rarity:               Uncommon
Description of Spell's Effect:  Extends the scouting range of a friendly target city to five squares in any direction, 
                                revealing all lands and all non-invisible enemy troops within that radius.






Draw_Maps()
    Draw_Minimap()
    Draw_Map_Window()
        Draw_Map_Terrain()
        Draw_Map_Minerals()
        Draw_Map_Terrain_Specials()
        Draw_Map_Roads()
        Draw_Map_Cities()
        Draw_Map_Towers()  /* Towers of Wizardry */
        Draw_Map_Lairs()  /* Monster Lairs - Ancient Ruins, Ancient Temples, Underround Lairs */
        Draw_Map_Units()
        Draw_Map_Nodes()
        Draw_Map_Unexplored_Area()
        Cycle_Map_Animations()
    Draw_Active_Unit_Stack()


OVL_DrawMinerals
Draw_Map_Minerals

OVL_DrawTerrSpecials
Draw_Map_Terrain_Specials
corruption, wild games, and nightshades

OVL_DrawRoads
Draw_Map_Roads

OVL_DrawCities
Draw_Map_Cities

OVL_DrawTowers
Draw_Map_Towers

OVL_DrawEncounters
Draw_Map_Lairs

OVL_DrawUnits
Draw_Map_Units

OVL_DrawNodeFX
Draw_Map_Nodes

OVL_DrawScouting
Draw_Map_Unexplored_Area

OVL_StepMapAnims
Cycle_Map_Animations
    ...
    OVL_Anim_Stepper = (1 - OVL_Anim_Stepper)
    OVL_TileAnim_Stage
    OVL_NodeSprkl_Stage
    OVL_EnchRoad_Stage


¿ Conclusions ?

    x,y of movement map
    x,y of the map and x,y of *target*

    ¿ game code tracks curr/prev to decide if it needs to call out ?
    ¿ engine code tracks curr/prev to decide if it needs to draw ?


¿ Use-Case ?

    Right-Click - Move Map
        Mouse X,Y => Grid Field X,Y => World X,Y  ¿ mx,my; gx,gy; wx,wy ?


`Center_Map`            Center_Map(int16_t * map_x, int16_t * map_y, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane)

    uses the world x,y to calculate the top-left corner coordinates for the movement map

    ~ *map_x = world_x - (MAP_WIDTH/2)
    ~ *map_y = world_y - (MAP_HEIGHT/2)

    _prev_world_x,y = map_x,y

    passes to Set_Entities_On_Map_Window()


GAME_Overland_Init()
    ...
    _prev_world_x = 0
    _prev_world_y = 0
    _map_x = 0
    _map_y = 0
    ...
    _active_world_x = _FORTRESSES[0].world_x
    _active_world_y = _FORTRESSES[0].world_y
    _map_plane = _FORTRESSES[0].world_plane
    ...
    Center_Map(&_map_x, &_map_y, _FORTRESSES[0].world_x, _FORTRESSES[0].world_y, _map_plane)
    ...
    WIZ_NextIdleStack(_human_player_idx, _map_x, _map_y, _map_plane)


Main_Screen() 'C'
    ...
    Center_Map(&_map_x, &_map_y, _UNITS[unit_idx].world_x, _UNITS[unit_idx].world_y, _UNITS[unit_idx].world_plane)
    ...
    Reset_Map_Draw()
        map_draw_prev_x = ST_UNDEFINED_DW
        map_draw_prev_y = ST_UNDEFINED_DW


IDK_CheckSet_MapDisplay_XY()
    bounds Y
    wraps X
    updates _prev_world_x,y and _map_x

Main_Screen_Draw() |->
Main_Screen_Draw_Do_Draw(&_map_x, &_map_y, _map_plane, _prev_world_x, _prev_world_y, _human_player_idx)

So, ...
    for Center_Map()
        it uses world x,y to set map x,y
        also, sets _prev_world_x,y to be the same as map_x,y
        the world x,y here is that of the stack on which it wants to center the movement map
        with the (-half), it is setting map x,y for the top-left corner world coordinates to draw the movement map
        What is the impact of setting _prev_world_x,y to be the same as map_x,y?
            ?
    for right-click Scroll Map
        it sets _prev_world_x,y to _main_map_grid_x,y (-half)
        no change to _map_x,y
        so, _prev_world_x,y is also the top-left corner of the movement map
        and, here, _main_map_grid_x,y is equivalent to the Unit world x,y in Center_Map()
        So, the difference is in not setting _map_x,y

                _prev_world_x = _main_map_grid_x - (MAP_WIDTH / 2);  // ¿ grid x - (map width / 2) = map x ?
                _prev_world_y = _main_map_grid_y - (MAP_HEIGHT / 2);  // ¿ grid y - (map height / 2) = map y ?

Main_Screen_Draw()
Main_Screen_Draw_Do_Draw()

DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 2006]: END: Main_Screen_Draw_Do_Draw()
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1650]: before_map_x: 26
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1651]: before_map_y: 0
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1652]: after_map_x: 27
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1653]: after_map_y: 1
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1657]: _map_x: 27
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1658]: _map_y: 1
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1659]: _map_plane: 0
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1660]: _prev_world_x: 54
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1661]: _prev_world_y: 4
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1662]: _human_player_idx: 0
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1666]: END: Main_Screen_Draw()

The impact from changing _prev_world_x,y is in the path from Main_Screen_Draw() through Main_Screen_Draw_Do_Draw()
    if _map_x,y is not the same as _prev_world_x,y
        it changes _map_x,y to be 1 square closer to _prev_world_x,y

is _prev_world_x,y only job to facilitate the map scrolling?

didn't I see code somewhere on the game side that skips changing the coords if they are the same as last time?
like, right-clicking in the same spot is a No-Op?

this would be equivalent to the code on the engine side that does not do a full draw if the map hasn't moved
    with the caveat that there are a lot of calls around to force a full draw,
        presumably because that chunk of code feels that it has changed the state of the something that impacts the movement map

Back to the business at-hand, ...
    for my current purposes - right-click scroll map -
    I need to *see* the right-click mouse x,y to screen x,y to movement map window x,y to world x,y to map TL x,y
    So, ...
        what should mouse x,y be?
            starts at 158,100
            if I click on the bottom-left-most square, it should be ~ {0,182} ... {19,199}  >=(0+(20 * 0)+0) <(0+(20 * 0)+20), >=(20+(18 * 9)+0) <(20+(18 * 9)+18)
        where can I *see* this?
            Interpret_Mouse_Input()
                Mouse_Movement_Handler()
                    MD_GetButtonStatus()
                    Pointer_X()
                    Pointer_Y()
        Should change until after the call to Mouse_Button_Handler()?


Going into Main_Screen(), values are correct - as set by MoM_Main()/game initialization...
    DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 771]: _map_x: 18
    DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 772]: _map_y: 11
    DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 773]: _prev_world_x: 18
    DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 774]: _prev_world_y: 11
    DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 775]: _main_map_grid_x: 0
    DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 776]: _main_map_grid_y: 0


values are correct and holding throughout Interpret_Mouse_Input()
DEBUG: [C:\devel\STU-MoM_Rasm\src\Input.C, 344]: Mouse_Movement_Handler()
DEBUG: [C:\devel\STU-MoM_Rasm\src\Input.C, 345]: MD_GetButtonStatus(): 2
DEBUG: [C:\devel\STU-MoM_Rasm\src\Input.C, 346]: Pointer_X(): 6
DEBUG: [C:\devel\STU-MoM_Rasm\src\Input.C, 347]: Pointer_Y(): 192
...
...
...
DEBUG: [C:\devel\STU-MoM_Rasm\src\Input.C, 510]: MD_GetButtonStatus(): 2
DEBUG: [C:\devel\STU-MoM_Rasm\src\Input.C, 511]: Pointer_X(): 6
DEBUG: [C:\devel\STU-MoM_Rasm\src\Input.C, 512]: Pointer_Y(): 192
DEBUG: [C:\devel\STU-MoM_Rasm\src\Input.C, 513]: Mouse_Button_Handler()


Point to Screen to Field to Grid is correct
DEBUG: [C:\devel\STU-MoM_Rasm\src\Input.C, 344]: Mouse_Movement_Handler()
DEBUG: [C:\devel\STU-MoM_Rasm\src\Input.C, 345]: MD_GetButtonStatus(): 2
DEBUG: [C:\devel\STU-MoM_Rasm\src\Input.C, 346]: Pointer_X(): 6
DEBUG: [C:\devel\STU-MoM_Rasm\src\Input.C, 347]: Pointer_Y(): 192

DEBUG: [C:\devel\STU-MoM_Rasm\src\Fields.C, 805]: screen_x: 6
DEBUG: [C:\devel\STU-MoM_Rasm\src\Fields.C, 806]: screen_y: 192
DEBUG: [C:\devel\STU-MoM_Rasm\src\Fields.C, 808]: p_fields[field_num].x1: 0
DEBUG: [C:\devel\STU-MoM_Rasm\src\Fields.C, 809]: p_fields[field_num].y1: 20
DEBUG: [C:\devel\STU-MoM_Rasm\src\Fields.C, 811]: field_x: 6
DEBUG: [C:\devel\STU-MoM_Rasm\src\Fields.C, 812]: field_y: 172
DEBUG: [C:\devel\STU-MoM_Rasm\src\Fields.C, 814]: p_fields[field_num].Param1: 20
DEBUG: [C:\devel\STU-MoM_Rasm\src\Fields.C, 815]: p_fields[field_num].Param2: 18
DEBUG: [C:\devel\STU-MoM_Rasm\src\Fields.C, 817]: grid_x: 0
DEBUG: [C:\devel\STU-MoM_Rasm\src\Fields.C, 818]: grid_y: 9

DEBUG: [C:\devel\STU-MoM_Rasm\src\Fields.C, 853]: *((int64_t *)p_fields[field_num].Param3): 0
DEBUG: [C:\devel\STU-MoM_Rasm\src\Fields.C, 854]: *((int64_t *)p_fields[field_num].Param4): 9


_main_map_grid_x,y is the value as set by Push_Down_Field(), by way of the memory address in Param3,4 members of the Grid Field
so, shouldn't translating that into _prev_world_x,y includ translatin the map square coordinats to world square coordinates?
but, _main_map_grid_x,y is what is used to index into _entities_on_movement_map[], so it should already be in world square coordinates?
No, entities_on_movement_map[] is entities_on_movement_map[(MAP_WIDTH*MAP_HEIGHT)].
...
It sets _prev_world_x,y to _main_map_grid_x,y (-half) and calls j_IDK_CheckSet_MapDisplay_XY()
So, IDK_CheckSet_MapDisplay_XY() knows that _prev_world_x,y needs translated from map squares to world squares?

Alright. So, the map was at 18,11 and I right-clicked at 0,9...
0,9 (-half) = -6,4
This'd be translating from the clicked map square to where the top-left should end up
e.g., treats the clicked map square as the new map center
Seems like you'd need the current map TL to translate that into world squares / new map TL
the existing center would be map TL + half ... 18,11 + 6,5 = 24,16
map TL x,y + 0,9 would be the new center
the new map TL would map TL x,y + 0,9 - 6,5  AKA map TL x,y + -6,4
...
Doh!!
The code in Main_Screen() for 'Scroll Map' is not assigning to _prev_world_x,y, it is ADDing to _prev_world_x,y.


_map_x: 18
_map_y: 11

_main_map_grid_x: 0
_main_map_grid_y: 9

_prev_world_x: 12  =  18 + -6  =  18 + (0 - (12/2))
_prev_world_y: 15  =  11 +  4  =  11 + (9 - (10/2))




DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1253]: ScrollTheMap
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1259]: _main_map_grid_x: 0
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1260]: _main_map_grid_y: 9
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1261]: _main_map_grid_x - (MAP_WIDTH / 2): -6
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1262]: _main_map_grid_y - (MAP_HEIGHT / 2): 4

DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1263]: _prev_world_x: -6
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1264]: _prev_world_y: 4
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1265]: _map_x: 18

DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1269]: _prev_world_x: 54
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1270]: _prev_world_y: 4
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1271]: _map_x: 18

DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1636]: BEGIN: Main_Screen_Draw()
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1643]: _map_x: 18
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1644]: _map_y: 11
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1645]: _map_plane: 0
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1646]: _prev_world_x: 54
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1647]: _prev_world_y: 4











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

¿ same code used to draw the "City Map Window" ?

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

