


### Color-Map Re-Map

...before, during, after, ...

function to *dump* portion of frame-buffer
pointer to the current/target frame-buffer
x,y offset in frame-buffer
width, height of portion
stride/pitch/line-width of frame-buffer
(re-)scale

memory allocation to build-up bitmap image data for debug *dump*


#ifdef STU_DEBUG
    if(DBG_Draw_Invisibility = 1)  /* Unit Has Invisibility */
    {
        DLOG("(DBG_Draw_Invisibility = 1)");
        // screen_page = (video_page_buffer[1 - draw_page_num]);
        // screen_ofst = (80 * SCREEN_WIDTH) + 248;
        vbb_ptr = (video_page_buffer[1 - draw_page_num] + ((80 * SCREEN_WIDTH) + 248));
        for(itr_height = 0; itr_height < STATFIG_HEIGHT; itr_height++)
        {
            for(itr_width = 0; itr_width < STATFIG_WIDTH; itr_width++)
            {
                dbg_prn("%02X\n", *(vbb_ptr + ((itr_height * SCREEN_WIDTH) + itr_width)));
            }
        }
    }
#endif









// _main_screen_grid_field
// _main_screen_grid_x, _main_screen_grid_y
// _cur_map_x, _cur_map_y
if( input == -main_screen_grid_field )
{
    Scroll_Map( cur_map_x + Get_Up_Scaled_Value(_main_screen_grid_x),
                cur_map_y + Get_Up_Scaled_Value(_main_screen_grid_y) )
}



        0  1  2  3  4  5  6  7  8  9 10 11
   +----------------------------------------------------------+
   |   
   |   
0  |   +---------------------------------+
1  |   |                                 |
2  |   |                                 |
3  |   |        S2                       |
4  |   |                                 |
5  |   |                                 |
6  |   |                                 |
7  |   |                                 |
8  |   |S1                               |
9  |   +---------------------------------+
   |
   |
   |
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

WORLD_WIDTH 60
MAP_WIDTH   12
MAP_HEIGHT  10

Top-Left of Movement Map in World Coordinates
map_x
map_y

stack1_x
stack1_y

stack2_x
stack2_y


e.g.,
    map   x,y 51,17
        maps shows world x  51, 52, 53, 54, 55, 56, 57, 58, 59,  0,  1,  2
    stack x,y  1,18
        ( 1 > 51) && ( 1 <= 63)
        (61 > 51) && (61 <= 63)

    in_view = ST_FALSE;

    // test in-bounds
    if(l_unit_y > l_map_y && l_unit_y < l_map_y + MAP_HEIGHT)
    {
        l_unit_x = unit_x;
        if( (l_unit_x > l_map_x && l_unit_x < l_map_x + MAP_WIDTH) ||
            (l_unit_x + WORLD_WIDTH > l_map_x && l_unit_x + WORLD_WIDTH < l_map_x + MAP_WIDTH)
        )
        {
            if( (l_unit_x + WORLD_WIDTH > l_map_x && l_unit_x + WORLD_WIDTH < l_map_x + MAP_WIDTH) )
            {
                l_unit_x += WORLD_WIDTH;
            }
            l_unit_x = l_unit_x - l_map_x;
            in_view = ST_TRUE;
        }
    }








.\src\STU\STU_DBG.C .H





DEBUG: [C:\devel\STU-MoM_Rasm\src\SAVETEST.C, 50]: _num_players: 5
DEBUG: [C:\devel\STU-MoM_Rasm\src\SAVETEST.C, 51]: _landsize: 2
DEBUG: [C:\devel\STU-MoM_Rasm\src\SAVETEST.C, 52]: _magic: 2
DEBUG: [C:\devel\STU-MoM_Rasm\src\SAVETEST.C, 53]: _difficulty: 2
DEBUG: [C:\devel\STU-MoM_Rasm\src\SAVETEST.C, 54]: _cities: 58
DEBUG: [C:\devel\STU-MoM_Rasm\src\SAVETEST.C, 55]: _units: 74
DEBUG: [C:\devel\STU-MoM_Rasm\src\SAVETEST.C, 56]: _turn: 103
DEBUG: [C:\devel\STU-MoM_Rasm\src\SAVETEST.C, 57]: _unit: 95
DEBUG: [C:\devel\STU-MoM_Rasm\src\SAVETEST.C, 58]: grand_vizier: 0





¿ DBG_TST Flags ?

control debug output from Set_Entities_On_Map_Window()
control debug output from OVL_StackSelect()
control debug output from Build_Unit_Stack()
validate entities_on_movement_map[] after call to Set_Entities_On_Map_Window()
validate _unit_stack after call to Build_Unit_Stack()
validate _unit_stack after call to OVL_StackSelect()

¿ where to set and unset DBG_TST flags ?

