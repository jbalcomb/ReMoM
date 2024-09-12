/*
    WIZARDS.EXE
        ovr069
    
    Roads

*/

#include "MoX.H"
#include "MainScr_Maps.H"



// WZD dseg:327E BEGIN:  ovr069 - Initialized Data  (Road Build)

// WZD dseg:327E
char str_empty_string__ovr069[] = "";

// WZD dseg:327F
char str_hotkey_C__ovr069[] = "C";

// WZD dseg:3281
char str_hotkey_O__ovr069[] = "O";

// WZD dseg:3283 
char str_Road[] = "Road";

// WZD dseg:3288 
char str_Building[] = "Building";

// WZD dseg:3291
char str_Blocked_RoadsCanNot[] = "Blocked.  Roads can not be built across large bodies of water.";

// WZD dseg:32D0
char str_ItWillTake[] = "It will take ";

// WZD dseg:32DE
char str_Turn_2[] = " turn";

// WZD dseg:32E4
char str_S_4[] = "s";

// WZD dseg:32E6
char str_ToCompleteTheConstr[] = " to complete the construction of this road.";

// WZD dseg:32E6 END:  ovr069 - Initialized Data  (Road Build)



// WZD dseg:C128 BEGIN:  ovr069 - Uninitialized Data  (Roads)

// WZD dseg:C128
int16_t m_construction_points;

// WZD dseg:C12A
int16_t m_road_builder_unit_idx;

// WZD dseg:C12C
int16_t roadbuild_ok_button_field;

// WZD dseg:C12E
// MoO2  Module: MOX  _multi_colored_line_start
int16_t m_multi_colored_line_start;

// WZD dseg:C130
int16_t main_cancel_button_field;

// WZD dseg:C132
int16_t m_road_path_length;

// WZD dseg:C134
int16_t m_turns_to_build_road;

// WZD dseg:C136
int16_t m_road_dst_y;

// WZD dseg:C138
int16_t m_road_dst_x;

// WZD dseg:C13A
int16_t unit_wy;

// WZD dseg:C13C
int16_t unit_wx;

// WZD dseg:C13C END:  ovr069 - Uninitialized Data  (Roads)



/*
    WIZARDS.EXE ovr069
*/

