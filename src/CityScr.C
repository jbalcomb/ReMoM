
#include "MoM.H"
#include "CityScr.H"
#include "City_ovr55.H"
#include "CITYSCAP.H"
#include "MainScr.H"
#include "MainScr_Maps.H"
#include "ProdScr.H"
#include "UnitStat.H"



// WZD dseg:2A8A                                                 BEGIN:  ovr054 - Strings  (City Screen)

// WZD dseg:2A8A 58                                              
char str_hotkey_X[] = "X";
// WZD dseg:2A8B
char emptystring__ovr054[] = "";  // string optimizer

// WZD dseg:2A8C
char aPopulation[] = "population";
// WZD dseg:2A97
char aProduction[] = "production";
// WZD dseg:2AA2
char aCity_0[] = "city";
// WZD dseg:2AA7
char aWalls[] = "walls";
// WZD dseg:2AAD
char aDoYouWishToSpend[] = "Do you wish to spend \x02";
// WZD dseg:2AC4
char aGold_3[] = " Gold";
// WZD dseg:2ACB
char aByPurchasing[] = "\x01  by purchasing ";
// WZD dseg:2AD9
char cnst_Space[] = " ";  // string optimizer
// WZD dseg:2ADB
char cnst_QuestionMark[] = "?";
// WZD dseg:2ADD
char cnst_SpellCancel_Msg[] = "Do you wish to turn off the \x02";
// WZD dseg:2AFB
char cnst_SpellCnclMsg_12[] = "\x01  spell?";
// WZD dseg:2B04
char aYouCanOnlySellBackO[] = "You can only sell back one building each turn.";
// WZD dseg:2B31
char cnst_Dot[] = ".";  // string optimizer
// WZD dseg:2B33
char aSellingBackYour[] = "Selling back your \x02";
// WZD dseg:2B33
char aWillCeaseProductionOfYour[] = "\x01 will cease production of your \x02";
// WZD dseg:2B69
char aWillCeaseProductionOfYour2[] = "\x01.";
// WZD dseg:2B6C
char str_sell_back_1[] = "Do you wish to sell back the \x02";
// WZD dseg:2B8B
char str_sell_back_2[] = "\x01 for ";
// WZD dseg:2B92
char str_sell_back_3[] = " gold?";
// WZD dseg:2B99
char str_cannot_sell_back_1[] = "You cannot sell back the ";
// WZD dseg:2BB3
char str_cannot_sell_back_2[] = " because it is required by the ";

// WZD dseg:2BD3
char proddescr_tradegoods[] = "Converts production to gold.";
// WZD dseg:2BF0
char proddescr_housing[] = "Increases population growth rate.";

// WZD dseg:2C12
char cnst_Sp_Of_Sp[] = " of ";
// WZD dseg:2C17
char aPlague[] = " (Plague)";
// WZD dseg:2C21
char aPop_Boom[] = " (Pop. Boom)";
// WZD dseg:2C2E
char aPopulation_0[] = "Population: ";
// WZD dseg:2C3B
char cnst_Comma_0[] = ",";
// WZD dseg:2C3D
char a00[] = "00";
// WZD dseg:2C40
char cnst_SpaceOpenBrace[] = " (";
// WZD dseg:2C43
char cnst_Plus[] = "+";
// WZD dseg:2C45
char aTurns_0[] = "Turns";
// WZD dseg:2C4B
char aTurn_0[] = "Turn";
// WZD dseg:2C50
char str_hotkey_B[] = "B";
// WZD dseg:2C52
char str_hotkey_C[] = "C";
// WZD dseg:2C54
char str_hotkey_O[] = "O";
// WZD dseg:2C56
char str_hotkey_ESC[] = "\x0B";
// WZD dseg:2C58
char aThe_2[] = "The ";
// WZD dseg:2C5D
char aHasCompletedTheCons[] = " has completed the construction of ";
// WZD dseg:2C81
char music_lbx_file__ovr054[] = "MUSIC";
// WZD dseg:2C87
char cstr_NameStartingCity[] = "Name Starting City";


// WZD dseg:2C87                                                 END:  ovr054 - Strings  (City Screen)





// WZD dseg:92B0
// drake178: cityscape_bldgs@


// WZD dseg:92B4
int16_t CTY_Entity_Count;



// WZD dseg:9936
SAMB_ptr city_background_seg;

// WZD dseg:9938
SAMB_ptr city_block_out_seg;

// WZD dseg:993A
SAMB_ptr city_full_resource_seg;

// WZD dseg:993C
SAMB_ptr city_reqd_resource_seg;

// WZD dseg:993E
SAMB_ptr city_lock_buy_button_seg;

// WZD dseg:9940
SAMB_ptr city_ok_button_seg;

// WZD dseg:9942
SAMB_ptr city_buy_button_seg;

// WZD dseg:9944
SAMB_ptr city_change_button_seg;





