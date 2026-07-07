
// #ifdef STU_DEBUG
#include "../../STU/src/STU_DBG.h"
#include "../../STU/src/STU_LOG.h"
#include "../../STU/src/STU_PRN.h"
// #endif

#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_BASE.h"
#include "../../MoX/src/MOX_DAT.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_T4.h"  // ~ MOX & MOX2 //MoO2
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/LOADSAVE.h"
#include "../../MoX/src/MOX_SET.h"  /* magic_set */
#include "../../MoX/src/Mouse.h"
#include "../../MoX/src/SOUND.h"
#include "../../MoX/src/Video.h"

#include "AdvsrScr.h"
#include "ArmyList.h"
#include "CITYCALC.h"   /* Player_Resource_Income_Total() */
#include "CityList.h"
#include "CityScr.h"
#include "Combat.h"
#include "INITGAME.h"  /* gd_dump_cities (post-load GD capture) */
#include "DIPLOMAC.h"
#include "ItemScrn.h"
#include "LOADER.h"
#include "LoadScr.h"
#include "MainMenu.h"
#include "MagicScr.h"
#include "MainScr.h"
#include "NewGame.h"
#include "NEXTTURN.h"
#include "ProdScr.h"
#include "Roads.h"
#include "SBookScr.h"
#include "SCORE.h"      // Hall Of Fame (HoF)
#include "SPLMASTR.h"

#include "../../ext/stu_compat.h"

#include <stdio.h>

#include "../../platform/include/Platform.h"  /* CLAUDE: MOUSE_LOG for screen transition timestamps */

#include "MOM_SCR.h"



