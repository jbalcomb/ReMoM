/*
    WIZARDS.EXE
        ovr077

    Moo2
        Module: ¿ ?

*/

#include "MOX/MOX_DAT.H"  /* _screen_seg */
#include "MOX/MOX_SET.H"  /* magic_set */
#include "MOX/SOUND.H"

#include "MOM.H"



// WZD dseg:39D6                                                 BEGIN:  ovr077 - Initialized Data (Outpost Screen)
// WZD dseg:39D6
char music_lbx_file__ovr077[] = "MUSIC";
// WZD dseg:39DB
char emptystring__ovr077[] = "";
// WZD dseg:39DC
char str_NoMoreOutposts[] = "No more outposts may be built.";
// WZD dseg:39FB
char str_ESC__ovr077[] = "\x1B";
// WZD dseg:39FD
char str_TurnOffSpell_1__ovr077[] = "Do you wish to turn off the \x02";
// WZD dseg:3A1B
char str_TurnOffSpell_2__ovr077[] = "\x01 spell?";
// WZD dseg:3A24
char str_OutpostHeader_1[] = "New Outpost Founded";
// WZD dseg:3A38
char str_OutpostHeader_2[] = "Outpost Of ";
// WZD dseg:3A44
char str_NewOutpost[] = "New Outpost";
// WZD dseg:3A44                                                 END:  ovr077 - Initialized Data (Outpost Screen)

// WZD dseg:C2A6                                                 BEGIN:  ovr077 - Uninitialized Data  (Outpost Screen)
// WZD dseg:C2A6
int16_t G_CTY_SomeVar_0;
// WZD dseg:C2A8
int16_t m_new_outpost_flag;
// WZD dseg:C2AA
int16_t m_troop_count__ovr077;
// WZD dseg:C2AC
int16_t CTY_Garrison_Units[MAX_STACK];
// WZD dseg:C2BE 00 00                                           UU_IDK_OutpostScreen dw 0
// WZD dseg:C2AC                                                 END:  ovr077 - Uninitialized Data  (Outpost Screen)



/*
    WIZARDS.EXE ovr077
*/

// WZD o077p01
// drake178: TILE_Settle()
// Colonize_Planet_() |-> Init_Colony_()
/*
; creates a new outpost on the tile if possible,
; removing the settler from play in the process, and
; handling any necessary human player interactions
; returns 1 if successful, or 0 otherwise
*/
int16_t Create_Outpost(int16_t outpost_wx, int16_t outpost_wy, int16_t outpost_wp, int16_t outpost_race, int16_t outpost_owner, int16_t settler_unit_idx)
{
    SAMB_ptr sound_seg;
    int16_t Result;
    int16_t city_idx = 0;  // _SI_
    int16_t itr_players;  // _DI_
// TODO  warning: variable 'city_idx' is used uninitialized whenever 'if' condition is true [-Wsometimes-uninitialized]
 
    Result = Map_Square_Survey(outpost_wx, outpost_wy, outpost_wp);

    if(Result != 0)
    {
        Result = ST_FALSE;
    }
    else
    {

        if(Init_Outpost() != ST_TRUE)   
        {
            if(outpost_owner == _human_player_idx)
            {
                Warn0(str_NoMoreOutposts);  // "No more outposts may be built."
            }
            Result = ST_FALSE;
        }
        else
        {
            Kill_Unit(settler_unit_idx, 1);

            city_idx = (_cities - 1);
            _city_idx = city_idx;
            _CITIES[city_idx].Pop_10s = 3;

            _CITIES[city_idx].wx = outpost_wx;
            _CITIES[city_idx].wy = outpost_wy;
            _CITIES[city_idx].wp = outpost_wp;
            _CITIES[city_idx].owner_idx = outpost_owner;
            _CITIES[city_idx].race = outpost_race;


            if(outpost_owner == _human_player_idx)
            {
                Stop_All_Sounds__STUB();

                if(magic_set.event_music == ST_TRUE)
                {
                    sound_seg = LBX_Reload(music_lbx_file__ovr077, MUSIC_New_Outpost, SND_Music_Segment);
                    Play_Sound__STUB(sound_seg);
                }

                // BUG  Outpost_Screen(1, ST_UNDEFINED, ST_UNDEFINED);
                // ; displays and processes the new outpost naming dialog,
                // ; suggesting a default name, and falling back to it if
                // ; nothing is entered
                Change_City_Name_Popup(_city_idx, outpost_owner);

                Play_Background_Music__STUB();

                TILE_ExploreRadius__WIP(outpost_wx, outpost_wy, outpost_wp, 2);

            }
            else
            {
                Random_City_Name_By_Race(_CITIES[city_idx].race, _CITIES[city_idx].name);
            }

            for(itr_players = 0; itr_players < _num_players; itr_players++)
            {
                if(_players[itr_players].Globals[AWARENESS] > 0)
                {
                    if(itr_players == _human_player_idx)
                    {
                        TILE_ExploreRadius__WIP(outpost_wx, outpost_wy, outpost_wp, 1);
                    }
                    else
                    {
                        Set_Bit_Field(itr_players, &_CITIES[city_idx].PlayerBits);
                    }
                }
            }
        }

        // BUG  ; all of these have been set already
        _CITIES[city_idx].wx = outpost_wx;
        _CITIES[city_idx].wy = outpost_wy;
        _CITIES[city_idx].wp = outpost_wp;
        _CITIES[city_idx].owner_idx = outpost_owner;
        _CITIES[city_idx].race = outpost_race;

        NOOP_Current_Player_All_City_Areas();

        Do_City_Calculations(city_idx);

        TILE_CreateRoad(outpost_wx, outpost_wy, outpost_wp);

        Reset_City_Area_Bitfields();

        Reset_City_Road_Connection_Bitfields();

        Result = ST_TRUE;

    }

    return Result;

}


