/*
    ¿ ~== City (Screen) Util / Misc ?

    WIZARDS.EXE
        ovr055

*/

#include "MoM.H"
#include "City_ovr55.H"
#include "CityScr.H"
#include "ProdScr.H"



// WZD dseg:2C9A                                                 BEGIN:  ovr055 - Initialized Data

// WZD dseg:2C9A
char reload_lbx_file__ovr055[] = "RELOAD";

// WZD dseg:2CA0
char str_empty_string__ovr055[] = "";

// WZD dseg:2CA1
char str_hotkey_ESC__ovr055[] = "\x1B";

// WZD dseg:2CA3
char str_hotkey_X__ovr055[] = "X";

// WZD dseg:2CA5
char str_City[] = "city";

// WZD dseg:2CAA
char str_hotkey_Y__ovr055[] = "Y";

// WZD dseg:2CAC
char str_Plane[] = "plane";

// WZD dseg:2CB2
char str_TurnOffSpell_1[] = "Do you wish to turn off the \x02";

// WZD dseg:2CD0
char str_TurnOffSpell_2[] = "\x01 spell?";

// WZD dseg:2CD9
char str_Sp_Of_Sp[] = " of ";

// WZD dseg:2CDE
char str_Units__ovr055[] = "Units";

// WZD dseg:2CE4
char str_NoReport[] = "No Report";

// WZD dseg:2CEE
char str_Destroyed[] = "Destroyed";

// WZD dseg:2CF8
char str_Nightshade[] = "Nightshade";

// WZD dseg:2D03
char str_Genitive_NoS[] = "\x27";

// WZD dseg:2D05
char str_Genitive_S[] = "\x27s";

// WZD dseg:2D08
char help_lbx_file__ovr055[] = "HELP";

// WZD dseg:2D0D
char str_listdat_lbx_file__ovr055[] = "LISTDAT";

// WZD dseg:2D15
char str_hotkey_M__ovr055[] = "M";

// WZD dseg:2D17 46 6F 6F 64 00                                  cnst_Food db 'Food',0
// WZD dseg:2D1C 20 28 52 65 70 6C 61 63 65 64 29 00             aReplaced db ' (Replaced)',0
// WZD dseg:2D28 50 72 6F 64 75 63 74 69 6F 6E 00                aProduction_0 db 'Production',0
// WZD dseg:2D33 42 75 69 6C 64 69 6E 67 20 4D 61 69 6E 74 65 6E+aBuildingMaintenance db 'Building Maintenance',0
// WZD dseg:2D48 4D 6F 72 65 00                                  aMore db 'More',0
// WZD dseg:2D4D 47 6F 6C 64 00                                  cnst_Gold db 'Gold',0                   ; could use dseg:2ac5
// WZD dseg:2D52 4D 61 67 69 63 61 6C 20 50 6F 77 65 72 00       aMagicalPower db 'Magical Power',0
// WZD dseg:2D60 2D 00                                           asc_39800 db '-',0
// WZD dseg:2D62 53 70 65 6C 6C 20 52 65 73 65 61 72 63 68 00    aSpellResearch db 'Spell Research',0

// WZD dseg:2D71 00                                              align 2

// WZD dseg:2D71                                                 END:  ovr055 - Initialized Data



// WZD dseg:BFE8                                                 BEGIN:  ovr055 - Uninitialized Data

// WZD dseg:BFE8
int16_t * m_troop_fields;

// WZD dseg:BFEA
int16_t * m_troops;

// WZD dseg:BFEC
int16_t m_troop_count;

// WZD dseg:BFEE
int16_t y_start;

// WZD dseg:BFF0
int16_t x_start;

// WZD dseg:BFF0                                                 END:  ovr055 - Uninitialized Data




/*
    WIZARDS.EXE  ovr055
*/

// WZD o55p01
void Enemy_City_Screen(void)
{
    int16_t y1;
    int16_t x1;
    int16_t hotkey_X;
    int16_t full_screen_ESC_field;
    int16_t screen_changed;
    int16_t leave_screen;
    int16_t itr_troops;  // _SI_
    int16_t itr;  // _SI_
    int16_t input_field_idx;  // _DI_

    Clear_Fields();

    Set_Page_Off();

    Copy_On_To_Off_Page();

    Copy_Off_To_Back();

    PageFlip_FX();

    Set_Mouse_List(1, mouse_list_default);

    Deactivate_Auto_Function();

    Assign_Auto_Function(Enemy_City_Screen_Draw, 1);

    x_start = 60;

    y_start = 0;

    Deactivate_Help_List();

    Set_Enemy_City_Screen_Help_List();

    Enemy_City_Screen_Load();

    Player_Army_At_Square(_CITIES[_city_idx].wx, _CITIES[_city_idx].wy, _CITIES[_city_idx].wp, _CITIES[_city_idx].owner_idx, &m_troop_count, &m_troops[0]);

    for(itr_troops = 0; itr_troops < m_troop_count; itr_troops++)
    {
        if(Unit_Has_Invisibility(m_troops[itr_troops]) == ST_TRUE)
        {
            Clear_Structure(itr_troops, (uint8_t *)&m_troops[0], 2, m_troop_count);
            m_troop_count--;
        }
    }


    // RELOAD.LBX, 026  ENEMYCIT
    _enemy_city_seg = LBX_Reload(reload_lbx_file__ovr055, 26, _screen_seg);

    Do_Build_City_Enchantment_List();

    Do_City_Calculations(_city_idx);

    city_screen_scanned_field = ST_UNDEFINED;

    cityscape_bldg_count = 0;

    Clear_Fields();

    Set_Input_Delay(1);

    screen_changed = ST_FALSE;
    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {
        Mark_Time();

        Clear_Fields();

        City_Add_Fields_City_Enchantments((x_start + 139), (y_start + 51));

        City_Add_Fields_Buildings();

        for(itr_troops = 0; itr_troops < m_troop_count; itr_troops++)
        {

            x1 = (x_start +  5 + ((itr_troops % 6) * 22));
            y1 = (y_start + 52 + ((itr_troops / 6) * 20));

            m_troop_fields[itr_troops] = Add_Hidden_Field(x1, y1, (x1 + 19), (y1 + 18), str_empty_string__ovr055[0], ST_UNDEFINED);
        }

        full_screen_ESC_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, str_hotkey_ESC__ovr055[0], ST_UNDEFINED);

        hotkey_X = Add_Hot_Key(str_hotkey_X__ovr055[0]);

        input_field_idx = Get_Input();

        city_screen_scanned_field = Scan_Input();


        if(input_field_idx == hotkey_X)
        {
            // ; draws a name-value pair using DBG_DrawTableCell if debug is enabled, or does nothing otherwise
            // TODO  RP_DBG_TblDrawValue(0, 0, _city_idx, str_City);  // "city"
            // TODO  RP_DBG_TblDrawValue(0, 1, _CITIES[_city_idx].wx, str_hotkey_X__ovr055);  // "X"
            // TODO  RP_DBG_TblDrawValue(0, 2, _CITIES[_city_idx].wy, str_hotkey_Y__ovr055);  // "Y"
            // TODO  RP_DBG_TblDrawValue(0, 2, _CITIES[_city_idx].wp, str_Plane);  // "plane"
            for(itr = 0; itr < 20; itr++)
            {
                // TODO  DBG_TblDrawS16(itr, 4, _CITIES[_city_idx].bldg_status[itr]);
                
            }
            for(itr = 20; itr < 36; itr++)
            {
                // TODO  DBG_TblDrawS16((itr - 20), 5, _CITIES[_city_idx].bldg_status[itr]);
            }
        }


        if(abs(input_field_idx) == full_screen_ESC_field)
        {
            // TODO  SND_LeftClickSound();
            leave_screen = ST_TRUE;
        }


        for(itr_troops = 0; itr_troops < m_troop_count; itr_troops++)
        {
            if(m_troop_fields[itr_troops] == input_field_idx)
            {
                // TODO  SND_LeftClickSound();
                leave_screen = ST_UNDEFINED;
            }
        }


        for(itr = 0; itr < city_cityscape_field_count; itr++)
        {
            if(city_cityscape_fields[itr] == input_field_idx)
            {
                // TODO  SND_LeftClickSound();
                leave_screen = ST_UNDEFINED;
            }
        }


        for(itr = 0; itr < city_enchantment_display_count; itr++)
        {
            if(
                (city_enchantment_fields[itr] == input_field_idx)
                &&
                (city_enchantment_owner_list[(city_enchantment_display_first + itr)] == _human_player_idx)
            )
            {
                // TODO  SND_LeftClickSound();
                Deactivate_Help_List();
                strcpy(GUI_String_1, str_TurnOffSpell_1);
                strcat(GUI_String_1, _city_enchantment_names[city_enchantment_list[(city_enchantment_display_first + itr)]]);
                strcat(GUI_String_1, str_TurnOffSpell_2);
                if(Confirmation_Box(GUI_String_1) == ST_TRUE)
                {
                    // TODO  CTY_ClearEnchant(_city_idx, city_enchantment_list[(city_enchantment_display_first + itr)]);
                    Build_City_Enchantment_List(_city_idx, city_enchantment_list, city_enchantment_owner_list, &city_enchantment_list_count);
                    city_enchantment_display_scroll_flag = 0;
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
                Assign_Auto_Function(Enemy_City_Screen_Draw, 1);
                screen_changed = ST_TRUE;
                Reset_Map_Draw();
                Deactivate_Help_List();
                Set_Enemy_City_Screen_Help_List();
            }
        }


        if(input_field_idx == city_up_button)
        {
            // TODO  SND_LeftClickSound();
            city_enchantment_display_first -= 6;
            if(city_enchantment_display_first < 0)
            {
                city_enchantment_display_first = ((city_enchantment_list_count / 6) * 6);
            }
            screen_changed = ST_TRUE;
        }


        if(input_field_idx == city_dn_button)
        {
            // TODO  SND_LeftClickSound();
            city_enchantment_display_first += 6;
            if(city_enchantment_display_first > city_enchantment_list_count)
            {
                city_enchantment_display_first = 0;
            }
            screen_changed = ST_TRUE;
        }


        if((leave_screen == ST_FALSE) && (screen_changed == ST_FALSE))
        {
            Copy_Back_To_Off();
            Enemy_City_Screen_Draw();
            PageFlip_FX();
            Release_Time(1);
        }
        screen_changed = ST_FALSE;
    }

    PageFlipEffect = 3;
    Deactivate_Auto_Function();
    Deactivate_Help_List();
    Reset_Window();
    Clear_Fields();

}