void City_Screen__WIP(void)
{
    int16_t zz_active_stack_flag;
    int16_t city_rebel_count;
    int16_t required_farmer_count;
    int16_t uv_y2;
    int16_t uv_x2;
    int16_t uv_y1;
    int16_t uv_x1;
    int16_t cityscape_bldg_idx;
    int16_t building_value;
    int16_t hotkey_X;
    int16_t city_wy;
    int16_t city_wx;
    // IDK_Row__prod_idx
    int16_t Row;
    int16_t unit_type;
    // IDK_Col__unit_stack_idx
    int16_t Col;  // IDK_Col__unit_stack_idx
    int16_t itr_stack;  // IDK_Col__unit_stack_idx
    int16_t screen_changed;
    int16_t leave_screen;
    int16_t input_field_idx;  // _DI_

    Set_City_Screen_Help_List();

    if((PageFlipEffect != 0) && (PageFlipEffect != 3))
    {
        PageFlipEffect = 0;
    }


    _active_stack_has_path = ST_FALSE;


    Deactivate_Auto_Function();
    Assign_Auto_Function(City_Screen_Draw__WIP, 1);


    City_Screen_Load();


    city_map_wx = 0;
    city_map_wy = 0;

    City_Center_Map(&city_map_wx, &city_map_wy, _CITIES[_city_idx].wx, _CITIES[_city_idx].wy, _map_plane);


    _garrison_window_start_x = 215;
    _garrison_window_start_y = 102;


    zz_active_stack_flag = ST_FALSE;
    if(_unit_stack_count != 0)
    {
        zz_active_stack_flag = ST_TRUE;
    }


    city_wx = _CITIES[_city_idx].wx;
    city_wy = _CITIES[_city_idx].wy;

    Select_Unit_Stack(_human_player_idx, &city_map_wx, &city_map_wy, _map_plane, city_wx, city_wy);

    Set_Draw_Active_Stack_Always();

    Set_Entities_On_Map_Window(city_map_wx, city_map_wy, _map_plane);

    Reset_Map_Draw();


    City_Screen_Required_Buildings_List(_city_idx);

    Do_Build_City_Enchantment_List();


    required_farmer_count = City_Minimum_Farmers(_city_idx);

    if(_CITIES[_city_idx].farmer_count < required_farmer_count)
    {
        _CITIES[_city_idx].farmer_count = required_farmer_count;
    }

    city_rebel_count = City_Rebel_Count(_city_idx);

    if(_CITIES[_city_idx].farmer_count > (_CITIES[_city_idx].population - city_rebel_count))
    {
        _CITIES[_city_idx].farmer_count = (_CITIES[_city_idx].population - city_rebel_count);
    }

    if(_CITIES[_city_idx].construction == bt_NONE)
    {
        _CITIES[_city_idx].construction = bt_Housing;
    }


    Do_City_Calculations(_city_idx);
    m_city_production_cost = City_Production_Cost(_CITIES[_city_idx].construction, _city_idx);
    m_city_n_turns_to_produce = City_N_Turns_To_Produce(m_city_production_cost, _city_idx);


    if(_CITIES[_city_idx].construction < 100)
    {
        strcpy(city_screen_product_name, bldg_data_table[_CITIES[_city_idx].construction].name);
    }
    else
    {
        Row = (_CITIES[_city_idx].construction - 100);
        strcpy(city_screen_product_name, *_unit_type_table[Row].name);
    }


    CTY_Entity_Count = 0;

    City_Can_Buy_Product();

    city_screen_scanned_field = ST_UNDEFINED;


    City_Screen_Allocate_First_Block();


    Do_City_Calculations(_city_idx);
    m_city_production_cost = City_Production_Cost(_CITIES[_city_idx].construction, _city_idx);
    m_city_n_turns_to_produce = City_N_Turns_To_Produce(m_city_production_cost, _city_idx);


    Reset_Cycle_Palette_Color();

    Set_Input_Delay(2);

    screen_changed = ST_FALSE;
    leave_screen = ST_FALSE;
    city_screen_scanned_field = ST_UNDEFINED;

    while(leave_screen == ST_FALSE)
    {
        Mark_Time();

        City_Screen_Add_Fields__WIP();

        // hotkey_X = Add_Hot_Key(cnst_HOTKEY_X);
        hotkey_X = Add_Hot_Key('X');

        input_field_idx = Get_Input();

        city_screen_scanned_field = Scan_Input();



        /*
            Left-Click Resource Row
        */
        /*
            loop 6 resources
            IDK_CityScr_RsrcBrkdn(rsrc)
        */



        /*
            Hot-Key X
        */
        if(input_field_idx == hotkey_X)
        {

        }



        /*
            Left-Click Up Button
        */



        /*
            Left-Click Down Button
        */


        /*
            BEGIN:  Left-Click Change Button
        */
        {
            if(input_field_idx == city_sceen_change_button)
            {
                // TODO  SND_LeftClickSound();
                production_screen_return_screen = 2;  // {1: CityList Screen, 2: City Screen}
                Production_Screen();

                City_Screen_Load();
                Deactivate_Help_List();
                Set_City_Screen_Help_List();
                Assign_Auto_Function(City_Screen_Draw__WIP, 1);
                Do_City_Calculations(_city_idx);
                m_city_production_cost = City_Production_Cost(_CITIES[_city_idx].construction, _city_idx);
                m_city_n_turns_to_produce = City_N_Turns_To_Produce(m_city_production_cost, _city_idx);
                City_Can_Buy_Product();
                if(_CITIES[_city_idx].construction < 100)
                {
                    // TODO  String_Copy_Far(IDK_production_title, bldg_data_table[_CITIES[_city_idx].construction]);
                    strcpy(city_screen_product_name, bldg_data_table[_CITIES[_city_idx].construction].name);
                }
                else
                {
                    Row = (_CITIES[_city_idx].construction - 100);
                    strcpy(city_screen_product_name, *_unit_type_table[Row].name);
                }
                City_Screen_Allocate_First_Block();

                production_screen_return_screen = 2;
                screen_changed = ST_TRUE;
            }
        }
        /*
            END:  Left-Click Change Button
        */


        /*
            BEGIN:  Left-Click OK Button  ||  Hot-Key ESC
        */
        {
            if((input_field_idx == city_screen_ok_button) || (input_field_idx == city_screen_esc_hotkey))
            {
                _unit_stack_count = 0;
                // TODO  SND_LeftClickSound()
                _prev_world_x = _map_x;
                _prev_world_y = _map_y;
                Deactivate_Auto_Function();
                Deactivate_Help_List();
                if(zz_active_stack_flag == ST_TRUE)
                {
                    _unit_stack_count = 0;  // BUGBUG  does this always above - what is controlled by _unit_stack_count?
                    // jmp     short $+2
                }
                leave_screen = ST_TRUE;
                current_screen = scr_Main_Screen;
            }
        }
        /*
            END:  Left-Click OK Button  ||  Hot-Key ESC
        */


        /*
            BEGIN:  Left-Click Buy Button
        */
        {
            if(input_field_idx == city_screen_buy_button)
            {
                // TODO  SND_LeftClickSound();
                building_value = _players[_human_player_idx].gold_reserve;
                if(
                    (m_city_n_turns_to_produce > 1)
                    &&
                    (City_Cost_To_Buy_Product(_city_idx) <= building_value)
                )
                {
                    Deactivate_Help_List();
                    Set_Mouse_List(1, mouse_list_default);
                    strcpy(GUI_String_1, aDoYouWishToSpend);  // "Do you wish to spend \x02"
                    itoa(City_Cost_To_Buy_Product(_city_idx), GUI_String_2, 10);
                    strcat(GUI_String_1, GUI_String_2);
                    strcat(GUI_String_1, aGold_3);  // " Gold"
                    strcat(GUI_String_1, aByPurchasing);  // "\x01 by purchasing"
                    strcat(GUI_String_1, STR_GetIndefinite(city_screen_product_name));
                    strcat(GUI_String_1, cnst_Space);  // " "
                    strcat(GUI_String_1, city_screen_product_name);
                    strcat(GUI_String_1, cnst_QuestionMark);  // "?"
                    if(Confirmation_Box(GUI_String_1) == ST_TRUE)
                    {
                        Player_City_Buy_Production(_human_player_idx, _city_idx);
                    }
                    Deactivate_Auto_Function();
                    Assign_Auto_Function(City_Screen_Draw__WIP, 1);
                    Do_City_Calculations(_city_idx);
                    m_city_production_cost = City_Production_Cost(_CITIES[_city_idx].construction, _city_idx);
                    m_city_n_turns_to_produce = City_N_Turns_To_Produce(m_city_production_cost, _city_idx);
                    City_Can_Buy_Product();
                    // ¿ BUG ? jmp     short $+2
                }
                // @@WrapUp_BuyButton:
                Deactivate_Auto_Function();
                Assign_Auto_Function(City_Screen_Draw__WIP, 1);
                screen_changed = ST_TRUE;
                Reset_Map_Draw();
                Deactivate_Help_List();
                Set_City_Screen_Help_List();
            }
        }
        /*
            END:  Left-Click Buy Button
        */


        /*
            BEGIN:  Left-Click City Enchantment
        */
        /*
            Confirmation_Box()
            "Do you wish to turn off the " ... " spell?"
        */
        /*
            END:  Left-Click City Enchantment
        */


        /*
            BEGIN:  Left-Click Cityscape Building
        */
        /*
            lotsa logic
                ...only one per turn
                ...will cease production
                ...do you wish to sell
            City_Can_Sell_Building()
        */
        /*
            END:  Left-Click Cityscape Building
        */


        /*
            BEGIN:  Left-Click Unit Window
        */
        {
            // IDK_Col__unit_stack_idx
            for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
            {
                if(g_unit_window_fields[itr_stack] == input_field_idx)
                {
                    // TODO  SND_LeftClickSound();
                    _prev_world_x = _map_x;
                    _prev_world_y = _map_y;
                    leave_screen = ST_TRUE;
                    current_screen = scr_Main_Screen;
                    if(all_units_moved == ST_TRUE)
                    {
                        all_units_moved = ST_FALSE;
                    }
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
                    City_Screen_Garrison_Window_Picture_Coords(itr_stack, &uv_x1, &uv_y1, &uv_x2, &uv_y2);
                    USW_FullDisplay(_unit_stack[itr_stack].unit_idx, (uv_x1 + 1), (uv_y1 + 1), (uv_x2 + 1), (uv_y2 + 1));
                    Assign_Auto_Function(City_Screen_Draw__WIP, 1);
                    City_Screen_Load();
                    City_Screen_Required_Buildings_List(_city_idx);
                    if(_CITIES[_city_idx].construction < 100)
                    {
                        strcpy(city_screen_product_name, bldg_data_table[_CITIES[_city_idx].construction].name);
                    }
                    else
                    {
                        unit_type = (_CITIES[_city_idx].construction - 100);
                        strcpy(city_screen_product_name, *_unit_type_table[unit_type].name);
                    }
                    screen_changed = ST_TRUE;
                    City_Screen_Allocate_First_Block();
                    city_wx = _CITIES[_city_idx].wx;
                    city_wy = _CITIES[_city_idx].wy;
                    Select_Unit_Stack(_human_player_idx, &city_map_wx, &city_map_wy, _map_plane, city_wx, city_wy);
                    Do_Build_City_Enchantment_List();
                    Reset_Map_Draw();
                    Deactivate_Help_List();
                    Set_City_Screen_Help_List();
                }
            }
        }
        /*
            END:  Right-Click Unit Window
        */


        /*
            BEGIN:  Left-Click Population Row
        */
        /*
            END:  Left-Click Population Row
        */



        if( (leave_screen == ST_FALSE) && (screen_changed == ST_FALSE) )
        {
            City_Screen_Draw__WIP();
            // DONT  j_o146p05_Empty_pFxn
            PageFlip_FX();
            Release_Time(1);
        }

        screen_changed = ST_FALSE;
    }


    // @@Leave_Screen
    cityscreen_city_built_bldg_idx = ST_UNDEFINED;
    Deactivate_Auto_Function();
    Deactivate_Help_List();
    Clear_Fields();
    Reset_Window();
    Reset_Draw_Active_Stack();

}