// WZD o077p02
/*

Main_Screen()
    target_world_x = (_main_map_grid_x * SQUARE_WIDTH);
    target_world_y = (MAP_SCREEN_Y + (_main_map_grid_y * SQUARE_HEIGHT));
    |-> Outpost_Screen(ST_FALSE);

*/
void Outpost_Screen(int16_t flag)
{
    int16_t screen_changed;
    int16_t screen_start_y;
    int16_t screen_start_x;
    int16_t y2;
    int16_t x2;
    int16_t y1;
    int16_t x1;
    int16_t full_screen_ESC_field;
    int16_t leave_screen;
    int16_t itr;  // _SI_
    int16_t itr_stack;  // _SI_
    int16_t input_field_idx;  // _DI_

    screen_changed = ST_FALSE;

    PageFlipEffect = 0;

    Set_Mouse_List(1, mouse_list_default);

    Clear_Fields();
    Set_Page_Off();
    Copy_On_To_Off_Page();
    PageFlip_FX();
    Copy_Off_To_Back();

    screen_start_x = 30;
    screen_start_y = 50;

    m_new_outpost_flag = flag;

    m_troop_count__ovr077 = 0;

    Army_At_City(_city_idx, &m_troop_count__ovr077, &CTY_Garrison_Units[0]);

    Outpost_Screen_Load();

    Do_Build_City_Enchantment_List();

    Deactivate_Auto_Function();

    Assign_Auto_Function(Outpost_Screen_Draw, 1);

    Deactivate_Help_List();

    Set_Outpost_Screen_Help_List();

    G_CTY_SomeVar_0 = 0;

    Set_Input_Delay(1);

    leave_screen = ST_FALSE;


    while(leave_screen == ST_FALSE)
    {
        Mark_Time();

        Clear_Fields();

        for(itr = 0; itr < MAX_STACK; itr++)
        {
            g_unit_window_fields[itr] = -100;  // ¿ BUG  typo? should be -1000 ?
        }

        if(m_new_outpost_flag == ST_FALSE)
        {
            for(itr = 0; itr < m_troop_count__ovr077; itr++)
            {
                Outpost_Garrison_Picture_Coords(itr, &x1, &y1, &x2, &y2);
                g_unit_window_fields[itr] = Add_Hidden_Field(x1, y1, x2, y2, emptystring__ovr077[0], ST_UNDEFINED);
            }
        }

        City_Add_Fields_City_Enchantments((screen_start_x + 114), (screen_start_y + 54));

        full_screen_ESC_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, str_ESC__ovr077[0], ST_UNDEFINED);

        input_field_idx = Get_Input();

        if(input_field_idx == full_screen_ESC_field)
        {
            Play_Left_Click__STUB();
            leave_screen = ST_UNDEFINED;
        }


        /*
            BEGIN:  City Enchantment List  (=== ¿ City_Screen() ?, Enemy_City_Screen(), Outpost_Screen)
        */
        {
            for(itr = 0; itr < city_enchantment_display_count; itr++)
            {
                if(
                    (city_enchantment_fields[itr] == input_field_idx)
                    &&
                    (city_enchantment_owner_list[(city_enchantment_display_first + itr)] == _human_player_idx)
                )
                {
                    Play_Left_Click__STUB();
                    Deactivate_Help_List();
                    strcpy(GUI_String_1, str_TurnOffSpell_1__ovr077);
                    strcat(GUI_String_1, _city_enchantment_names[city_enchantment_list[(city_enchantment_display_first + itr)]]);
                    strcat(GUI_String_1, str_TurnOffSpell_2__ovr077);
                    if(Confirmation_Box(GUI_String_1) == ST_TRUE)
                    {
                        // TODO  CTY_ClearEnchant(_city_idx, city_enchantment_list[(city_enchantment_display_first + itr)]);
                        Build_City_Enchantment_List(_city_idx, &city_enchantment_list[0], &city_enchantment_owner_list[0], &city_enchantment_list_count);
                        city_enchantment_display_scroll_flag = ST_FALSE;
                        if(city_enchantment_list_count > 6)
                        {
                            city_enchantment_display_scroll_flag = ST_TRUE;
                        }
                        else
                        {
                            city_enchantment_display_first = 0;
                        }
                    }
                    Deactivate_Auto_Function();
                    Assign_Auto_Function(Outpost_Screen_Draw, 1);
                    screen_changed = ST_TRUE;
                    Reset_Map_Draw();
                    Deactivate_Help_List();
                    Set_Enemy_City_Screen_Help_List();
                }
            }
        }
        /*
            END:  City Enchantment List  (=== ¿ City_Screen() ?, Enemy_City_Screen(), Outpost_Screen)
        */



        if(input_field_idx == city_up_button)
        {
            Play_Left_Click__STUB();
            city_enchantment_display_first -= 6;
            if(city_enchantment_display_first < 0)
            {
                city_enchantment_display_first = ((city_enchantment_list_count / 6) * 6);
            }
            screen_changed = ST_TRUE;
        }


        if(input_field_idx == city_dn_button)
        {
            Play_Left_Click__STUB();
            city_enchantment_display_first += 6;
            if(city_enchantment_display_first > city_enchantment_list_count)
            {
                city_enchantment_display_first = 0;
            }
            screen_changed = ST_TRUE;
        }


        /*
            BEGIN:  Left-Click Unit Window
        */
        {
            for(itr = 0; itr < m_troop_count__ovr077; itr++)
            {
                if(g_unit_window_fields[itr] == input_field_idx)
                {
                    Play_Left_Click__STUB();
                    if(_CITIES[_city_idx].owner_idx == _human_player_idx)
                    {
                        _active_world_x = _CITIES[_city_idx].wx;
                        _active_world_y = _CITIES[_city_idx].wy;
                        Select_Unit_Stack(_human_player_idx, &_map_x, &_map_y, _map_plane, _active_world_x, _active_world_y);
                    }
                    leave_screen = ST_UNDEFINED;
                }
            }
        }
        /*
            END:  Left-Click Unit Window
        */



        /*
            BEGIN:  Right-Click Unit Window
        */
        {
            // IDK_Col__unit_stack_idx
            for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
            {
                if(-(g_unit_window_fields[itr_stack]) == input_field_idx)
                {
                    Outpost_Garrison_Picture_Coords(itr_stack, &x1, &y1, &x2, &y2);
                    USW_FullDisplay(_unit_stack[itr_stack].unit_idx, (x1 + 1), (y1 + 1), (x2 + 1), (y2 + 1));
                    Outpost_Screen_Load();
                    Deactivate_Auto_Function();
                    Assign_Auto_Function(Outpost_Screen_Draw, 1);
                    Deactivate_Help_List();
                    Set_Outpost_Screen_Help_List();
                    screen_changed = ST_TRUE;
                }
            }
        }
        /*
            END:  Right-Click Unit Window
        */


        if((leave_screen == ST_FALSE) && (screen_changed == ST_FALSE))
        {
            Copy_Back_To_Off();
            Outpost_Screen_Draw();
            PageFlip_FX();
            Release_Time(1);
        }

        screen_changed = ST_FALSE;
    }


    Clear_Fields();
    Deactivate_Auto_Function();
    Deactivate_Help_List();
    PageFlipEffect = 3;

}


