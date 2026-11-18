
// #ifdef STU_DEBUG
#include "../../STU/src/STU_DBG.h"
#include "../../STU/src/STU_LOG.h"
#include "../../STU/src/STU_GD_MoM.h"  /* STU_GD_ONCE(), STU_GD_DO(), STU_GD_Dump_*() */  /* CLAUDE */
#include "../../STU/src/STU_PRN.h"
// #endif

#include "../../MoX/src/Allocate_Pool.h"  /* POOL_SENTINEL_BYTE */
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_BASE.h"
#include "../../MoX/src/MOX_DAT.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_T4.h"  // ~ MOX & MOX2 //MoO2
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/LBX_Load.h" /* LBX_Load_Data_Static() */
#include "../../MoX/src/LOADSAVE.h"
#include "../../MoX/src/MOX_SET.h"  /* magic_set */
#include "../../MoX/src/Mouse.h"
#include "../../MoX/src/MOX2.h"     /* _cmd_line_seed */
#include "../../MoX/src/random.h"   /* Set_Random_Seed(), Randomize() */
#include "../../MoX/src/SOUND.h"
#include "../../MoX/src/Video.h"

#include "AdvsrScr.h"
#include "ArmyList.h"
#include "CITYCALC.h"   /* Player_Resource_Income_Total() */
#include "CityList.h"
#include "CityScr.h"
#include "Combat.h"
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
#include "Spellbook.h"  /* spl_NONE */

#include "../../ext/stu_compat.h"

#include <stdio.h>
#include <string.h> /* memset() */

#include "../../platform/include/Platform.h"  /* CLAUDE: MOUSE_LOG for screen transition timestamps */

#include "MOM_SCR.h"



// MGC dseg:28D2
// WZD dseg:29DE
// 31D1:28D2 54 45 52 52 53 54 41 54 00                      terrstat_lbx_file db 'TERRSTAT',0       ; DATA XREF: Load_TERRSTAT+11o
// char terrstat_lbx_file[] = "TERRSTAT";
// MGC 31D1:28DB
char cnst_HLPENTRY_File[] = "hlpentry";



/* ST_TRUE once a game has reached the Main Screen; the globals then hold that game's state and need resetting before the next one */
static int16_t dirty_game_state = ST_FALSE;
/* Allocate_Space() returns its 16-byte header as element 0's first bytes; a fresh process still has it there, so a reset puts it back */
#define SAMB_HEADER_BYTES 16
static uint8_t units_samb_header[SAMB_HEADER_BYTES];
static uint8_t items_samb_header[SAMB_HEADER_BYTES];
static uint8_t lairs_samb_header[SAMB_HEADER_BYTES];
static int16_t samb_headers_saved = ST_FALSE;



/*
clears the game state when starting a new 'New Game'
*/
static void Reset_Game_State(void)
{

    TRACE_ENTER();

    /* set the random seed for the new game path, with support for deterministic behavior */
    if(_cmd_line_seed != 0)
    {
        Set_Random_Seed((uint32_t)_cmd_line_seed);
    }
    else
    {
        Randomize();
    }

    _turn = 0;

    _given_chance_to_rename_home_city = ST_FALSE;

    magic_master_idx = ST_UNDEFINED;

    /* EOG_HACK */  g_conquest_winner_idx = ST_UNDEFINED;

    m_magic_winner_idx = ST_UNDEFINED;

    memset(_players, 0, sizeof(_players));

    memset(_UNITS, POOL_SENTINEL_BYTE, (NUM_UNITS * sizeof(struct s_UNIT)));

    memset(_ITEMS, POOL_SENTINEL_BYTE, (NUM_ITEMS * sizeof(struct s_ITEM)));

    /* new game doesn't init farmer_count */
    memset(_CITIES, POOL_SENTINEL_BYTE, (NUM_CITIES * sizeof(struct s_CITY)));
    /* new game fills lairs 0..98 only; restore the never-written slots to what a fresh process has (pool fill) */
    memset(_LAIRS, POOL_SENTINEL_BYTE, (NUM_LAIRS * sizeof(struct s_LAIR)));

    memcpy(_UNITS, units_samb_header, SAMB_HEADER_BYTES);
    memcpy(_ITEMS, items_samb_header, SAMB_HEADER_BYTES);
    memcpy(_LAIRS, lairs_samb_header, SAMB_HEADER_BYTES);

    TRACE_EXIT();
}



