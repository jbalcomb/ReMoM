
# STU MoM-Rasm - Testing

Test Cases
Unit Tests
Mock Ups


New & Exciting Territoy


### Test-Case: Which Entities should be drawn on the Movement Map?
Owner
Explored
Scouted

Draw_Maps()
    |-> Draw_Map_Terrain()
    ...
    |-> Draw_Map_Units()

Draw_Map_Terrain()
    tests unexplored_area
    fills map square with BLACK

Draw_Map_Units()
    if entities_on_movement_map[] then Draw_Unit_Picture()

Set_Entities_On_Map_Window()
    clears entities_on_movement_map[]
    adds units
    adds cities
    for units, IsVisible(), in_view, if existing then draw_priority, also active_stack if draw_active_stack_flag
    for cities, in_view
    So, ...
        Cities inherently have a higher draw priority than units
        Units is principally based on square_scouted
        All owned Units are inherently explored and scouted
        All Cities are inherently explored and scouted
        All other Units are based on whether they are on a sqaure that was set in squared_scoute

So, ...
    Until I have the code for updating the square_scouted bit-field,
    which Units get drawn is based on the square_explored and square_scouted, as seen in SAVETEST.GAM
        square_scouted_p0[300]
        square_scouted_p1[300]
        square_explored[2400*2]
        entities_on_movement_map[12*10]
    And, the test would then be validating that entities_on_movement_map[] has been set accurately.
In Draw_Maps()
    if(map_moved_flag == ST_TRUE)
    {
        Set_Unit_Draw_Priority();
        Reset_Stack_Draw_Priority();
        Set_Entities_On_Map_Window(l_map_x, l_map_y, map_plane);
`TEST HERE`
