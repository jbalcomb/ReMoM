


Draw Main Screen Movement Map




MainScr_Maps.c
void Draw_Maps(int16_t screen_x, int16_t screen_y, int16_t map_width, int16_t map_height, int16_t * map_x, int16_t * map_y, int16_t map_plane, int16_t xpos, int16_t ypos, int16_t player_idx)


Main_Screen()
    Assign_Auto_Function(Main_Screen_Draw, 1);
    ...
    Main_Screen_Draw()
    PageFlip_FX();


Main_Screen_Draw()
    |-> Main_Screen_Draw_Do_Draw()
        |-> Draw_Maps()



Main_Screen_Draw();
    Main_Screen_Draw_Do_Draw(&_map_x, &_map_y, _map_plane, _prev_world_x, _prev_world_y, _human_player_idx);
        Draw_Maps(MAP_SCREEN_X, MAP_SCREEN_Y, MAP_WIDTH, MAP_HEIGHT, map_x, map_y, map_plane, x_pos, y_pos, player_idx);
            Draw_Map_Window(screen_x, screen_y, map_width, map_height, l_map_x, l_map_y, map_plane);


Draw_Maps()
    if(map_moved_flag == ST_TRUE)
        Set_Unit_Draw_Priority();
        Reset_Stack_Draw_Priority();
        Set_Entities_On_Map_Window(l_map_x, l_map_y, map_plane);
    Draw_Map_Window(screen_x, screen_y, map_width, map_height, l_map_x, l_map_y, map_plane);


Draw_Map_Window()
    Draw_Map_Terrain(screen_x, screen_y, map_w, map_h, map_draw_curr_x, map_draw_curr_y, map_p);
    Draw_Map_Minerals(screen_x, screen_y, map_w, map_h, map_draw_curr_x, map_draw_curr_y, map_p);
    Draw_Map_Biota(screen_x, screen_y, map_w, map_h, map_draw_curr_x, map_draw_curr_y, map_p);
    Draw_Map_Roads(screen_x, screen_y, map_w, map_h, map_draw_curr_x, map_draw_curr_y, map_p);
    Draw_Map_Cities(screen_x, screen_y, map_w, map_h, map_draw_curr_x, map_draw_curr_y, map_p);
    Draw_Map_Towers(screen_x, screen_y, map_w, map_h, map_draw_curr_x, map_draw_curr_y, map_p);
    Draw_Map_Lairs(screen_x, screen_y, map_w, map_h, map_draw_curr_x, map_draw_curr_y, map_p);
    Draw_Map_Units(screen_x, screen_y, map_w, map_h, map_draw_curr_x, map_draw_curr_y);
    Draw_Map_Nodes(screen_x, screen_y, map_w, map_h, map_draw_curr_x, map_draw_curr_y, map_p);
    Draw_Map_Unexplored_Area(screen_x, screen_y, map_w, map_h, map_draw_curr_x, map_draw_curr_y, map_p);