// WZD o069p01
void Road_Build_Screen(void)
{
    int16_t roadbuilders[MAX_STACK];
    int16_t turns_to_build_road;
    int16_t roadbuilder_count;
    int16_t target_wy;
    int16_t target_wx;
    int16_t itr_stack;
    int16_t scanned_field;
    int16_t leave_screen;
    int16_t input_field_idx;  // _DI_
    int16_t unit_idx;  // _SI_

    m_road_path_length = 0;

    m_turns_to_build_road = 0;

    m_multi_colored_line_start = 0;

    m_construction_points = 0;

    unit_idx = _unit_stack[0].unit_idx;

    unit_wx = _UNITS[unit_idx].wx;
    unit_wy = _UNITS[unit_idx].wy;

    if(Terrain_Is_Sailable(unit_wx, unit_wy, _UNITS[unit_idx].wp) == ST_TRUE)
    {
        return;
    }

    m_road_builder_unit_idx = unit_idx;

    for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
    {
        unit_idx = _unit_stack[itr_stack].unit_idx;

        if(_unit_type_table[_UNITS[_unit_stack[itr_stack].unit_idx].type].Construction > 0)
        {
            m_construction_points += _unit_type_table[_UNITS[unit_idx].type].Construction;
            m_road_builder_unit_idx = unit_idx;
            if(Unit_Has_Endurance(unit_idx) == ST_TRUE)
            {
                m_construction_points += 1;
                m_road_builder_unit_idx = unit_idx;
            }
        }
    }


    turns_to_build_road = Turns_To_Build_Road(unit_wx, unit_wy, _map_plane);
    // NOTE: can't be on ocean, so no check for -1
    if(m_construction_points > 0)
    {
        turns_to_build_road /= m_construction_points;
        SETMIN(turns_to_build_road, 1);
    }


    m_turns_to_build_road = turns_to_build_road;

    m_road_dst_x = unit_wx;
    m_road_dst_y = unit_wy;

    Reset_Draw_Active_Stack();

    Set_Outline_Color(0);

    Deactivate_Auto_Function();

    Assign_Auto_Function(Road_Build_Screen_Draw, 1);

    _reduced_map_seg = Allocate_First_Block(_screen_seg, 153);

    GUI_String_1 = (char *)Near_Allocate_First(100);

    GUI_String_2 = (char *)Near_Allocate_Next(100);

    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);

    Reset_Map_Draw();

    MainScr_Prepare_Reduced_Map();

    Deactivate_Help_List();

    Set_Road_Build_Screen_Help_List();

    Set_Input_Delay(1);

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {

        Mark_Time();

        Clear_Fields();

        Road_Build_Screen_Add_Fields();

        input_field_idx = Get_Input();

        scanned_field = Scan_Input();


        if(input_field_idx == main_cancel_button_field)
        {
            Play_Left_Click__STUB();
            current_screen = scr_Main_Screen;
            leave_screen = ST_UNDEFINED;
        }


        if(input_field_idx == roadbuild_ok_button_field)
        {
            Play_Left_Click__STUB();
            if(m_turns_to_build_road > 0)
            {
                Clear_Fields();
                Deactivate_Auto_Function();
                Active_Stack_Roadbuilders(&roadbuilder_count, &roadbuilders[0]);
                if(roadbuilder_count > 0)
                {
                    Set_Army_Road_Building(roadbuilder_count, &roadbuilders[0], m_road_dst_x, m_road_dst_y);
                    RdBd_UNIT_MoveStack__WIP(_human_player_idx, roadbuilders[0], m_road_dst_x, m_road_dst_y, &_map_x, &_map_y, _map_plane);
                    WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);
                }
            }
            current_screen = scr_Main_Screen;
            leave_screen = ST_UNDEFINED;
        }


        if(input_field_idx == _main_map_grid_field)
        {
            Play_Left_Click__STUB();
            target_wx = ((_map_x + _main_map_grid_x) % WORLD_WIDTH);
            target_wy = (_map_y + _main_map_grid_y);
            Reset_Map_Draw();
            Road_Build_Path(target_wx, target_wy);
        }


        if(input_field_idx == _minimap_grid_field)
        {
            /*
                Main_Screen()
                BEGIN: Reduced Map Grid Field
                Play_Left_Click__STUB();
                Reduced_Map_Coords(&target_world_x, &target_world_y, ((_map_x + (MAP_WIDTH / 2)) % WORLD_WIDTH), (_map_y + (MAP_HEIGHT / 2)), REDUCED_MAP_WIDTH, REDUCED_MAP_HEIGHT);
                _prev_world_x = _minimap_grid_x + target_world_x;
                _prev_world_y = _minimap_grid_y + target_world_y;
                _map_x = _prev_world_x;
                _map_y = _prev_world_y;
                Center_Map(&_map_x, &_map_y, _prev_world_x, _prev_world_y, _map_plane);
            */
            Play_Left_Click__STUB();
            Reduced_Map_Coords(&target_wx, &target_wy, ((_map_x + (MAP_WIDTH / 2)) % WORLD_WIDTH), (_map_y + (MAP_HEIGHT / 2)), REDUCED_MAP_WIDTH, REDUCED_MAP_HEIGHT);
            _prev_world_x = (_minimap_grid_x + target_wx);
            _prev_world_y = (_minimap_grid_y + target_wy);
            _map_x = _prev_world_x;
            _map_y = _prev_world_y;
            Center_Map(&_map_x, &_map_y, _prev_world_x, _prev_world_y, _map_plane);
            MainScr_Prepare_Reduced_Map();
            Set_Mouse_List_Default();
            Reset_Map_Draw();
        }


        if(-(_main_map_grid_field) == input_field_idx)
        {
            _prev_world_x = (_main_map_grid_x - (MAP_WIDTH / 2));
            _prev_world_y = (_main_map_grid_y - (MAP_HEIGHT / 2));
            IDK_CheckSet_MapDisplay_XY();
        }


        if(leave_screen == ST_FALSE)
        {
            Road_Build_Screen_Draw();
            PageFlip_FX();
            Release_Time(1);
        }
    }

    Clear_Fields();
    Deactivate_Auto_Function();
    Deactivate_Help_List();
    Reset_Window();

}


