/*
    City List Screen - Cities Button

    WIZARDS.EXE
        ovr
    
    MoO2:
        Module: ¿ Planets/Colonies Screen ?
*/

#include "MoX.H"
#include "CityList.H"
// #include "MainScr.H"  /* Select_Unit_Stack() */
#include "MainScr_Maps.H"  /* List_Screen_Draw_Reduced_Map() */

/*
City List Screen Background
City List Screen OK Button

RELOAD.LBX
    Entries: 27
    Type:    0  ("Pictures")

    Name        Description
21  CITYLIST
22  CITYLBUT
*/

// WZD dseg:31E4 4D 61 6E 61 20 53 68 6F 72 74 00                cstr_Mana_Short db 'Mana Short',0       ; DATA XREF: Main_Screen_Draw_Summary_Window+438o
// WZD dseg:31E4                                                 ? END: Main_Screen_Draw_Summary_Window ?
// WZD dseg:31EF 00                                              db    0

// WZD dseg:31F0                                                 ¿ BEGIN: CityList Screen - Initialized Data ?

// WZD dseg:31F0
char citylist_hotkey_NUL = '\0';
// WZD dseg:31F1
char cnst_HOTKEY_O_3 = 'O';
// WZD dseg:31F3
char cnst_HOTKEY_Esc4 = '\x1B';
// WZD dseg:31F5
char cnst_HOTKEY_U_2 = 'U';
// WZD dseg:31F7
char cnst_HOTKEY_D_2 = 'D';
// WZD dseg:31F9
char cnst_RELOAD_File2[] = "RELOAD";
// WZD dseg:3200
char cnst_ARMYLIST_File[] = "ARMYLIST";
// WZD dseg:3209
char aTheCitiesOf[] = "The Cities Of ";
// WZD dseg:3218
char aName[] = "Name";
// WZD dseg:321D
char aRace[] = "Race";
// WZD dseg:3222
char aPop[] = "Pop";
// WZD dseg:3226
char cnst_Gold_3[] = "Gold";
// WZD dseg:322B
char aPrd[] = "Prd";
// WZD dseg:322F
char aProducing[] = "Producing";
// WZD dseg:3239
char aTime[] = "Time";
// WZD dseg:323E
char cnst_GP_2[] = "GP";
// WZD dseg:3241
char cnst_MP_2[] = "MP";

// WZD dseg:3241                                                 ¿ END: CityList Screen - Initialized Data ?

// WZD dseg:3244
// WZD dseg:3244                                                 ¿ BEGIN: ArmyList Screen - Initialized Data ?
// WZD dseg:3244
// WZD dseg:3244 00                                              unk_39CE4 db    0                       ; DATA XREF: ArmyList_Screen+10Do ...


// WZD dseg:94F2 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IMG_CTY_Workers@ dw 0Eh dup(0)          ; DATA XREF: City_Screen_Load_Pictures+136w ...
// WZD dseg:94F2 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; array of 14 appended reserved EMM headers in
// WZD dseg:94F2                                                                                         ; GFX_Swap_Seg, 1 worker image per header

// WZD dseg:950E                                                 BEGIN: CityList_Screen_Load()
// WZD dseg:950E
SAMB_ptr citylist_ok_button_seg;            // ; 2 frame image (normal - clicked)
// WZD dseg:9510
SAMB_ptr citylist_down_button_seg;          // ; 2 frame image (normal - clicked)
// WZD dseg:9512
SAMB_ptr citylist_up_button_seg;            // ; 2 frame image (normal - clicked)
// WZD dseg:9514
SAMB_ptr citylist_background_seg;
// WZD dseg:9514                                                 END: CityList_Screen_Load()

// WZD dseg:9516                                                 BEGIN:  Main Screen Pictures
// WZD dseg:9516 00 00                                           next_turn_button_seg dw 0               ; DATA XREF: Main_Screen_Load_Pictures+299w ...




