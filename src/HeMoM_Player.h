/**
 * HeMoM_Player.h — Synthetic human player for headless testing.
 *
 * Reads a test scenario file (.hms) and injects input actions through
 * the platform input layer. Called once per frame from Platform_Event_Handler()
 * via a registered callback.
 *
 * Scenario file format:
 *   # comment
 *   wait <frames>                  — idle for N frames
 *   key <character>                — press a key (e.g., key n, key N, key 1)
 *   escape                         — press Escape
 *   enter                          — press Enter
 *   click <x> <y>                  — left-click at game coordinates
 *   rclick <x> <y>                 — right-click at game coordinates
 *   next_turn                      — press 'N' (Next Turn hotkey)
 *   quit                           — press Escape twice (quit to menu, then quit game)
 *   end                            — stop the synthetic player (idle forever or exit)
 */

#ifndef HEMOM_PLAYER_H
#define HEMOM_PLAYER_H

#ifdef __cplusplus
extern "C" {
#endif



/**
 * Load a test scenario from a .hms file.
 * @param filepath  Path to the scenario file.
 * @return  0 on success, non-zero on failure.
 */
int HeMoM_Player_Load_Scenario(const char *filepath);

/**
 * Called once per frame from Platform_Event_Handler().
 * Executes the current action in the scenario, advancing when wait counts expire.
 * Injects input through Platform_Keyboard_Buffer_Add_Key_Press() and User_Mouse_Handler().
 */
void HeMoM_Player_Frame(void);

/**
 * Return non-zero if the synthetic player is active (has a loaded scenario with actions remaining).
 */
int HeMoM_Player_Active(void);

/**
 * Shut down the synthetic player and free resources.
 */
void HeMoM_Player_Shutdown(void);



#ifdef __cplusplus
}
#endif

#endif /* HEMOM_PLAYER_H */