// WZD o069p02
void Road_Build_Screen_Add_Fields(void)
{

    _main_map_grid_field = INVALID_FIELD;
    _minimap_grid_field = INVALID_FIELD;
    roadbuild_ok_button_field = INVALID_FIELD;

    // TODO  CRP_OVL_UU_Control_4 = INVALID_FIELD;
    // TODO  CRP_OVL_UU_Control_3 = INVALID_FIELD;
    // TODO  CRP_OVL_UU_Control_2 = INVALID_FIELD;
    // TODO  CRP_OVL_UU_Control_1 = INVALID_FIELD;

    if(
        (_map_x == _prev_world_x)
        &&
        (_map_y == _prev_world_y)
    )
    {
        _main_map_grid_field = Add_Grid_Field(0, 20, 20, 18, 12, 10, &_main_map_grid_x, &_main_map_grid_y, ST_UNDEFINED);
    }

    if(
        (_map_x == _prev_world_x)
        &&
        (_map_y == _prev_world_y)
    )
    {
        _minimap_grid_field = Add_Grid_Field(251, 21, 1, 1, 58, 30, &_minimap_grid_x, &_minimap_grid_y, ST_UNDEFINED);
    }

    main_cancel_button_field = Add_Button_Field(280, 181, str_empty_string__ovr069, cast_cancel_button, str_hotkey_C__ovr069[0], ST_UNDEFINED);

    roadbuild_ok_button_field = Add_Button_Field(246, 181, str_empty_string__ovr069, road_ok_button, str_hotkey_O__ovr069[0], ST_UNDEFINED);

}


// WZD o069p03
void Road_Build_Screen_Draw(void)
{

    Reset_Window();

    Set_Page_Off();

    Reset_Map_Draw();

    Draw_Maps(MAP_SCREEN_X, MAP_SCREEN_Y, MAP_WIDTH, MAP_HEIGHT, &_map_x, &_map_y, _map_plane, _prev_world_x, _prev_world_y, _human_player_idx);

    FLIC_Draw(0, 0, main_background);

    Draw_World_Window(REDUCED_MAP_SCREEN_X, REDUCED_MAP_SCREEN_Y, REDUCED_MAP_WIDTH, REDUCED_MAP_HEIGHT);

    Main_Screen_Draw_Game_Buttons();

    Main_Screen_Draw_Status_Window();

    Draw_Road_Building_Window();

    Draw_Road_Build_Path_Line();

}


// WZD o069p04
// drake178: sub_5BDCE()
void Draw_Road_Building_Window(void)
{
    uint8_t colors[4];

    FLIC_Draw(242, 173, deselect_button_blockout);

    FLIC_Draw(240, 76, road_background);

    FLIC_Draw(240, 173, road_button_border);

    Set_Font_Style_Shadow_Down(1, 0, 0, 0);

    Set_Outline_Color(166);

    colors[0] = 8;
    colors[1] = 161;
    colors[2] = 161;

    Set_Font_Colors_15(1, &colors[0]);

    Set_Font_Style_Shadow_Down(4, 0, 0, 0);  // BUG  sets special colors but doens't use them

    Set_Outline_Color(148);

    Print_Centered(280, 80, str_Road);  // "Road"

    Print_Centered(280, 90, str_Building);  // "Building"

    colors[0] = 8;
    colors[1] = 161;
    colors[2] = 161;

    Set_Font_Colors_15(1, &colors[0]);

    Set_Font_Style_Shadow_Down(1, 15, 0, 0);

    Set_Outline_Color(148);

    strcpy(GUI_String_1, str_empty_string__ovr069);

    if(m_turns_to_build_road == ST_UNDEFINED)
    {
        strcpy(GUI_String_1, str_Blocked_RoadsCanNot);  // "Blocked.  Roads can not be built across large bodies of water."
        
    }
    else
    {
        if(m_turns_to_build_road > 0)
        {
            strcpy(GUI_String_1, str_ItWillTake);  // "It will take "
            itoa(m_turns_to_build_road, GUI_String_2, 10);
            strcat(GUI_String_1, GUI_String_2);
            strcat(GUI_String_1, str_Turn_2);  // " turn"
            if(m_turns_to_build_road > 1)
            {
                strcat(GUI_String_1, str_S_4);  // "s"
            }
            strcat(GUI_String_1, str_ToCompleteTheConstr);  // " to complete the construction of this road.""
        }
    }

    Print_Paragraph(250, 106, 60, GUI_String_1, 0);

}