// WZD o077p03
/*
; draws the outpost screen into the current draw
; segment complete with outpost scape, units, growth
; indicators, and enchantment list
;
; BUG: fails to draw the altered backgrounds of the
; Gaia's Blessing, Flying Fortress, Famine, and Cursed
; Lands enchantments
*/
void Outpost_Screen_Draw(void)
{
    uint8_t colors[6];
    int16_t race_house_type;
    int16_t house_count;
    int16_t figure_y2;
    int16_t figure_x2;
    int16_t figure_y1;
    int16_t figure_x1;
    int16_t window_start_y;
    int16_t window_start_x;  // _DI_
    int16_t itr;  // _SI_

    window_start_x = 30;
    window_start_y = 50;

    Reset_Window();

// TODO  mov     ax, [_city_idx]
// TODO  mov     dx, size s_CITY
// TODO  imul    dx
// TODO  les     bx, [_CITIES]
// TODO  add     bx, ax
// TODO  mov     al, [es:bx+s_CITY.race]
// TODO  cbw
// TODO  mov     dx, size s_RACE
// TODO  imul    dx
// TODO  mov     bx, ax
// TODO  mov     ax, [_race_type_table.house_type+bx]
// TODO  add     ax, HLP_OUTPOST_SIZE_1
// TODO  mov     [_help_entries.help_00.entry_idx], ax

    Set_Page_Off();

    FLIC_Draw(window_start_x, window_start_y, outpost_background_seg);

    Outpost_Cityscape(_city_idx, (window_start_x + 185), (window_start_y + 30));

    Reset_Window();

    if(m_troop_count__ovr077 > 0)
    {
        // TODO  mov     [_help_entries.help_01.entry_idx], HLP_CITY_GARRISONS
    }
    else
    {
        // TODO  mov     [_help_entries.help_01.entry_idx], e_ST_UNDEFINED
    }

    for(itr = 0; itr < m_troop_count__ovr077; itr++)
    {
        Outpost_Garrison_Picture_Coords(itr, &figure_x1, &figure_y1, &figure_x2, &figure_y2);
        Draw_Unit_StatFig(figure_x1, figure_y1, CTY_Garrison_Units[itr], 1);
    }

    race_house_type = _race_type_table[_CITIES[_city_idx].race].house_type;

    house_count = City_House_Count(_city_idx);

    for(itr = 0; itr < 10; itr++)
    {
        figure_x1 = (window_start_x + 8 + (itr * 14));

        figure_y1 = (window_start_y + 31);

        if(itr < house_count)
        {
            FLIC_Draw(figure_x1, figure_y1, outpost_bright_icon_seg[race_house_type]);
        }
        else
        {
            FLIC_Draw(figure_x1, figure_y1, outpost_dark_icon_seg[race_house_type]);
        }

    }

    Set_Font_Style_Shadow_Down(5, 5, 0, 0);

    Set_Alias_Color(26);

    Set_Outline_Color(1);

    if(m_new_outpost_flag == ST_TRUE)
    {
        Print_Centered((window_start_x + 124), (window_start_y + 6), str_OutpostHeader_1);  // "New Outpost Founded"
    }
    else
    {
        strcpy(GUI_String_1, str_OutpostHeader_2);  // "Outpost Of "
        strcpy(GUI_String_2, _CITIES[_city_idx].name);
        strcat(GUI_String_1, GUI_String_2);
        Print_Centered((window_start_x + 124), (window_start_y + 6), GUI_String_1);
    }

    City_Screen_Draw_City_Enchantments((window_start_x + 114), (window_start_y + 54));

    colors[0] = 190;
    colors[1] = 179;

    Set_Font_Colors_15(0, &colors[0]);
    Set_Outline_Color(19);
    Set_Font_Style_Shadow_Down(1, 15, 0, 0);

    Print((window_start_x + 6), (window_start_y + 23), *_race_type_table[_CITIES[_city_idx].race].name);


}