// WZD o55p02
void Enemy_City_Screen_Draw(void)
{
    int16_t var_C;
    int16_t var_A;
    uint8_t colors[6];
    int16_t unit_type;
    int16_t itr;  // _SI_

    // TODO  mov     [_help_entries.help_01.entry_idx], (HLP_BARBARIAN_TOWNSFOLK + _CITIES[_city_idx].race)
    // TODO  mov     [_help_entries.help_02.entry_idx], HLP_ENCHANTMENTS_City
    // TODO  mov     [_help_entries.help_03.entry_idx], HLP_CITY_SCAPE_2

    for(itr = 0; itr < city_cityscape_field_count; itr++)
    {
        if(city_cityscape_fields[itr] == city_screen_scanned_field)
        {
            if(cityscape_bldgs[itr].bldg_idx < 100)
            {
                // TODO  mov     [_help_entries.help_03.entry_idx], (HLP_GUISE + cityscape_bldgs[itr].bldg_idx)
            }
            else
            {
                // TODO  mov     [_help_entries.help_03.entry_idx], (HLP_LAND_SIZE + cityscape_bldgs[itr].bldg_idx)
            }
        }
    }

    for( itr = 0; itr < 6; itr++)
    {
        // mov     [_help_entries[itr].help_05.entry_idx+bx], e_ST_UNDEFINED
    }

    if(city_enchantment_display_count > 0)
    {
        // TODO  mov     [_help_entries.help_02.entry_idx], e_ST_UNDEFINED
        for(itr = 0; itr < city_enchantment_display_count; itr++)
        {
            // TODO  mov     [_help_entries[itr].help_05.entry_idx+bx], City_Enchantment_HelpIdx(city_enchantment_list[(city_enchantment_display_first + itr)]);
        }
    }


    Reset_Window();
    Set_Page_Off();



    FLIC_Draw((x_start - 1), y_start, _enemy_city_seg);

    Set_Font_Style_Shadow_Down(5, 5, 0, 0);

    Set_Font_Spacing_Width(1);

    Set_Outline_Color(0);

    strcpy(GUI_String_1, _city_size_names[_CITIES[_city_idx].size]);

    strcat(GUI_String_1, str_Sp_Of_Sp);

    strcpy(GUI_String_2, _CITIES[_city_idx].name);

    strcat(GUI_String_1, GUI_String_2);

    Print_Centered((x_start + 106), (y_start + 3), GUI_String_1);

    colors[0] = 190;
    colors[1] = 179;

    Set_Font_Colors_15(0, &colors[0]);

    Set_Outline_Color(19);

    Set_Font_Style_Shadow_Down(1, 15, 0, 0);

    Print((x_start + 6), (y_start + 19), *_race_type_table[_CITIES[_city_idx].race].name);

    Print((x_start + 6), 43, str_Units__ovr055);  // "Units"

    City_Screen_Draw_Population_Row(_city_idx, (x_start + 4), (y_start + 27));

    // TODO  mov     [_help_entries.help_04.entry_idx], HLP_CITY_GARRISONS

    Set_Font_Spacing_Width(1);

    if(Check_Square_Scouted(_CITIES[_city_idx].wx, _CITIES[_city_idx].wy, _CITIES[_city_idx].wp) == ST_TRUE)
    {
        for(itr = 0; itr < m_troop_count; itr++)
        {
            if(m_troop_fields[itr] == city_screen_scanned_field)
            {
                unit_type = _UNITS[m_troops[itr]].type;
                Print((x_start + 36), 43, *_unit_type_table[unit_type].name);
            }
        }
        
        var_A = (x_start + 5);

        var_C = (y_start + 52);

        for(itr = 0; itr < m_troop_count; itr++)
        {
            Draw_Unit_StatFig((var_A + ((itr % 6) * 22)), (var_C + ((itr / 6) * 20)), m_troops[itr], 1);
        }

        Cycle_Unit_Enchantment_Animation();
    }
    else
    {
        // mov     [_help_entries.help_04.entry_idx], HLP_NO_REPORT
        Print((x_start + 54), (y_start + 64), str_NoReport);  // "No Report"
    }

    City_Screen_Draw_City_Enchantments((x_start + 139), (y_start + 51));

    Cityscape_Draw__WIP(_city_idx, (x_start + 4), (y_start + 101), 0, ST_UNDEFINED);

    Cityscape_Draw_Scanned_Building_Name(_CITIES[_city_idx].owner_idx, x_start, city_screen_scanned_field);

}


// WZD o55p03
void Enemy_City_Screen_Load(void)
{

    GUI_String_1 = (char *)Near_Allocate_First(80);

    GUI_String_2 = (char *)Near_Allocate_Next(80);

    city_enchantment_list = (int16_t *)Near_Allocate_Next(52);

    city_enchantment_owner_list = (int16_t *)Near_Allocate_Next(52);

    city_enchantment_fields = (int16_t *)Near_Allocate_Next(52);

    city_cityscape_fields = (byte_ptr)Near_Allocate_Next(72);  // 36 2-byte values

    city_population_row_fields = (int16_t *)Near_Allocate_Next(40);

    m_troops = (int16_t *)Near_Allocate_Next(18);

    m_troop_fields = (int16_t *)Near_Allocate_Next(18);

}