// WZD o069p05
// sub_5BF7F()
void Draw_Road_Build_Path_Line(void)
{
    int16_t skip_draw_path_line;
    int16_t map_square_center_y_offset;
    int16_t map_square_center_x_offset;
    uint8_t color_start_palette_idx;  /* Dasm shows cast */
    // 2-byte aligment padding
    int16_t color_count;
    uint8_t color_array[8];  /* MoO2  8 words */
    int16_t screen_squares_y;
    int16_t screen_squares_x;
    int16_t cur_y;
    int16_t y1;
    int16_t x1;
    int16_t y2;
    int16_t itr;
    int16_t cur_x;  // _SI_
    int16_t x2;  // _DI_

    Set_Window(MAP_SCREEN_X1, MAP_SCREEN_Y1, MAP_SCREEN_X2, MAP_SCREEN_Y2);

    if(m_turns_to_build_road == ST_UNDEFINED)
    {
        color_start_palette_idx = ROAD_BUILD_RED;
    }
    else
    {
        color_start_palette_idx = ROAD_BUILD_BLUE;
    }

    color_count = 4;

    for(itr = 0; itr < color_count; itr++)
    {
        color_array[itr]  = (color_start_palette_idx + itr);
    }

    map_square_center_x_offset = (SQUARE_WIDTH / 2);

    map_square_center_y_offset = (SQUARE_HEIGHT / 2);

    screen_squares_x = (unit_wx - _map_x);  // count of map squares to calculate screen x

    if(screen_squares_x < -(WORLD_WIDTH / 2))
    {
        screen_squares_x += WORLD_WIDTH;
    }

    screen_squares_y = (unit_wy - _map_y);

    x1 = (screen_squares_x * SQUARE_WIDTH);
    y1 = (MAP_SCREEN_Y1 + (screen_squares_y * SQUARE_HEIGHT));

    x1 += map_square_center_x_offset;
    y1 += map_square_center_y_offset;

    for(itr = 0; itr < m_road_path_length; itr++)
    {

        cur_x = movepath_x_array[(2 + itr)];
        cur_y = movepath_y_array[(2 + itr)];

        cur_x -= _map_x;

        if(cur_x < -(WORLD_WIDTH / 2))
        {
            cur_x += WORLD_WIDTH;
        }

        // DEDU  What does it mean for cur_x to be 29, such that it would not draw the path line?
        skip_draw_path_line = ST_FALSE;
        if(cur_x == ((WORLD_WIDTH / 2) - 1))
        {
            skip_draw_path_line = ST_TRUE;
        }

        cur_y -= _map_y;

        x2 = (cur_x * SQUARE_WIDTH);

        y2 = (MAP_SCREEN_Y1 + (cur_y * SQUARE_HEIGHT));

        x2 += map_square_center_x_offset;

        y2 += map_square_center_y_offset;

        if(skip_draw_path_line == ST_FALSE)
        {
            Clipped_Multi_Colored_Line(x1, y1, x2, y2, &color_array[0], color_count, m_multi_colored_line_start);
        }

        x1 = x2;
        y1 = y2;
    }

    m_multi_colored_line_start--;
    if(m_multi_colored_line_start < 0)
    {
        m_multi_colored_line_start = (color_count - 1);
    }

}


// WZD o069p06
/*
    only called for left-click on main map grid field
    sets m_road_dst_x,y
    sets m_road_path_length
    sets or clears m_turns_to_build_road

In OG-MoM, this gets you a blue or red line, the turn count, and a message for turns to complete or blocked.

*/
void Road_Build_Path(int16_t x2, int16_t y2)
{
    int16_t zz_road_build_moves2_max__2;
    int16_t zz_road_build_moves2_max__1;
    int16_t zz_l_turns_to_build_road;
    int16_t turns_to_build_road;  // _SI_
    int16_t itr;  // _DI_

    if(TBL_Scouting[((_map_plane * WORLD_SIZE) + (y2 * WORLD_WIDTH) + x2)] == ST_FALSE)
    {
        return;
    }

    m_road_path_length = Path_Wrap(unit_wx, unit_wy, x2, y2, &movepath_x_array[2], &movepath_y_array[2], WORLD_WIDTH);

    SETMAX(m_road_path_length, 10);

    if(m_road_path_length > 0)
    {
        m_road_dst_x = movepath_x_array[(1 + m_road_path_length)];
        m_road_dst_y = movepath_y_array[(1 + m_road_path_length)];
    }
    else
    {
        m_road_dst_x = unit_wx;  // dst = src = cur
        m_road_dst_y = unit_wy;
    }

    zz_road_build_moves2_max__1 = Road_Build_Moves2_Max();

    zz_road_build_moves2_max__2 = zz_road_build_moves2_max__1;

    zz_l_turns_to_build_road = 0;

    m_turns_to_build_road = 0;

    turns_to_build_road = Turns_To_Build_Road(unit_wx, unit_wy, _map_plane);

    if(m_construction_points > 0)
    {
        turns_to_build_road /= m_construction_points;
        SETMIN(turns_to_build_road, 1);
    }

    m_turns_to_build_road = turns_to_build_road;

    for(itr = 0; itr < m_road_path_length; itr++)
    {
        turns_to_build_road = Turns_To_Build_Road(movepath_x_array[(2 + itr)], movepath_y_array[(2 + itr)], _map_plane);

        if(turns_to_build_road < 0)
        {
            m_road_path_length = (itr + 1);
            zz_l_turns_to_build_road = ST_UNDEFINED;
            m_turns_to_build_road = ST_UNDEFINED;
            break;
        }
        else
        {
            if(m_construction_points > 0)
            {
                turns_to_build_road /= m_construction_points;
                SETMIN(turns_to_build_road, 1);
            }

            if((m_turns_to_build_road + turns_to_build_road) > 120)  /* � MAX_MOVE_PATH_LENGTH = (2 * WORLD_WIDTH) ? */
            {
                m_road_path_length = itr;
                break;
            }
            else
            {
                m_turns_to_build_road += turns_to_build_road;
            }
        }
    }

}