/*
TODO(JimBalcomb,20230922): finish ArmyList_Screen()
    turn back on drawing city info, city name, enchantments, highlight
    add left-click for 'City Screen'
    add right-click for 'Production Screen'
    gold is broken for "Bremen" - should be 90, shouws -166 ... (Gold - Upkeed)
    *.* city garrison
    *.* city enchantments
    *.* production cost
    *.* production time
*/
// WZD o65p01
void CityList_Screen(void)
{
// Units@= word ptr -2Ah
// Count@= word ptr -18h
    int16_t hotkey_ESC;
    int16_t screen_changed;
    int16_t button_citylist_ok;
    int16_t hotkey_D;
    int16_t hotkey_U;
    int16_t citylist_down_button_right;
    int16_t citylist_up_button_right;
    int16_t citylist_down_button_left;
    int16_t citylist_up_button_left;
    int16_t scanned_field;
    int16_t leave_screen_flag;

    int16_t input_field_idx;
    int16_t hotkey_idx_ESC;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: CityList_Screen()\n", __FILE__, __LINE__);
#endif

    CityList_Screen_Load();

    Load_Palette_From_Animation(citylist_background_seg);
    Apply_Palette();
    Clear_Palette_Changes(0, 243);
    Set_Palette_Changes(0, 255);
    Update_Remap_Color_Range(0, 1);

    // TODO word_42B54 = 16;
    // TODO word_42B52 = 16;

    Build_City_List();

    list_first_item = IDK_CityList_list_first_item;

    Update_Cities_List();

    // TODO  CTY_GetGarrison(&Count, &Units[0]);

    // TODO  Deactivate_Auto_Function();
    // TODO  Assign_Auto_Function(CityList_Screen_Draw(), 1);
    Set_Input_Delay(1);

    CityList_Draw_Reduced_Map();

    Set_Mouse_List(1, mouse_list_default);

    CityList_Set_List_Item_Count();

    citylist_item_scanned_field = 0;

    // TODO  CityList_Load_Help();

    Set_Input_Delay(2);

    screen_changed = ST_FALSE;
    leave_screen_flag = ST_FALSE;

    while(leave_screen_flag == ST_FALSE)
    {
        Mark_Time();

        Clear_Fields();

        CityList_Add_List_Fields();


        // DEMO
        hotkey_idx_ESC = Add_Hidden_Field(0, 0, 319, 199, 27, -1);


        Set_Font(0, 0, 6, 15);
        button_citylist_ok = Add_Button_Field(239, 182, "", citylist_ok_button_seg, cnst_HOTKEY_O_3, ST_UNDEFINED);
        hotkey_ESC = Add_Hot_Key(cnst_HOTKEY_Esc4);

        // YayNay City List Scroll Up
        if(list_first_item != 0)
        {
            // TODO  _help_entries = 244
            // TODO  _help_entries+14h = 244

            hotkey_U = Add_Hot_Key(cnst_HOTKEY_U_2);
            citylist_up_button_left  = Add_Button_Field( 11, 26, "", citylist_up_button_seg, citylist_hotkey_NUL, ST_UNDEFINED);
            citylist_up_button_right = Add_Button_Field(299, 26, "", citylist_up_button_seg, citylist_hotkey_NUL, ST_UNDEFINED);
        }
        else
        {
            // TODO  _help_entries = ST_UNDEFINED
            // TODO  _help_entries+14h = ST_UNDEFINED

            hotkey_U = INVALID_FIELD;
            citylist_up_button_left = INVALID_FIELD;
            citylist_up_button_right = INVALID_FIELD;
        }

        // YayNay City List Scroll Down
        if( (((list_first_item + 9) - citylist_city_count) != 0) && ((list_item_count - citylist_city_count) != 0) )
        {
            hotkey_D = Add_Hot_Key(cnst_HOTKEY_D_2);
            citylist_down_button_left  = Add_Button_Field( 11, 139, "", citylist_down_button_seg, citylist_hotkey_NUL, ST_UNDEFINED);
            citylist_down_button_right = Add_Button_Field(299, 139, "", citylist_down_button_seg, citylist_hotkey_NUL, ST_UNDEFINED);
        }
        else
        {
            // TODO  _help_entries+0Ah = ST_UNDEFINED
            // TODO  _help_entries+1Eh = ST_UNDEFINED

            hotkey_D = INVALID_FIELD;
            citylist_down_button_left = INVALID_FIELD;
            citylist_down_button_right = INVALID_FIELD;
        }


        input_field_idx = Get_Input();

        scanned_field = Scan_Input();

        if(input_field_idx == hotkey_idx_ESC)
        {
            leave_screen_flag = ST_TRUE;
        }


        /*
            Leave Screen
        */
        if(input_field_idx == ST_UNDEFINED || input_field_idx == button_citylist_ok || input_field_idx == hotkey_ESC)
        {
            // SND_LeftClickSound();
            leave_screen_flag = ST_TRUE;
            current_screen = scr_Main_Screen;
        }

        // ...
        // ...
        // ...

        if(leave_screen_flag == ST_FALSE && screen_changed == ST_FALSE)
        {
            CityList_Screen_Draw();
            PageFlip_FX();
            Release_Time(1);
        }
        screen_changed = ST_FALSE;
    }

    // TODO  Deactivate_Help_List();
    // TODO  Deactivate_Auto_Function();
    Reset_Window();
    Clear_Fields();
    Clear_Palette_Changes(0, 255);
    Set_Palette_Changes(0, 223);
    // TODO  Update_Remap_Gray_Palette();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: CityList_Screen()\n", __FILE__, __LINE__);
#endif
}