// WZD o55p04
void Cityscape_Draw_Scanned_Building_Name(int16_t scanned_field, int16_t x_start, int16_t owner_idx)
{
    char string[26];
    int16_t var_2;
    int16_t itr;  // _SI_
    int16_t IDK;  // _DI_

    if(owner_idx == ST_UNDEFINED)
    {
        return;
    }

    var_2 = 175;

    if(x_start == 1)
    {
        IDK = 38;
    }
    else
    {
        IDK = (x_start + 38);
        var_2 = (x_start + 175);
    }

    for(itr = 0; itr < city_cityscape_field_count; itr++)
    {
        if(city_cityscape_fields[itr] != scanned_field)
        {
            continue;
        }

        if(cityscape_bldgs[itr].bldg_idx < 100)
        {
            if(_CITIES[_city_idx].bldg_status[cityscape_bldgs[itr].bldg_idx] == bs_Removed)
            {
                if(
                    (cityscape_bldgs[itr].IDK_x > IDK)
                    &&
                    (cityscape_bldgs[itr].IDK_x <= var_2)
                )
                {
                    Set_Font_Style_Outline(1, 2, 0, 0);
                }
                else
                {
                    Set_Font_Style_Outline(0, 2, 0, 0);
                    Set_Font_Spacing_Width(1);
                }
                Set_Alias_Color(166);
                Set_Outline_Color(0);
                Print_Centered((cityscape_bldgs[itr].IDK_x + 1), cityscape_bldgs[itr].IDK_y, str_Destroyed);  // "Destroyed"
                Print_Centered((cityscape_bldgs[itr].IDK_x + 1), (cityscape_bldgs[itr].IDK_y + 6), bldg_data_table[cityscape_bldgs[itr].bldg_idx].name);
            }
            else
            {
                if(
                    (cityscape_bldgs[itr].IDK_x > IDK)
                    &&
                    (cityscape_bldgs[itr].IDK_x <= var_2)
                )
                {
                    Set_Font_Style_Outline(1, 0, 0, 0);
                }
                else
                {
                    Set_Font_Style_Outline(0, 0, 0, 0);
                    Set_Font_Spacing_Width(1);
                }
                Set_Alias_Color(8);
                Set_Outline_Color(0);
                Print_Centered((cityscape_bldgs[itr].IDK_x + 1), (cityscape_bldgs[itr].IDK_y + 6), bldg_data_table[cityscape_bldgs[itr].bldg_idx].name);
            }
        }
        else
        {
            Set_Font_Style_Outline(1, 0, 0, 0);
            if(
                (cityscape_bldgs[itr].IDK_x <= IDK)
                ||
                (cityscape_bldgs[itr].IDK_x > var_2)
            )
            {
                    Set_Font_Style_Outline(0, 0, 0, 0);
                    Set_Font_Spacing_Width(1);
            }
            Set_Alias_Color(8);
            Set_Outline_Color(0);

            if(cityscape_bldgs[itr].bldg_idx == 104)  /* ¿ per IDK_Spell_Cityscape_1(), some spell as building  ? */
            {
                strcpy(string, _players[owner_idx].name);
                Possessive(&string[0]);
                Print_Centered((cityscape_bldgs[itr].IDK_x + 1), (cityscape_bldgs[itr].IDK_y - 4), string);
                Print_Centered((cityscape_bldgs[itr].IDK_x + 1), (cityscape_bldgs[itr].IDK_y + 3), STR_MagicBuildings[(cityscape_bldgs[itr].bldg_idx - 100)]);
            }
            else
            {
                Print_Centered((cityscape_bldgs[itr].IDK_x + 1), (cityscape_bldgs[itr].IDK_y + 3), STR_MagicBuildings[(cityscape_bldgs[itr].bldg_idx - 100)]);
            }

        }

    }

}

// WZD o55p05
// drake178: CTY_CreateEmpty()

// WZD o55p06
// drake178: CTY_TakeOver()
// ¿ MoO2  Module: INVASION  Change_Colony_Ownership_() ?  ... Check_Rebellion_() |->  Change_Colony_Ownership_()
void Change_City_Ownership(int16_t city_idx, int16_t player_idx)
{
    int16_t City_Count;
    int16_t summon_city_idx;
    int16_t fortress_city_idx;
    uint8_t * city_enchantments;
    int16_t city_owner_idx;
    int16_t itr;  // _DI_

    city_owner_idx = _CITIES[city_idx].owner_idx;

    city_enchantments = &_CITIES[city_idx].enchantments[0];

    // clear all enchantments belonging to the current owner
    // drake178: BUG: should also remove curses placed by the conquering player
    for(itr = 0; itr < NUM_CITY_ENCHANTMENTS; itr++)
    {
        if(city_enchantments[itr] > 0)
        {
            if((city_enchantments[itr] - 1) == city_owner_idx)
            {
                city_enchantments[itr] = 0;
            }
        }
    }

    fortress_city_idx = Player_Fortress_City(city_owner_idx);

    summon_city_idx = Player_Summon_City(city_owner_idx);

    City_Count = 0;
    for(itr = 0; ((itr < _cities) && (City_Count < 2)); itr++)
    {
        if(_CITIES[itr].owner_idx == city_owner_idx)
        {
            City_Count++;
        }
    }

    if(
        (city_owner_idx != NEUTRAL_PLAYER_IDX)
        &&
        (
            (city_idx == fortress_city_idx)
            ||
            (City_Count < 2)
        )
    )
    {
        // TODO  WIZ_Conquer(city_owner_idx, player_idx, city_idx);
        // |-> WIZ_Banishment()
        // MoO2 Module: COLCALC Eliminate_Player_
    }

    if(
        (city_idx == summon_city_idx)
        &&
        (fortress_city_idx != ST_UNDEFINED)
    )
    {
        _players[player_idx].summon_wp = _CITIES[fortress_city_idx].wp;
        _players[player_idx].summon_wx = _CITIES[fortress_city_idx].wx;
        _players[player_idx].summon_wy = _CITIES[fortress_city_idx].wy;
    }

    if(player_idx == _human_player_idx)
    {
        // drake178: ; BUG: ignores Oracle exploration radius
        TILE_ExploreRadius__WIP(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp, 2);  // TODO  manifest constant for default scout range
    }

    _CITIES[city_idx].owner_idx = player_idx;

    _CITIES[city_idx].construction = bt_TradeGoods;

    _CITIES[city_idx].Prod_Accu = 0;

    _CITIES[city_idx].did_sell_building = ST_FALSE;

    Do_City_Calculations(city_idx);

    Reset_City_Road_Connection_Bitfields();

    // DONT  NOOP_Current_Player_All_City_Areas()

    Reset_City_Area_Bitfields();

}

// WZD o55p07
/*
    called from End_Of_Combat()
        |-> CTY_ApplyDamage(OVL_Action_Structure, IDK_population_lost, Destruction_Chance, &Buildings_Lost[0]);

*/
void CTY_ApplyDamage(int16_t city_idx, int16_t PopLoss, int16_t DChance, int16_t BList[])
{
    int16_t Destruction_Roll;
    int16_t Destruction_Count;
    // int16_t city_idx;  // _SI_
    int16_t itr_buildings;  // _DI_

    if(DChance > 0)
    {
        Destruction_Count = 0;
        for(itr_buildings = 1; itr_buildings < NUM_BUILDINGS; itr_buildings++)
        {
            if(_CITIES[city_idx].bldg_status[itr_buildings] > bs_Built)
            {
                Destruction_Roll = Random(100);
                
                if(Destruction_Roll > DChance)
                {
                    if(City_Remove_Building(itr_buildings, city_idx) == ST_TRUE)
                    {
                        BList[Destruction_Count] = itr_buildings;
                        Destruction_Count++;
                    }
                }
            }
        }
    }

    if(PopLoss > 0)
    {
        _CITIES[city_idx].population -= PopLoss;
    }

    _CITIES[city_idx].size = ((_CITIES[city_idx].population + 3) / 4);
    SETMAX(_CITIES[city_idx].size, 5);

    City_Check_Production(city_idx);

    if
    (
        (_CITIES[city_idx].population < 1)
        ||
        (_CITIES[city_idx].size < 1)
    )
    {
        _CITIES[city_idx].size = 0;
        Destroy_City(city_idx);
    }

}


// WZD o55p08
// drake178: CTY_Remove()
// ~ MoO2 Destroy_Colony_()
void Destroy_City(int16_t city_idx)
{
    int16_t did_destroy_city;
    int16_t itr_cities;  // _DI_

    did_destroy_city = ST_FALSE;

    itr_cities = 0;

    City_Remove_Road(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp);

    // ¿ definitely a while-loop ?
    while((itr_cities < _cities) && (did_destroy_city == ST_FALSE))
    {
        if(itr_cities == city_idx)
        {
            Delete_Structure(itr_cities, (uint8_t *)&_CITIES[0], (int16_t)sizeof(struct s_CITY), _cities);

            City_Delete_Building_Complete_Messages(city_idx);

            _cities--;

            did_destroy_city = ST_TRUE;

            Reset_City_Area_Bitfields();

            Reset_City_Road_Connection_Bitfields();

        }

        itr_cities++;
    }

}


// WZD o55p09
/*

CityList_Screen_Draw()
    |-> Print_City_Enchantment_List(100, 168, city_enchantment_list, city_enchantment_owner_list, city_enchantment_list_count)
*/
void Print_City_Enchantment_List(int16_t start_x, int16_t start_y, int16_t * city_enchantment_list, int16_t * city_enchantment_owner_list, int16_t city_enchantment_list_count)
{
    uint8_t player_banner_id;
    int16_t y;
    int16_t x;
    uint8_t banner_colors[6];
    uint8_t colors[6];
    int16_t var_2;
    int16_t itr;  // _DI_
    int16_t itr_colors;  // _SI_

    banner_colors[0] = 221;
    banner_colors[1] = 218;
    banner_colors[2] = 125;
    banner_colors[3] = 201;
    banner_colors[4] = 211;
    banner_colors[5] =  50;  // brown

    var_2 = 0;

    x = start_x;

    for(itr = 0; itr < city_enchantment_list_count; itr++)
    {
        y = start_y + (7 * itr);

        if(itr > 3)
        {
            x = (start_x + 63);
            y = (start_y + ((itr - 4) * 7));
        }

        if(city_enchantment_owner_list[itr] == 10)
        {
            for(itr_colors = 0; itr_colors < 6; itr_colors++)
            {
                colors[itr_colors] = 6;
            }
        }
        else
        {
            player_banner_id = _players[city_enchantment_owner_list[itr]].banner_id;
            for(itr_colors = 0; itr_colors < 6; itr_colors++)
            {
                colors[itr_colors] = banner_colors[player_banner_id];
            }
        }

        Set_Font_Colors_15(1, &colors[0]);

        if(city_enchantment_list_count > 4)
        {
            Set_Font_Style(0, 15, 0, 0);
        }
        else
        {
            Set_Font_Style(1, 15, 0, 0);
        }

        Set_Font_Spacing_Width(1);

        Set_Alias_Color(2);

        Print(x, y, _city_enchantment_names[city_enchantment_list[itr]]);

    }

}

