
#include "MoX.H"

#include "AdvsrScr.H"
#include "ArmyList.H"
#include "CityList.H"
#include "CityScr.H"
#include "LoadScr.H"
#include "MainMenu.H"
#include "MainScr.H"
#include "MagicScr.H"
#include "SBookScr.H"

void TST_Load_SAVE_GAM(void);



// MoX.C  int16_t current_screen;
// MoX.C  int16_t previous_screen;



// WZD s01p02
// ~== MGC s01p03  Menu_Screen_Control()
// MoO2 Module: MOX2
void Screen_Control(void)
{
    int16_t prev__Settings_BG_Music;

    int quit_flag;

    quit_flag = ST_FALSE;

    while(quit_flag == ST_FALSE)
    {

        Clear_Fields();

        Set_Mouse_List(1, mouse_list_default);

        switch (current_screen)
        {

        case scr_Main_Menu_Screen:
        {
            Load_Palette(2, -1, 0);
            Apply_Palette();
            // TODO  Main_Menu_Screen_Control();
            Main_Menu_Screen();
            // MoO2  previous_screen = scr_Main_Menu_Screen
            previous_screen = scr_Main_Menu_Screen;
        } break;

        case scr_Continue:
        {
            // HACK:
            magic_set.Strategic_Combat = ST_TRUE;  // "Strategic Combat Only"

            // BEGIN: WZD main()
            Load_SAVE_GAM(-1);  // SAVETEST.GAM
            // TST_Load_SAVE_GAM();
            Loaded_Game_Update();


            // HACK:  (4) Magicians  @  {18, 11}
            _UNITS[156].enchantments |= UE_INVISIBILITY;

            // HACK:  (1) Pikemen  @ {,}
            _UNITS[808].enchantments |= UE_BLESS;

            // HACK: set Cantebury up to complete the 'Miners Guild' on the next 'Next Turn'
            _CITIES[54].Prod_Accu = 280;

            // HACK:  visibility to support highlighting the Plane shift feature, for the public alpha demo release video
            int16_t itr_world_size;
            for (itr_world_size = 0; itr_world_size < WORLD_SIZE; itr_world_size++)
            {
                TBL_Scouting[((1 * WORLD_SIZE) + itr_world_size)] = TBL_Scouting[((0 * WORLD_SIZE) + itr_world_size)];
            }

            // HACK:  move Stack to Lair
            _UNITS[110].wx = 5;
            _UNITS[110].wy = 21;
            _UNITS[110].Status = us_Ready;
            _UNITS[110].moves2 = 2;
            _UNITS[418].wx = 5;
            _UNITS[418].wy = 21;
            _UNITS[418].Status = us_Ready;
            _UNITS[418].moves2 = 2;
            _UNITS[534].wx = 5;
            _UNITS[534].wy = 21;
            _UNITS[534].Status = us_Ready;
            _UNITS[534].moves2 = 2;
            _UNITS[669].wx = 5;
            _UNITS[669].wy = 21;
            _UNITS[669].Status = us_Ready;
            _UNITS[669].moves2 = 2;
            // Stack not being display until after a Next-Turn
            // ¿ not getting an entity_idx in Draw_Map_Units() ?
            // No-Workie  Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
            
            // HACK:  demo "You can only sell back one building each turn."
            _CITIES[_city_idx].did_sell_building = ST_TRUE;

            // HACK:  trigger 'Event'
            // // DBG_trigger_event = STU_TRUE;
            // DBG_trigger_event = ST_TRUE;
            // HACK:  trigger 'Offer'
            DBG_trigger_offer_item = ST_TRUE;
            DBG_trigger_offer_merc = ST_TRUE;
            DBG_trigger_offer_hero = ST_TRUE;

            // // _map_x = ( 5 - (12 / 2));
            // // _map_y = (21 - (10 / 2));
            // _map_x = 59;
            // _map_y = 16;
            // // ...
            // int16_t entity_idx = 110;
            // // ...
            // _unit = entity_idx;
            // unit_idx = _unit;
            // Unit_X = _UNITS[unit_idx].wx;
            // Unit_Y = _UNITS[unit_idx].wy;
            // _active_world_x = _UNITS[unit_idx].wx;
            // _active_world_y = _UNITS[unit_idx].wy;
            // Select_Unit_Stack(_human_player_idx, &_map_x, &_map_y, _map_plane, Unit_X, Unit_Y);
            // Reset_Draw_Active_Stack();
            // draw_active_stack_flag = 0;
            // Set_Unit_Draw_Priority();
            // Reset_Stack_Draw_Priority();
            // Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
            // Set_Mouse_List_Default();
            // Reset_Map_Draw();


            // END: WZD main()
            current_screen = scr_Main_Screen;
        } break;

        case scr_Load_Screen:
        {
            // TODO  WZD vs. MGC
            /*
                WZD
                    GameState_01:                           ; case 0x1
            */
            prev__Settings_BG_Music = magic_set.background_music;

            Load_Screen();

            GAME_SoM_Cast_By = ST_UNDEFINED;

            GAME_RazeCity = ST_FALSE;

            if(prev__Settings_BG_Music != magic_set.background_music)
            {
                if(magic_set.background_music == ST_TRUE)
                {
                    Play_Background_Music();
                }
                else
                {
                    Stop_Music__STUB();
                }
            }

        } break;

        case scr_New_Game_Screen:
        {
            // TODO  New_Game_Screen();
        } break;

        case scr_Quit_To_DOS:
        {
            // TODO  Auto_Save_Game()  ~== F-10 Quick_Save()
            quit_flag = ST_TRUE;
            // TODO  Exit_With_Message("Thank you for playing Master of Magic!\n\n");
        } break;

        case scr_Hall_Of_Fame_Screen:
        {
            /*
                MGC
                    switch(main_menu_selection)
                        case fid4_HallOfFame:                          ; case 0x4
                        j_GAME_Hall_of_Fame()
                        j_Init_Credits()
            */
            Hall_Of_Fame_Screen();
            // TODO  loop back to Menu_Screen_Control()
            current_screen = scr_Main_Menu_Screen;
        } break;

        case scr_Settings_Screen:
        {
            // TODO  Settings_Screen();
            /* DEMO */  current_screen = scr_Main_Screen;
        } break;

        // scr_City = 100,
        case scr_City_Screen: /* WZD 0x00 */
        {
            City_Screen__WIP();
        } break;
        // scr_Load = 101,
        case scr_Armies_Screen: /* WZD 0x02 */
        {
            ArmyList_Screen();
        } break;
        case scr_Cities_Screen:  /* WZD 0x03 */
        {
            CityList_Screen();
        } break;
        // scr_Quit = 104,
        case scr_Main_Screen:
        {
            // BEGIN: WZD main()
            // Load_SAVE_GAM(8)
            // Load_WZD_Resources()
            Load_Palette(0, -1, 0);  // NOTE(JimBalcomb,20230111): this is the only Load_Palette() leading to the Main_Screen()
            // Calculate_Remap_Colors();
            Set_Button_Down_Offsets(1, 1);
            // Cycle_Palette_Color(198, 40, 0, 0, 63, 0, 0, 1)
            Apply_Palette();
            // Fade_In()
            // Clear_Fields()
            // Loaded_Game_Update_WZD()
            // END: WZD main()

            // IDK  Set_Outline_Color(0);
            // IDK  Set_Alias_Color(0);
            // IDK  Set_Font_Style_Shadow_Down(0, 0, 0, 0);

            Set_Page_Off();
            Fill(0, 0, SCREEN_XMAX, SCREEN_YMAX, 7);
            Set_Page_On();
            Fill(0, 0, SCREEN_XMAX, SCREEN_YMAX, 5);
            Set_Page_Off();

            Main_Screen();
            // MoO2  previous_screen = scr_Main_Screen
            previous_screen = scr_Main_Screen;
        } break;
        // scr_Magic = 106,
        case scr_Magic_Screen:
        {
            Magic_Screen();
        } break;
        case scr_Road_Build:
        {
        Road_Build_Screen();
        } break;
        case scr_Production_Screen:
        {
            Production_Screen();
        } break;

        case scr_Item_Screen:  /* 109 */
        {
            Item_Screen();
        } break;
        // scr_NextTurn = 110,
        case scr_NextTurn:
        {
            Next_Turn_Proc();
        } break;
        // /* ?default? */
        case scr_Spellbook_Screen:
        {
            Spellbook_Screen();
        } break;
        // /* ?default? */
        case scr_Advisor_Screen:
        {
            Advisor_Screen(ST_UNDEFINED);
        } break;

        case scr_Diplomacy_Screen:
        {
            Diplomacy_Screen__STUB();
        } break;

        case scr_Test_Screen:
        {
            // Test_Screen();
        } break;
        case scr_PoC_Screen:
        {
            // PoC_Screen();
        } break;

        }  /* switch(current_screen) */

    }  /* while(quit_flag == ST_FALSE) */

}
