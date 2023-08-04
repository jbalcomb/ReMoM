
#include "MoX.H"

#include "LoadScr.H"
#include "MainMenu.H"
#include "MainScr.H"

void TST_Load_SAVE_GAM(void);



int16_t current_screen;



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
                DLOG("case scr_Main_Menu_Screen:");
                Load_Palette(2, -1, 0);
                Apply_Palette();
                // TODO  Main_Menu_Screen_Control();
                Main_Menu_Screen();
                // MoO2  previous_screen = scr_Main_Menu_Screen
            } break;

            case scr_Continue:
            {
                DLOG("case scr_Continue:");
                // BEGIN: WZD main()
                Load_SAVE_GAM(-1);  // SAVETEST.GAM
                // TST_Load_SAVE_GAM();
                Loaded_Game_Update();
                // END: WZD main()
                current_screen = scr_Main_Screen;
            } break;

            case scr_Load_Screen:
            {
                DLOG("case scr_Load_Screen:");
                // TODO  Load_Screen();
            } break;

            case scr_New_Game_Screen:
            {
                DLOG("case scr_New_Game_Screen:");
                // New_Game_Screen();
            } break;
            
            case scr_Quit_To_DOS:
            {
                DLOG("case scr_Quit_To_DOS:");
                // TODO  Auto_Save_Game()  ~== F-10 Quick_Save()
                quit_flag = ST_TRUE;
                // DELETE  g_State_Run = ST_FALSE;
                // Exit_With_Message("Thank you for playing Master of Magic!\n\n");
            } break;

            case scr_Hall_Of_Fame_Screen:
            {
                DLOG("case scr_Hall_Of_Fame_Screen:");
                // Hall_Of_Fame_Screen();
            } break;

            case scr_Settings_Screen:
            {
                DLOG("case scr_Settings_Screen:");
                // Settings_Screen();
            } break;

        // scr_City = 100,
        // scr_Load = 101,
        // scr_Armies = 102,
        // scr_Cities = 103,
        // scr_Quit = 104,
            case scr_Main_Screen:
            {
                DLOG("case scr_MainGame:");
                // BEGIN: WZD main()
                // Load_SAVE_GAM(8)
                // Load_WZD_Resources()
                Load_Palette(0, -1, 0);  // NOTE(JimBalcomb,20230111): this is the only Load_Palette() leading to the Main_Screen()
                // VGA_SetShades_Grey0()
                // Set_Button_Down_Offsets(1, 1)
                // Cycle_Palette_Color(198, 40, 0, 0, 63, 0, 0, 1)
                Apply_Palette();
                // Fade_In()
                // Clear_Fields()
                // Loaded_Game_Update_WZD()
                // END: WZD main()

                Set_Outline_Color(0);
                Set_Alias_Color(0);
                Set_Font_Style1(0, 0, 0, 0);

                Set_Page_Off();
                Fill(0, 0, 319, 199, 7);
                Set_Page_On();
                Fill(0, 0, 319, 199, 5);
                Set_Page_Off();

                Main_Screen();
                // MoO2  previous_screen = scr_Main_Screen
            } break;
            // scr_Magic = 106,
            // scr_RoadBuilding = 107,
            // scr_Production = 108,
            // scr_Items = 109,
            // scr_NextTurn = 110,
            // /* ?default? */
            // scr_Spellbook = 112,
            // /* ?default? */
            // scr_Advisor = 114,
            // scr_Diplomac = 115

            case scr_Test_Screen:
            {
                DLOG("case scr_Test_Screen:");
                // Test_Screen();
            } break;
            case scr_PoC_Screen:
            {
                DLOG("case scr_PoC_Screen:");
                // PoC_Screen();
            } break;

        }  /* switch(current_screen) */

    }  /* while(quit_flag == ST_FALSE) */

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Screen_Control()\n", __FILE__, __LINE__);
#endif

}
