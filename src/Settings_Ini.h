/*
    Settings_Ini -- the player's ReMoM.ini (doc/@AI_Plans/PLAN-ReMoM-Ini.md)

    One file is in effect, as a whole:
      - ReMoM.ini next to the program, if there is one (next to ReMoM.exe, or next to the .AppImage -- the AppImage launcher
        passes that folder as REMOM_PROGRAM_DIR);
      - otherwise ReMoM.ini in the config folder (%APPDATA%\ReMoM\, ~/.config/ReMoM/), created with every setting commented out
        if it does not exist.
    Each ReMoM.ini that exists gets the settings it is missing (commented out, after a backup), and ReMoM_example.ini beside it.

    Player build only. DeMoM and the tests read ReMoM.ini from the current folder, as before.
*/

#ifndef SETTINGS_INI_H
#define SETTINGS_INI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Find, create and update the settings files. Runs before logging starts (the file says where the log goes), so it logs
   nothing itself; call Settings_Ini_Log() once STU_LOG is running. */
void Settings_Ini_Startup(void);

/* The ReMoM.ini in effect, or NULL if there is none and none could be created (ReMoM then runs on its defaults). */
const char * Settings_Ini_Path(void);

/* Log what Settings_Ini_Startup() found and did. */
void Settings_Ini_Log(void);

#ifdef __cplusplus
}
#endif

#endif /* SETTINGS_INI_H */
