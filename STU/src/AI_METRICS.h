#ifndef AI_METRICS_H
#define AI_METRICS_H

/*
    AI Metrics Collection System

    Collects structured per-turn CSV data for automated evaluation
    of AI behavior in both interactive (ReMoM) and headless (HeMoM) modes.

    Output files (when enabled):
        AI_TURN_SUMMARY.CSV  — per-player scoreboard each turn
        AI_NPC_EVENTS.CSV    — raider/monster spawns, NPC targeting, garrison culls

    Toggle:
        Compile-time:  #ifdef STU_DEBUG
        Runtime:       AI_Metrics_Enabled flag (set via --ai-metrics CLI)
*/

#ifndef STU_DEBUG

/* Release build: all metrics calls compile to nothing */
#define AI_Metrics_Startup()                        ((void)0)
#define AI_Metrics_Shutdown()                       ((void)0)
#define AI_Metrics_Emit_Turn_Summary(t,d,np)        ((void)0)
#define AI_Metrics_Emit_NPC_Event(t,et,si,sx,sy,sp,cnt,bud,tx,ty,acc,thr) ((void)0)

#else

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int16_t AI_Metrics_Enabled;

void AI_Metrics_Startup(void);
void AI_Metrics_Shutdown(void);

/*
    Turn Summary — one row per player per turn.
    Caller passes turn, difficulty, num_players; the function
    iterates _players[], _UNITS[], _CITIES[] internally.
*/
void AI_Metrics_Emit_Turn_Summary(int16_t turn, int16_t difficulty, int16_t num_players);

/*
    NPC Event — one row per spawn/target/cull event.
    event_type is a string constant: "RAIDER_SPAWN", "MONSTER_SPAWN", etc.
*/
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
);

#ifdef __cplusplus
}
#endif

#endif /* STU_DEBUG */

#endif /* AI_METRICS_H */
