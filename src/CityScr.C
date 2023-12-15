
#include "MoM.H"
#include "CityScr.H"
#include "City_ovr55.H"
#include "CITYSCAP.H"
#include "MainScr.H"
#include "MainScr_Maps.H"
#include "ProdScr.H"
#include "UnitStat.H"



// WZD dseg:92B0
// drake178: G_TBL_CityscapeBldngs@


// WZD dseg:92B4
int16_t CTY_Entity_Count;



// WZD dseg:9936
// drake178: IMG_CTY_Screen_BG
SAMB_ptr city_background_seg;                  // BACKGRND.LBX, 6

// WZD dseg:9938
// drake178: IMG_CTY_Block_Out
SAMB_ptr city_block_out_seg;                  // BACKGRND.LBX, 

// WZD dseg:993A
// drake178: IMG_CTY_ProdDone
SAMB_ptr city_full_resource_seg;                  // BACKGRND.LBX, 

// WZD dseg:993C
// drake178: IMG_CTY_ProdLeft
SAMB_ptr city_reqd_resource_seg;                  // BACKGRND.LBX, 

// WZD dseg:993E
// drake178: IMG_CTY_BUY_Lock
SAMB_ptr city_lock_buy_button_seg;                  // BACKGRND.LBX, 

// WZD dseg:9940
// drake178: IMG_CTY_OK_Btn
SAMB_ptr city_ok_button_seg;                  // BACKGRND.LBX, 

// WZD dseg:9942
// drake178: IMG_CTY_BUY_Btn
SAMB_ptr city_buy_button_seg;                  // BACKGRND.LBX, 

// WZD dseg:9944
// drake178: IMG_CTY_CHNG_Btn
SAMB_ptr city_change_button_seg;                  // BACKGRND.LBX, 