// WZD o55p10
// drake178: CTY_ClearEnchant()

// WZD o55p11
// drake178: N/A
// sub_4B9BF()

// WZD o55p12
// drake178: CTY_DestroyBuilding()
// MoO2  Module: ERICNET  Remove_Building_()
int16_t City_Remove_Building(int16_t city_idx, int16_t bldg_idx)
{
    int16_t has_reqd_bldg;
    int16_t itr_buildings;  // _CX_
    int16_t did_remove_building;  // DNE in Dasm

    has_reqd_bldg = ST_FALSE;

    for(itr_buildings = 1; ((itr_buildings < NUM_BUILDINGS) && (has_reqd_bldg == ST_FALSE)); itr_buildings++)
    {
        if(
            (_CITIES[city_idx].bldg_status[itr_buildings] == bs_Built) ||
            (_CITIES[city_idx].bldg_status[itr_buildings] == bs_Replaced)
        )
        {
            if(
                (bldg_data_table[itr_buildings].reqd_bldg_1 == bldg_idx) ||
                (bldg_data_table[itr_buildings].reqd_bldg_2 == bldg_idx)
            )
            {
                has_reqd_bldg = ST_TRUE;
            }
        }
    }

    if(has_reqd_bldg != ST_TRUE)
    {
        _CITIES[city_idx].bldg_status[bldg_idx] = bs_Removed;
        did_remove_building = ST_TRUE;
    }
    else
    {
        did_remove_building = ST_FALSE;
    }

    return did_remove_building;
}


// WZD o55p13
// drake178: N/A
int16_t City_Sell_Building(int16_t city_idx, int16_t bldg_idx, int16_t gold_amount)
{
    int16_t did_sell_building;  // DNE in Dasm

    if(City_Remove_Building(city_idx, bldg_idx) != ST_TRUE)
    {
        did_sell_building = ST_FALSE;
    }
    else
    {
        Player_Add_Gold(_CITIES[city_idx].owner_idx, gold_amount);
        did_sell_building = ST_TRUE;
    }

    return did_sell_building;
}


// WZD o55p14
// drake178: ¿ ?
/*
    if returns ST_FALSE, "You cannot sell back the [] because it is required by the []."
    BUGBUG  return boolean is backwards
    TODO  rename to ¿ ? ~ NOT required / can sell

    loops through all buildings
    that are not already built
    checks if any prerequisites are missing

    XREF: City Screen - Sell Building
        after valid bldg_idx, did_sell_building == ST_FALSE
        if this returns ST_TRUE
            it calls City_Building_Is_Currently_Required()

bldg_idx is the the from the building that was left-clicked in the 'Cityscape Window'
the in/out bldg_idx is subsequently checked for being -1
    and, then used to get the required building name for the Warning_Message()
        "You cannot sell back the " " because it is required by the "
if this returns ST_TRUE, that in/out bldg_idx is cleared/set to 0

there's actually no way the bldg_status could be bs_Removed, right?

if inquiring about a building that has already been removed
    responds with values meaning there's reqd_bldg, and it's set to INVALID

default to ST_TRUE
loop through all the buildings
    check for any that have been built
for any/all that have been built
    check if the building in question is a prerequisite
Hrrrmm...
    then iter through the balance and see if they aren't all just Replaced?
But, sets ST_FALSE and the in/out reqd_bldg_idx

*/
int16_t City_Building_Has_Requirement(int16_t bldg_idx, int16_t city_idx, int16_t * reqd_bldg_idx)
{
    int16_t has_reqd_bldg;  // _DI_
    int16_t itr_all;  // _CX_
    int16_t itr_reqd;  // _SI_

    if(_CITIES[city_idx].bldg_status[bldg_idx] != bs_Removed)
    {
        has_reqd_bldg = ST_TRUE;
        // TODO  find a sensible place to set this, after fixing this function
        *reqd_bldg_idx = ST_UNDEFINED;  // DNE in Dasm

        for(itr_all = 1; ((itr_all < NUM_BUILDINGS) && (has_reqd_bldg == ST_TRUE)); itr_all++)
        {
            if(
                (_CITIES[city_idx].bldg_status[itr_all] == bs_Built) ||
                (_CITIES[city_idx].bldg_status[itr_all] == bs_Replaced)
            )
            {
                if(
                    (bldg_data_table[itr_all].reqd_bldg_1 == bldg_idx) ||
                    (bldg_data_table[itr_all].reqd_bldg_2 == bldg_idx)
                )
                {
                    itr_reqd = itr_all;
                    while((itr_reqd < NUM_BUILDINGS) && (_CITIES[city_idx].bldg_status[itr_reqd] == bs_Replaced)) { itr_reqd++; }

                    if(itr_reqd < NUM_BUILDINGS)
                    {
                        *reqd_bldg_idx = itr_reqd;
                        has_reqd_bldg = ST_FALSE;
                    }
                }
            }
        }
    }
    else
    {
        *reqd_bldg_idx = ST_UNDEFINED;
        has_reqd_bldg = ST_FALSE;
    }

    return has_reqd_bldg;
}


// WZD o55p15
// drake178: N/A
/*
    OOX XREF: City Screen - Sell Building
        after valid bldg_idx, did_sell_building == ST_FALSE, and Bldg_PreChk() == ST_TRUE
        if returns ST_FALSE, Warning_Message() of "Selling back your " " will cease production of your " "."

the building you are trying to sell is a required building for the building you are currently constructing

*/
int16_t City_Building_Is_Currently_Required(int16_t city_idx, int16_t bldg_idx)
{
    int16_t product_idx;  // _CX_
    int16_t building_is_required;  // DNE in Dasm

    product_idx = _CITIES[city_idx].construction;

    if(product_idx < 100)
    {
        if(
            (bldg_data_table[product_idx].reqd_bldg_1 == bldg_idx) ||
            (bldg_data_table[product_idx].reqd_bldg_1 == bldg_idx)
        )
        {
            building_is_required = ST_TRUE;
        }
        else
        {
            building_is_required = ST_FALSE;
        }
    }
    else
    {
        product_idx -= 100;  // translate to unit_type_idx
        if(
            (_unit_type_table[product_idx].reqd_bldg_1 == bldg_idx) ||
            (_unit_type_table[product_idx].reqd_bldg_2 == bldg_idx)
        )
        {
            building_is_required = ST_TRUE;
        }
        else
        {
            building_is_required = ST_FALSE;
        }
    }

    return building_is_required;
}


// WZD o55p16
void City_Check_Production(int16_t city_idx)
{
    // int16_t city_idx;  // _SI_
    int16_t production_idx;  // _DI_

    production_idx = _CITIES[city_idx].construction;

    if(production_idx < 100)
    {
        if(bldg_data_table[production_idx].reqd_bldg_1 > 100)
        {
            if(
                (_CITIES[city_idx].bldg_status[bldg_data_table[production_idx].reqd_bldg_2] != bs_Built)
                &&
                (_CITIES[city_idx].bldg_status[bldg_data_table[production_idx].reqd_bldg_2] != bs_Replaced)
            )
            {
                City_Cancel_Production(city_idx);
            }
        }
        else  /* if(bldg_data_table[production_idx].reqd_bldg_1 > 100) */
        {
            if(
                (
                    (_CITIES[city_idx].bldg_status[bldg_data_table[production_idx].reqd_bldg_1] != bs_Built)
                    &&
                    (_CITIES[city_idx].bldg_status[bldg_data_table[production_idx].reqd_bldg_1] != bs_Replaced)
                )
                ||
                (
                    (_CITIES[city_idx].bldg_status[bldg_data_table[production_idx].reqd_bldg_2] != bs_Built)
                    &&
                    (_CITIES[city_idx].bldg_status[bldg_data_table[production_idx].reqd_bldg_2] != bs_Replaced)
                )
            )
            {
                City_Cancel_Production(city_idx);
            }
        }
    }
    else  /* if(_CITIES[city_idx].construction] < 100) */
    {
        production_idx -= 100;  // production index - 100 == unit type index

        if(
            (
                (_CITIES[city_idx].bldg_status[_unit_type_table[production_idx].reqd_bldg_1] != bs_Built)
                &&
                (_CITIES[city_idx].bldg_status[_unit_type_table[production_idx].reqd_bldg_1] != bs_Replaced)
            )
            ||
            (
                (_CITIES[city_idx].bldg_status[_unit_type_table[production_idx].reqd_bldg_2] != bs_Built)
                &&
                (_CITIES[city_idx].bldg_status[_unit_type_table[production_idx].reqd_bldg_2] != bs_Replaced)
            )
        )
        {
            City_Cancel_Production(city_idx);
        }
    }

}


