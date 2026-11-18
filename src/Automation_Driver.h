/**
 * Automation_Driver.h — Automation Driver: scripted input for windowless and windowed runs.
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
 *   click <x> <y> | <Screen.Alias> — left-click at game coordinates or a named field
 *   rclick <x> <y> | <Screen.Alias>— right-click at game coordinates or a named field
 *   move <x> <y> | <Screen.Alias>  — warp the cursor there (no click) — hover / demo motion
 *   wait_screen <name> [timeout]   — block until current_screen == scr_<name>
 *   wait_turn <n> [timeout]        — block until the strategic turn reaches n
 *   wait_field <Screen.Alias> [t]  — block until a field covers that named point
 *   next_turn                      — press 'N' (Next Turn hotkey)
 *   quit                           — press Escape twice (quit to menu, then quit game)
 *   end                            — stop the Automation Driver (idle forever or exit)
 */

#ifndef AUTOMATION_DRIVER_H
#define AUTOMATION_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif



/**
 * Load a test scenario from a .hms file.
 * @param filepath  Path to the scenario file.
 * @return  0 on success, non-zero on failure.
 */
int Automation_Driver_Load_Scenario(const char *filepath);

/**
 * Set the path to the named-action points table (Screen.Alias -> click point),
 * baked by `python -m tools.field_catalog.resolver export`. Overrides the default
 * "tools/fields/alias_points.fwv". Call before Automation_Driver_Load_Scenario.
 */
void Automation_Driver_Set_Alias_Points_Path(const char *filepath);

/**
 * Called once per frame from Platform_Event_Handler().
 * Executes the current action in the scenario, advancing when wait counts expire.
 * Injects input through Platform_Keyboard_Buffer_Add_Key_Press() and User_Mouse_Handler().
 */
void Automation_Driver_Frame(void);

/**
 * Return non-zero if the Automation Driver is active (has a loaded scenario with actions remaining).
 */
int Automation_Driver_Active(void);

/**
 * Shut down the Automation Driver and free resources.
 */
void Automation_Driver_Shutdown(void);



#ifdef __cplusplus
}
#endif

#endif /* AUTOMATION_DRIVER_H */
