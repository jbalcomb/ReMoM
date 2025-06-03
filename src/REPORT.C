/*
    WIZARDS.EXE
        ovr126

    MoO2
        Module: REPORT

*/

#include "MOX/MOX_DAT.H"  /* _screen_seg */

#include "MainScr.H"
#include "MainScr_Maps.H"
#include "REPORT.H"
#include "SBookScr.H"
#include "UNITTYPE.H"



// WZD dseg:6292                                                 BEGIN:  ovr126 - Initialized Data

// WZD dseg:6292
char scroll_lbx_file__ovr126__1[] = "Scroll";
// WZD dseg:6299
char aTheOutpostOf[] = "The outpost of ";
// WZD dseg:62A9
char aHasBeenDeserte[] = " has been deserted.";
// WZD dseg:62BD
char aHasGrownIntoAH[] = " has grown into a hamlet.";
// WZD dseg:62D7
char scroll_lbx_file__ovr126__2[] = "scroll";
// WZD dseg:62DE
char str_hotkey_U__ovr126[] = "U";
// WZD dseg:62E0
char str_hotkey_D__ovr126[] = "D";
// WZD dseg:62E2
char str_hotkey_ESC__ovr126[] = "\x1B";
// WZD dseg:62E4
char str_RANDOM_EVENTS[] = "RANDOM EVENTS";
// WZD dseg:62F2
char str_Plague[] = "Plague in ";
// WZD dseg:62F8
char aIn[] = " in ";  // String-Optimizer
// WZD dseg:62FD
char aPopulationBoom[] = "Population Boom in ";
// WZD dseg:6311
char aGoodMoon[] = "Good Moon";
// WZD dseg:631B
char aBadMoon[] = "Bad Moon";
// WZD dseg:6324
char aRedConjunction[] = "Red Conjunction";
// WZD dseg:6334
char aGreenConjuncti[] = "Green Conjunction";
// WZD dseg:6346
char aBlueConjunctio[] = "Blue Conjunction";
// WZD dseg:6357
char aManaShort[] = "Mana Short";
// WZD dseg:6362
char aUnitsDisbanded[] = "UNITS DISBANDED";
// WZD dseg:6372
char aDesertedLackOf[] = " - deserted: lack of food.";
// WZD dseg:638D
char aDesertedLack_0[] = " - deserted: lack of gold.";
// WZD dseg:63A8
char aDispelledLackO[] = " - dispelled: lack of mana.";
// WZD dseg:63C4
char aDesertedToMake[] = " deserted to make room for new units";
// WZD dseg:63E9
char aDrownedBecause[] = " drowned because unit carrying it was destroyed.";
// WZD dseg:641A
char aUnitsKilled[] = "UNITS KILLED";
// WZD dseg:6427
char aKilledBy[] = " killed by ";
// WZD dseg:6433
char aCityGrowth[] = "CITY GROWTH";
// WZD dseg:643F
char aHasGrownToAPop[] = " has grown to a population of ";
// WZD dseg:645E
char aCityDeaths[] = "CITY DEATHS";
// WZD dseg:646A
char aNowHasAPopulat[] = " now has a population of ";
// WZD dseg:6484
char aDestroyedBui_0[] = "Destroyed Buildings";
// WZD dseg:6498
char aHasLostIts[] = " has lost its ";
// WZD dseg:64A7
char aUnitSpellsDiss[] = "UNIT SPELLS DISSIPATED";
// WZD dseg:64BE
char aCitySpellsDiss[] = "CITY SPELLS DISSIPATED";
// WZD dseg:64D5
char aGlobalSpellsDi[] = "GLOBAL SPELLS DISSIPATED";
// WZD dseg:64EE
char aNoEventsThisMo[] = "NO EVENTS THIS MONTH";

// WZD dseg:6503 00                                              align 2

// WZD dseg:6503                                                 END:  ovr126 - Initialized Data



// WZD dseg:C9CC                                                 BEGIN:  ovr126 - Uninitialized Data  (EoT Summary / Scroll Chancellor / Combat Results)

