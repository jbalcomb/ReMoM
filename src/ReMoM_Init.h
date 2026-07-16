/**
 * ReMoM_Init.h — Shared engine initialization for ReMoM and HeMoM.
 *
 * ReMoM_Init_Engine() performs the core initialization sequence needed by
 * both the full game (ReMoMber) and the headless tool (HeMoM):
 *   - Load CONFIG.MOM and MAGIC.SET
 *   - Configure sound driver IDs
 *   - Validate save files
 *   - Init_Drivers(), Allocate_Data_Space()
 *   - Load_MGC_Resources(), Load_TERRSTAT(), Load_SPELLDAT()
 *   - Load_WZD_Resources()
 */

#ifndef REMOM_INIT_H
#define REMOM_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the game engine: drivers, memory, and resources.
 * Must be called after Startup_Platform() and before any game logic.
 */
void ReMoM_Init_Engine(void);

/**
 * Fail-soft check for the presence of the original Master of Magic game data.
 * Resolves a small set of required .LBX files via the STU_GRAF search path.
 * If any are missing, shows a clear, actionable error (GUI box on player
 * builds, stderr on headless) and returns non-zero; the caller should exit.
 * Returns 0 when the data is present.  Call after STU_GRAF_Init() and before
 * the first asset load.
 */
int ReMoM_Preflight_Game_Data(void);

/**
 * First-run seeding of the writable per-user files.  Copies CONFIG.MOM (and
 * MAGIC.SET, if the install ships one) from the discovered game-data dir into
 * the user-data dir the first time; thereafter the engine reads/writes those
 * copies and the originals are never modified.  A no-op under the HEADLESS
 * profile (HeMoM keeps CWD).  Call after ReMoM_Preflight_Game_Data() succeeds
 * and before ReMoM_Init_Engine().
 */
void ReMoM_Seed_User_Files(void);

/**
 * Non-blocking data-compatibility check.  Hashes the installed .LBX files
 * against the compiled-in known-good manifest (STU's g_lbx_manifest) and, when
 * some don't match a supported v1.31 distribution, shows a one-time
 * informational warning and continues.  A silent no-op when the manifest is
 * empty (not yet authored) or every file matches.  Player builds only; call
 * after preflight/seeding.
 */
void ReMoM_Check_Data_Compat(void);

#ifdef __cplusplus
}
#endif

#endif /* REMOM_INIT_H */
