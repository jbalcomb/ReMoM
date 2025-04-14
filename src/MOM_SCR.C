
#include "MOM_SCR.H"

#include "STU/STU_CHK.H"

#include "MOX/MOX_BASE.H"
#include "MOX/MOX_T4.H"  // ~ MOX & MOX2 //MoO2
#include "MOX/Fields.H"
#include "MOX/Fonts.H"
#include "MOX/Graphics.H"
#include "MOX/LOADSAVE.H"
#include "MOX/MOX_SET.H"  /* magic_set */
#include "MOX/Mouse.H"
#include "MOX/SOUND.H"
#include "MOX/Video.H"

#include "AdvsrScr.H"
#include "ArmyList.H"
#include "CityList.H"
#include "CityScr.H"
#include "Combat.H"
#include "DIPLOMAC.H"
#include "ItemScrn.H"
#include "LoadScr.H"
#include "MainMenu.H"
#include "MagicScr.H"
#include "MainScr.H"
#include "NEXTTURN.H"
#include "ProdScr.H"
#include "Roads.H"
#include "SBookScr.H"
#include "SCORE.H"      // Hall Of Fame (HoF)
#include "SPLMASTR.H"

#ifdef STU_DEBUG
#include "STU/STU_DBG.H"    /* DLOG() */
#endif



// WZD s01p02
// ~== MGC s01p03  Menu_Screen_Control()
// MoO2 Module: MOX2
void Screen_Control(void)
{
    int16_t prev__Settings_BG_Music;
    /*  BEGIN:  DEBUG */
    int16_t itr_lairs;
    int16_t lair_wp;
    struct s_LAIR * lair_ptr;
    /*  END:  DEBUG */
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

            Load_SAVE_GAM(-1);  // SAVETEST.GAM

            TST_Validate_GameData();
            
            Loaded_Game_Update();

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

Capture_Cities_Data();
            Load_Screen();
Check_Cities_Data();
Release_Cities_Data();

            GAME_SoM_Cast_By = ST_UNDEFINED;

            GAME_RazeCity = ST_FALSE;

            if(prev__Settings_BG_Music != magic_set.background_music)
            {
                if(magic_set.background_music == ST_TRUE)
                {
                    Play_Background_Music__STUB();
                }
                else
                {
                    Stop_Music__STUB();
                }
            }

        } break;

        case scr_New_Game_Screen:
        {
            // TODO  Newgame_Screen__STUB();
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
Capture_Cities_Data();
            City_Screen__WIP();
Check_Cities_Data();
Release_Cities_Data();
        } break;
        // scr_Load = 101,
        case scr_Armies_Screen: /* WZD 0x02 */
        {
Capture_Cities_Data();
            ArmyList_Screen();
Check_Cities_Data();
Release_Cities_Data();
        } break;
        case scr_Cities_Screen:  /* WZD 0x03 */
        {
Capture_Cities_Data();
            CityList_Screen();
Check_Cities_Data();
Release_Cities_Data();
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
            Cycle_Palette_Color__STUB(198, 40, 0, 0, 63, 0, 0, 1);
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

Capture_Cities_Data();
            Main_Screen();
Check_Cities_Data();
Release_Cities_Data();
            // MoO2  previous_screen = scr_Main_Screen
            previous_screen = scr_Main_Screen;
        } break;
        // scr_Magic = 106,
        case scr_Magic_Screen:
        {
Capture_Cities_Data();
            Magic_Screen();
Check_Cities_Data();
Release_Cities_Data();
        } break;
        case scr_Road_Build:
        {
Capture_Cities_Data();
            Road_Build_Screen();
Check_Cities_Data();
Release_Cities_Data();
        } break;
        case scr_Production_Screen:
        {
Capture_Cities_Data();
            Production_Screen();
Check_Cities_Data();
Release_Cities_Data();
        } break;

        case scr_Item_Screen:  /* 109 */
        {
Capture_Cities_Data();
            Item_Screen();
Check_Cities_Data();
Release_Cities_Data();
        } break;
        // scr_NextTurn = 110,
        case scr_NextTurn:
        {
Capture_Cities_Data();
            Next_Turn_Proc();
Check_Cities_Data();
Release_Cities_Data();
        } break;
        // /* ?default? */
        case scr_Spellbook_Screen:
        {
Capture_Cities_Data();
            Spellbook_Screen();
Check_Cities_Data();
Release_Cities_Data();
        } break;
        // /* ?default? */
        case scr_Advisor_Screen:
        {
Capture_Cities_Data();
            Advisor_Screen(ST_UNDEFINED);
Check_Cities_Data();
Release_Cities_Data();
        } break;

        case scr_Diplomacy_Screen:
        {
Capture_Cities_Data();
            Diplomacy_Screen__STUB();
Check_Cities_Data();
Release_Cities_Data();
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
