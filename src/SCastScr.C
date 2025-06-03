/*
    Spell Casting Screen

    WIZARDS.EXE
        ovr070
*/

#include "SCastScr.H"

#include "MOX/MOM_DATA.H"
#include "MOX/MOX_DEF.H"
#include "MOX/MOX_T4.H"
#include "MOX/MOX_TYPE.H"

#include "MOM_DEF.H"

#include "MainScr_Maps.H"  /* Add_Nodes_To_Entities_On_Map_Window() */

#include "SPELLDEF.H"

#include <assert.h>



// WZD dseg:3312                                                 BEGIN:  ovr070 - Initialized Data

// WZD dseg:3312
int16_t _osc_mouse_image_cycle = 0;
// WZD dseg:3314
char str_empty_string__ovr070[] = "";
// WZD dseg:3315
char str_hotkey_C__ovr070[] = "C'";
// WZD dseg:3317
char str_SelectA[] = "Select a";
// WZD dseg:3320
char str_FriendlyUnit[] = " friendly unit ";
// WZD dseg:3330
char str_FriendlyGroup[] = " friendly group ";
// WZD dseg:3341
char str_NEnemyUnit[] = "n enemy unit ";
// WZD dseg:334F
char str_NEnemyGroup[] = "n enemy group ";
// WZD dseg:335E
char aSpace[] = " space ";
// WZD dseg:3366
char str_FriendlyCity[] = " friendly city ";
// WZD dseg:3376
char str_NEnemyCity[] = "n enemy city ";
// WZD dseg:3384
char str_MagicNode[] = " magic node ";
// WZD dseg:3391
char str_AsTheTargetFor[] = "as the target for ";
// WZD dseg:33A4
char byte_39E44[] = " \x2\x0";
// WZD dseg:33A7
char byte_39E47[] = "\x1 spell.";
// WZD dseg:33B0
char str_Casting[] = "Casting";

// WZD dseg:33B0                                                 ¿ END:  ovr070 - Initialized Data ?



// WZD dseg:C13E                                                 BEGIN:  ovr070 - Unitialized Data

// WZD dseg:C13E
int16_t _osc_spell_idx;

// WZD dseg:C140
int16_t _osc_map_plane;

// WZD dseg:C142
int16_t word_42BE2;
// WZD dseg:C144
int16_t word_42BE4;
// WZD dseg:C146 00                                              db    0
// WZD dseg:C147 00                                              db    0
// WZD dseg:C148
byte_ptr _osc_panel_title;
// WZD dseg:C14A 00                                              db    0
// WZD dseg:C14B 00                                              db    0
// WZD dseg:C14C 00                                              db    0
// WZD dseg:C14D 00                                              db    0
// WZD dseg:C14E 00                                              db    0
// WZD dseg:C14F 00                                              db    0
// WZD dseg:C150 00                                              db    0
// WZD dseg:C151 00                                              db    0
// WZD dseg:C152 00                                              db    0
// WZD dseg:C153 00                                              db    0
// WZD dseg:C154 00                                              db    0
// WZD dseg:C155 00                                              db    0
// WZD dseg:C156 00                                              db    0
// WZD dseg:C157 00                                              db    0
// WZD dseg:C158 00                                              db    0
// WZD dseg:C159 00                                              db    0
// WZD dseg:C15A 00                                              db    0
// WZD dseg:C15B 00                                              db    0
// WZD dseg:C15C 00                                              db    0
// WZD dseg:C15D 00                                              db    0
// WZD dseg:C15E 00                                              db    0
// WZD dseg:C15F 00                                              db    0
// WZD dseg:C160
int16_t _osc_main_map_grid_y;
// WZD dseg:C162
int16_t _osc_main_map_grid_x;
// WZD dseg:C164
int16_t _osc_spell_target_type;
// WZD dseg:C166
int16_t _osc_wp_IDK;
// WZD dseg:C168
int16_t _osc_wy_IDK;
// WZD dseg:C16A
int16_t _osc_wx_IDK;
// WZD dseg:C16C
int16_t _osc_mouse_image_num;
// WZD dseg:C16E
struct s_mouse_list * current_mouse_list;
// WZD dseg:C170
int16_t word_42C10;
// WZD dseg:C172
int16_t word_42C12;
// WZD dseg:C174
int16_t _osc_cancel_button_field;