// WZD o65p02
void CityList_Draw_Reduced_Map(void)
{
    int16_t city_world_x;
    int16_t city_world_y;
    int16_t city_world_p;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: CityList_Draw_Reduced_Map()\n", __FILE__, __LINE__);
#endif

    city_world_x = _CITIES[list_cities[citylist_item_scanned_field]].wx;
    city_world_y = _CITIES[list_cities[citylist_item_scanned_field]].wy;
    city_world_p = _CITIES[list_cities[citylist_item_scanned_field]].wp;

    List_Screen_Draw_Reduced_Map(CITYLIST_REDUCED_MAP_X, CITYLIST_REDUCED_MAP_Y, CITYLIST_REDUCED_MAP_W, CITYLIST_REDUCED_MAP_H, city_world_p, city_world_x, city_world_y);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: CityList_Draw_Reduced_Map()\n", __FILE__, __LINE__);
#endif
}




// WZD o65p03
void CityList_Screen_Load(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: CityList_Screen_Load()\n", __FILE__, __LINE__);
#endif

    city_list = Near_Allocate_First(200);  // 100 2-byte ints  ~== sizeof(int) * CITY_COUNT_MAX

    list_cities = Near_Allocate_Next(18);  // ¿ 9 2-byte ints, for City List of 9 ?

    GUI_String_1 = (char *)Near_Allocate_Next(80);  // why 80 here, but 100 for ArmyList?

    city_enchantment_list = (int16_t *)Near_Allocate_Next(52);  // ¿ 26 2-byte ints ?
    city_enchantment_owner_list = (int16_t *)Near_Allocate_Next(52);  // ¿ 26 2-byte ints ?

    _reduced_map_seg = Allocate_First_Block(_screen_seg, 153);

    citylist_background_seg = LBX_Reload_Next("RELOAD", 21, _screen_seg);
    citylist_ok_button_seg = LBX_Reload_Next("RELOAD", 22, _screen_seg);
    citylist_up_button_seg = LBX_Reload_Next("ARMYLIST", 1, _screen_seg);
    citylist_down_button_seg = LBX_Reload_Next("ARMYLIST", 2, _screen_seg);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: CityList_Screen_Load()\n", __FILE__, __LINE__);
#endif
}


