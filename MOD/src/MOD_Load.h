/*
    MOD_Load -- loads a mod folder.

    Phase 1 layout:
        <mod_dir>/mod.ini           optional; [Mod] name=..., version=...  (logged)
        <mod_dir>/scripts/main.lua  run once at load; registers hook handlers with game.on(...)
*/

#ifndef MOD_LOAD_H
#define MOD_LOAD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Loads the mod in mod_dir.  Returns 0 on success, -1 on error (logged). */
int MOD_Load_Directory(const char * mod_dir);

#ifdef __cplusplus
}
#endif

#endif  /* MOD_LOAD_H */