// WZD o54p02
void City_Screen_Draw__WIP(void)
{
    int16_t itr;  // _SI_



    /*
        BEGIN:  Help Entries
    */

    /*
        END:  Help Entries
    */



    Reset_Window();
    Set_Page_Off();
    Reset_Map_Draw();
    Reduced_Map_Set_Dims(58, 30);
    Draw_Maps(215, 4, 5, 5, &city_map_wx, &city_map_wy, _map_plane, _prev_world_x, _prev_world_y, _human_player_idx);
    City_Screen_Draw_Map();
    Reset_Window();

    FLIC_Draw(0, 0, city_background_seg);

    City_Screen_Draw2__WIP();

    City_Screen_Draw_Buttons();

    Reset_Window();

    if(cityscreen_city_built_bldg_idx != ST_UNDEFINED)
    {
        /* jmp  short $+2 */
    }

    Cityscape_Draw__WIP(_city_idx, 4, 101, cityscreen_city_built_bldg_idx, cityscreen_city_built_bldg_idx);

    Cityscape_Draw_Scanned_Building_Name(city_screen_scanned_field, 1, _CITIES[_city_idx].owner_idx);

    if(
        (_CITIES[_city_idx].construction == bt_TradeGoods)
        ||
        (_CITIES[_city_idx].construction == bt_Housing)
    )
    {
        FLIC_Draw(260, 149, city_block_out_seg);
        Set_Font_Style_Shadow_Up(0, 0, 0, 0);
        Set_Outline_Color(4);
        Set_Alias_Color(26);
        Set_Font_LF(0);
        if(_CITIES[_city_idx].construction != 0x01)  /* _Trade_Goods */
        {
            Print_Paragraph(262, 159, 50, proddescr_tradegoods, 2);
        }
        else
        {
            Print_Paragraph(262, 159, 50, proddescr_housing, 2);
        }
    }
    else
    {
        City_Screen_Draw_Production_Coins(_city_idx);
    }

}