void City_Screen__WIP(void)
{
    int16_t IDK_active_stack_flag;
    int16_t city_rebel_count;
    int16_t required_farmer_count;
// var_1A= word ptr -1Ah
// var_18= word ptr -18h
// var_16= word ptr -16h
// var_14= word ptr -14h
// var_12= word ptr -12h
// value= word ptr -10h
    int16_t hotkey_X;
    int16_t city_wy;
    int16_t city_wx;
    int16_t Row;
    int16_t Col;
    int16_t screen_changed;
    int16_t leave_screen_flag;
    int16_t input_field_idx;  // _DI_



    // Fill(0, 0, 319, 199, 0);
    // Set_Page_On();
    // Fill(0, 0, 319, 199, 0);
    // Set_Page_Off();



    Set_City_Screen_Help_List();

    if( (PageFlipEffect != 0) && (PageFlipEffect != 3) )
    {
        PageFlipEffect = 0;
    }


    // TODO  OVL_StackHasPath = ST_FALSE;


    // TODO  Deactivate_Auto_Function();
    // TODO  Assign_Auto_Function(City_Screen_Draw(), 1);


    City_Screen_Load();


    city_map_x = 0;
    city_map_y = 0;

    City_Center_Map(&city_map_x, &city_map_y, _CITIES[_city_idx].wx, _CITIES[_city_idx].wy, _map_plane);


    _garrison_window_start_x = 215;
    _garrison_window_start_y = 102;


    IDK_active_stack_flag = ST_FALSE;

    if(_unit_stack_count == 0)
    {
        IDK_active_stack_flag = ST_TRUE;
    }


    city_wx = _CITIES[_city_idx].wx;
    city_wy = _CITIES[_city_idx].wy;

    Select_Unit_Stack(_human_player_idx, &city_map_x, &city_map_y, _map_plane, city_wx, city_wy);

    Set_Draw_Active_Stack_Always();

    Set_Entities_On_Map_Window(city_map_x, city_map_y, _map_plane);

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

    if(_CITIES[_city_idx].construction == 0)  /* NOTHING */
    {
        _CITIES[_city_idx].construction = 2;  /* Housing */
    }

    Do_City_Calculations(_city_idx);

    IDK_city_production_cost = City_Production_Cost(_CITIES[_city_idx].construction, _city_idx);

    IDK_city_n_turns_to_produce = City_N_Turns_To_Produce(IDK_city_production_cost, _city_idx);

    if(_CITIES[_city_idx].construction < 100)
    {
        // TODO  String_Copy_Far(IDK_production_title, bldg_data_table[_CITIES[_city_idx].construction]);
        strcpy(city_screen_product_name, bldg_data_table[_CITIES[_city_idx].construction].name);
    }
    else
    {
        Row = (_CITIES[_city_idx].construction - 100);
        strcpy(city_screen_product_name, *_unit_type_table[Row].Name);
    }

    CTY_Entity_Count = 0;

    City_Can_Buy_Product();

    city_screen_scanned_field = ST_UNDEFINED;

    Allocate_Reduced_Map_Load_Unit_Figure();

    Do_City_Calculations(_city_idx);

    IDK_city_production_cost = City_Production_Cost(_CITIES[_city_idx].construction, _city_idx);

    IDK_city_n_turns_to_produce = City_N_Turns_To_Produce(IDK_city_production_cost, _city_idx);

    Reset_Cycle_Palette_Color();

    Set_Input_Delay(2);

    screen_changed = ST_FALSE;
    leave_screen_flag = ST_FALSE;
    city_screen_scanned_field = ST_UNDEFINED;

    while(leave_screen_flag == ST_FALSE)
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
            Left-Click Change Button
        */
        // TODO  SND_LeftClickSound();
        // production_screen_return_screen = 2;  // {1: CityList Screen, 2: City Screen}
        // Production_Screen();



        /*
            Left-Click OK Button  ||  Hot-Key ESC
        */

        if(
            (input_field_idx == city_screen_ok_button)
            ||
            (input_field_idx == city_screen_esc_hotkey)
        )
        {
            _unit_stack_count = 0;
            // TODO  SND_LeftClickSound()
            _prev_world_x = _map_x;
            _prev_world_y = _map_y;
            Deactivate_Auto_Function();
            Deactivate_Help_List();
            if(IDK_active_stack_flag == ST_TRUE)
            {
                _unit_stack_count = 0;
            }
            leave_screen_flag = ST_TRUE;
            current_screen = scr_Main_Screen;
        }



        /*
            Left-Click Buy Button
        */


        /*
            Left-Click City Enchantment
        */
        /*
            Confirmation_Box()
            "Do you wish to turn off the " ... " spell?"
        */



        /*
            Left-Click Cityscape Building
        */
        /*
            lotsa logic
                ...only one per turn
                ...will cease production
                ...do you wish to sell
            City_Can_Sell_Building()
        */



        /*
            Left-Click Unit Window
        */



        /*
            Right-Click Unit Window
        */



        /*
            Left-Click Population Row
        */



        if( (leave_screen_flag == ST_FALSE) && (screen_changed == ST_FALSE) )
        {
            City_Screen_Draw__WIP();
            // DONT  j_o146p05_Empty_pFxn
            PageFlip_FX();
            Release_Time(1);
        }

    }



    IDK_completed_bldg_idx = ST_UNDEFINED;
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
        Help Entries
    */



    Reset_Window();
    Set_Page_Off();
    Reset_Map_Draw();
    Reduced_Map_Set_Dims(58, 30);
    Draw_Maps(215, 4, 5, 5, &city_map_x, &city_map_y, _map_plane, _prev_world_x, _prev_world_y, _human_player_idx);
    City_Screen_Draw_Map();
    Reset_Window();

    FLIC_Draw(0, 0, city_background_seg);

    City_Screen_Draw___WIP();

    City_Screen_Draw_Buttons();

    Reset_Window();

    if(IDK_completed_bldg_idx != ST_UNDEFINED)
    {
        /* jmp  short $+2 */
    }

    Cityscape_Draw__WIP(_city_idx, 4, 101, IDK_completed_bldg_idx, IDK_completed_bldg_idx);

    // TODO  IDK_City_Cityscape_Draw_MouseOver(city_screen_scanned_field, 1, _CITIES[_city_idx].owner_idx);

    if(
        (_CITIES[_city_idx].construction == bt_TradeGoods)
        ||
        (_CITIES[_city_idx].construction == bt_Housing)
    )
    {

        FLIC_Draw(260, 149, city_block_out_seg);

        Set_Font_Style2(0, 0, 0, 0);

        Set_Outline_Color(4);
        Set_Alias_Color(26);
        Set_Font_LF(0);

        if(_CITIES[_city_idx].construction != 0x01)  /* _Trade_Goods */
        {
            // WZD dseg:2BD3  str_CityScr_ProdDescr_TradeGoods  "Converts production to gold."
            Print_Paragraph(262, 159, 50, "Converts production to gold.", 2);
        }
        else
        {
            // WZD dseg:2BF0  str_CityScr_ProdDescr_Housing  "Increases population growth rate."
            Print_Paragraph(262, 159, 50, "Increases population growth rate.", 2);
        }

    }
    else
    {

        City_Screen_Draw_Production_Coins(_city_idx);

    }

}


