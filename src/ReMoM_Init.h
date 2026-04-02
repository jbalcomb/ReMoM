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

#ifdef __cplusplus
}
#endif

#endif /* REMOM_INIT_H */