// WZD o077p04
void Outpost_Screen_Load(void)
{
    SAMB_ptr temp_screen_seg;
    GUI_String_1 = (char *)Near_Allocate_First(100);
    GUI_String_2 = (char *)Near_Allocate_Next(100);
    city_enchantment_list = (int16_t *)Near_Allocate_Next(52);
    city_enchantment_owner_list = (int16_t *)Near_Allocate_Next(52);
    city_enchantment_fields = (int16_t *)Near_Allocate_Next(52);
    temp_screen_seg = Allocate_First_Block(_screen_seg, 1);
}


// WZD o077p05
/*

~== Unit_Window_Picture_Coords()
~== Item_Window_Picture_Coords()

*/
void Outpost_Garrison_Picture_Coords(int16_t slot, int16_t * x1, int16_t * y1, int16_t * x2, int16_t * y2)
{
    int16_t base_x;
    int16_t base_y;

    base_x = 30;
    base_y = 50;

    *x1 = (base_x + 6 + ((slot / 5) * 20));
    *y1 = (base_y + 53 + ((slot / 5) * 24));
    *x2 = (*x1 + 17);
    *y2 = (*y1 + 15);
}

// WZD o077p06
// MoO2  Module: MAINSCR  Change_Home_Star_Name_()
// MoO2  Module: NAMESTAR  Change_Home_Star_Name_Popup_()
/*
; displays and processes the new outpost naming dialog,
; suggesting a default name, and falling back to it if
; nothing is entered
*/
void Change_City_Name_Popup(int16_t city_idx, int16_t player_idx)
{
    uint8_t colors[14];
    char default_city_name[16];
    char city_name[16];
    int16_t y_start;
    int16_t x_start;
    int16_t UU_Edit_Return;

    PageFlipEffect = 0;

    colors[0] = 97;
    colors[1] = 97;
    colors[2] = 98;
    colors[3] = 98;
    colors[4] = 99;
    colors[5] = 99;
    colors[6] = 100;
    colors[7] = 100;
    colors[8] = 101;
    colors[9] = 101;
    colors[10] = 102;
    colors[11] = 102;
    colors[12] = 103;
    colors[13] = 103;

    x_start = 85;
    y_start = 100;

    Clear_Fields();

    Set_Page_Off();

    Copy_On_To_Off_Page();

    PageFlip_FX();

    Set_Mouse_List(1, mouse_list_default);

    Random_City_Name_By_Race(_CITIES[city_idx].race, _CITIES[city_idx].name);

    strcpy(city_name, _CITIES[city_idx].name);

    strcpy(default_city_name, city_name);

    Deactivate_Auto_Function();

    Assign_Auto_Function(Change_City_Name_Popup_Draw, 1);

    Clear_Fields();

    Set_Font_Style(4, 4, 3, 3);

    Set_Font_Spacing_Width(1);

    Set_Alias_Color(103);

    UU_Edit_Return = Input_Box_Popup((x_start + 16), (y_start + 21), 75, city_name, 12, 0, 0, 0, &colors[0], ST_UNDEFINED);

    // ; trims white space from the beginning and end of the passed string
    // ; BUGGED: will only remove one trailing space if there are multiple
    Trim(city_name);

    if(strlen(city_name) < 1)
    {
        strcpy(city_name, default_city_name);
    }

    strcpy(_CITIES[city_idx].name, city_name);

    Deactivate_Auto_Function();

    Clear_Fields();

}