// WZD o54p03
void City_Screen_Draw___WIP(void)
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

    strcpy(GUI_String_1, STR_TownSizes[_CITIES[_city_idx].size]);
    strcat(GUI_String_1, " of ");
    strcpy(GUI_String_2, _CITIES[_city_idx].name);
    strcat(GUI_String_1, GUI_String_2);

    Set_Font_Style1(5, 5, 0, 0);
    Set_Alias_Color(26);
    Set_Outline_Color(0);

    Print_Centered(105, 3, GUI_String_1);

    colors[0] = 190;
    colors[1] = 179;
    Set_Font_Colors_15(0, &colors[0]);
    Set_Outline_Color(19);
    Set_Font_Style1(1, 15, 0, 0);

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

    City_Screen_Draw_City_Enchantments__WIP(139, 51);

    City_Screen_Draw_Garrison_Window();

    colors[0] = 190;
    colors[1] = 179;

    Set_Font_Colors_15(0, &colors[0]);
    Set_Outline_Color(19);
    Set_Font_Style1(1, 15, 0, 0);

    if(
        (IDK_city_n_turns_to_produce > 0) &&
        (_CITIES[_city_idx].construction != 2) &&  /* _Housing */
        (_CITIES[_city_idx].construction != 1)  /* _Trade_Goods */
    )
    {
        Print_Integer_Right(286, 140, IDK_city_n_turns_to_produce);

        if(IDK_city_n_turns_to_produce > 1)
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

        x += (((44 - bitm_w) / 2) - bitm_x);
        y += (((31 - bitm_h) / 2) - bitm_y );

        FLIC_Set_LoopFrame_1(bitmap);

        Draw_Picture(x, y, bitmap);

    }

    Set_Font_Style2(1, 0, 0, 0);
    Set_Font_LF(0);
    Set_Alias_Color(26);
    Set_Font_Spacing_Width(1);

    para_width = Get_Paragraph_Max_Width(44, city_screen_product_name, 0);

    if((para_width > 42) || (para_width <= 0))
    {
        Set_Font_Style2(0, 0, 0, 0);
        Set_Font_LF(0);
        Set_Font_Spacing_Width(1);
    }

    if(Get_Paragraph_Max_Height(44, city_screen_product_name) > 6)
    {
        Set_Font_Style2(0, 0, 0, 0);
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
void City_Screen_Add_Fields__WIP(void)
{
    int16_t y2;
    int16_t x2;
    int16_t y1;
    int16_t x1;
    
    int16_t itr;  // _SI_

    Clear_Fields();

    G_CTY_ClickLabelCount = 0;
    _main_map_grid_field = INVALID_FIELD;
    _minimap_grid_field = INVALID_FIELD;
    UU_CityScreen_Field_Something = 0;

    for(itr = 0; itr < 9; itr++)
    {
        Unit_Window_Fields[itr] = -100;
    }

    city_screen_buy_button = INVALID_FIELD;
    city_sceen_change_button = INVALID_FIELD;
    city_screen_ok_button = INVALID_FIELD;
    UU_CityScreen_Field = INVALID_FIELD;

    // ~== Main Screen Add_Unit_Window_Fields()
    for(itr = 0; itr < _unit_stack_count; itr++)
    {
        City_Screen_Garrison_Window_Picture_Coords(itr, &x1, &y1, &x2, &y2);
        Unit_Window_Fields[itr] = Add_Hidden_Field(x1-1, y1-1, x2-2, y2-2, 0, ST_UNDEFINED);
    }

    City_Add_Fields_City_Enchantments(139, 51);

    City_Add_Fields_Buildings__WIP();

    City_Screen_Add_Fields_Production_Window();

    City_Screen_Add_Fields_Population_Row(_city_idx, 4, 27);

    City_Screen_Add_Fields_Resource_Window(_city_idx, 6, 52);

}


// WZD o54p05
// MoO2  Module: COLONY  Add_Screen_Fields_() |-> Add_Bldg_Fields_()
// 000BE486 cmp     _colony_bldgs[eax], 0
// 000BE4EF mov     _colony_bldg_fields[ebx], -1000
void City_Add_Fields_Buildings__WIP(void)
{
    int16_t y2;
    int16_t x2;
    int16_t y1;
    int16_t x1;
    int16_t itr;  // _SI_

    G_CTY_ClickLabelCount = 0;

    for(itr = 0; itr < IDK_Cityscape_Bldg_Cnt; itr++)
    {
        x1 = G_TBL_CityscapeBldngs[itr].x1;
        y1 = G_TBL_CityscapeBldngs[itr].y1;
        x2 = G_TBL_CityscapeBldngs[itr].x2;
        y2 = G_TBL_CityscapeBldngs[itr].y2;

        // G_CTY_ClickLabelArray[G_CTY_ClickLabelCount] = Add_Hidden_Field(x1, y1, x2, y2, cnst_ZeroString_25, 0xFFFF)
        G_CTY_ClickLabelArray[G_CTY_ClickLabelCount] = Add_Hidden_Field(x1, y1, x2, y2, 0, ST_UNDEFINED);

        G_CTY_ClickLabelCount++;
    }

}


// WZD o54p06
void City_Screen_Add_Fields_Production_Window(void)
{
    city_screen_buy_button = INVALID_FIELD;


    if(city_no_buy == ST_FALSE)
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

    if(city_no_buy != ST_FALSE)
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
// IDK_BuilingCompletedMessage

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
    int16_t IDK_CityScreen_ClickFieldSize;

    GUI_String_1 = (char *)Near_Allocate_First(100);
    GUI_String_2 = (char *)Near_Allocate_Next(100);

    city_screen_product_name = (char *)Near_Allocate_Next(40);

    city_screen_required_buildings_list = (int16_t *)Near_Allocate_Next(72);  // 36 int16_t  Buildings

    IDK_CityScreen_ClickFieldSize = 72;
    G_CTY_ClickLabelArray = Near_Allocate_Next(IDK_CityScreen_ClickFieldSize * 2);

    city_enchantment_fields  = (int16_t *)Near_Allocate_Next(52);  // 26 int16_t  City Enchantment Window Fields

    city_enchantment_list    = (int16_t *)Near_Allocate_Next(52);  // 26 int16_t  City Enchantments
    city_enchantment_owner_list  = (int16_t *)Near_Allocate_Next(52);  // 26 int16_t  City Enchantments

    city_population_row_fields = (int16_t *)Near_Allocate_Next(40);  // 20 int16_t  Population Row Field Indices

}


// WZD o54p14
// WZD o54p14
/*
    allocates the reduced_map_seg, resetting the screen_seg
    if the city's current contruction project is a Unit,
      loads the unit figure picture, into the UV/USW unit_figure_seg
*/
void Allocate_Reduced_Map_Load_Unit_Figure(void)
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
// drake178: CTY_GetBldngGFXSize()
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
        current_frame = FLIC_Get_CurrentFrame(IDK_buildings_35[bldg_idx]);
        frame_count = FLIC_Get_FrameCount(IDK_buildings_35[bldg_idx]);
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
            FLIC_Set_CurrentFrame(IDK_buildings_35[bldg_idx], itr_frames);
            Draw_Picture_To_Bitmap(IDK_buildings_35[bldg_idx], bitmap);
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
        FLIC_Set_CurrentFrame(IDK_buildings_35[bldg_idx], current_frame);
        Draw_Picture_To_Bitmap(IDK_buildings_35[bldg_idx], bitmap);
    }

}