// WZD dseg:C174                                                 END:  ovr070 - Unitialized Data



// WZD o70p01
int16_t Spell_Casting_Screen__WIP(int16_t spell_target_type, int16_t * wx, int16_t * wy, int16_t * wp, int16_t * target_wx, int16_t * target_wy, char * spell_name)
{
    int16_t Units = 0;
    int16_t Count = 0;
    int16_t var_14 = 0;
    int16_t var_12 = 0;
    int16_t var_10 = 0;
    int16_t didnt_cancel = 0;
    int16_t screen_changed = 0;
    int16_t TopY = 0;
    int16_t LeftX = 0;
    int16_t scanned_field = 0;
    int16_t input_field_idx = 0;
    int16_t leave_screen = 0;
    int16_t IDK_rightclicked_city_idx = 0;  // _DI_
    int16_t entity_idx = 0;  // _SI_

    PageFlipEffect = 0;

    _osc_spell_idx = _players[_human_player_idx].casting_spell_idx;

    word_42C10 = 0;

    word_42C12 = 0;

    _osc_spell_target_type = spell_target_type;

    didnt_cancel = ST_TRUE;

    _osc_wx_IDK = ST_UNDEFINED;

    _osc_wy_IDK = ST_UNDEFINED;

    _osc_wp_IDK = ST_UNDEFINED;

    word_42BE4 = 0;

    word_42BE2 = 1;

    _osc_map_plane = _map_plane;

    Set_Draw_Active_Stack_Always();

    Set_Outline_Color(ST_TRANSPARENT);

    Deactivate_Auto_Function();

    Assign_Auto_Function(Spell_Casting_Screen_Draw, 1);

    Spell_Casting_Screen_Allocate();

    Build_Select_Target_String(_osc_spell_target_type, spell_name);

    Set_Unit_Draw_Priority();

    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);

    Set_Mouse_List_Image_Nums();

    Reset_Map_Draw();

    MainScr_Create_Reduced_Map_Picture();

    Deactivate_Help_List();

    Set_Spell_Cast_Screen_Help_List();

    Set_Input_Delay(4);

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {

        Mark_Time();

        Clear_Fields();

        Spell_Casting_Screen_Add_Fields();

        var_10 = INVALID_FIELD;

        var_12 = INVALID_FIELD;

        var_14 = INVALID_FIELD;

        input_field_idx = Get_Input();

        scanned_field = Scan_Input();

        if(input_field_idx == _osc_cancel_button_field)
        {
            Play_Left_Click();
            didnt_cancel = 0;
            leave_screen = ST_UNDEFINED;
        }

        if(input_field_idx == _plane_button)
        {
            Play_Left_Click();
            Do_Plane_Button__WIP(_human_player_idx, &_map_x, &_map_y, &_map_plane);
            Reset_Map_Draw();
            MainScr_Create_Reduced_Map_Picture();
            Deactivate_Auto_Function();
            Assign_Auto_Function(Spell_Casting_Screen_Draw, 1);
            Set_Mouse_List_Image_Nums();
        }

        /*
            BEGIN:  Left-Click Movement Map
        */
        if(input_field_idx == _main_map_grid_field)
        {
            IDK_rightclicked_city_idx = ST_FALSE;
            if(_osc_spell_target_type == stt_Magic_Node)
            {
                entity_idx = Get_Map_Square_Magic_Node(((_map_x + _main_map_grid_x) % WORLD_WIDTH), ((_main_map_grid_y + _map_y) % WORLD_HEIGHT), _map_plane);
                if(entity_idx != ST_UNDEFINED)
                {
                    entity_idx += 1100;
                }
            }
            else
            {
                entity_idx = GET_MAIN_MAP_ENTITY();
            }

            if(entity_idx == ST_UNDEFINED)
            {
                Play_Left_Click();
                entity_idx = abs(entity_idx);
                if (_osc_spell_target_type == stt_Map_Square)
                {
                    leave_screen = IDK_Map_Square_Is_Targetable(_main_map_grid_x, _main_map_grid_y);
                }
                else
                {
                    if (_osc_spell_target_type == stt_Map_Square)
                    {
                        leave_screen = IDK_Map_Square_Is_Targetable(_main_map_grid_x, _main_map_grid_y);
                    }
                }
            }
            else
            {

            }
        }  /* (input_field_idx == _main_map_grid_field) */
        /*
            END:  Left-Click Movement Map
        */

        /*
            BEGIN:  Any-Click Reduced Map
        */
        if(abs(input_field_idx) == _minimap_grid_field)
        {
            Play_Left_Click();
            Reduced_Map_Coords(&LeftX, &TopY, (((12 / 2) + _map_x) % WORLD_WIDTH), ((10 / 2) + _map_y), 58, 30);
            _prev_world_x = (LeftX + _minimap_grid_x);
            _prev_world_y = (TopY + _minimap_grid_y);
            IDK_CheckSet_MapDisplay_XY();
            Center_Map(&_map_x, &_map_y, _prev_world_x, _prev_world_y, _map_plane);
            MainScr_Create_Reduced_Map_Picture();
            Set_Mouse_List_Image_Nums();
            Reset_Map_Draw();
            screen_changed = ST_TRUE;
        }
        /*
            END:  Any-Click Reduced Map
        */

        /*
            BEGIN:  Right-Click Movement Map
        */
        if(-input_field_idx == _main_map_grid_field)
        {

        }
        /*
            END:  Right-Click Movement Map
        */

        if(
            (leave_screen == ST_FALSE)
            &&
            (screen_changed == ST_FALSE)
        )
        {
            Spell_Casting_Screen_Draw();
            PageFlip_FX();
            Release_Time(1);
        }

    }

    Set_Mouse_List(1, mouse_list_default);

    Deactivate_Help_List();

    Deactivate_Auto_Function();

    Clear_Fields();

    Reset_Window();

    Reset_Map_Draw();

    *wx = _osc_wx_IDK;

    *wy = _osc_wy_IDK;

    *wp = _osc_wp_IDK;

    // Main_Screen() target_world_x = (_main_map_grid_x * SQUARE_WIDTH);
    // Main_Screen() target_world_y = (MAP_SCREEN_Y + (_main_map_grid_y * SQUARE_HEIGHT));
    *target_wx = (_osc_main_map_grid_x * SQUARE_WIDTH);
    *target_wy = (MAP_SCREEN_Y + (_osc_main_map_grid_y * SQUARE_HEIGHT));

    return didnt_cancel;
    
}


