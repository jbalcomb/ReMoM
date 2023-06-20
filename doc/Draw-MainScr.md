
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



main() |-> Screen_Control() |-> Main_Screen() |-> Main_Screen_Draw()



#### Main_Screen_Draw()
    Reset_Window()
    Set_Page_Off()
    Main_Screen_Draw_Do_Draw(_curr_world_x, _curr_world_y, _world_plane, _prev_world_x, _prev_world_y, _human_player_idx);

#### Main_Screen_Draw_Do_Draw(_curr_world_x, _curr_world_y, _world_plane, _prev_world_x, _prev_world_y, _human_player_idx);

BackGround:
    FLIC_Draw(0, 0, main_background);

Movement Map Window:
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
|                                         ||      Map      |
|                                         ||    Window     |
|                                         |+---------------+
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
|                                         ||      Map      |
|                                         ||    Window     |
|                                         |+---------------+
|                                         |+---------------+
|                                         ||     Unit      |
|                                         ||    Window     |
|                                         ||               |
|                                         ||               |
|                                         ||               |
|                                         ||               |
|                                         ||               |
|                                         |+---------------+
|                                         |+---------------+
|                                         ||  Unit Action  |
|                                         ||    Buttons    |
+-----------------------------------------++---------------+