// WZD o077p07
void Change_City_Name_Popup_Draw(void)
{
    int16_t start_x;  // _SI_
    int16_t start_y;  // _DI_

    start_x = 85;
    start_y = 100;

    FLIC_Draw(start_x, start_y, outpost_name_background_seg);

    Set_Font_Style(4, 4, 0, 0);

    Print((start_x + 10), (start_y + 6), str_NewOutpost);  // "New Outpost"

}


// WZD o077p08
/*
; returns 0 if the tile can be settled, or an error value if not:
;   1 - tile is in the ocean
;   2 - tile has an intact encounter zone
;   3 - tile has a tower of wizardry
;   4 - tile has a node
;   5 - there's already a city within 3 tiles
*/
int16_t Map_Square_Survey(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t Tile_Distance;
    int16_t itr;  // _SI_

    if(Square_Is_Sailable(wx, wy, wp) == ST_TRUE)
    {
        return 1;
    }

    for(itr = 0; itr < NUM_TOWERS; itr++)
    {
        if(
            (_TOWERS[itr].wx == wx)
            &&
            (_TOWERS[itr].wy == wy)
        )
        {
            return 3;
        }
    }

    for(itr = 0; itr < NUM_NODES; itr++)
    {
        if(
            (_NODES[itr].wp == wp)
            &&
            (_NODES[itr].wy == wy)
            &&
            (_NODES[itr].wx == wx)
        )
        {
            return 4;
        }
    }

    for(itr = 0; itr < NUM_LAIRS; itr++)
    {
        if(
            (_LAIRS[itr].wp == wp)
            &&
            (_LAIRS[itr].Intact == ST_TRUE)
            &&
            (_LAIRS[itr].wy == wy)
            &&
            (_LAIRS[itr].wx == wx)
        )
        {
            return 2;
        }
    }

    for(itr = 0; itr < NUM_LAIRS; itr++)
    {
        if(
            (_LAIRS[itr].wp == wp)
            &&
            (_LAIRS[itr].Intact == ST_TRUE)
            &&
            (_LAIRS[itr].wy == wy)
            &&
            (_LAIRS[itr].wx == wx)
        )
        {
            return 2;
        }
    }

    for(itr = 0; itr < _cities; itr++)
    {
        if(_CITIES[itr].wp == wp)
        {
            Tile_Distance = Delta_XY_With_Wrap(wx, wy, _CITIES[itr].wx, _CITIES[itr].wy, WORLD_WIDTH);
            if(Tile_Distance <= 3)
            {
                return 5;
            }
        }
    }

    return 0;
}

// WZD o077p09
// sub_67918()
