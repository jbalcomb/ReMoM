
/*
    WIZARDS.EXE
    ovr057
    ovr058
    ovr059
    ovr061
    ovr062
    ovr063
    ovr064
    ovr095
    ovr097

¿ MoO2  Module: SHIPSTK ?

*/



## Data Segments  {Strings, Initialized, Uninitialized}

dseg:C03E                                                 ¿ BEGIN: Main_Screen - Uninitialized Data ?
...
dseg:C098                                                     ¿ BEGIN: List / ArmyList / CityList - Uninitialized Data ?



## target_world_x, target_world_y
AKA reduced_map_window_wx, reduced_map_window_wy

    BEGIN: Right-Click Unit Window Grid Field
        Unit_Window_Picture_Coords(itr_stack, &target_world_x, &target_world_y, &usw_x2, &usw_y2);
        USW_FullDisplay(_unit_stack[itr_stack].unit_idx, target_world_x, target_world_y, (target_world_x + 18), (target_world_y + 18));

    BEGIN:  Left-Click Move Stack
        target_world_x = ((_map_x + _main_map_grid_x) % WORLD_WIDTH);
        target_world_y =  (_map_y + _main_map_grid_y);
        if(EarthGateTeleport__WIP(target_world_x, target_world_y, _map_plane) == ST_FALSE)
        ...
        Move_Stack(target_world_x, target_world_y, _human_player_idx, &_map_x, &_map_y, &_map_plane);

    Right-Click Movement Map Grid Field
    /* #### Section 9.2.2.2      Right-Click Movement Map - Stack - Other */
        target_world_x = (_main_map_grid_x * SQUARE_WIDTH);
        target_world_y = (_main_map_grid_y * SQUARE_HEIGHT);
        Unit_List_Window(entity_idx, 0, target_world_x, target_world_y);

    Right-Click Movement Map Grid Field
    /* #### Section 9.2.3.1.1      Right-Click Movement Map - City - Own - Outpost */
        target_world_x = (_main_map_grid_x * SQUARE_WIDTH);
        target_world_y = (MAP_SCREEN_Y + (_main_map_grid_y * SQUARE_HEIGHT));

    BEGIN: Reduced Map Grid Field
        Reduced_Map_Coords(&reduced_map_window_wx, &reduced_map_window_wy, ((_map_x + (MAP_WIDTH / 2)) % WORLD_WIDTH), (_map_y + (MAP_HEIGHT / 2)), REDUCED_MAP_WIDTH, REDUCED_MAP_HEIGHT);
        _prev_world_x = reduced_map_window_wx + _minimap_grid_x;
        _prev_world_y = reduced_map_window_wy + _minimap_grid_y;
        _map_x = _prev_world_x;
        _map_y = _prev_world_y;
        Center_Map(&_map_x, &_map_y, _prev_world_x, _prev_world_y, _map_plane);







Screen_Control()
    case scr_Main_Screen: { Main_Screen(); } break;
But, ...
    ¿ need everything from MGC & WZD, from main() through New_Game()/Load_Game() to Main_Screen() ?



        case scr_Main_Screen:
        {
            // BEGIN: WZD main()
            // Load_SAVE_GAM(8)
            // Load_WZD_Resources()
            Load_Palette(0, -1, 0);  // NOTE(JimBalcomb,20230111): this is the only Load_Palette() leading to the Main_Screen()
            // Calculate_Remap_Colors();
            Set_Button_Down_Offsets(1, 1);
            // Cycle_Palette_Color(198, 40, 0, 0, 63, 0, 0, 1)
            Apply_Palette();
            // Fade_In()
            // Clear_Fields()
            // Loaded_Game_Update_WZD()
            // END: WZD main()

            // IDK  Set_Outline_Color(0);
            // IDK  Set_Alias_Color(0);
            // IDK  Set_Font_Style_Shadow_Down(0, 0, 0, 0);

            Set_Page_Off();
            Fill(0, 0, SCREEN_XMAX, SCREEN_YMAX, 7);
            Set_Page_On();
            Fill(0, 0, SCREEN_XMAX, SCREEN_YMAX, 5);
            Set_Page_Off();

            Main_Screen();
            // MoO2  previous_screen = scr_Main_Screen
            previous_screen = scr_Main_Screen;
        } break;