// WZD o55p17
void City_Cancel_Production(int16_t city_idx)
{
    if(_CITIES[city_idx].owner_idx == _human_player_idx)
    {
        if(MSG_Building_Complete_Count < 20)
        {
            MSG_Building_Complete[MSG_Building_Complete_Count].city_idx = city_idx;
            MSG_Building_Complete[MSG_Building_Complete_Count].bldg_type_idx = -(_CITIES[city_idx].construction);  // DEDU  negative means?  ("...can no longer produce...")
            MSG_Building_Complete_Count++;
        }
        else
        {
            _CITIES[city_idx].construction = bt_Housing;
        }
    }
    else
    {
        _CITIES[city_idx].construction = bt_GRANDVIZIER;
    }
}


// WZD o55p18
int16_t City_Sell_Building_Value(int16_t bldg_idx)
{
    int16_t building_value;

    building_value = (bldg_data_table[bldg_idx].construction_cost / 3);

    if(bldg_idx == bt_CityWalls)
    {
        building_value /= 2;
    }

    SETMIN(building_value, 1);

    return building_value;
}

// WZD o55p19
// drake178: N/A
// sub_4BEF0()


// WZD o55p20
// drake178: CTY_BuyProduction()
// MoO2  
// 1oom  
void Player_City_Buy_Production(int16_t player_idx, int16_t city_idx)
{
    int16_t cost;
    cost = City_Cost_To_Buy_Product(city_idx);
    _players[player_idx].gold_reserve -= cost;
    _CITIES[city_idx].Prod_Accu += City_Production_Cost(_CITIES[city_idx].construction, city_idx);
}


// WZD o55p21
// drake178: CTY_GetProdBuyCost()
// MoO2  Module: COLCALC  Colony_Cost_To_Buy_Product_()
/*
    Production Cost:
        0%: 4x
    <  50%: 3x
    < 100%: 2x
*/
int16_t City_Cost_To_Buy_Product(int16_t city_idx)
{
    int16_t production_cost;  // _SI_

    production_cost = City_Production_Cost(_CITIES[city_idx].construction, city_idx);

    if(_CITIES[city_idx].Prod_Accu >= 1)
    {
        if(_CITIES[city_idx].Prod_Accu < (production_cost / 2))
        {
            production_cost = (production_cost * 3);
        }
        else
        {
            production_cost = (production_cost * 2);
        }
    }
    else
    {
        production_cost = (production_cost * 4);
    }

    return production_cost;
}


// WZD o55p22
// drake178: ¿ ?
/*
    Why does this also draw the icons?
    Just to get the space of the fields for Food Group Two and Gold Group Two?

*/
void City_Screen_Add_Fields_Resource_Window(int16_t city_idx, int16_t xstart, int16_t ystart)
{
    int16_t group_amount;
    int16_t xpos;
//     int16_t food_units_p1;
//     int16_t production_units;
//     int16_t var_C;
//     int16_t mana_units;
//     int16_t research_units;
    int16_t resources[5];
    int16_t abs_diff_gold;
    int16_t abs_diff_food;
    int16_t city_resource_row_field_idx;
    int16_t itr_resource_types;  // _SI_
    int16_t group_two_x1;  // DNE in Dasm, uses city_resource_row_field_idx

    abs_diff_food = abs(_CITIES[city_idx].food_units - _CITIES[city_idx].population);
    abs_diff_gold = abs(_CITIES[city_idx].gold_units - _CITIES[city_idx].building_maintenance);

    resources[0] = (_CITIES[city_idx].food_units + 1);
    resources[1] = _CITIES[city_idx].production_units;
    resources[3] = _CITIES[city_idx].mana_units;
    resources[4] = _CITIES[city_idx].research_units;

    // IDGI: 
    if(_CITIES[city_idx].gold_units < _CITIES[city_idx].building_maintenance)
    {
            resources[2] = _CITIES[city_idx].gold_units + abs(_CITIES[city_idx].gold_units - _CITIES[city_idx].building_maintenance);
    }
    else
    {
            resources[2] = _CITIES[city_idx].building_maintenance + abs(_CITIES[city_idx].gold_units - _CITIES[city_idx].building_maintenance);
    }

    for(itr_resource_types = 0; itr_resource_types < NUM_RESOURCE_TYPES; itr_resource_types++)
    {
        if(itr_resource_types == 0)
        {

            if(_CITIES[city_idx].population < _CITIES[city_idx].food_units)
            {
                group_amount = _CITIES[city_idx].population;
            }
            else
            {
                group_amount = _CITIES[city_idx].food_units;
            }

            xpos = xstart;

            Draw_Resource_Icons(&xpos, (ystart + (itr_resource_types * 8)), resources[itr_resource_types], group_amount, city_big_resource_icon_seg[itr_resource_types], city_lil_resource_icon_seg[itr_resource_types]);

            if(group_amount > 0)
            {
                xpos += 6;
            }

            city_resources_rows_fields[0] = Add_Hidden_Field(xstart, (ystart + (itr_resource_types * 8)), xpos, (ystart + (itr_resource_types * 8) + 7),  str_empty_string__ovr055[0], ST_UNDEFINED);

            // city_resource_row_field_idx = xpos;
            group_two_x1 = xpos;

            if(_CITIES[city_idx].food_units < _CITIES[city_idx].population)
            {
                Draw_Resource_Icons(&xpos, (ystart + (itr_resource_types * 8)), resources[itr_resource_types], abs_diff_food, city_grey_big_bread_icon_seg, city_grey_lil_bread_icon_seg);
            }
            else
            {
                Draw_Resource_Icons(&xpos, (ystart + (itr_resource_types * 8)), resources[itr_resource_types], abs_diff_food, city_big_resource_icon_seg[itr_resource_types], city_lil_resource_icon_seg[itr_resource_types]);
            }

            city_resources_rows_fields[1] = Add_Hidden_Field(group_two_x1, (ystart + (itr_resource_types * 8)), 132, (ystart + (itr_resource_types * 8) + 7),  str_empty_string__ovr055[0], ST_UNDEFINED);

        }
        else if (itr_resource_types == 2)
        {

            if(_CITIES[city_idx].gold_units < _CITIES[city_idx].building_maintenance)
            {
                group_amount = _CITIES[city_idx].gold_units;
            }
            else
            {
                group_amount = _CITIES[city_idx].building_maintenance;
            }

            xpos = xstart;

            Draw_Resource_Icons(&xpos, (ystart + (itr_resource_types * 8)), resources[itr_resource_types], group_amount, city_big_resource_icon_seg[itr_resource_types], city_lil_resource_icon_seg[itr_resource_types]);

            if(group_amount > 0)
            {
                xpos += 5;  // BUGBUG: ¿ should be 6 like food, because (icon space - min icon space) = (8 - 2) = 6 ?
            }

            city_resources_rows_fields[3] = Add_Hidden_Field(xstart, (ystart + (itr_resource_types * 8)), xpos, (ystart + (itr_resource_types * 8) + 7),  str_empty_string__ovr055[0], ST_UNDEFINED);

            // BUGBUG: recalculates abs_diff_gold as group_amount  probably just shouldn't calculate either in the prep-work and just do this for food as well
            group_amount = abs(_CITIES[city_idx].gold_units - _CITIES[city_idx].building_maintenance);

            // city_resource_row_field_idx = xpos;
            group_two_x1 = xpos;

            if(_CITIES[city_idx].gold_units < _CITIES[city_idx].building_maintenance)
            {
                Draw_Resource_Icons(&xpos, (ystart + (itr_resource_types * 8)), resources[itr_resource_types], group_amount, city_big_resource_icon_seg[5], black_gold1_icon_seg);
            }
            else
            {
                Draw_Resource_Icons(&xpos, (ystart + (itr_resource_types * 8)), resources[itr_resource_types], group_amount, city_big_resource_icon_seg[itr_resource_types], city_lil_resource_icon_seg[itr_resource_types]);
            }

            city_resources_rows_fields[4] = Add_Hidden_Field(group_two_x1, (ystart + (itr_resource_types * 8)), 132, (ystart + (itr_resource_types * 8) + 7),  str_empty_string__ovr055[0], ST_UNDEFINED);

        }
        else
        {

            xpos = xstart;

            Draw_Resource_Icons(&xpos, (ystart + (itr_resource_types * 8)), resources[itr_resource_types], resources[itr_resource_types], city_big_resource_icon_seg[itr_resource_types], city_lil_resource_icon_seg[itr_resource_types]);

            // Eh? food has two groups, so it's 0,1 and gold has two groups, so it's 3,4
            switch(itr_resource_types)
            {
                case 1:  // Production
                {
                    city_resource_row_field_idx = 2;
                } break;
                case 3:  // Magic Power
                {
                    city_resource_row_field_idx = 5;
                } break;
                case 4:  // Research
                {
                    city_resource_row_field_idx = 6;
                } break;
            }

            // city_resources_rows_fields[city_resource_row_field_idx] = Add_Hidden_Field(xstart, (ystart + (itr_resource_types * 8)), 132, (ystart + (itr_resource_types * 8) + 7),  cnst_ZeroString_30, 0xFFFF);
            city_resources_rows_fields[city_resource_row_field_idx] = Add_Hidden_Field(xstart, (ystart + (itr_resource_types * 8)), 132, (ystart + (itr_resource_types * 8) + 7),  0, ST_UNDEFINED);

        }

    }

}