// WZD o54p03
void City_Screen_Draw2__WIP(void)
{
    uint8_t colors[6];
    int16_t y;
    int16_t x;
    int16_t bitm_h;
    int16_t bitm_w;
    int16_t bitm_y;
    int16_t bitm_x;
    SAMB_ptr bitmap;
    int16_t product_idx;
    int16_t pops;  // _SI_
    int16_t unit_type;  // _DI_
    int16_t para_width;  // _DI_
    int16_t para_height;  // _DI_
    int16_t print_y;  // _DI_

    strcpy(GUI_String_1, _city_size_names[_CITIES[_city_idx].size]);
    strcat(GUI_String_1, " of ");
    strcpy(GUI_String_2, _CITIES[_city_idx].name);
    strcat(GUI_String_1, GUI_String_2);

    Set_Font_Style_Shadow_Down(5, 5, 0, 0);
    Set_Alias_Color(26);
    Set_Outline_Color(0);

    Print_Centered(105, 3, GUI_String_1);

    colors[0] = 190;
    colors[1] = 179;
    Set_Font_Colors_15(0, &colors[0]);
    Set_Outline_Color(19);
    Set_Font_Style_Shadow_Down(1, 15, 0, 0);

    strcpy(GUI_String_1, *_race_type_table[_CITIES[_city_idx].race].name);

    /*
        Plague
    */
    if((events_table[12] == 2) && (events_table[14] == _city_idx))
    {
        // TODO  [_help_entries.help_idx+8Ch], HLP_PLAGUE

        strcat(GUI_String_1, " (Plague)");
    }

    /*
        Population Boom
    */
    if((events_table[22] == 2) && (events_table[23] == _city_idx))
    {
        // TODO  [_help_entries.help_idx+8Ch], HLP_POPULATION_BOOM

        strcat(GUI_String_1, " (Pop. Boom)");
    }

    Print(6, 19, GUI_String_1);

    pops = (_CITIES[_city_idx].population * 1000) + (_CITIES[_city_idx].Pop_10s * 10);

    strcpy(GUI_String_1, "Population: ");

    if(pops > 999)
    {
        itoa((pops / 1000), GUI_String_2, 10);
        strcat(GUI_String_1, GUI_String_2);
        strcat(GUI_String_1, ",");
        pops = (pops - ((pops / 1000) * 1000));
        itoa(pops, GUI_String_2, 10);
        if(pops < 10)
        {
            strcat(GUI_String_1, "00");
        }
        else
        {
            if(pops < 100)
            {
                strcat(GUI_String_1, "0");
            }
        }
    }
    else
    {
        itoa(pops, GUI_String_2, 10);
    }

    strcat(GUI_String_1, GUI_String_2);

    strcat(GUI_String_1, " (");

    // TODO  EMM_Map_DataH()

    pops = (City_Growth_Rate(_city_idx) * 10);

    itoa(pops, GUI_String_2, 10);

    if(pops > -1)
    {
        strcat(GUI_String_1, "+");
    }

    strcat(GUI_String_1, GUI_String_2);

    strcat(GUI_String_1, ")");

    Set_Font_Spacing_Width(1);

    Print_Right(208, 19, GUI_String_1);

    City_Screen_Draw_Population_Row(_city_idx, 4, 27);

    City_Screen_Draw_Resource_Icons(_city_idx, 6, 52);

    City_Screen_Draw_City_Enchantments(139, 51);

    City_Screen_Draw_Garrison_Window();

    colors[0] = 190;
    colors[1] = 179;

    Set_Font_Colors_15(0, &colors[0]);
    Set_Outline_Color(19);
    Set_Font_Style_Shadow_Down(1, 15, 0, 0);

    if(
        (m_city_n_turns_to_produce > 0) &&
        (_CITIES[_city_idx].construction != 2) &&  /* _Housing */
        (_CITIES[_city_idx].construction != 1)  /* _Trade_Goods */
    )
    {
        Print_Integer_Right(286, 140, m_city_n_turns_to_produce);

        if(m_city_n_turns_to_produce > 1)
        {
            strcpy(GUI_String_1, "Turns");
        }
        else
        {
            strcpy(GUI_String_1, "Turn");
        }

        Print(291, 140, GUI_String_1);
    }

    product_idx = _CITIES[_city_idx].construction;

    if(product_idx >= 100)
    {
        unit_type = (product_idx - 100);

        if(_unit_type_table[unit_type].Transport > 0)
        {
            FLIC_Draw(219, 157, unit_water_diamond_seg);
        }
        else
        {
            FLIC_Set_CurrentFrame(unit_grass_diamond_seg, 0);
            FLIC_Draw(218, 144, unit_grass_diamond_seg);
        }

        Draw_Unit_Figure(224, 136, unit_type, 0);

    }
    else
    {
        x = 215;
        y = 147;

        Mark_Block(_screen_seg);

        bitmap = Allocate_Next_Block(_screen_seg, 500);  // ¿ sizeof(header) + 22 x 22 ?

        Release_Block(_screen_seg);

        Draw_Building_Picture_To_Bitmap(_city_idx, product_idx, &bitm_x, &bitm_y, &bitm_w, &bitm_h, bitmap);

        // bldg_pict_draw_x -= (((44 - bldg_pict_w) / 2) - bldg_pict_x1);
        // bldg_pict_draw_y += (((36 - bldg_pict_h) / 2) - bldg_pict_y1);
        // 44 is width of building picture space
        // (44 - bitm_w) is how much of the space the picture will take up
        // ... / 2) is half of the balance, so the picture will be centered horizontally
        // ¿ ... - bitm_x) ? 
        x += (((44 - bitm_w) / 2) - bitm_x);
        y += (((31 - bitm_h) / 2) - bitm_y );

        FLIC_Set_LoopFrame_1(bitmap);

        Draw_Picture(x, y, bitmap);

    }

    Set_Font_Style_Shadow_Up(1, 0, 0, 0);
    Set_Font_LF(0);
    Set_Alias_Color(26);
    Set_Font_Spacing_Width(1);

    para_width = Get_Paragraph_Max_Width(44, city_screen_product_name, 0);

    if((para_width > 42) || (para_width <= 0))
    {
        Set_Font_Style_Shadow_Up(0, 0, 0, 0);
        Set_Font_LF(0);
        Set_Font_Spacing_Width(1);
    }

    if(Get_Paragraph_Max_Height(44, city_screen_product_name) > 6)
    {
        Set_Font_Style_Shadow_Up(0, 0, 0, 0);
        Set_Font_LF(0);
        Set_Font_Spacing_Width(1);
    }

    Set_Alias_Color(26);
    Set_Outline_Color(4);

    para_height = Get_Paragraph_Max_Height(44, city_screen_product_name);

    if(para_height > 0)
    {
        print_y = (179 - (para_height - 6));
        Print_Paragraph(216, print_y, 44, city_screen_product_name, 2);
    }
    else
    {
        Print(216, 179, city_screen_product_name);
    }

    Set_Outline_Color(0);

}


