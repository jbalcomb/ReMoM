# Contributing to ReMoM

Reassembly of Master of Magic v1.31 (Simtex, 1995)

ReMoM remakes Master of Magic by disassembling the original executables, then reassembling and translating them to C. The goal is working source code for the game, so the community can improve and expand the Simtex universe to their hearts' content. The reconstruction aims for 100% fidelity to the original program: the disassembly is the ground truth, and original-game bugs are kept, not fixed.

The project's home is the [#mom-reverse-engineering](https://discord.gg/kSNA8bE6Wt) channel on the *Master of Magic – Fans* Discord server.

## Where to find things

| Document | What it covers |
|----------|----------------|
| [BUILDING.md](BUILDING.md) | Compiling ReMoM: prerequisites, SDL, CMake presets, Windows and Linux |
| [AGENTS.md](AGENTS.md) | The house rules for editing reconstructed game code, naming and style (for people and AI agents alike) |
| [TESTING.md](TESTING.md) | The diagnostic build, logs and crash reports (for testers) |
| [RELEASES.md](RELEASES.md) | Cutting, publishing and rolling back a release |
| [PLAYING.md](PLAYING.md) | The player's guide: data files, settings, save locations |
| [doc/](doc/) | Notes on the game's internals, screen by screen and system by system |
| [doc/@Devel/](doc/@Devel/) | Developer guides: DeMoM and the Automation Driver, logging, graphics, crash reports |
| [doc/@AI_Plans/](doc/@AI_Plans/) | Product requirements and implementation plans |

## Project Overview

ReMoM is a C-based reassembly/reconstruction of the classic DOS game Master of Magic. The codebase is organized into several libraries that mirror the original binary structure.

### Directory Structure

- `MoM/src/` — Main game logic (MoM library: `momlib`)
- `MoX/src/` — Shared engine code (MoX library: `MOX`)
- `STU/src/` — Utility/debug code (STU library)
- `src/` — Application entry point (`ReMoM` executable)
- `MoM/tests/` — Google Test unit tests for MoM
- `MoX/tests/` — Google Test unit tests for MoX
- `tests/` — Top-level test targets

## Language & Compiler

- **Source language:** C (compiled as C)
- **Test language:** C++ (compiled as C++17 for Google Test compatibility)
- **Build system:** CMake (minimum 3.22)
- **Toolchain:** MinGW-w64 (GCC) or MSVC via Visual Studio 2022

## Building

After modifying `CMakeLists.txt` (e.g., adding new test source files), rebuild the project using:

```
cmake --workflow --preset=MSVC-debug
```

## Running Tests

After rebuilding, run specific tests using `ctest` with a name filter:

```
ctest --preset=MSVC-debug -R "<test name>"
```

For example, to run all `Square_Food2_NewGame` tests:   

```
ctest --preset=MSVC-debug -R "Square_Food2_NewGame"
```

To run all tests:

```
ctest --preset=MSVC-debug
```

## Developer hotkeys

- **Ctrl+Alt+Shift+Q** quits the game. All three modifiers are required, in every backend (SDL2, SDL3, Win32), so a Q typed with
  only one of them, such as a capital Q in a name, reaches the game as a Q. It exits at once, from any screen, without saving.
  It is not a player feature and is not in `PLAYING.md`.
- **Checking quit on Windows:** `python3 tools/window_quit_check.py out/build/MSVC-debug/bin/Debug/ReMoM.exe all` launches the build and
  sends the window's X, Ctrl+Alt+Shift+Q and Alt+Q as real OS input, then checks that the first two exit at once, the third does not,
  and the captured cursor is released. Works for the SDL2, SDL3 and Win32 builds; don't type while it runs.

## Coding Standards

### Types

- Use **fixed-width integer types** from `<stdint.h>`: `int8_t`, `int16_t`, `int32_t`, `uint8_t`, `uint16_t`, `uint32_t`.
- Use `byte_ptr` (`unsigned char *`) and `SAMB_ptr` (`unsigned char *`) for raw memory/segment pointers.
- Avoid plain `int`, `short`, `long` in game logic code.

### Structs

- Use `#pragma pack(push)` / `#pragma pack(2)` / `#pragma pack(pop)` for all game data structures to match the original 16-bit DOS memory layout.
- Document struct size in a comment: `// sizeof: XXh YYd`
- Document field offsets with hex comments: `/* 00 */`, `/* 02 */`, etc.
- Mark 2-byte alignment padding explicitly: `/* 2-byte alignment padding */`

### Header Files

- Use `#ifndef FILENAME_H` / `#define FILENAME_H` / `#endif /* FILENAME_H */` include guards.
- Wrap all function declarations in `extern "C"` blocks for C++ compatibility:
  ```c
  #ifdef __cplusplus
  extern "C" {
  #endif

  // declarations here

  #ifdef __cplusplus
  }
  #endif
  ```

### Naming Conventions

[AGENTS.md](AGENTS.md) is the authority on naming; this is a summary.

- **Functions:** `Proper_Snake_Case` (e.g., `Simulate_World_Map_Generation`, `City_Maximum_Size_NewGame`)
- **Variables:** `lower_snake_case` (e.g., `map_square_count`)
- **Global variables:** new globals take a `g_` prefix (e.g., `g_graphics_cache_seg`). Names starting with `_` are reserved in C, so don't add new ones; the reconstructed globals keep their existing names (e.g., `_world_maps`, `_CITIES`).
- **Constants and macros:** `UPPER_SNAKE_CASE` (e.g., `WORLD_WIDTH`, `NUM_PLANES`)
- **Enum values:** a short prefix for the enum, then the name (e.g., `tt_Grasslands1`, `bt_AlchemistsGuild`)
- **Shared modules:** name them by what they do, not by the first program that uses them. A `DeMoM_*` prefix on code that ReMoM also links reads as a layering violation.

### Testing

- Use **Google Test** framework.
- Test files: `test_<FunctionName>.cpp`
- Test fixture class: `<FunctionName>_test`
- Test names: `TEST_F(<FunctionName>_test, <DescriptiveName>)`
- Follow Arrange/Act/Assert pattern with comments.
- Wrap C headers in `extern "C"` blocks in test files