// WZD o55p23
// drake178: ¿ ?
void City_Screen_Draw_Resource_Icons(int16_t city_idx, int16_t xstart, int16_t ystart)
{
    int16_t group_amount;
    int16_t xpos;
    int16_t resources[5];
    int16_t abs_diff_gold;
    int16_t abs_diff_food;
    int16_t itr_resource_types;  // _SI_

    abs_diff_food = abs(_CITIES[city_idx].food_units - _CITIES[city_idx].population);
    abs_diff_gold = abs(_CITIES[city_idx].gold_units - _CITIES[city_idx].building_maintenance);

    resources[0] = (_CITIES[city_idx].food_units + 1);
    resources[1] = _CITIES[city_idx].production_units;
    resources[3] = _CITIES[city_idx].mana_units;
    resources[4] = _CITIES[city_idx].research_units;

    // IDGI: 
    if(_CITIES[city_idx].gold_units < _CITIES[city_idx].building_maintenance)
    {
            resources[2] = _CITIES[city_idx].gold_units + abs(_CITIES[city_idx].gold_units - _CITIES[city_idx].building_maintenance);
    }
    else
    {
            resources[2] = _CITIES[city_idx].building_maintenance + abs(_CITIES[city_idx].gold_units - _CITIES[city_idx].building_maintenance);
    }

    for(itr_resource_types = 0; itr_resource_types < NUM_RESOURCE_TYPES; itr_resource_types++)
    {

        if(itr_resource_types == 0)
        {

            if(_CITIES[city_idx].population < _CITIES[city_idx].food_units)
            {
                group_amount = _CITIES[city_idx].population;
            }
            else
            {
                group_amount = _CITIES[city_idx].food_units;
            }

            xpos = xstart;

            Draw_Resource_Icons(&xpos, (ystart + (itr_resource_types * 8)), resources[itr_resource_types], group_amount, city_big_resource_icon_seg[itr_resource_types], city_lil_resource_icon_seg[itr_resource_types]);

            if(group_amount > 0)
            {
                xpos += 6;
            }

            if(_CITIES[city_idx].food_units < _CITIES[city_idx].population)
            {
                Draw_Resource_Icons(&xpos, (ystart + (itr_resource_types * 8)), resources[itr_resource_types], abs_diff_food, city_grey_big_bread_icon_seg, city_grey_lil_bread_icon_seg);
            }
            else
            {
                Draw_Resource_Icons(&xpos, (ystart + (itr_resource_types * 8)), resources[itr_resource_types], abs_diff_food, city_big_resource_icon_seg[itr_resource_types], city_lil_resource_icon_seg[itr_resource_types]);
            }

        }
        else if (itr_resource_types == 2)
        {

            xpos = xstart;

            if(_CITIES[city_idx].gold_units < _CITIES[city_idx].building_maintenance)
            {
                group_amount = _CITIES[city_idx].gold_units;
            }
            else
            {
                group_amount = _CITIES[city_idx].building_maintenance;
            }

            Draw_Resource_Icons(&xpos, (ystart + (itr_resource_types * 8)), resources[itr_resource_types], group_amount, city_big_resource_icon_seg[itr_resource_types], city_lil_resource_icon_seg[itr_resource_types]);

            if(group_amount > 0)
            {
                xpos += 5;  // BUGBUG: ¿ should be 6 like food, because (icon space - min icon space) = (8 - 2) = 6 ?
            }

            city_resources_rows_fields[3] = Add_Hidden_Field(xstart, (ystart + (itr_resource_types * 8)), xpos, (ystart + (itr_resource_types * 8) + 7),  str_empty_string__ovr055[0], ST_UNDEFINED);

            // BUGBUG: recalculates abs_diff_gold as group_amount  probably just shouldn't calculate either in the prep-work and just do this for food as well
            group_amount = abs(_CITIES[city_idx].gold_units - _CITIES[city_idx].building_maintenance);

            if(_CITIES[city_idx].gold_units < _CITIES[city_idx].building_maintenance)
            {
                Draw_Resource_Icons(&xpos, (ystart + (itr_resource_types * 8)), resources[itr_resource_types], group_amount, city_big_resource_icon_seg[5], black_gold1_icon_seg);
            }
            else
            {
                Draw_Resource_Icons(&xpos, (ystart + (itr_resource_types * 8)), resources[itr_resource_types], group_amount, city_big_resource_icon_seg[itr_resource_types], city_lil_resource_icon_seg[itr_resource_types]);
            }

        }
        else
        {

            xpos = xstart;

            Draw_Resource_Icons(&xpos, (ystart + (itr_resource_types * 8)), resources[itr_resource_types], resources[itr_resource_types], city_big_resource_icon_seg[itr_resource_types], city_lil_resource_icon_seg[itr_resource_types]);

        }

    }

}


// WZD o55p24
// drake178: CTY_DrawResourceCnt()
/*
    UNIT_DrawUpkeep() |-> j_Draw_Resource_Icons() |-> Draw_Resource_Icons()
    City_Screen_Add_Fields_Resource_Window()      |-> Draw_Resource_Icons()
    City_Screen_Draw_Resource_Icons()             |-> Draw_Resource_Icons()

does a FLIC_Draw() of the big and lil resource icons, of the 1of5 resources
resource count/points/units
logic for draw position to accomodate left and right groups, (only) for food and gold
logic for overlapping the icons

total_amount vs. group_amount  (¿ only food & gold ?)
    for spacing/overlapping the icons
    group_amount is the amount to draw, for this group

check if we need to do any big icons  (~== if count is 10+)
handle overlapping big icons
    if icon space * icon count is > 79  (¿ 79 ? ¿ ~ 5.6 big icons ?)

if we're on the second/right group, how to we get away with drawing the big icons
    without having the spacing from the lil icons of the first/left group?
Something about the <= 79, the initial spacing being calculated off of total_amount, and *knowing* that the max lil icon count is 9?
max lil icon count is 9, therefore, max lil icon width is 9 * 3 = 27?
max group width is 79 + 27 = 106
min big icon space is 3
3 * n + 11?
min lil icon space is 2
2 * n + 6?
max resource amount is 255
25 big, 5 lil
(3 * 25 + 11) + (2 * 5 + 6) = (75 + 11) + (10 + 6) = 86 + 16 = 102

resources window dimensions
x1   6
y1  52
x2 132
y2  91  (52 + resource type * 8 + 7 = 52 + 4 * 8 + 7 = 52 + 32 + 7 = 52 + 39 = 91)

*/
void Draw_Resource_Icons(int16_t * xstart, int16_t ystart, int16_t total_amount, int16_t group_amount, SAMB_ptr big_icon_pict, SAMB_ptr lil_icon_pict)
{
    int16_t icon_space_remainder;
    int16_t big_icon_space;
    int16_t itr_icon_count;
    int16_t icon_count;
    int16_t space_is_set;
    int16_t curr_xstart;
    int16_t icon_space;  //_SI_
    int16_t group_big_amount;  // DNE in Dasm

    curr_xstart = *xstart;
    icon_space_remainder = 0;
    big_icon_space = 0;

    if((total_amount / 10) > 0)
    {
        icon_space = 14;  // width of big icon
        space_is_set = ST_FALSE;
        icon_count = total_amount / 10;  // total big icon count

        while(space_is_set == ST_FALSE)
        {
            if((icon_count * icon_space) <= 79)
            {
                space_is_set = ST_TRUE;
            }
            else
            {
                icon_space--;
            }

            if(icon_space < 4)  // minimum icon space 3 / maximum overlap 11
            {
                space_is_set = ST_TRUE;
            }
        }

        for(itr_icon_count = 0; itr_icon_count < (group_amount / 10); itr_icon_count++)
        {
            FLIC_Draw((curr_xstart + (itr_icon_count * icon_space)), ystart, big_icon_pict);
        }

        if((group_amount / 10) > 0)
        {
            curr_xstart += (((group_amount / 10) * icon_space) + 1);
            icon_space_remainder = 0;
        }

        group_big_amount = ((group_amount / 10) * 10);
        total_amount -= group_big_amount;
        group_amount -= group_big_amount;
        big_icon_space = (curr_xstart - *xstart);
    }

    // HERE: total_amount was <10 or is the balance after subtracting the 10's of the group_amount
    if(total_amount > 0)
    {
        icon_space = 8;  // width of lil icon
        space_is_set = ST_FALSE;
        icon_count = total_amount;
        while(space_is_set == ST_FALSE)
        {
            if((icon_count * icon_space) < (125 - big_icon_space))
            {
                space_is_set = ST_TRUE;
            }
            else
            {
                icon_space--;
            }

            if(icon_space < 3)
            {
                space_is_set = ST_TRUE;
            }
        }
        
        for(itr_icon_count = 0; itr_icon_count < group_amount; itr_icon_count++)
        {
            FLIC_Draw((curr_xstart + (itr_icon_count * icon_space)), ystart, lil_icon_pict);
        }

        if(group_amount > 0)
        {
            curr_xstart += (group_amount * icon_space);
            icon_space_remainder = (8 - icon_space);
        }
    }

    *xstart = curr_xstart + icon_space_remainder;
}