// WZD dseg:C9CC
SAMB_ptr _scroll_dn_arrow_seg = 0;

// WZD dseg:C9CE
SAMB_ptr _scroll_up_arrow_seg = 0;

// WZD dseg:C9D0
// MoM_Data  SAMB_ptr _scroll_paper_seg = 0;

// WZD dseg:C9D2
int16_t IDK_chancellor_screen_incr_mod_7 = 0;

// WZD dseg:C9D4
// MoM_Data  int16_t CMB_Scroll_MinHeight = 0;

// WZD dseg:C9D6
// MoM_Data  int16_t _scroll_text_height = 0;

// WZD dseg:C9D8
// drake178: G_CMB_NextScrollLineTop
// MoM_Data  int16_t _scroll_start_x = 0;

// WZD dseg:C9DA
int16_t m_report_scroll_text_height = 0;

// WZD dseg:C9DC
// MoM_Data  int16_t _scroll_text_top = 0;

// WZD dseg:C9DC                                                 END:  ovr126 - Uninitialized Data  (EoT Summary / Scroll Chancellor / Combat Results)




/*
    WIZARDS.EXE ovr126
*/

// WZD o126p01
void Chancellor_Screen_Draw(void)
{

    Copy_On_To_Off_Page();

    if(_scroll_text_top < 0)
    {
        FLIC_Draw(156, 13, _scroll_up_arrow_seg);
    }

    if((-(m_report_scroll_text_height) + 127) < _scroll_text_top)
    {
        FLIC_Draw(156, 172, _scroll_dn_arrow_seg);
    }

}

// WZD o126p02
void Chancellor_Screen_Load(void)
{
    Allocate_Reduced_Map();
    // SCROLL.LBX, 005  "SCRLPAPR"  ""
    // SCROLL.LBX, 000  "SCROLARR"  "up arrow"
    // SCROLL.LBX, 001  "SCROLARR"  "down arrow"
    _scroll_paper_seg    = LBX_Reload_Next(scroll_lbx_file__ovr126__1, 5, _screen_seg);
    _scroll_up_arrow_seg = LBX_Reload_Next(scroll_lbx_file__ovr126__1, 0, _screen_seg);
    _scroll_dn_arrow_seg = LBX_Reload_Next(scroll_lbx_file__ovr126__1, 1, _screen_seg);
}

