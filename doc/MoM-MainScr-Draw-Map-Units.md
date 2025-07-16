

Is it especially meaningful for draw priority to be 0?
Must be?
    ...Set_Unit_Draw_Priority() checks 0 and sets 1
Reset_Stack_Draw_Priority() sets all units in the active stack to draw priority 0
...so, 0 is equivalent to 'this unit is in the active stack'
...and that is handled sperately, special-like elsewhere

EOD: one unit_idx per stack in entities_on_movement_map[] and that is always drawn by Draw_Map_Units() |-> Draw_Unit_Picture()


entities_on_movement_map[]



                Set_Unit_Draw_Priority();
                Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);

                if(map_moved_flag == ST_TRUE)
                    Set_Unit_Draw_Priority();
                    Reset_Stack_Draw_Priority();
                    Set_Entities_On_Map_Window(l_map_x, l_map_y, map_plane);



Main_Screen()
    |-> Main_Screen_Draw()
        |-> Main_Screen_Draw_Do_Draw()
            |-> Draw_Maps()
                |-> Draw_Map_Window()
                    |-> Draw_Map_Units()

Main_Screen_Draw();
    Main_Screen_Draw_Do_Draw(&_map_x, &_map_y, _map_plane, _prev_world_x, _prev_world_y, _human_player_idx);
        Draw_Maps(MAP_SCREEN_X, MAP_SCREEN_Y, MAP_WIDTH, MAP_HEIGHT, map_x, map_y, map_plane, x_pos, y_pos, player_idx);
            Draw_Map_Window(screen_x, screen_y, map_width, map_height, l_map_x, l_map_y, map_plane);
                Draw_Map_Units(screen_x, screen_y, map_w, map_h, map_draw_curr_x, map_draw_curr_y);
                    Draw_Unit_Picture(itr_screen_x, itr_screen_y, unit_idx, 2);



## entities_on_movement_map[]

MOM_Data.c
// WZD dseg:964C
// MoO2: ~== _ship_node
/*
ST_UNDEFINED or unit_idx or (city_idx + MAX_UNIT_COUNT)
*/
int16_t entities_on_movement_map[(MAP_WIDTH * MAP_HEIGHT)];


Set_Entities_On_Map_Window()
    clears all to -1
    iters over units then cities
    for units
        if owner_idx not -1
        if square scouted
        if same plane or in tower
        if in-view
        checks if a unit has already been placed, adds if draw order is higher
        or adds if draw order is higher than 0
        or adds if draw is 0 but draw active stack is AWLAYS/undefined
    for cities
        if square explored
        if in-view
        adds (city_idx + MAX_UNIT_COUNT)

## Draw_Priority
_unit_type_table[].Melee + _unit_type_table[].Ranged
~ base strength / unit type strength
if 0, then 1
if boat, then 50
if no owner, then -1
if invisible, then -1
...
valid values: -1, 50, or [melee + ranged]