// WZD o55p25
// drake178: ¿ ?
void City_Screen_Draw_Production_Coins(int16_t city_idx)
{
    int16_t coins_per_row;
    int16_t points_per_coin;
    int16_t accumulated_partial_coin;
    int16_t accumulated_whole_coins;
    int16_t product_coin_count;
    int16_t ypos;
    int16_t icon_space;
    int16_t ystart;
    int16_t xstart;
    int16_t itr;  // _SI_
    int16_t xpos;  // _DI_

    xstart = 262;
    ystart = 151;

    icon_space = 5;  // coin icon is 4px wide + 1px pad

    points_per_coin = 10;

    coins_per_row = 10;

    accumulated_whole_coins = (_CITIES[city_idx].Prod_Accu / points_per_coin);

    accumulated_partial_coin = (_CITIES[city_idx].Prod_Accu % points_per_coin);

    product_coin_count = (City_Current_Product_Cost(city_idx) / points_per_coin);

    if(product_coin_count == 0)
    {
        product_coin_count = 1;
    }

    if(product_coin_count > 50)
    {
        icon_space = 2;
        coins_per_row = 20;
    }

    for(itr = 0; itr < product_coin_count; itr++)
    {
        xpos = (xstart + (itr % coins_per_row) * icon_space);
        ypos = (ystart + (itr / coins_per_row) * 6);

        if(itr < accumulated_whole_coins)
        {
            FLIC_Draw(xpos, ypos, city_full_resource_seg);
        }
        else
        {
            FLIC_Draw(xpos, ypos, city_reqd_resource_seg);

            if((itr == accumulated_whole_coins) && (accumulated_partial_coin > 0))
            {

                accumulated_partial_coin = (accumulated_partial_coin / 2);

                Set_Window(0, 0, (xpos + accumulated_partial_coin), 199);

                Clipped_Draw(xpos, ypos, city_full_resource_seg);

                Reset_Window();
            }

        }

    }

}


// WZD o55p26
// drake178: ¿ ?
void Build_City_Enchantment_List(int16_t city_idx, int16_t city_enchantment_list[], int16_t city_enchantment_owner_list[], int16_t * city_enchantment_list_count)
{
    uint8_t * city_enchantments;
    int16_t city_owner_idx;
    int16_t itr1_city_enchantment_count;
    int16_t itr_city_enchantments;  // _SI_
    int16_t itr2_city_enchantment_count;  // _SI_
    int16_t city_enchantment_count;  // _DI_

    city_owner_idx = _CITIES[city_idx].owner_idx;

    city_enchantments = _CITIES[city_idx].enchantments;

    city_enchantment_count = 0;

    for(itr_city_enchantments = 0; itr_city_enchantments < NUM_CITY_ENCHANTMENTS; itr_city_enchantments++)
    {
        if(city_enchantments[itr_city_enchantments] > 0)
        {
            city_enchantment_list[city_enchantment_count] = itr_city_enchantments;

            if(stricmp(_city_enchantment_names[itr_city_enchantments], str_Nightshade) != 0)
            {
                city_enchantment_owner_list[city_enchantment_count] = (city_enchantment_list[itr_city_enchantments] - 1);
            }
            else
            {
                city_enchantment_owner_list[city_enchantment_count] = 10;
            }

            city_enchantment_count++;
        }
    }

    *city_enchantment_list_count = city_enchantment_count;

    for(itr1_city_enchantment_count = 0; itr1_city_enchantment_count < city_enchantment_count; itr1_city_enchantment_count++)
    {
        itr2_city_enchantment_count = 0;
        while((city_enchantment_count - 1) > itr2_city_enchantment_count)
        {
            if(city_enchantment_owner_list[itr2_city_enchantment_count] < city_enchantment_owner_list[(itr2_city_enchantment_count + 1)])
            {
                Swap_Short(&city_enchantment_owner_list[itr2_city_enchantment_count], &city_enchantment_owner_list[(itr2_city_enchantment_count + 1)]);
                Swap_Short(&city_enchantment_list[itr2_city_enchantment_count], &city_enchantment_list[(itr2_city_enchantment_count + 1)]);
            }
            itr2_city_enchantment_count++;
        }
    }

}


// WZD o55p27
// drake178: ¿ ?
void Do_Build_City_Enchantment_List(void)
{

    Build_City_Enchantment_List(_city_idx, city_enchantment_list, city_enchantment_owner_list, &city_enchantment_list_count);

    city_enchantment_display_first = 0;

    city_enchantment_display_scroll_flag = ST_FALSE;

    if(city_enchantment_list_count > 6)
    {
        city_enchantment_display_scroll_flag = ST_TRUE;
    }

}


// WZD o55p28
// drake178: ¿ ?
void City_Screen_Draw_City_Enchantments(int16_t xstart, int16_t ystart)
{
    uint8_t banner_id;
    uint8_t banner_colors[6];
    uint8_t colors[6];
    int16_t itr;  // _DI_
    int16_t itr_colors;  // _SI_
    
    banner_colors[0] = 222;
    banner_colors[1] =  79;
    banner_colors[2] = 127;
    banner_colors[3] = 201;
    banner_colors[4] = 211;
    banner_colors[5] =  50;

    for(itr = 0; itr < city_enchantment_display_count; itr++)
    {
        if(city_enchantment_owner_list[(city_enchantment_display_first + itr)] == 10)
        {
            for(itr_colors = 0; itr_colors < 6; itr_colors++)
            {
                colors[itr_colors] = ST_BLACK;
            }
        }
        else
        {
            if(city_enchantment_owner_list[(city_enchantment_display_first + itr)] == 5)
            {
                _players[NEUTRAL_PLAYER_IDX].banner_id = 5;  // BNR_Brown
            }

            banner_id = _players[city_enchantment_owner_list[(city_enchantment_display_first + itr)]].banner_id;

            for(itr_colors = 0; itr_colors < 6; itr_colors++)
            {
                colors[itr_colors] = banner_colors[banner_id];
            }
        }

        Set_Font_Colors_15(0, &colors[0]);
        Set_Font_Style_Shadow_Up(1, 15, 0, 0);
        Set_Outline_Color(6);
        Set_Alias_Color(7);
        Set_Font_Spacing(1);

        Print(xstart, (ystart + (itr * 7)), _city_enchantment_names[city_enchantment_list[(city_enchantment_display_first + itr)]]);

    }

}


