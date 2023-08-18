


¿ Draw Active Unit Stack Mark ?

Draw_Active_Unit_Stack()
    if(draw_active_stack_flag != -2)  /* -2: never draw*/

Set_Draw_Active_Stack_Never()
XREF:
    OON  j_Set_Draw_Active_Stack_Never()

j_Set_Draw_Active_Stack_Never()
XREF: 
    Main_Screen_Draw_Next_Turn_Button()
    OVL_MoveUnitStack()


// ? Quit Action / Unselect Unit ?
    does not touch _unit_stack[]
    sets _unit_stack_count = 0;
    calls Set_Draw_Active_Stack_Always()
        which sets draw_active_stack_flag = -1
So, ...
    Draw_Active_Unit_Stack() shouldn't draw if draw_active_stack_flag == -1?


Set_Draw_Active_Stack_Always()
XREF:
    OON  j_Set_Draw_Active_Stack_Always()
j_Set_Draw_Active_Stack_Always()
XREF:
    City_Screen()
    Main_Screen()
    IDK_EoT_s52B09()
    WIZ_NextIdleStack()
    IDK_Spell_Casting_Screen()
    IDK_Cartographer_Screen()
    EVNT_ShowMessage()
    EZ_Resolve()
    Surveyor_Screen()
    STK_Move()

G_WLD_StaticAssetRfrsh()
    sets all_units_moved = ST_FALSE
WIZ_NextIdleStack()
    tests all_units_moved
        sets _unit_stack_count = 0
        calls Set_Draw_Active_Stack_Always()

GAME_Overland_Init()
    sets _unit = 0





MainScr_Prepare_Reduced_Map()
    Minimap_Set_Dims(REDUCED_MAP_W, REDUCED_MAP_H);
    Minimap_Coords(&minimap_x, &minimap_y, ((_map_x + (MAP_WIDTH / 2)) % WORLD_WIDTH), (_map_y + (MAP_HEIGHT / 2)), minimap_width, minimap_height);
    Draw_Reduced_Map(minimap_x, minimap_y, _map_plane, _reduced_map_seg, minimap_width, minimap_height, 0, 0, 0);


Minimap_Coords()

takes world center x,y and map w,h
calcs top left map x,y

world center x - half map w
world center y - half map h

world wrap

e.g.,
    map w,h is 58,30
    map x, y is 18,11
    world mid x,y is 24,16

24 - 58 / 2 = 24 - 29 = -5 + 60 = 55
16 - 30 / 2 = 16 - 15 =  1

*minimap_x = 0, *minimap_y = 1
