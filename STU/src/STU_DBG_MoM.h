/*
    STU_DBG_MoM -- Master of Magic debug helpers that need the game's types and globals (struct s_MAGIC_SET, the record structs, _num_players, ...).
    The MoM-free debug knobs (event / offer / research triggers, DBG_debug_flag, the map-square trace filter) live in STU_DBG.

    Debug-build only: everything here is compiled out without STU_DEBUG, so call it from inside #ifdef STU_DEBUG.
    Reverse-engineering evaluation code: no counterpart in the original game.

    Moved from MoM/src/Settings.c (DBG_Print_MAGIC_SET, DBG_Compare_MAGIC_SET), MoM/src/MOM_DBG.c (Test_Run),
    MoM/src/Combat.c (combat debug globals, battle-unit counters, DBG_Compare_Battle_Units) and MoX/src/MOM_DAT.c (DBG_battle_units).
*/

#ifndef STU_DBG_MOM_H
#define STU_DBG_MOM_H

#ifdef STU_DEBUG

#include "../../MoX/src/MOX_SET.h"  /* struct s_MAGIC_SET */
#include "../../MoX/src/MOM_DAT.h"  /* struct s_BATTLE_UNIT, MAX_BATTLE_UNIT_SLOT_COUNT */

#ifdef __cplusplus
extern "C" {
#endif

/* LOG_DEBUG every MAGIC.SET field, bracketed by "===== MAGIC_SET [label] =====" lines */
void DBG_Print_MAGIC_SET(const char * label);
/* LOG_DEBUG each MAGIC.SET field that differs between before and after, then the change count */
void DBG_Compare_MAGIC_SET(const struct s_MAGIC_SET * before, const struct s_MAGIC_SET * after, const char * label);

/* assert the game-data record sizes and the 2-byte game-state globals; no callers */
void Test_Run(void);

/* combat: the move-path knobs (DBG_cgx, DBG_cgy, DBG_target_*, DBG_PFA_*, DBG_player_idx) are declared in STU_DBG_Combat_Move_Path.h, with the macros */
#include "STU_DBG_Combat_Move_Path.h"

/* combat: attacker / defender battle-unit counts by status, refreshed by DBG_Count_Battle_Units() each combat-loop pass; drawn by Combat_Screen_Draw_Debug_Information() */
extern int16_t DBG_atk_active, DBG_atk_dead, DBG_atk_gone, DBG_def_active, DBG_def_dead, DBG_def_gone;
void DBG_Count_Battle_Units(void);

/* combat: battle_units[] snapshot and the field-by-field compare against it, called from Combat_Screen around Combat_Cache_Read() */
extern struct s_BATTLE_UNIT DBG_battle_units[MAX_BATTLE_UNIT_SLOT_COUNT];
void DBG_Snapshot_Battle_Units(void);
void DBG_Compare_Battle_Units(const char * label);

#ifdef __cplusplus
}
#endif

#endif  /* STU_DEBUG */

#endif  /* STU_DBG_MOM_H */
