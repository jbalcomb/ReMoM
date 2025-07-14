

## Center_Map()
Main_Screen()
if(abs(input_field_idx) == _minimap_grid_field)
    Reduced_Map_Coords(&reduced_map_window_wx, &reduced_map_window_wy, ((_map_x + (MAP_WIDTH / 2)) % WORLD_WIDTH), (_map_y + (MAP_HEIGHT / 2)), REDUCED_MAP_WIDTH, REDUCED_MAP_HEIGHT);
    _prev_world_x = reduced_map_window_wx + _minimap_grid_x;  // ...is the 'wx' of the clicked square
    _prev_world_y = reduced_map_window_wy + _minimap_grid_y;  // ...is the 'wy' of the clicked square
    _map_x = _prev_world_x;
    _map_y = _prev_world_y;
    Center_Map(&_map_x, &_map_y, _prev_world_x, _prev_world_y, _map_plane);
    assert(_prev_world_x >= WORLD_XMIN && _prev_world_x <= WORLD_XMAX);  /*  0 & 59 */
    assert(_prev_world_y >= WORLD_YMIN && _prev_world_y <= WORLD_YMAX);  /*  0 & 39 */
    assert(_map_x >= WORLD_XMIN && _map_x <= WORLD_XMAX);  /*  0 & 59 */
    assert(_map_y >= WORLD_YMIN && _map_y <= WORLD_YMAX);  /*  0 & 39 */
