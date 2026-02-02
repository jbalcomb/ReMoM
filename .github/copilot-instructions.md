# Copilot instructions — ReMoM

Purpose: help an AI coding agent be productive quickly in this reverse-engineered Master of Magic repo.

- **Big picture:** ReMoM is a C translation / reassembly of the 1994–95 game Master of Magic. Primary source trees live under `src/` and are grouped into logical libraries/modules such as `MOX` (platform/graphics/audio helpers), `STU` (utility helpers), and the main game code. The canonical build outputs live under `out/build/` (CMake presets produce these). See [README.md](README.md#L1-L10).

- **Key files / entry points:**
  - `CmakePresets.json` — official configure/build/test presets used by CI and local dev. Use the `MSVC-debug` preset on Windows. See [CmakePresets.json](CmakePresets.json#L1-L20).
  - `MK_MoM.BAT` — legacy, hand-crafted MSVC build steps showing source grouping and flags; useful for understanding target groupings and manual compiler flags. See [MK_MoM.BAT](MK_MoM.BAT#L1-L20).
  - `cmack.bat` — simple local helper that configures and builds via CMake; useful quick reference. See [cmack.bat](cmack.bat#L1-L10).
  - `doc/Devel-CMake.md` — notes and tips for configuring SDL2 and CMake on different platforms. See [doc/Devel-CMake.md](doc/Devel-CMake.md#L1-L20).

- **Build / test / debug (explicit commands you can run):**
  - Configure (Windows, recommended preset):
    - `cmake --preset MSVC-debug`
  - Build (using the preset):
    - `cmake --build --preset MSVC-debug`
    - or, to build from a path: `cmake --build out/build/MSVC-debug --config Debug`
  - Run unit tests (CTest) from the build tree:
    - `ctest --test-dir out/build/MSVC-debug -C Debug -V`
  - Open in Visual Studio: open the generated solution in `out/build/MSVC-debug` and use the usual VS debugger.
  - When running CMake commands, specifiy the full path C:\Program Files\CMake\bin\

- **Project-specific conventions / gotchas:**
  - File names and historical C-style naming: many files use upper-case `.C`/`.c` and legacy names (e.g., `MOM.C`, `MOX`); be careful on case-sensitive filesystems — see the repo note `cross-platform - ensure failure on case-sensitive filenames.txt`.
  - The codebase emulates the original game structure. You will see many direct translations from assembly/old code; look at the grouping in `MK_MoM.BAT` for how authors conceptually group sources into static libs (`MOX.lib`, `STU.lib`) before linking the final binary.
  - SDL2 is consumed either via FetchContent or via environment variables (`SDL2DIR`, `SDL2MIXERDIR`). See `doc/Devel-CMake.md` for platform notes.

- **Testing layout:** tests live under `MoM/tests` and are wired to GoogleTest via CMake FetchContent; the `testPresets` in `CmakePresets.json` point at the same workflow preset (`MSVC-debug`). Use the CTest command above to run tests from the build directory.

- **When changing build rules or adding deps:** prefer adding CMake changes to honor existing presets (update `CmakePresets.json`) and keep platform-specific fallbacks in `doc/Devel-CMake.md` for future contributors.

- **Examples of useful search targets for code patterns:**
  - `src/MOX` — platform helpers and graphics/audio glue.
  - `src/STU` — utility library used across the project.
  - `MK_MoM.BAT` — manual grouping of translation units and flags; good for mapping logical modules to CMake targets.

If any of these sections are unclear or you'd like more details (for example: a short FAQ for debugging failing tests or where to find game data files), tell me which area to expand.

**FAQ — Tests, debugging and game data**

- **How to run a single test binary:** change into the test output directory and run the exe. Example (PowerShell):

  ```powershell
  cd out/build/MSVC-debug/MoM/tests/Debug
  .\MOM_tests.exe
  ```

- **When tests fail immediately / crash:** most test binaries expect the game's data files (the `.lbx` files) available as the process working directory. The repo root contains all the LBX files. Run the test with CWD set to the repo root or copy the required `.lbx` files into the test working directory.

- **Quick reproduce and debug workflow:**
  - Build with the `MSVC-debug` preset: `cmake --preset MSVC-debug` then `cmake --build --preset MSVC-debug`.
  - Open the solution in Visual Studio from `out/build/MSVC-debug`.
  - Set the test executable (for example `MoM/tests/Debug/MOM_tests.exe`) as the startup project or create a debug target, and set its working directory to the repository root so the binary finds `.lbx` files.

- **Common causes of test failures:** missing data files (LBX), platform-specific SDL2 or mixer misconfiguration, or mismatched compile flags. Check `doc/Devel-CMake.md` for SDL2 setup and `MK_MoM.BAT` for historical compiler flags to match behavior.

- **Where to find game data files:** top-level `.lbx` files live in the repository root (e.g., `armylist.lbx`, `magic.lbx`, `main.lbx`, `spells.lbx`) — these are the resources the binaries expect at runtime. If creating CI/test runners, ensure those files are available in the runner working directory or copied into the build/test output directories.