// WZD o65p04
void CityList_Screen_Draw(void)
{
    uint8_t colors1[6];
    uint8_t colors2[6];
    int16_t production_time;
    int16_t production_cost;
    int16_t city_construction;
    int16_t unit_type;
    int16_t city_idx;

    int16_t itr_colors;
    int16_t itr_list_item_count;
    int16_t y_offset;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: CityList_Screen_Draw()\n", __FILE__, __LINE__);
#endif

    CityList_Set_List_Item_Count();

    Set_Page_Off();
    
    FLIC_Draw(0, 0, citylist_background_seg);

    
    /*
        Up Arrows
    */
    if(list_first_item == 0 && citylist_city_count > 9)
    {
        FLIC_Set_CurrentFrame(citylist_up_button_seg, 1);
        FLIC_Draw(11, 26, citylist_up_button_seg);
        FLIC_Set_CurrentFrame(citylist_up_button_seg, 1);
        FLIC_Draw(299, 26, citylist_up_button_seg);
    }
    else
    {
        FLIC_Set_CurrentFrame(citylist_up_button_seg, 0);
        FLIC_Draw(11, 26, citylist_up_button_seg);
        FLIC_Set_CurrentFrame(citylist_up_button_seg, 0);
        FLIC_Draw(299, 26, citylist_up_button_seg);
    }

    /*
        Down Arrows
    */
    if( citylist_city_count > 6 && ( (((list_first_item + 9) - citylist_city_count) == 0)  || ((list_item_count - citylist_city_count) == 0) ) )
    {
        FLIC_Set_CurrentFrame(citylist_down_button_seg, 1);
        FLIC_Draw(11, 139, citylist_down_button_seg);
        FLIC_Set_CurrentFrame(citylist_down_button_seg, 1);
        FLIC_Draw(299, 139, citylist_down_button_seg);
    }
    else
    {
        FLIC_Set_CurrentFrame(citylist_down_button_seg, 0);
        FLIC_Draw(11, 139, citylist_down_button_seg);
        FLIC_Set_CurrentFrame(citylist_down_button_seg, 0);
        FLIC_Draw(299, 139, citylist_down_button_seg);
    }


    // BEGIN: Print Title

    strcpy(GUI_String_1, aTheCitiesOf);
    strcat(GUI_String_1, _players[_human_player_idx].Name);
    for(itr_colors = 0; itr_colors < 5; itr_colors++)
    {
        colors1[itr_colors] = 237;
    }
    Set_Font_Colors_15(4, &colors1[0]);
    Print_Centered(162, 5, GUI_String_1);
    Print_Centered(161, 5, GUI_String_1);
    Print_Centered(162, 4, GUI_String_1);
    for(itr_colors = 0; itr_colors < 5; itr_colors++)
    {
        colors1[itr_colors] = 238;
    }
    Set_Font_Colors_15(4, &colors1[0]);
    Print_Centered(161, 3, GUI_String_1);
    Print_Centered(160, 4, GUI_String_1);
    for(itr_colors = 0; itr_colors < 5; itr_colors++)
    {
        colors1[itr_colors] = 239;
    }
    Set_Font_Colors_15(4, &colors1[0]);
    Print_Centered(161, 4, GUI_String_1);
    colors2[0] = 236;
    colors2[1] = 177;
    colors2[2] = 178;
    colors2[3] = 177;
    colors2[4] = 176;
    Set_Font_Colors_15(4, &colors2[0]);
    Print_Centered(160, 3, GUI_String_1);

    // END: Print Title


    Set_Outline_Color(231);
    colors2[0] = 236;
    colors2[1] = 129;
    colors2[2] = 129;
    Set_Font_Colors_15(1, &colors2[0]);  // ¿ sets font style 1 and font remap colors block ?
    Set_Font_Style1(1, 15, 0, 0);  // shadow - bottom-right, 1 pixel; use special/custom colors font color block

    Print(32, 17, aName);
    Print(88, 17, aRace);
    Print(136, 17, aPop);
    Print(154, 17, cnst_Gold_3);
    Print(176, 17, aPrd);
    Print(197, 17, aProducing);
    Print(272, 17, aTime);

    itoa(_players[_human_player_idx].gold_reserve, GUI_String_1, 10);
    strcat(GUI_String_1, cnst_GP_2);
    Print_Centered(242, 172, GUI_String_1);

    itoa(_players[_human_player_idx].mana_reserve, GUI_String_1, 10);
    strcat(GUI_String_1, cnst_MP_2);
    Print_Centered(276, 172, GUI_String_1);


    Set_Font_Spacing_Width(1);

    /*
        City Info
        Scanned City Highlight
    */
    for(itr_list_item_count = 0; itr_list_item_count < list_item_count; itr_list_item_count++)
    {
        city_idx = list_cities[itr_list_item_count];

        if(itr_list_item_count == citylist_item_scanned_field)
        {
            // jmp     short $+2
        }

        Set_Outline_Color(231);

        if(itr_list_item_count == citylist_item_scanned_field)
        {
            y_offset = 25 + (14 * itr_list_item_count);
            // DEMO  Gradient_Fill( 30, (y_offset + 1),  81, (y_offset + 9), 15, 1, ST_NULL, ST_NULL, ST_NULL);
            // DEMO  Gradient_Fill( 86, (y_offset + 1), 130, (y_offset + 9), 15, 1, ST_NULL, ST_NULL, ST_NULL);
            // DEMO  Gradient_Fill(135, (y_offset + 1), 150, (y_offset + 9), 15, 1, ST_NULL, ST_NULL, ST_NULL);
            // DEMO  Gradient_Fill(155, (y_offset + 1), 170, (y_offset + 9), 15, 1, ST_NULL, ST_NULL, ST_NULL);
            // DEMO  Gradient_Fill(175, (y_offset + 1), 190, (y_offset + 9), 15, 1, ST_NULL, ST_NULL, ST_NULL);
            // DEMO  Gradient_Fill(195, (y_offset + 1), 270, (y_offset + 9), 15, 1, ST_NULL, ST_NULL, ST_NULL);
            // DEMO  Gradient_Fill(275, (y_offset + 1), 289, (y_offset + 9), 15, 1, ST_NULL, ST_NULL, ST_NULL);
        }

        // DOS  Print_Far(31, (27 + 14 * itr_list_item_count, (FP_OFF(_CITIES) + (sizeof(s_CITY) * city_idx)), FP_SEG(_CITIES));
        // DEMO  Print(31, (27 + 14 * itr_list_item_count), _CITIES[city_idx].name);
        // TODO  Print(87, (27 + 14 * itr_list_item_count), _unit_race_table[_CITIES[city_idx].race]);
        // DEMO  Print_Integer_Right(148, (27 + 14 * itr_list_item_count), _CITIES[city_idx].Pop_K);
        // DEMO  Print_Integer_Right(168, (27 + 14 * itr_list_item_count), (_CITIES[city_idx].Gold - _CITIES[city_idx].Upkeep));
        // DEMO  Print_Integer_Right(188, (27 + 14 * itr_list_item_count), _CITIES[city_idx].Production);

        city_construction = _CITIES[city_idx].construction;
        if(city_construction < 100)  /* building */
        {
            // DOS  String_Copy_Far(GUI_String_1, 0, (FP_OFF(build_data_table) + (sizeof(build_data_table) * city_construction)), FP_SEG(build_data_table));
            // DEMO  strcpy(GUI_String_1, build_data_table[city_construction].name);
        }
        else  /* unit */
        {
            unit_type = city_construction - 100;
            // TODO  strcpy(GUI_String_1, *_unit_type_table[unit_type].Name);
        }
        // DEMO  Print(196, (27 + 14 * itr_list_item_count), GUI_String_1);

        // TODO  production_cost = CTY_GetProduceCost(city_construction, city_idx);
        // TODO  production_time = sub_340E2(production_cost, city_idx);
        // DEMO  Print_Integer_Right(287, (27 + 14 * itr_list_item_count), production_time);

    }


    if(citylist_city_count > 0)
    {
        city_idx = list_cities[citylist_item_scanned_field];
        Set_Outline_Color(231);
        colors1[0] = 236;
        colors1[1] = 129;
        colors1[2] = 129;
        Set_Font_Colors_15(1, &colors1[0]);
        Set_Font_Style1(1, 15, 0, 0);
        strcpy(GUI_String_1, _CITIES[city_idx].name);
        String_To_Upper(GUI_String_1);
        // DEMO  Print(99, 158, GUI_String_1);

        // TODO  CTY_GetEnchants(city_idx, city_enchantment_list, city_enchantment_owner_list, &CTY_EnchantCount);
        // TODO  CTY_Print_Enchantments(100, 168, city_enchantment_list, city_enchantment_owner_list, (CTY_EnchantCount > 8 ? 8 : CTY_EnchantCount)) // maybe just MAX() macro?

        // DEMO  CityList_Draw_Reduced_Map();
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: CityList_Screen_Draw()\n", __FILE__, __LINE__);
#endif
}


// WZD o65p05
// ~ MoO2 Build_Global_Colony_List_()
void Build_City_List(void)
{
    int16_t itr_cities;

    citylist_city_count = 0;  /* MoO2  DNE, always does 250 - max colonies */

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)  /* MoO2  _NUM_COLONIES ~== _cities */
    {
        if(_CITIES[itr_cities].owner_idx == _human_player_idx)  /* MoO2  _PLAYER_NUM ~== _human_player_idx */
        {
            city_list[citylist_city_count] = itr_cities;  /* MoO2  _g_colony_list_ptr */

            citylist_city_count++;

            if(_CITIES[itr_cities].construction == 0x00)  /* ¿ None/Nothing/Unset ? */
            {
                _CITIES[itr_cities].construction = 0x02;  /* ¿ Housing ? */
            }
        }
    }
}