// WZD o54p04
// MoO2  Module: COLONY  Add_Screen_Fields_()
void City_Screen_Add_Fields__WIP(void)
{
    int16_t y2;
    int16_t x2;
    int16_t y1;
    int16_t x1;
    int16_t itr;  // _SI_
    int16_t itr_stack;  // _SI_

    Clear_Fields();

    city_cityscape_field_count = 0;
    _main_map_grid_field = INVALID_FIELD;
    _minimap_grid_field = INVALID_FIELD;
    UU_CityScreen_Field_Something = 0;

    for(itr = 0; itr < MAX_STACK; itr++)
    {
        g_unit_window_fields[itr] = -100;
    }

    city_screen_buy_button = INVALID_FIELD;
    city_sceen_change_button = INVALID_FIELD;
    city_screen_ok_button = INVALID_FIELD;
    UU_CityScreen_Field = INVALID_FIELD;

    // ~== Main Screen Add_Unit_Window_Fields()
    for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
    {
        City_Screen_Garrison_Window_Picture_Coords(itr_stack, &x1, &y1, &x2, &y2);
        g_unit_window_fields[itr_stack] = Add_Hidden_Field((x1 - 1), (y1 - 1), (x2 - 2), (y2 - 2), emptystring__ovr054[0], ST_UNDEFINED);
    }

    City_Add_Fields_City_Enchantments(139, 51);

    City_Add_Fields_Buildings();

    City_Screen_Add_Fields_Production_Window();

    City_Screen_Add_Fields_Population_Row(_city_idx, 4, 27);

    City_Screen_Add_Fields_Resource_Window(_city_idx, 6, 52);

}


// WZD o54p05
// MoO2  Module: COLONY  Add_Screen_Fields_() |-> Add_Bldg_Fields_()
// 000BE486 cmp     _colony_bldgs[eax], 0
// 000BE4EF mov     _colony_bldg_fields[ebx], -1000
// MoO2  Module: COLONY  N_Bldgs_()
void City_Add_Fields_Buildings(void)
{
    int16_t y2;
    int16_t x2;
    int16_t y1;
    int16_t x1;
    int16_t itr;  // _SI_

    city_cityscape_field_count = 0;

    for(itr = 0; itr < cityscape_bldg_count; itr++)
    {
        if(cityscape_bldgs[itr].bldg_idx == 104)
        {
            __debugbreak;
        }

        x1 = cityscape_bldgs[itr].x1;
        y1 = cityscape_bldgs[itr].y1;
        x2 = cityscape_bldgs[itr].x2;
        y2 = cityscape_bldgs[itr].y2;

        city_cityscape_fields[city_cityscape_field_count] = Add_Hidden_Field(x1, y1, x2, y2, emptystring__ovr054[0], ST_UNDEFINED);

        city_cityscape_field_count++;
    }

}


// WZD o54p06
void City_Screen_Add_Fields_Production_Window(void)
{
    city_screen_buy_button = INVALID_FIELD;

    if(m_city_no_buy == ST_FALSE)
    {
        // city_screen_buy_button = Add_Button_Field(214, 188, cnst_ZeroString_25, IMG_CTY_BUY_Btn, cnst_HOTKEY_B, 0xFFFF);
        city_screen_buy_button = Add_Button_Field(214, 188, "", city_buy_button_seg, 'B', ST_UNDEFINED);
    }

    // city_sceen_change_button = Add_Button_Field(214, 188, cnst_ZeroString_25, IMG_CTY_BUY_Btn, cnst_HOTKEY_C, 0xFFFF);
    city_sceen_change_button = Add_Button_Field(247, 188, "", city_change_button_seg, 'C', ST_UNDEFINED);

    // city_screen_ok_button = Add_Button_Field(214, 188, cnst_ZeroString_25, IMG_CTY_BUY_Btn, cnst_HOTKEY_O, 0xFFFF);
    city_screen_ok_button = Add_Button_Field(286, 188, "", city_ok_button_seg, 'O', ST_UNDEFINED);

    // city_screen_esc_hotkey = Add_Hot_Key(cnst_HOTKEY_Esc);
    city_screen_esc_hotkey = Add_Hot_Key('\x1B');

}