// WZD o54p16
// drake178: N/A
// MoO2  Module: COLONY  Colony_Can_Buy_Product_0_()  |-> Module: COLCALC  Colony_Can_Afford_To_Buy_Product_()
int16_t City_Can_Buy_Product(void)
{
    city_no_buy = ST_FALSE;

    if(IDK_city_n_turns_to_produce < 2)
    {
        city_no_buy = ST_TRUE;
    }

    if( (_CITIES[_city_idx].construction == 2) || (_CITIES[_city_idx].construction == 1) )  /* _Housing or _Trade_Goods*/
    {
        city_no_buy = ST_TRUE;
    }
    else
    {
        if(City_Cost_To_Buy_Product(_city_idx) > _players[_human_player_idx].gold_reserve)
        {
            city_no_buy = ST_TRUE;
        }
    }

    if(IDK_city_production_cost == 0)
    {
        city_no_buy = ST_TRUE;
    }

    return city_no_buy;
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
    my = city_map_y;
    ystart = 4;

    for(itr_y = 0; itr_y < 5; itr_y++)
    {
        mx = city_map_x;
        xstart = 215;

        for(itr_x = 0; itr_x < 5; itr_x++)
        {
            corner_square = ST_FALSE;

            if
            (
                ((itr_y == 0) || (itr_y == 4))
                &&
                ((itr_x == 0) || (itr_x == 4))
            )
            {

                // fill_type 15, same as 3, except not default grayscale;  remap/color block 7?  ...Slope, Scale, Seed
                Gradient_Fill(xstart, ystart, (xstart + 19), (ystart + 17), 15, 7, ST_NULL, ST_NULL, ST_NULL);

                corner_square = ST_TRUE;
            }

            if(corner_square == ST_FALSE)
            {
                // TODO  EMM_Map_DataH();

                if(City_Map_Square_Is_Shared__ALWAYS_FALSE(mx, my, mp) == ST_TRUE)
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

    Redraw_Map_Unexplored_Area(215, 4, 5, 5, city_map_x, city_map_y, _map_plane);

    Draw_Map_Units(215, 4, 5, 5, city_map_x, city_map_y);

}
