# PLAN: one documented `ReMoM.ini`

2026-11-23. Follows a player report: settings written to `~/.config/ReMoM/ReMoM.ini` were ignored by the AppImage, because the AppImage sat
next to the game files and its launcher silently switched to "portable mode", which reads `remom_app_dir/ReMoM.ini` instead.

## The rule

The player-facing story, the same for every download (Windows installer, Windows zip, `.deb`, AppImage):

- **Your settings are in `ReMoM.ini` in the config folder**: `%APPDATA%\ReMoM\ReMoM.ini` on Windows, `~/.config/ReMoM/ReMoM.ini` on Linux.
- **A `ReMoM.ini` next to the program, if you make one, is used instead**, as a whole file. "Next to the program" is the folder holding
  `ReMoM.exe` on Windows and the folder holding the `.AppImage` file for the AppImage. The two files are never merged: one whole file wins.
- **If neither exists, ReMoM creates the one in the config folder**, every setting in it commented out at its default.
- **When a release adds settings**, ReMoM backs up each `ReMoM.ini` that exists (either or both) as `ReMoM_backup_YYYYMMDDHHMMSS.ini`, then
  adds the missing settings to it, commented out with their defaults and an explanation. Nothing else in the file changes.
- **`ReMoM_example.ini`** sits next to each `ReMoM.ini`: every setting at its standard default, live, with its explanation. ReMoM never
  reads it, and rewrites it whenever the release's version differs.

Decisions behind it, so they are not re-argued:

| Decision | Why |
|---|---|
| The config folder is the documented home | It is the only place every download can write without admin rights, it survives updates and uninstalls, and it is one sentence per OS. Scored 18/24 against 8/24 for "next to the program"; its one weakness, being hard to find, is answered by creating the file and the example. |
| The local file is a secondary, whole-file override | AppImage and zip players put the program next to their game files and look there. Whole-file, not per-key: what you see in the one file is what you get. |
| Missing settings are added commented out | A live default would freeze each player on the default of the day the setting was added; commented, the setting is visible and documented and a later default change still reaches them. A key counts as present whether live or commented, so a player who deletes a line on purpose doesn't get it back. |
| Backups only when something is added | Normal launches never create files. |
| No more AppImage "portable mode" | Players never asked for it. Game files next to the AppImage are still used as the game data; settings, saves and logs go to the normal per-user places. |

## Scope

**Player build only (`ReMoM`).** DeMoM, the tests and the matchup tools keep reading `ReMoM.ini` from the current folder: their CTest
fixtures and `--newgame` templates depend on it.

**Every player read goes through the one resolved file.** Today the player build reads `ReMoM.ini` from two places:

| Setting | Read from today | After |
|---|---|---|
| `[Paths] log_dir`, `save_dir` | the current folder | the resolved file |
| `[Logging]` | the current folder | the resolved file |
| `[Video]` | the current folder, then the config folder, per key | the resolved file |
| `[Paths] game_data` | the config folder | the resolved file |

**The dev build keeps working.** `copy_assets` stages `assets/ReMoM.ini` next to the dev `ReMoM.exe`, so it is the dev build's local file:
logs and saves next to the exe (`log_dir = .`, `save_dir = .`), as now. `copy_assets` overwrites it every build, so the upsert never
accumulates there.

**No download ships a `ReMoM.ini`** (root `CMakeLists.txt` install rules, the release workflow). That stays: a shipped one would be a
local file and override every player's config-folder settings.

**The player settings** (the schema the upsert and the example are built from) are the documented ones only:
`[Paths] game_data`, `[Video] startup_window`, `scale_mode`, `aspect_ratio`. The dev keys (`log_dir`, `save_dir`, `[Logging]`, the
`--newgame` sections) are read as before but never added to a player's file.

## Phases

### Phase 1 — `STU_INI`: the file operations (library)

New `STU/src/STU_INI.c` / `.h`, generic (no ReMoM names), unit-tested in `STU/tests/test_STU_INI.cpp`:

- `STU_INI_Has_Key(path, section, key)`: the key appears in the section, live or commented out (`;` or `#`).
- `STU_INI_Add_Missing(path, settings[], count, backup_path_out)`: for each setting whose key is absent, append its block at the end of its
  section (a missing section is added at the end of the file). Writes a timestamped backup first, and only if something is added. Keeps
  the file's line endings; touches no other line.
- `STU_INI_Write_If_Different(path, text)`: for the example file.
- `STU_INI_Choose(local_path, config_path)`: which file is in effect (local if it exists, else config).

Each setting is `{ section, key, default_value, explanation }`.

### Phase 2 — ReMoM uses the one file

- New `src/Settings_Ini.c` / `.h`: the player-setting table, the text of the new file and of `ReMoM_example.ini`, and
  `Settings_Ini_Startup()`, which resolves the file, creates it if neither exists, adds missing settings to each existing `ReMoM.ini`,
  and writes the example beside each. Silent until logging is up; it keeps what it did and logs it once `STU_LOG` is running.
- "Next to the program": `REMOM_PROGRAM_DIR` if set (the AppImage launcher sets it to the `.AppImage`'s folder), else
  `STU_GRAF_Executable_Dir()`.
- `ReMoM.c`: every read in the table above takes the resolved path. The log says which file is in effect and why
  (`next to the program` / `config folder` / `created`), any backup made, and each `[Video]` value with `(ReMoM.ini)` or `(default)`.
- `STU_GRAF`: `[Paths] game_data` is read from the resolved file (`STU_GRAF_Set_Settings_Ini()` before `STU_GRAF_Init()`), falling back
  to the config folder when unset, so STU's own tests are unchanged.

### Phase 3 — AppImage launcher

`packaging/AppRun`:
- Stop setting `XDG_*_HOME` and `REMOM_APP_DIR`. Keep `REMOM_DATA_DIR` when the game files are next to the `.AppImage`.
- Export `REMOM_PROGRAM_DIR` = the `.AppImage`'s folder.
- One-time move from an existing `remom_app_dir/`: copy `ReMoM.ini` to the config folder and the saves and `MAGIC.SET` to
  `~/.local/share/ReMoM/`, never overwriting a file already there; then leave `remom_app_dir/MOVED.txt` saying where they went, so it
  runs once.

### Phase 4 — Docs

README settings section, `PLAYING.md`, `packaging/ReMoM.6`, `doc/@Devel/Devel-Linux-AppImage.md` (portable mode removed),
the comments in `STU_GRAF.h` and the NSIS template that describe where the ini is.

## Checks

- Unit: `test_STU_INI` (has-key live/commented, add-missing into existing and new sections, no change → no backup, CRLF kept, choose).
- Build `MSVC-debug`, `MSVC-sdl3-nosound-debug`, `MSVC-win32-debug`, 0 warnings; `ctest -R "DeMoM_|Viewport|STU"`.
- Windows offscreen runs with `APPDATA` pointed at a scratch folder: none → created; config only; local only; both → local wins and both
  get the additions; an old file without `[Video]` → backup + additions; second run → no new backup.
- AppImage on the Linux workstation: game files next to it, settings in `~/.config/ReMoM/ReMoM.ini` read; `remom_app_dir/` moved once.