// WZD o54p07
void City_Screen_Draw_Buttons(void)
{

    if(m_city_no_buy != ST_FALSE)
    {
        FLIC_Draw(214, 188, city_lock_buy_button_seg);
    }
    else
    {
        FLIC_Set_CurrentFrame(city_buy_button_seg, 0);
        FLIC_Draw(214, 188, city_buy_button_seg);
    }

    FLIC_Set_CurrentFrame(city_change_button_seg, 0);
    FLIC_Draw(247, 188, city_change_button_seg);


    FLIC_Set_CurrentFrame(city_ok_button_seg, 0);
    FLIC_Draw(286, 188, city_ok_button_seg);

    if(city_enchantment_display_scroll_flag == ST_TRUE)
    {

        FLIC_Set_CurrentFrame(city_spell_up_arrow_button_seg, 0);
        FLIC_Draw(201, 50, city_spell_up_arrow_button_seg);

        FLIC_Set_CurrentFrame(city_spell_dn_arrow_button_seg, 0);
        FLIC_Draw(201, 85, city_spell_dn_arrow_button_seg);
    }

}

// WZD o54p08
/*
    ignored params: x, y, city_idx
    bldg_bitm_seg is only used to get the x,y,w,h of the bldg pict

*/
void City_Built_Building_Message(int16_t x, int16_t y, int16_t city_idx, int16_t bldg_idx)
{
    SAMB_ptr bldg_bitm_seg;
    int16_t tmp_strlen;
    int16_t Sound_Data_Seg;
    int16_t width;
    int16_t height;
    int16_t ystart;
    int16_t xstart;

    int16_t bitm_x;  // _SI_
    int16_t bitm_y;  // _DI_

    // TODO  SND_Silence();
    Allocate_Reduced_Map();
    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);


    // TODO  Full_Draw_Main_Screen();


    Copy_On_To_Off_Page();
    GUI_String_1 = (char *)Near_Allocate_First(100);
    GUI_String_2 = (char *)Near_Allocate_Next(100);
    bldg_bitm_seg = Allocate_First_Block(_screen_seg, 500);
    strcpy(GUI_String_1, "The ");
    strcat(GUI_String_1, _city_size_names[_CITIES[_city_idx].size]);
    strcat(GUI_String_1, " of ");
    strcpy(GUI_String_2, _CITIES[_city_idx].name);
    strcat(GUI_String_1, GUI_String_2);
    strcat(GUI_String_1, " has completed the construction of ");
    strcpy(GUI_String_2, bldg_data_table[city_built_bldg_idx].name);
    strcat(GUI_String_1, STR_GetIndefinite(&GUI_String_2[0]));
    strcat(GUI_String_1, " ");
    if(city_built_bldg_idx == bt_CityWalls)
    {
        tmp_strlen = strlen(GUI_String_2);
        GUI_String_2[(tmp_strlen - 1)] = 0;
    }
    strcat(GUI_String_1, GUI_String_2);
    strcat(GUI_String_1, ".");

    if(magic_set.Event_Music == ST_TRUE)
    {
        // TODO  Sound_Data_Seg = LBX_Reload(music_lbx_file, MUSIC_Bldng_Finished, SND_Music_Segment);
        // TODO  SND_PlayFile(Sound_Data_Seg);
    }

    bitm_x = 0;
    bitm_y = 8;
    if(city_built_bldg_idx == bt_CityWalls)
    {
        Draw_Picture_To_Bitmap(cityscape_big_city_wall_seg, bldg_bitm_seg);
    }
    else
    {
        Draw_Picture_To_Bitmap(bldg_picts_seg[bldg_idx], bldg_bitm_seg);
    }
    Get_Bitmap_Actual_Size(bldg_bitm_seg, &xstart, &ystart, &width, &height);  // 0, 7, 23, 25
    bitm_x += (((41 -  width) / 2) - xstart);  // (0 + (((41 - 23) / 2) - 0)) = (0 + ((18 / 2) - 0)( = (0 + (9 - 0)) = (0 + 9) =  9
    bitm_y += (((43 - height) / 2) - ystart);  // (8 + (((43 - 25) / 2) - 7)) = (8 + ((18 / 2) - 7)) = (8 + (9 - 7)) = (8 + 2) = 10
    
    Copy_On_To_Off_Page();

    if(city_built_bldg_idx == bt_CityWalls)
    {
        Notify2(160, 60, tb_Green, GUI_String_1, 0, city_new_build_notify_grass_seg, 0, 8, cityscape_big_city_wall_seg, bitm_x, bitm_y, 0, 0);
    }
    else
    {
        Notify2(
            160,                              /* UU x ¿ (320 / 2) ?        */
            60,                               /* UU y ¿ ((200 - 20) / 3) ? */
            tb_Green,                         /* notify color/type   */
            GUI_String_1,                     /* notify message text */
            0,                                /* pict border {pict box, gem box} */
            city_new_build_notify_grass_seg,  /* pict1   */
            0,                                /* pict1 x */
            8,                                /* pict1 y */
            bldg_picts_seg[bldg_idx],         /* pict2   */
            bitm_x,                           /* pict2 x */
            bitm_y,                           /* pict2 y */
            0,                                /* CSlide {none, mod 8, mod 11} */
            0                                 /* UU BGShade <NO> */
        );
    }

    // TODO  SND_PlayBkgrndTrack();

    IDK_Clear_Cityscape_Vanish_Percent();

    cityscreen_city_built_bldg_idx = bldg_idx;
}