// WZD o126p03
/*

XREF:
    j_Chancellor_Screen__WIP()
        Next_Turn_Proc__WIP()
        Advisor_Screen()

Next_Turn_Proc__WIP()
    Chancellor_Screen__WIP(0)

Advisor_Screen()
    Chancellor_Screen__WIP(1)

*/
void Chancellor_Screen__WIP(int16_t flag)
{
    char temp_string[LEN_TEMP_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t ongoing_event_count = 0;
    int16_t hotkey_D = 0;
    int16_t hotkey_U = 0;
    int16_t full_screen_ESC_field = 0;
    int16_t leave_screen = 0;
    int16_t input_field_idx = 0;
    int16_t total_message_count = 0;
    int16_t record_index = 0;
    int16_t arg_0 = 0;
    int16_t itr_msgs = 0;  // _SI_
    int16_t scroll_text_line_count = 0;  // _DI_

    scroll_text_line_count = 0;

    total_message_count = 0;

    ongoing_event_count = 0;

    _scroll_text_top = 0;

    IDK_chancellor_screen_incr_mod_7 = 0;

    Chancellor_Screen_Load();

    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);

    Full_Draw_Main_Screen();

    Copy_On_To_Off_Page();

    // Outpost Deserted
    for(itr_msgs = 0; MSG_CityLost_Count > itr_msgs; itr_msgs++)
    {

        strcpy(GUI_NearMsgString, aTheOutpostOf);  // "The outpost of "

        // ; BUG: incorrect array element size, should be 14
        // strcat(GUI_NearMsgString, (MSG_CityLost_Names + (itr_msgs * 20)));
        strcat(GUI_NearMsgString, &MSG_CityLost_Names[(itr_msgs * 20)]);
        // strcat(GUI_NearMsgString, MSG_CityLost_Names[itr_msgs]);

        strcat(GUI_NearMsgString, aHasBeenDeserte);  // " has been deserted."

        Warn0(GUI_NearMsgString);

    }

    // Outpost Grown
    for(itr_msgs = 0; MSG_CityGained_Count > itr_msgs; itr_msgs++)
    {

        strcpy(GUI_NearMsgString, aTheOutpostOf);  // "The outpost of "

        // String_Copy_Far(temp_string, _CITIES[MSG_CityGained_Array[itr_msgs]].name);
        strcpy(temp_string, _CITIES[MSG_CityGained_Array[itr_msgs]].name);

        strcat(GUI_NearMsgString, temp_string);

        strcat(GUI_NearMsgString, aHasGrownIntoAH);  // " has grown into a hamlet."

        Warn0(GUI_NearMsgString);

    }


    for(itr_msgs = 0; MSG_CityGrowth_Count > itr_msgs; itr_msgs++)
    {

        for(record_index = 0; MSG_CityDeath_Count > record_index; record_index++)
        {

            // same city_idx
            if(MSG_CityGrowth_Array[itr_msgs] == MSG_CityDeath_Array[record_index])
            {

                Clear_Structure(record_index, (uint8_t *)&MSG_CityDeath_Array[0], 1, MSG_CityDeath_Count);

                MSG_CityDeath_Count -= 1;

                record_index--;

            }

        }

    }

    total_message_count = (MSG_UnitLost_Count + MSG_CityGrowth_Count + MSG_BldLost_Count + MSG_UEsLost_Count + MSG_CEsLost_Count + MSG_UnitKilled_Count + MSG_GEs_Lost + MSG_CityDeath_Count);

    if(flag == 1)
    {

        if(
            (events_table->Plague_Status == 2)
            &&
            (events_table->Plague_Player == HUMAN_PLAYER_IDX)
        )
        {

            ongoing_event_count++;

        }

        if(
            (events_table->Population_Boom_Status == 2)
            &&
            (events_table->Population_Boom_Player == HUMAN_PLAYER_IDX)
        )
        {

            ongoing_event_count++;

        }

        if(events_table->Good_Moon_Status == 2)
        {

            ongoing_event_count++;

        }

        if(events_table->Bad_Moon_Status == 2)
        {

            ongoing_event_count++;

        }

        if(events_table->Conjunction_Chaos_Status == 2)
        {

            ongoing_event_count++;

        }

        if(events_table->Conjunction_Nature_Status == 2)
        {

            ongoing_event_count++;

        }

        if(events_table->Conjunction_Sorcery_Status == 2)
        {

            ongoing_event_count++;

        }

        if(events_table->Mana_Short_Status == 2)
        {

            ongoing_event_count++;

        }

        total_message_count += ongoing_event_count;

    }


    if(total_message_count == 0)
    {

        if(flag == 1)
        {

            scroll_text_line_count++;

        }
        else
        {

            return;

        }

    }


    if(ongoing_event_count > 0)
    {

        scroll_text_line_count++;

    }

    if(MSG_UnitLost_Count > 0)
    {

        scroll_text_line_count++;

    }

    if(MSG_UnitKilled_Count > 0)
    {

        scroll_text_line_count++;

    }

    if(MSG_CityGrowth_Count > 0)
    {

        scroll_text_line_count++;

    }

    if(MSG_CityDeath_Count > 0)
    {

        scroll_text_line_count++;

    }

    if(MSG_BldLost_Count > 0)
    {

        scroll_text_line_count++;

    }

    if(MSG_UEsLost_Count > 0)
    {

        scroll_text_line_count++;

    }

    if(MSG_CEsLost_Count > 0)
    {

        scroll_text_line_count++;

    }

    if(MSG_GEs_Lost > 0)
    {

        scroll_text_line_count++;

    }

    // ~ (report count * report line height) + (message count * message line height)
    _scroll_text_height = ((total_message_count * 7) + (scroll_text_line_count * 12));

    m_report_scroll_text_height = _scroll_text_height;

    SETMAX(_scroll_text_height, 127);

    _scroll_start_x = (2 + (99 - (_scroll_text_height / 2)));

    Clear_Fields();

    // SCROLL.LBX, 002      "SCROLOUT"  ""
    Open_File_Animation__HACK(scroll_lbx_file__ovr126__2, 2);

    // IDGI  for(itr_msgs = 0; ((_scroll_text_height + 7) / 8) >= itr_msgs; itr_msgs++)
    for (itr_msgs = 0; itr_msgs < ((_scroll_text_height + 7) / 8); itr_msgs++)
    {

        Set_Page_Off();

        Reset_Map_Draw();

        Main_Screen_Draw();

        Set_Window(0, (88 - (itr_msgs * 4)), 319, (108 + (itr_msgs * 4)));

        Clipped_Draw(62, 29, _scroll_paper_seg);

        Chancellor_Screen_Scroll_Draw__WIP(flag);

        Reset_Window();

        Draw_File_Animation__HACK();

        if(itr_msgs == 0)
        {

            Apply_Palette();

            VGA_MosaicFlip__STUB();

        }
        else
        {

            PageFlip_FX();

        }

    }

    Copy_On_To_Off_Page();

    Assign_Auto_Function(Chancellor_Screen_Draw, 1);

    leave_screen = ST_FALSE;

    if(m_report_scroll_text_height > 127)
    {

        hotkey_U = Add_Hidden_Field(28, 9, 288, 25, str_hotkey_U__ovr126[0], ST_UNDEFINED);

        hotkey_D = Add_Hidden_Field(28, 170, 288, 190, str_hotkey_D__ovr126[0], ST_UNDEFINED);

    }
    else
    {

        hotkey_U = INVALID_FIELD;

        hotkey_D = INVALID_FIELD;

    }

    full_screen_ESC_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, str_hotkey_ESC__ovr126[0], ST_UNDEFINED);

    while(leave_screen == ST_FALSE)
    {

        input_field_idx = Get_Input();


        if(input_field_idx == full_screen_ESC_field)
        {

            leave_screen = ST_TRUE;

        }

        if(input_field_idx == hotkey_U)
        {

            // TODO
            // TODO
            // TODO

        }

        if(input_field_idx == hotkey_D)
        {

            // TODO
            // TODO
            // TODO

        }

        if(leave_screen == ST_FALSE)
        {

            Set_Page_Off();

            Chancellor_Screen_Draw();

            PageFlip_FX();

        }

    }

    Deactivate_Auto_Function();

    Release_Block(_screen_seg);

    Full_Draw_Main_Screen();

    Copy_On_To_Off_Page();

}