// WZD o70p02
/*

*/
void Spell_Casting_Screen_Add_Fields(void)
{

    _main_map_grid_field = INVALID_FIELD;

    _minimap_grid_field = INVALID_FIELD;

    // DONT  CRP_OVL_UU_Control_4 = INVALID_FIELD;
    // DONT  CRP_OVL_UU_Control_3 = INVALID_FIELD;
    // DONT  CRP_OVL_UU_Control_2 = INVALID_FIELD;
    // DONT  CRP_OVL_UU_Control_1 = INVALID_FIELD;

    Add_Game_Button_Fields();

    if(
        (_map_x = _prev_world_x)
        &&
        (_map_y = _prev_world_y)
    )
    {
        _main_map_grid_field = Add_Grid_Field(MAP_SCREEN_X, MAP_SCREEN_Y, SQUARE_WIDTH, SQUARE_HEIGHT, MAP_WIDTH, MAP_HEIGHT, &_main_map_grid_x, &_main_map_grid_y, ST_UNDEFINED);
    }

    if(
        (_map_x == _prev_world_x)
        &&
        (_map_y == _prev_world_y)
    )
    {
        _minimap_grid_field = Add_Grid_Field(REDUCED_MAP_SCREEN_X, REDUCED_MAP_SCREEN_Y, REDUCED_MAP_SQUARE_WIDTH, REDUCED_MAP_SQUARE_HEIGHT, REDUCED_MAP_WIDTH, REDUCED_MAP_HEIGHT, &_minimap_grid_x, &_minimap_grid_y, ST_UNDEFINED);
    }

    _osc_cancel_button_field = Add_Button_Field(263, 181, str_empty_string__ovr070, cast_cancel_button, str_hotkey_C__ovr070[0], ST_UNDEFINED);
    
}


