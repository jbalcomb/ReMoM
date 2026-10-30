/*
    MOD_Load -- loads a mod folder.

    Phase 1 layout:
        <mod_dir>/mod.ini           optional; [Mod] name=..., version=...  (logged)
        <mod_dir>/scripts/main.lua  optional; run once at load; registers hook handlers with game.on(...)

    Caster of Magic scripts are bound to hooks in mod.ini:
        [CAS]
        master=scripts/MASTER.CAS     ; optional, run first; its variables are readable from every script (CoM2: System)
        city_gold=scripts/EcoGold.CAS ; hook name, or CoM2's [Scripts] name (EconomyGold, EconomyFood, ...)
    A hook script gets CITYID and VALUE (the value so far) and RESULT = VALUE; it returns the total of its ADDRESLINE
    lines if it added any, otherwise RESULT.  Script warnings (things CoM2 tolerates) are logged at load.
*/

#ifndef MOD_LOAD_H
#define MOD_LOAD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Loads the mod in mod_dir.  Returns 0 on success, -1 on error (logged). */
int MOD_Load_Directory(const char * mod_dir);

/* Loads the rules profile named by `--rules NAME` on the command line, or "original" without one (MOD_Rules.h).
   `--rules none` loads no profile, so every rule runs its C version.  Call it before loading any mod.  Returns 0 on
   success, -1 if a profile asked for with --rules failed to load.  If the default profile cannot be found the game
   still runs on the C versions, which behave the same; that is logged as a warning and returns 0. */
int MOD_Rules_Load_Command_Line(int argc, char * argv[]);

/* Removes every hook handler, frees the CAS scripts and shuts Lua down. */
void MOD_Unload(void);

#ifdef __cplusplus
}
#endif

#endif  /* MOD_LOAD_H */