// WZD s01p02
// ~== MGC s01p03  Menu_Screen_Control()
// MoO2 Module: MOX2
/*

main_menu_selection = Main_Menu_Screen()
switch(main_menu_selection)
    0 case scr_Continue
    1 case scr_LoadGame
    2 case scr_NewGame
    3 case scr_QuitToDos
    4 case scr_HallOfFame

*/
void Screen_Control(void)
{
    int16_t prev__Settings_BG_Music = 0;
    /*  BEGIN:  DEBUG */
    int16_t itr_lairs = 0;
    int16_t lair_wp = 0;
    struct s_LAIR * lair_ptr = 0;
    /*  END:  DEBUG */
    int quit_flag = 0;
    int16_t DBG_player_idx = 0;
    int16_t DBG_gold_total = 0;
    int16_t DBG_food_total = 0;
    int16_t DBG_mana_total = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    quit_flag = ST_FALSE;

    while(quit_flag == ST_FALSE)
    {

        Clear_Fields();

        Set_Mouse_List(1, mouse_list_default);

        switch(current_screen)
        {

            case scr_Main_Menu_Screen:
            {
#ifdef MOUSE_DEBUG
                MOUSE_LOG("SCR t=%llu ENTER screen=Main_Menu\n", (unsigned long long)Platform_Get_Millies());
#endif
#ifdef STU_DEBUG
                LOG_TRACE(LOG_CAT_GENERAL, "[SCR] ENTER scr_Main_Menu_Screen");
                LOG_DEBUG(LOG_CAT_GENERAL, "[SCR] ENTER scr_Main_Menu_Screen");
#endif
                Load_Palette(2, -1, 0);
                Apply_Palette();
                // TODO  Main_Menu_Screen_Control();
                Main_Menu_Screen();
                // MoO2  previous_screen = scr_Main_Menu_Screen
                previous_screen = scr_Main_Menu_Screen;
#ifdef MOUSE_DEBUG
                MOUSE_LOG("SCR t=%llu LEAVE screen=Main_Menu next=%d\n", (unsigned long long)Platform_Get_Millies(), current_screen);
#endif
#ifdef STU_DEBUG
                LOG_TRACE(LOG_CAT_GENERAL, "[SCR] LEAVE scr_Main_Menu_Screen -> next=%d", current_screen);
                LOG_DEBUG(LOG_CAT_GENERAL, "[SCR] LEAVE scr_Main_Menu_Screen -> next=%d", current_screen);
#endif
            } break;

            case scr_Continue:
            {
#ifdef STU_DEBUG
                LOG_INFO(LOG_CAT_MOM_SCR, "DEBUG: [%s, %d]: Screen_Control(): swicth(): scr_Continue", __FILE__, __LINE__);
                LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: Screen_Control(): swicth(): scr_Continue", __FILE__, __LINE__);
                LOG_TRACE(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: Screen_Control(): swicth(): scr_Continue", __FILE__, __LINE__);
#endif
#ifdef MOUSE_DEBUG
                MOUSE_LOG("SCR t=%llu ENTER screen=Continue\n", (unsigned long long)Platform_Get_Millies());
#endif

    // WZD Load_Palette(0, ST_UNDEFINED);
    // WZD Calculate_Remap_Colors();
    // WZD Set_Button_Down_Offsets(1, 1,);
    // WZD Cycle_Palette_Color(198, 40, 0, 0, 63, 0, 0, 1);  // (color_num, red_min, green_min, blue_min, red_max, green_max, blue_max, step_value)
    // WZD Apply_Palette();
    // WZD Fade_In();
    // WZD current_screen = scr_Main_Screen
    // WZD _players[NEUTRAL_PLAYER_IDX].banner_id = 5;  // enum Banner_Color {BNR_Brown = 5 }
    // WZD Clear_Fields();
    // WZD Loaded_Game_Update_WZD();
    // WZD GAME_SoM_Cast_By = ST_UNDEFINED;

                // MGC  fid0_Continue:                            ; case 0x0
                // MGC  Stop_Music__STUB()
                // MGC  j_GAME_WizardsLaunch__WIP(e_SAVE9GAM)
                // /* HACK */ Load_SAVE_GAM(-1);  // SAVETEST.GAM
                /* HACK */  Load_WZD_Resources();
                Load_SAVE_GAM(8);

                /* 300-series: snapshot every save-loaded array right after Load_SAVE_GAM
                 * (matches OG's gd_wzd_landmark_probe @ Load_SAVE_GAM on-return). Numbered
                 * in save-load order; gaps (303/304/306/312/313) are Phase 2. */
                gd_dump_heroes          ("301_Load_Heroes_H");
                gd_dump_players         ("302_Load_Players_P");
                gd_dump_world_map       ("303_Load_World_Maps_W");
                gd_dump_landmasses      ("304_Load_Landmasses_L");
                gd_dump_nodes           ("305_Load_Nodes_N");
                gd_dump_fortresses      ("306_Load_Fortresses_F");
                gd_dump_towers          ("307_Load_Towers_T");
                gd_dump_lairs           ("308_Load_Lairs_L");
                gd_dump_items           ("309_Load_Items_I");
                gd_dump_cities          ("310_Load_Cities_C");
                gd_dump_units           ("311_Load_Units_U");
                gd_dump_terrain_specials("312_Load_Terrain_Specials_T");
                gd_dump_map_square_flags("313_Load_Map_Flags_M");
                /* 314: _ai_continents is derived AI scratch state, NOT saved data
                 * (no loader) -- appended after the 13 canonical load arrays as an
                 * at-load in-memory baseline, to compare against 603 (AICWL entry)
                 * and 610 (AI_Reevaluate_Continent entry). */
                gd_dump_ai_continents   ("314_Load_Continents");

                current_screen = scr_Main_Screen;
                _players[NEUTRAL_PLAYER_IDX].banner_id = BNR_Brown;
                Clear_Fields();
                Loaded_Game_Update();
                GAME_SoM_Cast_By = ST_UNDEFINED;
#ifdef MOUSE_DEBUG
                MOUSE_LOG("SCR t=%llu LEAVE screen=Continue next=%d\n", (unsigned long long)Platform_Get_Millies(), current_screen);
#endif
            } break;

            case scr_Load_Screen:
            {
#ifdef MOUSE_DEBUG
                MOUSE_LOG("SCR t=%llu ENTER screen=Load\n", (unsigned long long)Platform_Get_Millies());
#endif
                // TODO  WZD vs. MGC
                /* WZD  GameState_01:  ; case 0x1 */
                /* HACK */ Load_WZD_Resources();
                prev__Settings_BG_Music = magic_set.background_music;
                Load_Screen();  /* ... |-> Loaded_Game_Update() */
#ifdef STU_DEBUG
                Print_Unit_Structure(0);
#endif
#ifdef STU_DEBUG
                LOG_INFO(LOG_CAT_MOM_SCR, "_num_players: %d", _num_players);
                LOG_DEBUG(LOG_CAT_GENERAL, "_num_players: %d", _num_players);
                // Meh. ~random AVRL in Player_Magic_Power_Distribution()
                // for(DBG_player_idx = 0; DBG_player_idx < (_num_players + 1); DBG_player_idx++)
                for(DBG_player_idx = 0; DBG_player_idx < _num_players; DBG_player_idx++)
                {
                    Player_Resource_Income_Total(DBG_player_idx, &DBG_gold_total, &DBG_food_total, &DBG_mana_total);
                    LOG_DEBUG(LOG_CAT_GENERAL, "DBG_player_idx: %d, DBG_gold_total: %d, DBG_food_total: %d, DBG_mana_total: %d", DBG_player_idx, DBG_gold_total, DBG_food_total, DBG_mana_total);
                }
#endif

                GAME_SoM_Cast_By = ST_UNDEFINED;
                GAME_RazeCity = ST_FALSE;
                if(prev__Settings_BG_Music != magic_set.background_music)
                {
                    if(magic_set.background_music == ST_TRUE)
                    {
                        // DOMSDOS  Play_Background_Music__STUB();
                        Play_Background_Music();
                    }
                    else
                    {
                        // DOMSDOS  Stop_Music__STUB();
                    }
                }
#ifdef MOUSE_DEBUG
                MOUSE_LOG("SCR t=%llu LEAVE screen=Load next=%d\n", (unsigned long long)Platform_Get_Millies(), current_screen);
#endif
            } break;

            case scr_New_Game_Screen:
            {
#ifdef MOUSE_DEBUG
                MOUSE_LOG("SCR t=%llu ENTER screen=New_Game\n", (unsigned long long)Platform_Get_Millies());
#endif
                // MoO2  if(Newgame_Screen_() != ST_FALSE) { Init_New_Game() }
                // Newgame_Control();  // MAGIC.EXE  ovr050  o050p001
                /* HACK */  if(Newgame_Control())
                /* HACK */  {
                /* HACK */      current_screen = scr_Continue;
                /* HACK */  }
                /* HACK */  else
                /* HACK */  {
                /* HACK */      current_screen = scr_Main_Menu_Screen;
                /* HACK */  }
                // MoO2  _previous_screen = scr_New_Game
#ifdef MOUSE_DEBUG
                MOUSE_LOG("SCR t=%llu LEAVE screen=New_Game next=%d\n", (unsigned long long)Platform_Get_Millies(), current_screen);
#endif
            } break;

            case scr_Quit_To_DOS:
            {
#ifdef MOUSE_DEBUG
                MOUSE_LOG("SCR t=%llu ENTER screen=Quit\n", (unsigned long long)Platform_Get_Millies());
#endif
#ifdef STU_DEBUG
                LOG_TRACE(LOG_CAT_GENERAL, "[SCR] ENTER scr_Quit_To_DOS -> setting quit_flag");
                LOG_DEBUG(LOG_CAT_GENERAL, "[SCR] ENTER scr_Quit_To_DOS -> setting quit_flag");
#endif
                // TODO  Auto_Save_Game()  ~== F-10 Quick_Save()
                quit_flag = ST_TRUE;
                // TODO  Exit_With_Message("Thank you for playing Master of Magic!\n\n");
#ifdef MOUSE_DEBUG
                MOUSE_LOG("SCR t=%llu LEAVE screen=Quit\n", (unsigned long long)Platform_Get_Millies());
#endif
            } break;

            case scr_Hall_Of_Fame_Screen:
            {
#ifdef MOUSE_DEBUG
                MOUSE_LOG("SCR t=%llu ENTER screen=Hall_Of_Fame\n", (unsigned long long)Platform_Get_Millies());
#endif
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
#ifdef MOUSE_DEBUG
                MOUSE_LOG("SCR t=%llu LEAVE screen=Hall_Of_Fame next=%d\n", (unsigned long long)Platform_Get_Millies(), current_screen);
#endif
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
#ifdef MOUSE_DEBUG
                MOUSE_LOG("SCR t=%llu ENTER screen=Main\n", (unsigned long long)Platform_Get_Millies());
#endif
                // BEGIN: WZD main()
                // Load_SAVE_GAM(8)
                // Load_WZD_Resources()
                Load_Palette(0, -1, 0);  // NOTE(JimBalcomb,20230111): this is the only Load_Palette() leading to the Main_Screen()
                // Calculate_Remap_Colors();
                Set_Button_Down_Offsets(1, 1);
                Cycle_Palette_Color(198, 40, 0, 0, 63, 0, 0, 1);
                Apply_Palette();
                // Fade_In()
                // Clear_Fields()
                // Loaded_Game_Update_WZD()
                // END: WZD main()

                // IDK  Set_Outline_Color(0);
                // IDK  Set_Alias_Color(0);
                // IDK  Set_Font_Style_Shadow_Down(0, 0, 0, 0);

                Set_Page_Off();
                Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, 7);
                Set_Page_On();
                Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, 5);
                Set_Page_Off();

                Main_Screen();

                // MoO2  previous_screen = scr_Main_Screen
                previous_screen = scr_Main_Screen;
#ifdef MOUSE_DEBUG
                MOUSE_LOG("SCR t=%llu LEAVE screen=Main next=%d\n", (unsigned long long)Platform_Get_Millies(), current_screen);
#endif
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
#ifdef STU_DEBUG
                {
                    uint64_t nt_start_ms = Platform_Get_Millies();
                    LOG_INFO(LOG_CAT_MOM_SCR, "[NEXTTURN] BEGIN Next_Turn_Proc() at %llu ms", (unsigned long long)nt_start_ms);
                    LOG_DEBUG(LOG_CAT_GENERAL, "[NEXTTURN] BEGIN Next_Turn_Proc() at %llu ms", (unsigned long long)nt_start_ms);
                    LOG_TRACE(LOG_CAT_GENERAL, "[NEXTTURN] BEGIN Next_Turn_Proc() at %llu ms", (unsigned long long)nt_start_ms);
                    Next_Turn_Proc();
                    {
                        uint64_t nt_end_ms = Platform_Get_Millies();
                        uint64_t nt_dur_ms = nt_end_ms - nt_start_ms;
                        LOG_INFO(LOG_CAT_MOM_SCR, "[NEXTTURN] END   Next_Turn_Proc() at %llu ms (duration=%llu ms)", (unsigned long long)nt_end_ms, (unsigned long long)nt_dur_ms);
                        LOG_DEBUG(LOG_CAT_GENERAL, "[NEXTTURN] END   Next_Turn_Proc() at %llu ms (duration=%llu ms)", (unsigned long long)nt_end_ms, (unsigned long long)nt_dur_ms);
                        LOG_TRACE(LOG_CAT_GENERAL, "[NEXTTURN] END   Next_Turn_Proc() at %llu ms (duration=%llu ms)", (unsigned long long)nt_end_ms, (unsigned long long)nt_dur_ms);
                    }
                }
#else
                Next_Turn_Proc();
#endif

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

                Diplomacy_Screen__WIP();

            } break;

            case scr_Test_Screen:
            {
                STU_DEBUG_BREAK();
                // Test_Screen();
            } break;
            case scr_PoC_Screen:
            {
                STU_DEBUG_BREAK();
                // PoC_Screen();
            } break;

        }  /* switch(current_screen) */

    }  /* while(quit_flag == ST_FALSE) */

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}
