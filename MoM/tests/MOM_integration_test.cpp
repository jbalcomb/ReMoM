#include <gtest/gtest.h>

#include <cstdio>
#include <cstdlib>

// // #include "MOM_Test_Helpers.hpp"
// #include "MOM_Test_Helpers.cpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/Allocate.h" /* _cities[], _UNITS[] */
#include "../../MoX/src/LBX_Load.h" /* LBX_Load_Data_Static() */
#include "../../MoX/src/LOADSAVE.h"
#include "../../MoX/src/MOM_DAT.h" /* _cities[], _UNITS[] */
#include "../../MoX/src/MOX_DAT.h"  /* _players[] */
#include "../../MoX/src/MOX_DEF.h"  /* HUMAN_PLAYER_IDX */
#include "../../MoX/src/MOX_SET.h"
#include "../../MoX/src/MOX_UPD.h"  /* UNITS_FINISHED */
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/SOUND.h"
#include "../../MoX/src/Util.h"  /* Delete_Structure() */

#include "../src/ALLOC.h"
#include "../src/CITYCALC.h"
#include "../src/Init.h"
#include "../src/LOADER.h"
#include "../src/LoadScr.h"
#include "../src/MOM_SCR.h"
#include "../src/NEXTTURN.h"
#include "../src/Settings.h"
#include "../src/Spells130.h"       /* Hero_Slot_Types () */
#include "../src/Spells132.h"       /* WIZ_HireHero() */
#include "../src/UNITTYPE.h"

#include "../../STU/src/STU_DBG.h"  /* Debug_Log_Startup(), Debug_Log_Shutdown() */



/*
    Forward Declare, Private
*/
// Spells132.c
// WZD dseg:68A8
extern char names_lbx_file__ovr132[];
extern char DEFAULT_FONT_FILE[];

#ifdef __cplusplus
}
#endif

namespace
{
    class Mom_Test_Environment : public ::testing::Environment
    {
    public:
        void SetUp() override
        {
#ifdef STU_DEBUG
            Debug_Log_Startup();
#endif
        }

        void TearDown() override
        {
#ifdef STU_DEBUG
            if(Debug_Log_File != nullptr)
            {
                Debug_Log_Shutdown();
                Debug_Log_File = nullptr;
            }
#endif
        }
    };

    ::testing::Environment * const mom_test_environment = ::testing::AddGlobalTestEnvironment(new Mom_Test_Environment());

    void Log_Real_Save_Test_Step(const char * message)
    {
        FILE * log_file;

        log_file = std::fopen("MOM_REAL_SAVE_TEST.LOG", "a");
        if(log_file != nullptr)
        {
            std::fprintf(log_file, "%s\n", message);
            std::fclose(log_file);
        }
    }

    void Fail_Real_Save_Test(const char * message)
    {
        Log_Real_Save_Test_Step(message);
        std::fprintf(stderr, "%s\n", message);
        std::fflush(stderr);
        std::exit(1);
    }

    void Bootstrap_Real_Save_Game(void)
    {
        std::remove("MOM_REAL_SAVE_TEST.LOG");
        Log_Real_Save_Test_Step("Bootstrap start");

        // NOTE(JimBalcomb,20260430): no idea how Copilot got to deciding to start the debug log, but then not use it, and also not shut it down
#ifdef STU_DEBUG
        Debug_Log_Startup();
        Log_Real_Save_Test_Step("Debug_Log_Startup ok");
#endif

        Set_Default_Game_Settings();
        magic_set.background_music = ST_FALSE;
        magic_set.sound_effects = ST_FALSE;
        magic_set.event_music = ST_FALSE;
        magic_set.end_of_turn_summary = ST_FALSE;
        magic_set.end_of_turn_wait = ST_FALSE;
        magic_set.enemy_moves = ST_FALSE;
        magic_set.enemy_spells = ST_FALSE;
        Log_Real_Save_Test_Step("Settings configured");

        Allocate_Data_Space(6100);
        Log_Real_Save_Test_Step("Allocate_Data_Space ok");
        Load_MGC_Resources();
        Log_Real_Save_Test_Step("Load_MGC_Resources ok");
        Load_TERRSTAT();
        Log_Real_Save_Test_Step("Load_TERRSTAT ok");
        Load_SPELLDAT();
        Log_Real_Save_Test_Step("Load_SPELLDAT ok");
        Load_WZD_Resources();
        Log_Real_Save_Test_Step("Load_WZD_Resources ok");

        current_screen = scr_Main_Screen;
        g_bldg_msg_ctr = 0;

        Load_SAVE_GAM(ST_UNDEFINED);
        Log_Real_Save_Test_Step("Load_SAVE_GAM ok");
        Loaded_Game_Update();
        Log_Real_Save_Test_Step("Loaded_Game_Update ok");

        magic_set.background_music = ST_FALSE;
        magic_set.sound_effects = ST_FALSE;
        magic_set.event_music = ST_FALSE;
        magic_set.end_of_turn_summary = ST_FALSE;
        magic_set.end_of_turn_wait = ST_FALSE;
        magic_set.enemy_moves = ST_FALSE;
        magic_set.enemy_spells = ST_FALSE;
        current_screen = scr_Main_Screen;
        Log_Real_Save_Test_Step("Bootstrap complete");
    }

