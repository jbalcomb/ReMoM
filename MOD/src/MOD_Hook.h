/*
    MOD_Hook -- the hook dispatcher for mods.

    Game code calls a hook at a point a mod may change (e.g. a city's gold income).  Each hook keeps a short chain of
    handlers (Lua functions, later CAS scripts, or C functions).  The game's own C result goes in first; each handler
    receives the previous result and returns the next one.  With no handler registered the C result comes back
    unchanged, so the vanilla game behaves exactly as before.

    A handler that ignores the value it receives overrides the C computation (Caster of Magic's model); one that
    adjusts it lets several mods stack.

    See the "ReMoM Modding Design" doc for the full list of planned hooks.
*/

#ifndef MOD_HOOK_H
#define MOD_HOOK_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Phase 1: the five city-economy hooks.  Each wraps one per-resource function called from Do_City_Calculations(). */
enum e_MOD_HOOK
{
    MOD_HOOK_CITY_FOOD       = 0,  /* City_Food_Production()       ~ CoM2 EconomyFood     */
    MOD_HOOK_CITY_PRODUCTION = 1,  /* City_Production_Production() ~ CoM2 EconomyProd     */
    MOD_HOOK_CITY_GOLD       = 2,  /* City_Gold_Production()       ~ CoM2 EconomyGold     */
    MOD_HOOK_CITY_RESEARCH   = 3,  /* City_Research_Production()   ~ CoM2 EconomyResearch */
    MOD_HOOK_CITY_POWER      = 4,  /* City_Mana_Production()       ~ CoM2 EconomyPower    */
    MOD_HOOK_COUNT           = 5   /* keep last */
};

#define MOD_HOOK_MAX_HANDLERS 8

/* A handler: returns the new value for hook_id given the previous value.  subject_idx is the city/unit/player index. */
typedef int32_t (*MOD_Hook_Fn)(int16_t hook_id, int16_t subject_idx, int32_t value, void * user_data);

/* Appends a handler to the hook's chain.  Returns 0 on success, -1 for an unknown hook or a full chain (logged). */
int MOD_Hook_Register(int16_t hook_id, MOD_Hook_Fn fn, void * user_data);

/* Removes every handler from every hook. */
void MOD_Hook_Clear_All(void);

/* Number of handlers registered on hook_id (0 for an unknown hook). */
int16_t MOD_Hook_Count(int16_t hook_id);

/* Runs hook_id's chain over value and returns the result.  Fast path: no handler -> value unchanged. */
int32_t MOD_Hook_City_Value(int16_t hook_id, int16_t city_idx, int32_t value);

/* Script-facing name of a hook ("city_gold"), or NULL for an unknown hook. */
const char * MOD_Hook_Name(int16_t hook_id);

/* Hook id for a script-facing name, or -1 if there is no such hook. */
int16_t MOD_Hook_Find(const char * name);

#ifdef __cplusplus
}
#endif

#endif  /* MOD_HOOK_H */