// WZD o70p03
/*

*/
void Spell_Casting_Screen_Draw(void)
{
    Reset_Window();
    Set_Page_Off();
    Reset_Map_Draw();
    Draw_Maps(0, 20, 12, 10, &_map_x, &_map_y, _map_plane, _prev_world_x, _prev_world_y, _human_player_idx);
    FLIC_Draw(0, 0, main_background_seg);
    Draw_World_Window(251, 21, 58, 30);
    Main_Screen_Draw_Unit_Action_Locked_Buttons();  // BUGBUG  ¿ isn't this completly hidden by the deselect bk and cancel butt ?
    Main_Screen_Draw_Game_Buttons();
    Main_Screen_Draw_Status_Window();
    Spell_Casting_Screen_Draw_Panel();
    Set_Mouse_List_Image_Nums();
    FLIC_Set_CurrentFrame(cast_cancel_button, 0);
    FLIC_Draw(263, 181, cast_cancel_button);
}

// WZD o70p04
// drake178: OVL_Tile2DrawCoords()
/*
; converts the tile coordinates at the passed pointer
; locations into draw pixel coordinates for the large
; overland 12x10 map display; returning 1 if the tile
; is visible in the map window, or 0 if it is not
*/
/*
World_To_Screen()
XREF:
    j_World_To_Screen()
        Draw_Active_Stack_Movement_Path()
        Draw_Active_Unit_Stack()
        Move_Units_Draw()
        Move_Units_Draw()
        Move_Units_Draw()
        Move_Units_Draw()
        Cast_ChangeTerain()
        Cast_Corruption()
        Cast_PlaneShift_DrawSwitchPlane()
        IDK_SplCst_sB529D()
        IDK_SplCst_sB529D()
        IDK_SpellAnim_Screen_Driver()
        IDK_Spell_Cityscape_1()
*/
/*
    returns in_view {F,T}
    in-outs Unit's World X,Y to Screen X,Y
*/
int16_t World_To_Screen(int16_t map_wx, int16_t map_wy, int16_t * unit_wx, int16_t * unit_wy)
{
    int16_t in_view;
    int16_t unit_my;
    int16_t unit_mx;
    int16_t screen_y;  // DNE in Dasm
    int16_t screen_x;  // DNE in Dasm

    assert( map_wx  >= WORLD_XMIN &&  map_wx  <= WORLD_XMAX);  /*  0 & 59 */
    assert( map_wy  >= WORLD_YMIN &&  map_wy  <= WORLD_YMAX);  /*  0 & 39 */
    assert(*unit_wx >= WORLD_XMIN && *unit_wx <= WORLD_XMAX);  /*  0 & 59 */
    assert(*unit_wy >= WORLD_YMIN && *unit_wy <= WORLD_YMAX);  /*  0 & 39 */

    in_view = ST_FALSE;

    if(
        (*unit_wy >= map_wy)
        &&
        (*unit_wy <= (map_wy + MAP_Y_MAX))
    )
    {
        unit_my = *unit_wy - map_wy;  // e.g, unit is at 37,21 and move-map is at 29,12; this Y is 21-12=9, which is Y relative to the move-map viewport 

        if( (*unit_wx >= map_wx) && (*unit_wx <= (map_wx + MAP_X_MAX)) )
        {
            unit_mx = *unit_wx - map_wx;
            in_view = ST_TRUE;
        }
        else
        {
            *unit_wx += WORLD_WIDTH;
            if((*unit_wx >= map_wx) && (*unit_wx <= (map_wx + MAP_X_MAX)))
            {
                unit_mx = *unit_wx - map_wx;
                in_view = ST_TRUE;
            }
        }
    }

    if(in_view == ST_TRUE)
    {
        screen_x = MAP_SCREEN_X + (unit_mx * SQUARE_WIDTH);
        screen_y = MAP_SCREEN_Y + (unit_my * SQUARE_HEIGHT);
    }
    else
    {
        screen_x = 0;
        screen_y = 0;
    }

    assert(screen_x >= MAP_SQUARE_SCREEN_X_MIN && screen_x <= MAP_SQUARE_SCREEN_X_MAX);
    assert(screen_y >= MAP_SQUARE_SCREEN_Y_MIN && screen_y <= MAP_SQUARE_SCREEN_Y_MAX);

    *unit_wx = screen_x;
    *unit_wy = screen_y;

    return in_view;
}


// WZD o70p05
// drake178: sub_5CFC9()
void Spell_Casting_Screen_Allocate(void)
{
    _reduced_map_seg = Allocate_First_Block(_screen_seg, 303);
    current_mouse_list = Near_Allocate_First(1560);  // mouse lists?  1560 / 12 = 130 ...targets?
    _osc_panel_title = Near_Allocate_Next(100);
}

