
#include "MoX.H"

#include "AdvsrScr.H"
#include "ArmyList.H"
#include "CityList.H"
#include "LoadScr.H"
#include "MainMenu.H"
#include "MainScr.H"
#include "MagicScr.H"

void TST_Load_SAVE_GAM(void);



int16_t current_screen;
int16_t previous_screen;



// WZD s01p02
// ~== MGC s01p03  Menu_Screen_Control()
// MoO2 Module: MOX2
void Screen_Control(void)
{
    int quit_flag;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Screen_Control()\n", __FILE__, __LINE__);
#endif

    Clear_Fields();
    Set_Mouse_List(1, mouse_list_default);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: current_screen: %d\n", __FILE__, __LINE__, current_screen);
#endif

    quit_flag = ST_FALSE;
    while(quit_flag == ST_FALSE)
    {

        switch(current_screen)
        {

            case scr_Main_Menu_Screen:
            {
                DLOG("switch(current_screen)  case scr_Main_Menu_Screen:");
                Load_Palette(2, -1, 0);
                Apply_Palette();
                // TODO  Main_Menu_Screen_Control();
                Main_Menu_Screen();
                // MoO2  previous_screen = scr_Main_Menu_Screen
                previous_screen = scr_Main_Menu_Screen;
            } break;

            case scr_Continue:
            {
                DLOG("switch(current_screen)  case scr_Continue:");
                // BEGIN: WZD main()
                // Load_SAVE_GAM(-1);  // SAVETEST.GAM
                // TST_Load_SAVE_GAM();
                Loaded_Game_Update();

                // HACK:  (4) Magicians  @  {18, 11}
                _UNITS[156].Enchants_HI = _UNITS[156].Enchants_HI = 0x8000;  // UE_Invisibility 0x8000

                // HACK:  visibility to support highlighting the Plane shift feature, for the public alpha demo release video
                int16_t itr_world_size;
                for(itr_world_size = 0; itr_world_size < WORLD_SIZE; itr_world_size++)
                {
                    TBL_Scouting[((1 * WORLD_SIZE) + itr_world_size)] = TBL_Scouting[((0 * WORLD_SIZE) + itr_world_size)];
                }

                // _unit_stack[]
                // _unit_stack_count
                // draw_active_stack_flag
#ifdef STU_DEBUG
        dbg_prn("DEBUG: [%s, %d]: all_units_moved: %d\n", __FILE__, __LINE__, all_units_moved);
        dbg_prn("DEBUG: [%s, %d]: draw_active_stack_flag: %d\n", __FILE__, __LINE__, draw_active_stack_flag);
        dbg_prn("DEBUG: [%s, %d]: _unit_stack_count: %d\n", __FILE__, __LINE__, _unit_stack_count);
        dbg_prn("DEBUG: [%s, %d]: _unit_stack[0].unit_idx: %d\n", __FILE__, __LINE__, _unit_stack[0].unit_idx);
        dbg_prn("DEBUG: [%s, %d]: _UNITS[_unit_stack[0].unit_idx].owner_idx: %d\n", __FILE__, __LINE__, _UNITS[_unit_stack[0].unit_idx].owner_idx);
#endif

                // END: WZD main()
                current_screen = scr_Main_Screen;
            } break;

            case scr_Load_Screen:
            {
                DLOG("switch(current_screen)  case scr_Load_Screen:");
                /*
                    WZD
                        GameState_01:                           ; case 0x1
                            prev__Settings_BG_Music = magic_set.BG_Music;
                            Load_Screen();
                */
                Load_Screen();
                /*
                    WZD
                        GAME_SoM_Cast_By = ST_UNDEFINED;
                        GAME_RazeCity = ST_FALSE;
                        if(prev__Settings_BG_Music != magic_set.BG_Music)
                        {
                            if(magic_set.BG_Music == ST_TRUE)
                            {
                                SND_PlayBkgrndTrack();
                            }
                            else
                            {
                                SND_Stop_Music();
                            }
                        }
                */
            } break;

            case scr_New_Game_Screen:
            {
                DLOG("switch(current_screen)  case scr_New_Game_Screen:");
                // New_Game_Screen();
            } break;
            
            case scr_Quit_To_DOS:
            {
                DLOG("switch(current_screen)  case scr_Quit_To_DOS:");
                // TODO  Auto_Save_Game()  ~== F-10 Quick_Save()
                quit_flag = ST_TRUE;
                // DELETE  g_State_Run = ST_FALSE;
                // Exit_With_Message("Thank you for playing Master of Magic!\n\n");
            } break;

            case scr_Hall_Of_Fame_Screen:
            {
                DLOG("switch(current_screen)  case scr_Hall_Of_Fame_Screen:");
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
                DLOG("switch(current_screen)  case scr_Settings_Screen:");
                // Settings_Screen();
            } break;

        // scr_City = 100,
        // scr_Load = 101,
        case scr_Armies_Screen: /* WZD 0x02 */
        {
            DLOG("switch(current_screen)  case scr_Armies_Screen:");
            ArmyList_Screen();
        } break;
        case scr_Cities_Screen:  /* WZD 0x03 */
        {
            DLOG("switch(current_screen)  case scr_Cities_Screen:");
            CityList_Screen();
        } break;
        // scr_Quit = 104,
            case scr_Main_Screen:
            {
                DLOG("switch(current_screen)  case scr_MainGame:");
                // BEGIN: WZD main()
                // Load_SAVE_GAM(8)
                // Load_WZD_Resources()
                Load_Palette(0, -1, 0);  // NOTE(JimBalcomb,20230111): this is the only Load_Palette() leading to the Main_Screen()
                // Calculate_Remap_Colors();
                // Set_Button_Down_Offsets(1, 1)
                // Cycle_Palette_Color(198, 40, 0, 0, 63, 0, 0, 1)
                Apply_Palette();
                // Fade_In()
                // Clear_Fields()
                // Loaded_Game_Update_WZD()
                // END: WZD main()

                // IDK  Set_Outline_Color(0);
                // IDK  Set_Alias_Color(0);
                // IDK  Set_Font_Style1(0, 0, 0, 0);

                Set_Page_Off();
                Fill(0, 0, 319, 199, 7);
                Set_Page_On();
                Fill(0, 0, 319, 199, 5);
                Set_Page_Off();

                Main_Screen();
                // MoO2  previous_screen = scr_Main_Screen
                previous_screen = scr_Main_Screen;
            } break;
            // scr_Magic = 106,
            case scr_Magic_Screen:
            {
                DLOG("switch(current_screen)  case scr_Magic_Screen:");
                Magic_Screen();
            } break;
            // scr_RoadBuilding = 107,
            // scr_Production = 108,
            // scr_Items = 109,
            // scr_NextTurn = 110,
            // /* ?default? */
            case scr_Spellbook_Screen:
            {
                DLOG("switch(current_screen)  case scr_Spellbook_Screen:");
                Spellbook_Screen();
            } break;
            // /* ?default? */
            case scr_Advisor_Screen:
            {
                DLOG("switch(current_screen)  case scr_Advisor_Screen:");
                Advisor_Screen(ST_UNDEFINED);
            } break;

            // scr_Diplomacy = 115

            case scr_Test_Screen:
            {
                DLOG("switch(current_screen)  case scr_Test_Screen:");
                // Test_Screen();
            } break;
            case scr_PoC_Screen:
            {
                DLOG("switch(current_screen)  case scr_PoC_Screen:");
                // PoC_Screen();
            } break;

        }  /* switch(current_screen) */

    }  /* while(quit_flag == ST_FALSE) */

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Screen_Control()\n", __FILE__, __LINE__);
#endif

}