// WZD o126p04
void Chancellor_Screen_Scroll_Draw__WIP(int16_t flag)
{
    char temp_string[LEN_TEMP_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t ongoing_event_count = 0;
    int16_t x = 0;
    uint8_t colors2[6] = { 0, 0, 0, 0, 0, 0};
    uint8_t colors1[6] = { 0, 0, 0, 0, 0, 0};
    int16_t var_2 = 0;
    int16_t x_add = 0;  // _SI_
    int16_t itr = 0;  // _DI_

    ongoing_event_count = 0;

    /*
        Plague
        Population Boom
        Good Moon
        Bad Moon
        Conjuction - Chaos
        Conjuction - Nature
        Conjuction - Sorcery
        Mana Short
    */
    if(flag != 0)
    {

        if(
            (events_table->Plague_Status == 2)
            &&
            (events_table->Plague_Player == HUMAN_PLAYER_IDX)
        )
        {
            ongoing_event_count++;
        }

        if(
            (events_table->Population_Boom_Status == 2)
            &&
            (events_table->Population_Boom_Player == HUMAN_PLAYER_IDX)
        )
        {
            ongoing_event_count++;
        }

        if(events_table->Good_Moon_Status == 2)
        {
            ongoing_event_count++;
        }

        if(events_table->Bad_Moon_Status == 2)
        {
            ongoing_event_count++;
        }

        if(events_table->Conjunction_Chaos_Status == 2)
        {
            ongoing_event_count++;
        }

        if(events_table->Conjunction_Nature_Status == 2)
        {
            ongoing_event_count++;
        }

        if(events_table->Conjunction_Sorcery_Status == 2)
        {
            ongoing_event_count++;
        }

        if(events_table->Mana_Short_Status == 2)
        {
            ongoing_event_count++;
        }

    }


    for(itr = 0; itr < 5; itr++)
    {
        colors1[itr] = 47;
    }

    for(itr = 0; itr < 5; itr++)
    {
        colors2[itr] = 55;
    }

    colors2[0] = 247;
    colors1[0] = 240;

    x_add = 0;


    if(ongoing_event_count > 0)
    {
        Set_Font_Colors_15(4, &colors1[0]);

        Clipped_Print_Centered(165, (_scroll_start_x + _scroll_text_top + x_add), str_RANDOM_EVENTS);

        x_add += 11;

        Set_Font_Colors_15(1, &colors2[0]);

        Set_Font_Spacing_Width(2);

        /*
            Plague
            Population Boom
            Good Moon
            Bad Moon
            Conjuction - Chaos
            Conjuction - Nature
            Conjuction - Sorcery
            Mana Short
        */

        if(
            (events_table->Plague_Status == 2)
            &&
            (events_table->Plague_Player == HUMAN_PLAYER_IDX)
        )
        {

            x = Clipped_Print(70, (_scroll_start_x + _scroll_text_top + x_add), str_Plague);

            Clipped_Print_Far(x, (_scroll_start_x + _scroll_text_top + x_add), _CITIES[events_table->Plague_Data].name);

            x_add += 7;
        }


        if(
            (events_table->Population_Boom_Status == 2)
            &&
            (events_table->Population_Boom_Player == HUMAN_PLAYER_IDX)
        )
        {

            x = Clipped_Print(70, (_scroll_start_x + _scroll_text_top + x_add), aPopulationBoom);

            Clipped_Print_Far(x, (_scroll_start_x + _scroll_text_top + x_add), _CITIES[events_table->Population_Boom_Data].name);

            x_add += 7;
        }


        if(events_table->Good_Moon_Status == 2)
        {

            x = Clipped_Print(70, (_scroll_start_x + _scroll_text_top + x_add), aGoodMoon);

            x_add += 7;
        }

        if(events_table->Bad_Moon_Status == 2)
        {

            x = Clipped_Print(70, (_scroll_start_x + _scroll_text_top + x_add), aBadMoon);

            x_add += 7;
        }

        if(events_table->Conjunction_Chaos_Status == 2)
        {

            x = Clipped_Print(70, (_scroll_start_x + _scroll_text_top + x_add), aRedConjunction);

            x_add += 7;
        }

        if(events_table->Conjunction_Nature_Status == 2)
        {

            x = Clipped_Print(70, (_scroll_start_x + _scroll_text_top + x_add), aGreenConjuncti);

            x_add += 7;
        }

        if(events_table->Conjunction_Sorcery_Status == 2)
        {

            x = Clipped_Print(70, (_scroll_start_x + _scroll_text_top + x_add), aBlueConjunctio);

            x_add += 7;
        }

        if(events_table->Mana_Short_Status == 2)
        {

            x = Clipped_Print(70, (_scroll_start_x + _scroll_text_top + x_add), aManaShort);

            x_add += 7;
        }

        x_add += 2;

    }


    /*
        MSG_UnitLost_Count
        MSG_UnitKilled_Count
        MSG_CityGrowth_Count
        MSG_CityDeath_Count
        MSG_BldLost_Count
        MSG_UEsLost_Count
        MSG_CEsLost_Count
        MSG_GEs_Lost
    */


    if(MSG_UnitLost_Count > 0)
    {
        Set_Font_Colors_15(4, &colors1[0]);

        Clipped_Print_Centered(165, (_scroll_start_x + _scroll_text_top + x_add), aUnitsDisbanded);

        x_add += 11;

        Set_Font_Colors_15(1, &colors2[0]);

        Set_Font_Spacing_Width(2);

        for(itr = 0; MSG_UnitLost_Count > itr; itr++)
        {

            x = Clipped_Print(70, (_scroll_start_x + _scroll_text_top + x_add), *_unit_type_table[MSG_UnitLost_Array[itr].Unit_Type].name);

            switch(MSG_UnitLost_Array[itr].Cause)
            {
                case 0:
                {
                    Clipped_Print(x, (_scroll_start_x + _scroll_text_top + x_add), aDesertedLackOf);
                } break;
                case 1:
                {
                    Clipped_Print(x, (_scroll_start_x + _scroll_text_top + x_add), aDesertedLack_0);
                } break;
                case 2:
                {
                    Clipped_Print(x, (_scroll_start_x + _scroll_text_top + x_add), aDispelledLackO);
                } break;
                case 3:
                {
                    Clipped_Print(x, (_scroll_start_x + _scroll_text_top + x_add), aDesertedToMake);
                } break;
                case 4:
                {
                    Clipped_Print(x, (_scroll_start_x + _scroll_text_top + x_add), aDrownedBecause);
                } break;
            }

            x_add += 7;
        }

        x_add += 2;
    }
    
    if(MSG_UnitKilled_Count > 0)
    {
        Set_Font_Colors_15(4, &colors1[0]);

        Clipped_Print_Centered(165, (_scroll_start_x + _scroll_text_top + x_add), aUnitsKilled);

        x_add += 11;

        Set_Font_Colors_15(1, &colors2[0]);

        Set_Font_Spacing_Width(2);

        for(itr = 0; MSG_UnitKilled_Count > itr; itr++)
        {

            if(MSG_UnitKilled_Array[itr].Unit_Type <= ut_Chosen)
            {
                _fstrcpy(GUI_NearMsgString, hero_names_table[MSG_UnitKilled_Array[itr].Unit_Type].name);
            }
            else
            {
                strcpy(GUI_NearMsgString, *_unit_type_table[MSG_UnitKilled_Array[itr].Unit_Type].name);
            }

            strcat(GUI_NearMsgString, aKilledBy);

            _fstrcpy(temp_string, spell_data_table[MSG_UnitKilled_Array[itr].Spell].name);

            strcat(GUI_NearMsgString, temp_string);

            if(MSG_UnitKilled_Array[itr].City > 0)
            {

                strcat(GUI_NearMsgString, aIn);

                _fstrcpy(temp_string, _CITIES[MSG_UnitKilled_Array[itr].City].name);

                strcat(GUI_NearMsgString, temp_string);

            }

            Clipped_Print(70, (_scroll_start_x + _scroll_text_top + x_add), GUI_NearMsgString);

            x_add += 7;

        }

        x_add += 2;
    }
    
    if(MSG_CityGrowth_Count > 0)
    {
        Set_Font_Colors_15(4, &colors1[0]);

        Clipped_Print_Centered(165, (_scroll_start_x + _scroll_text_top + x_add), aCityGrowth);

        x_add += 11;

        Set_Font_Colors_15(1, &colors2[0]);

        Set_Font_Spacing_Width(2);

        for(itr = 0; MSG_CityGrowth_Count > itr; itr++)
        {

            var_2 = Clipped_Print_Far(70, (_scroll_start_x + _scroll_text_top + x_add), _CITIES[MSG_CityGrowth_Array[itr]].name);

            var_2 = Clipped_Print((var_2 + 1), (_scroll_start_x + _scroll_text_top + x_add), aHasGrownToAPop);

            Clipped_Print_Integer((var_2 + 1), (_scroll_start_x + _scroll_text_top + x_add), _CITIES[MSG_CityGrowth_Array[itr]].population);

            x_add += 7;

        }

        x_add += 2;
    }
    
    if(MSG_CityDeath_Count > 0)
    {
        Set_Font_Colors_15(4, &colors1[0]);

        Clipped_Print_Centered(165, (_scroll_start_x + _scroll_text_top + x_add), aCityDeaths);

        x_add += 11;

        Set_Font_Colors_15(1, &colors2[0]);

        Set_Font_Spacing_Width(2);

        for(itr = 0; MSG_CityDeath_Count > itr; itr++)
        {

            var_2 = Clipped_Print_Far(70, (_scroll_start_x + _scroll_text_top + x_add), _CITIES[MSG_CityDeath_Array[itr]].name);

            var_2 = Clipped_Print((var_2 + 1), (_scroll_start_x + _scroll_text_top + x_add), aNowHasAPopulat);

            Clipped_Print_Integer((var_2 + 1), (_scroll_start_x + _scroll_text_top + x_add), _CITIES[MSG_CityDeath_Array[itr]].population);

            x_add += 7;

        }

        x_add += 2;
    }
    
    if(MSG_BldLost_Count > 0)
    {
        Set_Font_Colors_15(4, &colors1[0]);

        Clipped_Print_Centered(165, (_scroll_start_x + _scroll_text_top + x_add), aUnitsKilled);

        x_add += 11;

        Set_Font_Colors_15(1, &colors2[0]);

        Set_Font_Spacing_Width(2);

        for(itr = 0; MSG_BldLost_Count > itr; itr++)
        {

            var_2 = Clipped_Print_Far(70, (_scroll_start_x + _scroll_text_top + x_add), _CITIES[MSG_BldLost_Array[itr].city_idx].name);

            var_2 = Clipped_Print((var_2 + 1), (_scroll_start_x + _scroll_text_top + x_add), aHasLostIts);

            var_2 = Clipped_Print_Far((var_2 + 1), (_scroll_start_x + _scroll_text_top + x_add), bldg_data_table[MSG_BldLost_Array[itr].bldg_type_idx].name);

            x_add += 7;

        }

        x_add += 2;
    }
    
    if(MSG_UEsLost_Count > 0)
    {
        Set_Font_Colors_15(4, &colors1[0]);

        Clipped_Print_Centered(165, (_scroll_start_x + _scroll_text_top + x_add), aUnitSpellsDiss);

        x_add += 11;

        Set_Font_Colors_15(1, &colors2[0]);

        Set_Font_Spacing_Width(2);

        for(itr = 0; MSG_UEsLost_Count > itr; itr++)
        {

            var_2 = Clipped_Print((var_2 + 1), (_scroll_start_x + _scroll_text_top + x_add), *_unit_type_table[MSG_UEsLost_Array[itr]].name);

            x_add += 7;

        }

        x_add += 2;
    }
    
    if(MSG_CEsLost_Count > 0)
    {
        Set_Font_Colors_15(4, &colors1[0]);

        Clipped_Print_Centered(165, (_scroll_start_x + _scroll_text_top + x_add), aCitySpellsDiss);

        x_add += 11;

        Set_Font_Colors_15(1, &colors2[0]);

        Set_Font_Spacing_Width(2);

        for(itr = 0; MSG_CEsLost_Count > itr; itr++)
        {

            var_2 = Clipped_Print((var_2 + 1), (_scroll_start_x + _scroll_text_top + x_add), _CITIES[MSG_CEsLost_Array[itr]].name);
            
            x_add += 7;

        }

        x_add += 2;
    }

    // ¿ BUGBUG:  no loop / no array ?
    if(MSG_GEs_Lost > 0)
    {
        Set_Font_Colors_15(4, &colors1[0]);

        Clipped_Print_Centered(158, (_scroll_start_x + _scroll_text_top + x_add), aGlobalSpellsDi);

        x_add += 13;  // Coincidence? matches the 11 + 2 used everywhere else?
    }


    if(
        (x_add == 0)
        &&
        (flag == ST_TRUE)
    )
    {

        Set_Font_Colors_15(4, &colors1[0]);

        Clipped_Print_Centered(158, (_scroll_start_x + _scroll_text_top + x_add), aNoEventsThisMo);

    }


    Reset_Window();

}