// WZD o70p06
// drake178: sub_5CFFA()
/*

e.g.,
    Select a
    space as the
    target for a
    Floating
    Island spell.
*/
void Build_Select_Target_String(int16_t spell_target_type, char * spell_name)
{

    strcpy(_osc_panel_title, str_SelectA);  // "Select a"

    switch(spell_target_type)
    {
        case 0:
        {
            strcat(_osc_panel_title, str_FriendlyUnit);
        } break;
        case 1:
        {
            strcat(_osc_panel_title, str_FriendlyGroup);
        } break;
        case 2:
        {
            strcat(_osc_panel_title, str_NEnemyUnit);
        } break;
        case 3:
        {
            strcat(_osc_panel_title, str_NEnemyGroup);
        } break;
        case 4:
        {
            strcat(_osc_panel_title, aSpace);
        } break;
        case 5:
        {
            strcat(_osc_panel_title, str_FriendlyCity);
        } break;
        case 6:
        {
            strcat(_osc_panel_title, str_NEnemyCity);
        } break;
        case 7:
        {
            strcat(_osc_panel_title, str_MagicNode);
        } break;
    }

    strcat(_osc_panel_title, str_AsTheTargetFor);

    strcat(_osc_panel_title, STR_GetIndefinite(spell_name));

    strcat(_osc_panel_title, byte_39E44);
    
    strcat(_osc_panel_title, spell_name);

    strcat(_osc_panel_title, byte_39E47);

}

// WZD o70p07
// drake178: sub_5D0B3()
// sub_5D0B3()

// WZD o70p08
// drake178: sub_5D0E8()
// sub_5D0E8()

// WZD o70p09
// drake178: sub_5D0FF()
// sub_5D0FF()

// WZD o70p10
// drake178: sub_5D116()
// sub_5D116()

// WZD o70p11
// drake178: sub_5D12D()
// sub_5D12D()

// WZD o70p12
// drake178: sub_5D144()
// sub_5D144()

// WZD o70p13
// drake178: sub_5D15B()
int16_t IDK_Map_Square_Is_Targetable(int16_t mx, int16_t my)
{
    int16_t return_value = 0;  // _DI_

    return_value = ST_FALSE;

    _osc_main_map_grid_x = mx;
    _osc_main_map_grid_y = my;

    if(_osc_main_map_grid_x == 0)
    {
        _osc_main_map_grid_x += 1;
    }

    if(_osc_main_map_grid_x == 11)
    {
        _osc_main_map_grid_x -= 1;
    }

    if(_osc_main_map_grid_y == 0)
    {
        _osc_main_map_grid_y += 1;
    }

    if(_osc_main_map_grid_y == 9)
    {
        _osc_main_map_grid_y -= 1;
    }

    _osc_wx_IDK = (mx + _map_x);

    if(_osc_wx_IDK >= WORLD_WIDTH)
    {
        _osc_wx_IDK -= WORLD_WIDTH;
    }

    _osc_wy_IDK = (_map_y + my);

    _osc_wp_IDK = _map_plane;

    if(
        (SQUARE_UNEXPLORED(_osc_wx_IDK, _osc_wy_IDK, _osc_wp_IDK))
        &&
        (_osc_spell_idx != spl_Earth_Lore)
    )
    {
        _osc_wx_IDK = ST_UNDEFINED;
        _osc_wy_IDK = ST_UNDEFINED;
        _osc_wp_IDK = ST_UNDEFINED;
    }
    else
    {
        return_value = ST_TRUE;
    }

    return return_value;

}

