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
static FILE * Build_Detail_File = NULL;
static FILE * Unit_Outcomes_File = NULL;


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

    Build_Detail_File = stu_fopen("AI_BUILD_DETAIL.CSV", "w");
    if (Build_Detail_File != NULL)
    {
        fprintf(Build_Detail_File,
            "turn,player_idx,city_idx,city_pop,city_units,chosen_product,chosen_weight,alt_count,top3_products,top3_weights\n");
        fflush(Build_Detail_File);
    }

    Unit_Outcomes_File = stu_fopen("AI_UNIT_OUTCOMES.CSV", "w");
    if (Unit_Outcomes_File != NULL)
    {
        fprintf(Unit_Outcomes_File,
            "turn,player_idx,unit_idx,unit_type,status,src_wx,src_wy,dst_wx,dst_wy,end_wx,end_wy,move_failed,arrived\n");
        fflush(Unit_Outcomes_File);
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

    if (Build_Detail_File != NULL)
    {
        fflush(Build_Detail_File);
        stu_fclose(Build_Detail_File);
        Build_Detail_File = NULL;
    }

    if (Unit_Outcomes_File != NULL)
    {
        fflush(Unit_Outcomes_File);
        stu_fclose(Unit_Outcomes_File);
        Unit_Outcomes_File = NULL;
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


void AI_Metrics_Emit_Build(
    int16_t turn,
    int16_t player_idx,
    int16_t city_idx,
    int16_t city_pop,
    int16_t city_units,
    int16_t product_count,
    int16_t chosen_weight,
    int16_t chosen_product,
    int16_t * product_array,
    int16_t * weights
)
{
    int16_t top3_products[3] = { -1, -1, -1 };
    int16_t top3_weights[3] = { 0, 0, 0 };
    int16_t i;
    int16_t j;

    if (!AI_Metrics_Enabled || Build_Detail_File == NULL)
    {
        return;
    }

    /* Find top 3 by weight */
    for (i = 0; i < product_count; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (weights[i] > top3_weights[j])
            {
                /* Shift down */
                int16_t k;
                for (k = 2; k > j; k--)
                {
                    top3_weights[k] = top3_weights[k - 1];
                    top3_products[k] = top3_products[k - 1];
                }
                top3_weights[j] = weights[i];
                top3_products[j] = product_array[i];
                break;
            }
        }
    }

    fprintf(Build_Detail_File,
        "%d,%d,%d,%d,%d,%d,%d,%d,%d;%d;%d,%d;%d;%d\n",
        turn,
        player_idx,
        city_idx,
        city_pop,
        city_units,
        chosen_product,
        chosen_weight,
        product_count,
        top3_products[0], top3_products[1], top3_products[2],
        top3_weights[0], top3_weights[1], top3_weights[2]
    );

    fflush(Build_Detail_File);
}


void AI_Metrics_Emit_Unit_Outcome(
    int16_t turn,
    int16_t player_idx,
    int16_t unit_idx,
    int16_t unit_type,
    int16_t status,
    int16_t src_wx,
    int16_t src_wy,
    int16_t dst_wx,
    int16_t dst_wy,
    int16_t end_wx,
    int16_t end_wy,
    int16_t move_failed
)
{
    int16_t arrived;

    if (!AI_Metrics_Enabled || Unit_Outcomes_File == NULL)
    {
        return;
    }

    arrived = (end_wx == dst_wx && end_wy == dst_wy && dst_wx != 0 && dst_wy != 0) ? 1 : 0;

    fprintf(Unit_Outcomes_File,
        "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
        turn,
        player_idx,
        unit_idx,
        unit_type,
        status,
        src_wx,
        src_wy,
        dst_wx,
        dst_wy,
        end_wx,
        end_wy,
        move_failed,
        arrived
    );

    fflush(Unit_Outcomes_File);
}

#endif /* STU_DEBUG */