// WZD o55p29
// drake178: CTY_CreateEnchCtrls()
void City_Add_Fields_City_Enchantments(int16_t xstart, int16_t ystart)
{
    int16_t itr;  // _SI_

    city_up_button = -1000;
    city_dn_button = -1000;

    if(city_enchantment_display_scroll_flag == ST_TRUE)
    {
        city_up_button = Add_Button_Field((xstart + 62), (ystart -  1), str_empty_string__ovr055, city_spell_up_arrow_button_seg, str_empty_string__ovr055[0], ST_UNDEFINED);
        city_dn_button = Add_Button_Field((xstart + 62), (ystart + 34), str_empty_string__ovr055, city_spell_dn_arrow_button_seg, str_empty_string__ovr055[0], ST_UNDEFINED);
    }

    city_enchantment_display_count = 0;
    city_enchantment_display_count = (city_enchantment_list_count - city_enchantment_display_first);
    if(city_enchantment_display_count > 6)
    {
        city_enchantment_display_count = 6;
    }
    if(city_enchantment_display_count < 0)
    {
        city_enchantment_display_first = 0;
        city_enchantment_display_count = (city_enchantment_list_count - city_enchantment_display_first);
        if(city_enchantment_display_count > 6)
        {
            city_enchantment_display_count = 6;
        }
    }
    
    for(itr = 0; itr < city_enchantment_display_count; itr++)
    {
        // city_enchantment_fields[itr] = Add_Hidden_Field(xstart, (ystart + (itr * 7)), (xstart + 60), (ystart + (itr * 7) + 6), cnst_ZeroString_30, 0xFFFF);
        city_enchantment_fields[itr] = Add_Hidden_Field(xstart, (ystart + (itr * 7)), (xstart + 60), (ystart + (itr * 7) + 6), 0, ST_UNDEFINED);
    }

}


// WZD o55p30
// drake178: ¿ ?
void City_Screen_Draw_Population_Row(int16_t city_idx, int16_t xstart, int16_t ystart)
{
    int16_t rebel_count;
    int16_t did_min_farmers;
    int16_t icon_space;
    int16_t min_farmer_count;
    int16_t farmer_count;
    int16_t itr_townsfolk;
    int16_t xadd;  // _DI_

    min_farmer_count = City_Minimum_Farmers(city_idx);

    rebel_count = City_Rebel_Count(city_idx);

    if(_CITIES[city_idx].farmer_count < min_farmer_count)
    {
        _CITIES[city_idx].farmer_count = min_farmer_count;
    }

    if(_CITIES[city_idx].farmer_count > (_CITIES[city_idx].population - rebel_count))
    {
        _CITIES[city_idx].farmer_count = (_CITIES[city_idx].population - rebel_count);
    }

    farmer_count = _CITIES[city_idx].farmer_count;

    icon_space = 9;

    if(_CITIES[city_idx].population > 19)
    {
        icon_space = 8;
    }

    xadd = 0;

    did_min_farmers = ST_FALSE;

    if(_CITIES[city_idx].population == 1)
    {
        did_min_farmers = ST_TRUE;
    }

    for(itr_townsfolk = 0; itr_townsfolk < farmer_count; itr_townsfolk++)
    {
        if((itr_townsfolk == min_farmer_count) && (did_min_farmers == ST_FALSE))
        {
            xadd += 4;
            did_min_farmers = ST_TRUE;
        }
        FLIC_Draw((xstart + xadd), ystart, city_farmer_icon_seg[_CITIES[city_idx].race]);
        xadd += icon_space;
    }

    if(did_min_farmers == ST_FALSE)
    {
        xadd += 4;
    }

    for(itr_townsfolk = 0; itr_townsfolk < (_CITIES[city_idx].population - farmer_count - rebel_count); itr_townsfolk++)
    {
        FLIC_Draw((xstart + xadd), ystart, city_worker_icon_seg[_CITIES[city_idx].race]);
        xadd += icon_space;
    }

    xadd += 4;

    for(itr_townsfolk = 0; itr_townsfolk < rebel_count; itr_townsfolk++)
    {
        FLIC_Draw((xstart + xadd), ystart, city_rebel_icon_seg[_CITIES[city_idx].race]);
        xadd += icon_space;
    }

}


// WZD o55p31
// drake178: ¿ ?
void City_Screen_Add_Fields_Population_Row(int16_t city_idx, int16_t xstart, int16_t ystart)
{
    int16_t rebel_count;
    int16_t farmer_count;
    int16_t skip_xadd;
    int16_t figure_space;
    int16_t min_farmer_count;
    int16_t y2;
    int16_t x2;
    int16_t y1;
    int16_t x1;
    int16_t itr_figures;

    int16_t xadd;  // _DI_

    min_farmer_count = City_Minimum_Farmers(city_idx);

    rebel_count = City_Rebel_Count(city_idx);

    if(_CITIES[city_idx].farmer_count < min_farmer_count)
    {
        _CITIES[city_idx].farmer_count = min_farmer_count;
    }

    if(_CITIES[city_idx].farmer_count > (_CITIES[city_idx].population - rebel_count))
    {
        _CITIES[city_idx].farmer_count = (_CITIES[city_idx].population - rebel_count);
    }

    figure_space = 9;

    if(_CITIES[city_idx].population > 19)
    {
        figure_space = 8;
    }

    xadd = 0;

    farmer_count = _CITIES[city_idx].farmer_count;

    skip_xadd = ST_FALSE;

    if(_CITIES[city_idx].population == 1)
    {
        skip_xadd = ST_TRUE;
    }

    itr_figures = 0;
    while((_CITIES[city_idx].population - rebel_count) > itr_figures)
    {
        if((itr_figures == min_farmer_count) && (skip_xadd == ST_FALSE) )
        {
            xadd += 4;
            skip_xadd = ST_TRUE;
        }
        
        if((itr_figures == farmer_count) && (skip_xadd == ST_FALSE) )
        {
            xadd += 4;
            skip_xadd = ST_TRUE;
        }

        x1 = xstart+ xadd;
        y1 = ystart;
        x2 = x1 + 9;  // BUGBUG: ¿ should use figure_space ?
        y2 = y1 + 14;

        // Add_Hidden_Field(x1, y1, x2, y2, cnst_ZeroString_30, 0xFFFF);
        city_population_row_fields[itr_figures] = Add_Hidden_Field(x1, y1, x2, y2, 0, ST_UNDEFINED);

        itr_figures++;
    }

}


// WZD o55p32
// drake178: ¿ ?
/*
    ~== WZD o64p09  Main Screen  Unit_Window_Picture_Coords()
        row is 5, vs. 3
        x1 is * 20, vs. * 23
        y1 is * 17, vs. * 29
        x2 is + 17, vs. + 22
        y2 is + 15, vs. + 28
    both use g_unit_window_fields[9]

*/
void City_Screen_Garrison_Window_Picture_Coords(int16_t stack_idx, int16_t * x1, int16_t * y1, int16_t * x2, int16_t * y2)
{
    *x1 = _garrison_window_start_x + ((stack_idx % 5) * 20);
    *y1 = _garrison_window_start_y + ((stack_idx / 5) * 17);
    *x2 = *x1 + 17;
    *y2 = *y1 + 15;
}


// WZD o55p33
// drake178: N/A
// IDK_CtyBld_s4D357()

// WZD o55p34
// drake178: WIZ_AddGold()
void Player_Add_Gold(int16_t player_idx, int16_t amount)
{
    if(_players[player_idx].gold_reserve < 30000)
    {
        if((30000 - _players[player_idx].gold_reserve) >= amount)
        {
            _players[player_idx].gold_reserve += amount;
        }
        else
        {
            _players[player_idx].gold_reserve = 30000;
        }
    }
    else
    {
        if(amount > 0)
        {
            _players[player_idx].gold_reserve = 30000;
        }
        else
        {
            _players[player_idx].gold_reserve += amount;
        }
    }
}

// WZD o55p35
void Player_Add_Mana(int16_t player_idx, int16_t amount)
{
    if(_players[player_idx].mana_reserve < 30000)
    {
        if((30000 - _players[player_idx].mana_reserve) >= amount)
        {
            _players[player_idx].mana_reserve += amount;
        }
        else
        {
            _players[player_idx].mana_reserve = 30000;
        }
    }
    else
    {
        if(amount > 0)
        {
            _players[player_idx].mana_reserve = 30000;
        }
        else
        {
            _players[player_idx].mana_reserve += amount;
        }
    }
}


// WZD o55p36
// drake178: ¿ ?
// City_Enchantment_HelpIdx()

// WZD o55p37
// drake178: STR_GenitiveCase()
/*
; adds the possessive clitic suffix of the genitive
; case to a string based on whether it ends with the
; letter 's' (') or not ('s)
*/
/*
    add ' or 's
    posessive
*/
void Possessive(char * string)
{
    int16_t string_length;  // _DI_

    string_length = strlen(string);

    if(
        (string[(string_length - 1)] == 's')
        ||
        (string[(string_length - 1)] == 'S')
    )
    {
        strcat(string, str_Genitive_NoS);
    }
    else
    {
        strcat(string, str_Genitive_S);
    }

}


// WZD o55p38
// drake178: N/A
// sub_4D5EA()

// WZD o55p39
// drake178: N/A
// sub_4DA19()

// WZD o55p40
// drake178: N/A
// sub_4E507()