// WZD o65p06
// ~ MoO2  Update_Col_List_() ... sets _list_col from _g_colony_list_ptr, based on _first + itr
/*
    sets the list of Cities to display, from list_first_item to list_item_count
*/
void Update_Cities_List(void)
{
    int16_t itr_list_items;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Update_Cities_List()\n", __FILE__, __LINE__);
#endif

    CityList_Set_List_Item_Count();

    for(itr_list_items = 0; itr_list_items < list_item_count; itr_list_items++)
    {
        list_cities[itr_list_items] = city_list[(list_first_item + itr_list_items)];
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Update_Cities_List()\n", __FILE__, __LINE__);
#endif
}


// WZD o65p07
void CityList_Add_List_Fields(void)
{
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
    int16_t itr_list_item_count;

    citylist_item_count = 0;

    CityList_Set_List_Item_Count();

    for(itr_list_item_count = 0; itr_list_item_count < list_item_count; itr_list_item_count++)
    {
        x1 = 30;
        y1 = (26 + (14 * itr_list_item_count));
        x2 = x1 + 259;
        y2 = y1 + 8;
        citylist_item_fields[citylist_item_count] = Add_Hidden_Field(x1, y1, x2, y2, citylist_hotkey_NUL, ST_UNDEFINED);
        citylist_item_count++;
    }

}


// WZD o65p08
// ~== IDK_ArmyList_Set_List_Item_Count
// ~ MoO2  Update_Col_List_()
/*
    sets list item count to min of 9 or city count
*/
void CityList_Set_List_Item_Count(void)
{
    list_item_count = 9 + list_first_item;

    if(list_item_count > 9)
    {
        list_item_count = 9;
    }

    if(list_item_count > citylist_city_count)
    {
        list_item_count = citylist_city_count;
    }

}