// MGC s001p09
// UU_HLP_Load_MainMenu()
/*
; Unused in MoM
; loads and sets the GUI help entry area array for the banner selection screen
*/
/*
31D1:28D2 54 45 52 52 53 54 41 54 00                      terrstat_lbx_file db 'TERRSTAT',0       ; DATA XREF: Load_TERRSTAT+11o
31D1:28DB 68 6C 70 65 6E 74 72 79 00                      cnst_HLPENTRY_File db 'hlpentry',0      ; DATA XREF: UU_HLP_Load_MainMenu+16o
*/
void UU_HLP_Load_MainMenu(void)
{
    // TODO  HLPENTRY.LBX, 025 ...
    LBX_Load_Data_Static(cnst_HLPENTRY_File, 25, (SAMB_ptr)_help_entries, 0, 4, 10);  // "hlpentry"
    Set_Help_List(_help_entries, 4);
}


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

    TRACE_ENTER();

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
                LOG_TRACE(LOG_CAT_GENERAL, "[SCR] ENTER scr_Main_Menu_Screen");
                LOG_DEBUG(LOG_CAT_GENERAL, "[SCR] ENTER scr_Main_Menu_Screen");
                /* game-state reset for {New, Continue, Load}, from four paths; (NOTE: Load during a running game does not reset state) */
                if(dirty_game_state == ST_TRUE)
                {
                    Reset_Game_State();
                    dirty_game_state = ST_FALSE;
                }
                /* HACK */  g_magic_exe_context = ST_TRUE;  /* MAGIC.EXE: OG Respawned MAGIC.EXE to show the Main Menu */
                if(samb_headers_saved == ST_FALSE)
                {
                    memcpy(units_samb_header, _UNITS, SAMB_HEADER_BYTES);
                    memcpy(items_samb_header, _ITEMS, SAMB_HEADER_BYTES);
                    memcpy(lairs_samb_header, _LAIRS, SAMB_HEADER_BYTES);
                    samb_headers_saved = ST_TRUE;
                }


                Load_Palette(2, -1, 0);
                Apply_Palette();
                // TODO  Main_Menu_Screen_Control();
                Main_Menu_Screen();
                previous_screen = scr_Main_Menu_Screen;  // MoO2
#ifdef MOUSE_DEBUG
                MOUSE_LOG("SCR t=%llu LEAVE screen=Main_Menu next=%d\n", (unsigned long long)Platform_Get_Millies(), current_screen);