...press 'T' for "Test" on the Main Game Screen...
...set coordinates to trigger Scroll_Map(), on next call to Draw_Main_Screen()...  ~== Right-Click Movement Map Grid Field
...manifests expectation for map_moved == ST_TRUE, in Draw_Maps()...

get Right-Click on Unit, match to DBG_TST target Unit



Where was I?
I dunno.
But, something like...
    Draw_Map_Units() should not draw the active unit stack
        which it won't do if it is not in entities_on_movement_map[]
    which should be the case
    by way of
    in Draw_Maps()
        if(map_moved_flag == ST_TRUE)
        {
            DLOG("(map_moved_flag == ST_TRUE)");
            Set_Unit_Draw_Priority();
            Reset_Stack_Draw_Priority();
            Set_Entities_On_Map_Window(l_map_x, l_map_y, map_plane);
    So, ...
        test that (map_moved_flag == ST_TRUE) is correct
        test that Set_Entities_On_Map_Window() doesn't add it
    
    Then, ...
        test Draw_Active_Unit_Stack()



set DBG_TST_Selected_Stack
Draw_Map_Units()
entities_on_movement_map[]
        if(unit_idx == 55 || unit_idx == 536 || unit_idx == 828)
        dbg_prn("DEBUG: [%s, %d]: DBG_TST_FAILURE: unit_idx: %d\n", __FILE__, __LINE__, unit_idx);








Test Unit/Stack

_UNITS[55]

_unit
_unit_stack_count

_UNITS[55].world_x == 19
_UNITS[55].world_y == 10
_UNITS[55].world_plane == 0

_UNITS[55].Draw_Priority == 0

screen_x == 247
screen_y ==  79

Set_Entities_On_Map_Window(world_x = 18, world_y = 11, world_plane = 0)



if(map_moved_flag == ST_TRUE)
    DLOG("(map_moved_flag == ST_TRUE)");

    Set_Unit_Draw_Priority();
    Reset_Stack_Draw_Priority();

    Set_Entities_On_Map_Window(l_map_x, l_map_y, map_plane);


// WZD dseg:964C
int16_t entities_on_movement_map[120];  //  12 * 10  MAP_WIDTH * MAP_HEIGHT

entities_on_movement_map[(itr_map_y * MAP_WIDTH) + itr_map_x] = ST_UNDEFINED;


Main_Screen()
/* Right-Click Movement Map Grid Field */
entity_idx = entities_on_movement_map[( (_main_map_grid_y * MAP_WIDTH) + _main_map_grid_x )];
DLOG("if(entity_idx != ST_UNDEFINED)");
DLOG("if(entity_idx < 1000)");
DLOG("if(_UNITS[entity_idx].owner_idx == _human_player_idx)");
    _unit = entity_idx;
    unit_idx = _unit;
    Unit_X = _UNITS[unit_idx].world_x;
    Unit_Y = _UNITS[unit_idx].world_y;
    OVL_Map_CenterX = _UNITS[unit_idx].world_x;
    OVL_Map_CenterY = _UNITS[unit_idx].world_y;
    OVL_StackSelect(_human_player_idx, &_map_x, &_map_y, _map_plane, Unit_X, Unit_Y);

entity_idx: 55

DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1946]: BEGIN: OVL_StackSelect(player_idx = 0, map_x = 1003327158, map_y = 1003328400, map_plane = 0, unit_x = 19, unit_y = 10)
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 2016]: BEGIN: Build_Unit_Stack()
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 2085]: BEGIN: Build_Unit_Stack()
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1999]: END: OVL_StackSelect(player_idx = 0, map_x = 1003327158, map_y = 1003328400, map_plane = 0, unit_x = 19, unit_y = 10)



DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr_Maps.C, 600]: BEGIN: Set_Entities_On_Map_Window(world_x = 18, world_y = 11, world_plane = 0)
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr_Maps.C, 621]: itr_units: 55
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr_Maps.C, 622]: _UNITS[itr_units].Draw_Priority: 0
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr_Maps.C, 743]: END: Set_Entities_On_Map_Window(world_x = 18, world_y = 11, world_plane = 0)

DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 2241]: BEGIN: Draw_Unit_Picture(x = 0, y = 146, unit_idx = 12, flag = 2)
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 2278]: END: Draw_Unit_Picture(x = 0, y = 146, unit_idx = 12, flag = 2)

DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1875]: BEGIN: Main_Screen_Draw_Unit_Window(start_x = 247, start_y = 79)
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1884]: _unit_stack_count: 1
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 2241]: BEGIN: Draw_Unit_Picture(x = 247, y = 79, unit_idx = 55, flag = 0)
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 2278]: END: Draw_Unit_Picture(x = 247, y = 79, unit_idx = 55, flag = 0)
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 1912]: END: Main_Screen_Draw_Unit_Window(start_x = 247, start_y = 79)