// WZD o70p14
// drake178: sub_5D20A()
void Set_Mouse_List_Image_Nums(void)
{
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t troop_count = 0;
    int16_t my_y2 = 0;
    int16_t mx_x2 = 0;
    int16_t my_y1 = 0;
    int16_t mx_x1 = 0;
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t itr_my = 0;
    int16_t itr_mx = 0;
    int16_t mouse_list_count = 0;  // _SI_
    int16_t entity_idx = 0;  // _DI_

    if(_osc_spell_target_type == stt_Magic_Node)
    {
        Add_Nodes_To_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
    }

    current_mouse_list->image_num = crsr_Finger;
    current_mouse_list->center_offset = 0;
    current_mouse_list->x1 = SCREEN_XMIN;
    current_mouse_list->y1 = SCREEN_YMIN;
    current_mouse_list->x2 = SCREEN_XMAX;
    current_mouse_list->y2 = SCREEN_YMAX;

    _osc_mouse_image_num = (crsr_CastAnim1 + _osc_mouse_image_cycle);

    mouse_list_count = 1;

    for(itr_my = 0; itr_my < MAP_HEIGHT; itr_my++)
    {

        for(itr_mx = 0; itr_mx < MAP_WIDTH; itr_mx++)
        {

            mx_x1 = (itr_mx * SQUARE_WIDTH);

            my_y1 = (MAP_SCREEN_Y + (itr_my * SQUARE_HEIGHT));

            mx_x2 = (mx_x1 + 19);

            my_y2 = (my_y1 + 17);

            current_mouse_list[mouse_list_count].center_offset = 0;
            current_mouse_list[mouse_list_count].x1 = mx_x1;
            current_mouse_list[mouse_list_count].y1 = my_y1;
            current_mouse_list[mouse_list_count].x2 = mx_x2;
            current_mouse_list[mouse_list_count].y2 = my_y2;
            current_mouse_list[mouse_list_count].center_offset = 0;

            if(_osc_spell_target_type == stt_Map_Square)
            {

                current_mouse_list[mouse_list_count].image_num = _osc_mouse_image_num;

                wx = (_map_x + itr_mx);

                if(wx >= WORLD_WIDTH)
                {
                    wx -= WORLD_WIDTH;
                }

                wy = (_map_y + itr_my);
                
                if(
                    SQUARE_UNEXPLORED(wx, wy, _map_plane)
                    &&
                    (_osc_spell_idx == spl_Earth_Lore)
                )
                {
                    current_mouse_list[mouse_list_count].image_num = crsr_RedCross;
                }

            }
            else
            {

                current_mouse_list[mouse_list_count].image_num = crsr_RedCross;

                entity_idx = entities_on_movement_map[((itr_my * MAP_WIDTH) + itr_mx)];

                if(entity_idx != ST_UNDEFINED)
                {

                    entity_idx = abs(entity_idx);

                    if(entity_idx < MAX_UNIT_COUNT)
                    {

                    }
                    else if(entity_idx < (MAX_UNIT_COUNT + 100))  // ¿ MAX_CITY_COUNT ?
                    {

                    }
                    else
                    {

                        if(_osc_spell_target_type == stt_Magic_Node)
                        {

                            current_mouse_list[mouse_list_count].image_num = _osc_mouse_image_num;

                        }

                    }

                }

            }

            mouse_list_count++;

        }

    }

    Set_Mouse_List(mouse_list_count, &current_mouse_list[0]);

    if(_osc_spell_target_type == stt_Magic_Node)
    {
        Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
    }

    _osc_mouse_image_cycle = ((_osc_mouse_image_cycle + 1) % 5);

}


// WZD o70p15
// drake178: sub_5D5D5()
void Spell_Casting_Screen_Draw_Panel(void)
{
    uint8_t colors[4] = { 0, 0, 0, 0 };
    int16_t itr = 0;
    int16_t IDK_first = 0;
    int16_t var_8[4] = { 0, 0, 0, 0 };
    int16_t IDK_ctr = 0;  // _SI_

    FLIC_Draw(242, 173, deselect_button_blockout);

    IDK_first = 124;

    IDK_ctr = 4;

    for(itr = 0; itr < IDK_ctr; itr++)
    {
        var_8[itr] = (IDK_first + itr);
    }

    FLIC_Draw(240, 76, cast_background);

    FLIC_Draw(240, 173, cast_button_border);

    colors[0] = 8;
    colors[1] = 161;
    colors[2] = 161;

    Set_Font_Colors_15(1, &colors[0]);

    Set_Font_Style_Shadow_Down(4, 0, 0, 0);

    Set_Outline_Color(148);

    Print_Centered(280, 80, str_Casting);  // panel title

    Set_Font_Style_Shadow_Down(1, 0, 15, 15);

    Set_Outline_Color(148);

    word_42C10--;
    if(word_42C10 < 0)
    {
        word_42C10 = (IDK_ctr - 1);
    }

    word_42C12++;
    if(word_42C12 == IDK_ctr)
    {
        word_42C12 = 0;
    }

    Print_Paragraph(249, 114, 60, _osc_panel_title, 0);

    Set_Outline_Color(ST_TRANSPARENT);

}