#endif
                LOG_TRACE(LOG_CAT_GENERAL, "[SCR] LEAVE scr_Main_Menu_Screen -> next=%d", current_screen);
                LOG_DEBUG(LOG_CAT_GENERAL, "[SCR] LEAVE scr_Main_Menu_Screen -> next=%d", current_screen);
            } break;

            case scr_Continue:
            {
                LOG_DEBUG(LOG_CAT_MOM_SCR, "DEBUG: [%s, %d]: Screen_Control(): swicth(): scr_Continue", __FILE__, __LINE__);
                LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: Screen_Control(): swicth(): scr_Continue", __FILE__, __LINE__);
                LOG_TRACE(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: Screen_Control(): swicth(): scr_Continue", __FILE__, __LINE__);
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
    // WZD m_magic_winner_idx = ST_UNDEFINED;

                // MGC  fid0_Continue:                            ; case 0x0
                // MGC  Stop_Music__STUB()
                // MGC  j_GAME_WizardsLaunch__WIP(e_SAVE9GAM)
                // /* HACK */ Load_SAVE_GAM(-1);  // SAVETEST.GAM
                // FU CLAUDE  /* HACK */  Load_WZD_Resources();
                /* HACK */ Load_WZD_Resources_Once();  /* CLAUDE  L57 */

                /* CLAUDE */  /* WIZARDS.EXE _main: call Release_Version / call Enable_Cancel / ... / call Load_SAVE_GAM - the game runs with right-click cancel off
                                 (mouse_cancel_disabled = ST_TRUE), so a right-click reaches Main_Screen as -field; MAGIC's Disable_Cancel() (ReMoM_Init_Engine(),
                                 Newgame_Screen_4()) was still in effect here, so every right-click in the game came back as -1 (cancel) */
                Enable_Cancel();

                Load_SAVE_GAM(8);

                /* 300-series: snapshot every save-loaded array right after Load_SAVE_GAM
                 * (matches OG's gd_wzd_landmark_probe @ Load_SAVE_GAM on-return). Numbered
                 * in save-load order; gaps (303/304/306/312/313) are Phase 2. */
                STU_GD_DO(STU_GD_Dump_Heroes("301_Load_Heroes_H"));  /* CLAUDE */
                STU_GD_DO(STU_GD_Dump_Players("302_Load_Players_P"));  /* CLAUDE */
                STU_GD_DO(STU_GD_Dump_World_Map("303_Load_World_Maps_W"));  /* CLAUDE */
                STU_GD_DO(STU_GD_Dump_Landmasses("304_Load_Landmasses_L"));  /* CLAUDE */
                STU_GD_DO(STU_GD_Dump_Nodes("305_Load_Nodes_N"));  /* CLAUDE */
                STU_GD_DO(STU_GD_Dump_Fortresses("306_Load_Fortresses_F"));  /* CLAUDE */
                STU_GD_DO(STU_GD_Dump_Towers("307_Load_Towers_T"));  /* CLAUDE */
                STU_GD_DO(STU_GD_Dump_Lairs("308_Load_Lairs_L"));  /* CLAUDE */
                STU_GD_DO(STU_GD_Dump_Items("309_Load_Items_I"));  /* CLAUDE */
                STU_GD_DO(STU_GD_Dump_Cities("310_Load_Cities_C"));  /* CLAUDE */
                STU_GD_DO(STU_GD_Dump_Units("311_Load_Units_U"));  /* CLAUDE */
                STU_GD_DO(STU_GD_Dump_Terrain_Specials("312_Load_Terrain_Specials_T"));  /* CLAUDE */
                STU_GD_DO(STU_GD_Dump_Map_Square_Flags("313_Load_Map_Flags_M"));  /* CLAUDE */
                /* 314: _ai_continents is derived AI scratch state, NOT saved data
                 * (no loader) -- appended after the 13 canonical load arrays as an
                 * at-load in-memory baseline, to compare against 603 (AICWL entry)
                 * and 610 (AI_Reevaluate_Continent entry). */
                STU_GD_DO(STU_GD_Dump_AI_Continents("314_Load_Continents"));  /* CLAUDE */

                Load_Palette(0, ST_UNDEFINED, ST_NULL);
                Calculate_Remap_Colors();
                Set_Button_Down_Offsets(1, 1);
                Cycle_Palette_Color(198, 40, 0, 0, 63, 0, 0, 1);  // (color_num, red_min, green_min, blue_min, red_max, green_max, blue_max, step_value)
                Apply_Palette();
                Fade_In();
                current_screen = scr_Main_Screen;
                _players[NEUTRAL_PLAYER_IDX].banner_id = BNR_Brown;
                Clear_Fields();
                Loaded_Game_Update();
                m_magic_winner_idx = ST_UNDEFINED;
                /* EOG_HACK */  g_conquest_winner_idx = ST_UNDEFINED;
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
                // FU CLAUDE  /* HACK */  Load_WZD_Resources();
                /* HACK */ Load_WZD_Resources_Once();  /* CLAUDE  L57 */
                prev__Settings_BG_Music = magic_set.background_music;
                Load_Screen();  /* ... |-> Loaded_Game_Update() */
                /* CLAUDE */  /* a game was loaded: as for scr_Continue, WIZARDS.EXE _main's Enable_Cancel(); a return to MAGIC's main menu keeps MAGIC's Disable_Cancel() */
                if(current_screen == scr_Main_Screen)
                {
                    Enable_Cancel();
                }
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

                m_magic_winner_idx = ST_UNDEFINED;
                /* EOG_HACK */  g_conquest_winner_idx = ST_UNDEFINED;
                destroy_combat_city = ST_FALSE;
                if(prev__Settings_BG_Music != magic_set.background_music)
                {
                    if(magic_set.background_music == ST_TRUE)
                    {
                        // DOMSDOS  Play_Background_Music();
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
                LOG_TRACE(LOG_CAT_GENERAL, "[SCR] ENTER scr_Quit_To_DOS -> setting quit_flag");
                LOG_DEBUG(LOG_CAT_GENERAL, "[SCR] ENTER scr_Quit_To_DOS -> setting quit_flag");
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

                City_Screen();

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
                dirty_game_state = ST_TRUE;
                /* HACK */  g_magic_exe_context = ST_FALSE;  /* WIZARDS.EXE: a game is running */
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

    TRACE_EXIT();

}