// WZD o54p09
// NameStartingCity_Dialog_Popup

// WZD o54p10
// Draw_NameStartingCity_Background


// WZD o54p11
/*
    loops 1 to 36
    default: ST_TRUE
    stays ST_TRUE is bldg status is already NotBuilt or Replaced
    stays ST_TRUE if Prereq() == ST_TRUE
    only ST_FALSE if (Build || Removed) && Prereq() == ST_FALSE
if this is a building is required list, ...
    what would it mean to only test for Built or Removed?
        makes no sense to test for a requirement on a building that been removed
        if a building had been replaced, it is testing for the requirement on the replacement building
            so, buildings that have been replaced are inplicitly required
EoD,
    if NotBuilt, then ST_TRUE
    if Replaced, then ST_TRUE
    if Built   and No Requirement, then ST_TRUE
    if Removed and No Requirement, then ST_TRUE
PoI?
    if ST_FALSE, then it's Built, not Replaced or Removed, and Has Requirement
        so,
            it will be shown in the Cityscape,
        and,
            you can't sell it
*/
void City_Screen_Required_Buildings_List(int16_t city_idx)
{
    int16_t reqd_bldg_idx;
    int16_t itr;  // _SI_

    for(itr = 1; itr < NUM_BUILDINGS; itr++)
    {
        city_screen_required_buildings_list[itr] = ST_TRUE;

        if(_CITIES[city_idx].bldg_status[itr] >= 1)  // ¿ testing for Built and Removed, but not Replaced ?
        {
            if(City_Building_Has_Requirement(itr, city_idx, &reqd_bldg_idx) == ST_FALSE)
            {
                city_screen_required_buildings_list[itr] = ST_FALSE;
            }
        }
    }
}


// WZD o54p12
void City_Screen_Draw_Garrison_Window(void)
{
    int16_t itr_stack;
    int16_t unit_idx;
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;

    if(_unit != ST_UNDEFINED)
    {
        for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
        {
            unit_idx = _unit_stack[itr_stack].unit_idx;
            City_Screen_Garrison_Window_Picture_Coords(itr_stack, &x1, &y1, &x2, &y2);
            Draw_Unit_Picture(x1, y1, unit_idx, ST_TRUE);
        }
        Cycle_Unit_Enchantment_Animation();
    }
}


// WZD o54p13
void City_Screen_Load(void)
{
    int16_t IDK_size_city_cityscape_fields;

    GUI_String_1 = (char *)Near_Allocate_First(100);
    GUI_String_2 = (char *)Near_Allocate_Next(100);

    city_screen_product_name = (char *)Near_Allocate_Next(40);

    city_screen_required_buildings_list = (int16_t *)Near_Allocate_Next(72);  // 36 int16_t  Buildings

    IDK_size_city_cityscape_fields = 72;  // ¿ 36 int16_t  Buildings ?
    city_cityscape_fields = Near_Allocate_Next(IDK_size_city_cityscape_fields * 2);

    city_enchantment_fields  = (int16_t *)Near_Allocate_Next(52);  // 26 int16_t  City Enchantment Window Fields

    city_enchantment_list    = (int16_t *)Near_Allocate_Next(52);  // 26 int16_t  City Enchantments
    city_enchantment_owner_list  = (int16_t *)Near_Allocate_Next(52);  // 26 int16_t  City Enchantments

    city_population_row_fields = (int16_t *)Near_Allocate_Next(40);  // 20 int16_t  Population Row Field Indices

}


// WZD o54p14
// MoO2  Module: MAINSCR  Allocate_First_Block_()
/*
    allocates the reduced_map_seg, resetting the screen_seg
    if the city's current contruction project is a Unit,
      loads the unit figure picture, into the UV/USW unit_figure_seg
*/
void City_Screen_Allocate_First_Block(void)
{
    int16_t unit_type;

    _reduced_map_seg = Allocate_First_Block(_screen_seg, 303);  // 303 PR  4848 B

    if(_CITIES[_city_idx].construction >= 100)
    {
        unit_type = (_CITIES[_city_idx].construction - 100);
        Load_Unit_Figure(unit_type, 1);
    }

}