// WZD o069p07
int16_t Road_Build_Moves2_Max(void)
{
    int16_t distance;  // _SI_
    int16_t unit_idx;  // _DI_
    int16_t itr_stack;  // _DX_

    distance = 1000;

    for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
    {
        unit_idx = _unit_stack[itr_stack].unit_idx;
        if(
            (_unit_stack[itr_stack].active == ST_TRUE)
            &&
            (_UNITS[unit_idx].Finished != ST_FALSE)
        )
        {
            if(_UNITS[unit_idx].moves2_max < distance)
            {
                distance = _UNITS[unit_idx].moves2_max;
            }
        }
    }

    if(distance == 1000)
    {
        distance = 0;
    }

    return distance;
}

// WZD o069p08
void Active_Stack_Roadbuilders(int16_t * troop_count, int16_t troops[])
{
    int16_t itr_stack;  // _SI_
    int16_t unit_idx;  // _DI_

    *troop_count = 0;

    for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
    {
        unit_idx = _unit_stack[itr_stack].unit_idx;

        if(
            (_unit_stack[itr_stack].active != ST_FALSE)
            &&
            (_UNITS[unit_idx].moves2 > 0)
            &&
            (_unit_type_table[_UNITS[unit_idx].type].Construction > 0)
        )
        {
            troops[*troop_count] = unit_idx;
            *troop_count += 1;
        }
    }

}


// WZD o069p09
/*
    sets Status, dst_wx, dst_wy, Rd_Constr_Left, Rd_From_X, Rd_From_Y, wp

*/
void Set_Army_Road_Building(int16_t troop_count, int16_t troops[], int16_t dst_wx, int16_t dst_wy)
{
    int16_t src_wy;
    int16_t src_wx;
    int16_t path_distance;
    int16_t itr_troops;
    int16_t unit_idx;  // _SI_

    src_wx = _UNITS[troops[0]].wx;
    src_wy = _UNITS[troops[0]].wy;

    path_distance = Path_Wrap(src_wx, src_wy, dst_wx, dst_wy, &movepath_x_array[1], &movepath_y_array[1], WORLD_WIDTH);

    if(Terrain_Is_Sailable(src_wx, src_wy, _map_plane) == ST_TRUE)
    {
        return;
    }

    for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
    {
        unit_idx = troops[itr_troops];

        _UNITS[unit_idx].Status = us_GOTO;

        _UNITS[unit_idx].dst_wx = dst_wx;
        _UNITS[unit_idx].dst_wy = dst_wy;

        if(_unit_type_table[_UNITS[unit_idx].type].Construction > 0)
        {
            _UNITS[unit_idx].Rd_Constr_Left = Turns_To_Build_Road(_UNITS[unit_idx].wx, _UNITS[unit_idx].wx, _map_plane);

            if(m_construction_points > 0)
            {
                _UNITS[unit_idx].Rd_Constr_Left /= m_construction_points;
            }

            SETMIN(_UNITS[unit_idx].Rd_Constr_Left, 1);

            _UNITS[unit_idx].Rd_From_X = _UNITS[unit_idx].wx;
            _UNITS[unit_idx].Rd_From_Y = _UNITS[unit_idx].wy;

            _UNITS[unit_idx].wp = _map_plane;
        }

    }

}