    void Run_Real_Save_Next_Turn_Delete_Dead_Units_Check(void)
    {
        int initial_units;
        int first_dead_unit_idx;
        int second_dead_unit_idx;

        Bootstrap_Real_Save_Game();

        initial_units = _units;
        if(initial_units < 2)
        {
            Fail_Real_Save_Test("SAVETEST.GAM did not load enough units for the Next_Turn_Proc regression test.");
        }

        first_dead_unit_idx = ST_UNDEFINED;
        second_dead_unit_idx = ST_UNDEFINED;

        for(int itr_units = 0; itr_units < (_units - 1); itr_units++)
        {
            if(
                (_UNITS[itr_units].owner_idx >= HUMAN_PLAYER_IDX)
                &&
                (_UNITS[itr_units].owner_idx <= NEUTRAL_PLAYER_IDX)
                &&
                (_UNITS[itr_units + 1].owner_idx >= HUMAN_PLAYER_IDX)
                &&
                (_UNITS[itr_units + 1].owner_idx <= NEUTRAL_PLAYER_IDX)
                &&
                (_UNITS[itr_units].Hero_Slot == ST_UNDEFINED)
                &&
                (_UNITS[itr_units + 1].Hero_Slot == ST_UNDEFINED)
            )
            {
                first_dead_unit_idx = itr_units;
                second_dead_unit_idx = (itr_units + 1);
                break;
            }
        }

        if(first_dead_unit_idx == ST_UNDEFINED)
        {
            Fail_Real_Save_Test("SAVETEST.GAM did not contain two consecutive non-hero units to kill.");
        }

        Kill_Unit(first_dead_unit_idx, kt_Normal);
        Kill_Unit(second_dead_unit_idx, kt_Normal);
        Log_Real_Save_Test_Step("Killed selected units");

        if((_UNITS[first_dead_unit_idx].owner_idx != ST_UNDEFINED) || (_UNITS[second_dead_unit_idx].owner_idx != ST_UNDEFINED))
        {
            Fail_Real_Save_Test("Kill_Unit failed to mark both selected real-save units as dead.");
        }

        _cities = 0;
        _num_players = 1;
        _human_player_idx = HUMAN_PLAYER_IDX;
        _players[HUMAN_PLAYER_IDX].casting_spell_idx = 0;
        _players[HUMAN_PLAYER_IDX].researching_spell_idx = 0;
        magic_set.random_events = ST_FALSE;
        g_timestop_player_num = 0;
        Log_Real_Save_Test_Step("Constrained loaded state for focused Next_Turn_Proc coverage");

        g_bldg_msg_ctr = 0;
        Next_Turn_Proc();
        Log_Real_Save_Test_Step("Next_Turn_Proc ok");

        if(_units != (initial_units - 2))
        {
            char failure_message[256];

            std::snprintf(failure_message, sizeof(failure_message), "Expected _units to drop from %d to %d after killing loaded units %d and %d, but got %d.", initial_units, (initial_units - 2), first_dead_unit_idx, second_dead_unit_idx, _units);
            Fail_Real_Save_Test(failure_message);
        }

        for(int itr_units = 0; itr_units < _units; itr_units++)
        {
            if(_UNITS[itr_units].owner_idx == ST_UNDEFINED)
            {
                char failure_message[160];

                std::snprintf(failure_message, sizeof(failure_message), "Dead unit survived compaction at index %d after Next_Turn_Proc.", itr_units);
                Fail_Real_Save_Test(failure_message);
            }
        }

        std::exit(0);
    }
}

TEST(MOMIntegrationTest, NextTurnProc_RealSave_RemovesKilledUnits)
{
    ASSERT_EXIT(Run_Real_Save_Next_Turn_Delete_Dead_Units_Check(), ::testing::ExitedWithCode(0), "");
}