// WZD o54p15
// MoO2  Module: HAROLD   Get_Picture_Actual_Size_()
// MoO2  Module: MAINSCR  Get_Star_Anim_Dimension_()
// MoO2  Module: CMBTDRW1 Get_Ship_Anim_Dimensions_()
void Draw_Building_Picture_To_Bitmap(int16_t city_idx, int16_t bldg_idx, int16_t * x1, int16_t * y1, int16_t * width, int16_t * height, SAMB_ptr bitmap)
{
    int16_t itr_frames;
    int16_t l_width;
    int16_t l_height;
    int16_t l_y1;
    int16_t l_x1;
    int16_t max_height;
    int16_t max_width;
    int16_t min_y1;
    int16_t min_x1;
    int16_t frame_count;
    int16_t current_frame;

    if(bldg_idx == bt_TradeGoods)
    {
        current_frame = FLIC_Get_CurrentFrame(building_trade_goods_seg);
        frame_count = FLIC_Get_FrameCount(building_trade_goods_seg);
    }
    else if(bldg_idx == bt_Housing)
    {
        current_frame = FLIC_Get_CurrentFrame(bldg_housing_seg[_race_type_table[_CITIES[city_idx].race].house_type]);
        frame_count = FLIC_Get_FrameCount(bldg_housing_seg[_race_type_table[_CITIES[city_idx].race].house_type]);
    }
    else if(bldg_idx == bt_CityWalls)
    {
        current_frame = FLIC_Get_CurrentFrame(cityscape_big_city_wall_seg);
        frame_count = FLIC_Get_FrameCount(cityscape_big_city_wall_seg);
    }
    else
    {
        // Building Type / Product Idx:  {3, ..., 34}
        current_frame = FLIC_Get_CurrentFrame(bldg_picts_seg[bldg_idx]);
        frame_count = FLIC_Get_FrameCount(bldg_picts_seg[bldg_idx]);
    }

    min_x1 = 0;
    min_y1 = 0;
    max_height = 0;
    max_width = 0;

    for(itr_frames = 0; itr_frames < frame_count; itr_frames++)
    {

        if(bldg_idx == bt_TradeGoods)
        {
            FLIC_Set_CurrentFrame(building_trade_goods_seg, itr_frames);
            Draw_Picture_To_Bitmap(building_trade_goods_seg, bitmap);
            Get_Bitmap_Actual_Size(bitmap, &l_x1, &l_y1, &l_width, &l_height);
        }
        else if(bldg_idx == bt_Housing)
        {
            FLIC_Set_CurrentFrame(bldg_housing_seg[_race_type_table[_CITIES[city_idx].race].house_type], itr_frames);
            Draw_Picture_To_Bitmap(bldg_housing_seg[_race_type_table[_CITIES[city_idx].race].house_type], bitmap);
            Get_Bitmap_Actual_Size(bitmap, &l_x1, &l_y1, &l_width, &l_height);
        }
        else if(bldg_idx == bt_CityWalls)
        {
            FLIC_Set_CurrentFrame(cityscape_big_city_wall_seg, itr_frames);
            Draw_Picture_To_Bitmap(cityscape_big_city_wall_seg, bitmap);
            Get_Bitmap_Actual_Size(bitmap, &l_x1, &l_y1, &l_width, &l_height);
        }
        else
        {
            // Building Type / Product Idx:  {3, ..., 34}
            FLIC_Set_CurrentFrame(bldg_picts_seg[bldg_idx], itr_frames);
            Draw_Picture_To_Bitmap(bldg_picts_seg[bldg_idx], bitmap);
            Get_Bitmap_Actual_Size(bitmap, &l_x1, &l_y1, &l_width, &l_height);
        }

        if((l_width > max_width) || (l_height > max_width))
        {
            min_x1 = l_x1;
            min_y1 = l_y1;
            max_width = l_width;
            max_height = l_height;
        }

    }

    *x1 = min_x1;
    *y1 = min_y1;
    *height = max_height;
    *width = max_width;

    if(bldg_idx == bt_TradeGoods)
    {
        FLIC_Set_CurrentFrame(building_trade_goods_seg, current_frame);
        Draw_Picture_To_Bitmap(building_trade_goods_seg, bitmap);
    }
    else if(bldg_idx == bt_Housing)
    {
        FLIC_Set_CurrentFrame(bldg_housing_seg[_race_type_table[_CITIES[city_idx].race].house_type], current_frame);
        Draw_Picture_To_Bitmap(bldg_housing_seg[_race_type_table[_CITIES[city_idx].race].house_type], bitmap);
    }
    else if(bldg_idx == bt_CityWalls)
    {
        FLIC_Set_CurrentFrame(cityscape_big_city_wall_seg, current_frame);
        Draw_Picture_To_Bitmap(cityscape_big_city_wall_seg, bitmap);
    }
    else
    {
        // Building Type / Product Idx:  {3, ..., 34}
        FLIC_Set_CurrentFrame(bldg_picts_seg[bldg_idx], current_frame);
        Draw_Picture_To_Bitmap(bldg_picts_seg[bldg_idx], bitmap);
    }

}


// WZD o54p16
// MoO2  Module: COLONY  Colony_Can_Buy_Product_0_()  |-> Module: COLCALC  Colony_Can_Afford_To_Buy_Product_()
int16_t City_Can_Buy_Product(void)
{
    m_city_no_buy = ST_FALSE;

    if(m_city_n_turns_to_produce < 2)
    {
        m_city_no_buy = ST_TRUE;
    }

    if( (_CITIES[_city_idx].construction == bt_Housing) || (_CITIES[_city_idx].construction == bt_TradeGoods) )
    {
        m_city_no_buy = ST_TRUE;
    }
    else
    {
        if(City_Cost_To_Buy_Product(_city_idx) > _players[_human_player_idx].gold_reserve)
        {
            m_city_no_buy = ST_TRUE;
        }
    }

    if(m_city_production_cost == 0)
    {
        m_city_no_buy = ST_TRUE;
    }

    return m_city_no_buy;
}


// WZD o54p17
void City_Screen_Draw_Map(void)
{
    int16_t corner_square;
    int16_t mp;
    int16_t my;
    int16_t mx;
    int16_t ystart;
    int16_t xstart;
    int16_t itr_y;  // _DI_
    int16_t itr_x;  // _SI_

    mp = _map_plane;
    my = city_map_wy;
    ystart = 4;

    for(itr_y = 0; itr_y < 5; itr_y++)
    {
        mx = city_map_wx;
        xstart = 215;

        for(itr_x = 0; itr_x < 5; itr_x++)
        {
            corner_square = ST_FALSE;

            // on a corner {{0,0},{0,4},{4,0},{4,4}}
            if(((itr_y == 0) || (itr_y == 4)) && ((itr_x == 0) || (itr_x == 4)))
            {
                Gradient_Fill(xstart, ystart, (xstart + SQUARE_WIDTH - 1), (ystart + SQUARE_HEIGHT - 1), 15, 7, ST_NULL, ST_NULL, ST_NULL);
                corner_square = ST_TRUE;
            }

            if(corner_square == ST_FALSE)
            {
                // TODO  EMM_Map_DataH();

                if(City_Area_Square_Is_Shared(mx, my, mp) == ST_TRUE)
                {
                    FLIC_Draw((xstart + 7), (ystart + 4), city_half_background_seg);
                }
            }

            xstart += SQUARE_WIDTH;
            mx++;

            if(mx >= WORLD_WIDTH)
            {
                mx = 0;
            }

        }

        ystart += SQUARE_HEIGHT;
        my++;

    }

    Redraw_Map_Unexplored_Area(215, 4, 5, 5, city_map_wx, city_map_wy, _map_plane);

    Draw_Map_Units(215, 4, 5, 5, city_map_wx, city_map_wy);

}
