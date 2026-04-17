/*
    AI Metrics Collection

    Writes structured CSV data for automated evaluation of AI behavior.
    Follows the STU_DBG.c lifecycle pattern: startup opens files,
    shutdown closes them, emit functions write rows with fflush.
*/

#ifdef STU_DEBUG

#include "../../ext/stu_compat.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_DAT.h"

#include "AI_METRICS.h"


int16_t AI_Metrics_Enabled = 0;

static FILE * Turn_Summary_File = NULL;
static FILE * NPC_Events_File = NULL;


void AI_Metrics_Startup(void)
{
    if (!AI_Metrics_Enabled)
    {
        return;
    }

    Turn_Summary_File = stu_fopen("AI_TURN_SUMMARY.CSV", "w");
    if (Turn_Summary_File != NULL)
    {
        fprintf(Turn_Summary_File,
            "turn,player_idx,name,gold,mana,fame,power_base,unit_count,city_count,active,raider_acc,monster_acc\n");
        fflush(Turn_Summary_File);
    }

    NPC_Events_File = stu_fopen("AI_NPC_EVENTS.CSV", "w");
    if (NPC_Events_File != NULL)
    {
        fprintf(NPC_Events_File,
            "turn,event_type,source_idx,source_wx,source_wy,source_wp,count,budget,target_wx,target_wy,accumulator,threshold\n");
        fflush(NPC_Events_File);
    }
}


void AI_Metrics_Shutdown(void)
{
    if (Turn_Summary_File != NULL)
    {
        fflush(Turn_Summary_File);
        stu_fclose(Turn_Summary_File);
        Turn_Summary_File = NULL;
    }

    if (NPC_Events_File != NULL)
    {
        fflush(NPC_Events_File);
        stu_fclose(NPC_Events_File);
        NPC_Events_File = NULL;
    }
}


void AI_Metrics_Emit_Turn_Summary(int16_t turn, int16_t difficulty, int16_t num_players)
{
    int16_t player_idx = 0;
    int16_t itr = 0;
    int16_t unit_counts[NUM_PLAYERS];
    int16_t city_counts[NUM_PLAYERS];

    if (!AI_Metrics_Enabled || Turn_Summary_File == NULL)
    {
        return;
    }

    for (player_idx = 0; player_idx < NUM_PLAYERS; player_idx++)
    {
        unit_counts[player_idx] = 0;
        city_counts[player_idx] = 0;
    }

    for (itr = 0; itr < _units; itr++)
    {
        if (_UNITS[itr].owner_idx >= 0 && _UNITS[itr].owner_idx < NUM_PLAYERS)
        {
            unit_counts[_UNITS[itr].owner_idx]++;
        }
    }

    for (itr = 0; itr < _cities; itr++)
    {
        if (_CITIES[itr].owner_idx >= 0 && _CITIES[itr].owner_idx < NUM_PLAYERS)
        {
            city_counts[_CITIES[itr].owner_idx]++;
        }
    }

    for (player_idx = 0; player_idx < NUM_PLAYERS; player_idx++)
    {
        fprintf(Turn_Summary_File,
            "%d,%d,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
            turn,
            player_idx,
            _players[player_idx].name,
            _players[player_idx].gold_reserve,
            _players[player_idx].mana_reserve,
            _players[player_idx].fame,
            _players[player_idx].Power_Base,
            unit_counts[player_idx],
            city_counts[player_idx],
            (player_idx < num_players && player_idx != NEUTRAL_PLAYER_IDX)
                ? _FORTRESSES[player_idx].active
                : (player_idx == NEUTRAL_PLAYER_IDX ? 1 : 0),
            (player_idx == NEUTRAL_PLAYER_IDX)
                ? _players[NEUTRAL_PLAYER_IDX].casting_cost_original
                : 0,
            (player_idx == NEUTRAL_PLAYER_IDX)
                ? _players[NEUTRAL_PLAYER_IDX].average_unit_cost
                : 0
        );
    }

    fflush(Turn_Summary_File);
}


void AI_Metrics_Emit_NPC_Event(
    int16_t turn,
    const char * event_type,
    int16_t source_idx,
    int16_t source_wx,
    int16_t source_wy,
    int16_t source_wp,
    int16_t count,
    int16_t budget,
    int16_t target_wx,
    int16_t target_wy,
    int16_t accumulator,
    int16_t threshold
)
{
    if (!AI_Metrics_Enabled || NPC_Events_File == NULL)
    {
        return;
    }

    fprintf(NPC_Events_File,
        "%d,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
        turn,
        event_type,
        source_idx,
        source_wx,
        source_wy,
        source_wp,
        count,
        budget,
        target_wx,
        target_wy,
        accumulator,
        threshold
    );

    fflush(NPC_Events_File);
}

#endif /* STU_DEBUG */
