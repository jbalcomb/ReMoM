# GitHub Copilot Instructions for ReMoM

## Project Overview

ReMoM is a **C-language reassembly** of Master of Magic v1.31 (Simtex, 1995). The codebase reconstructs the original 16-bit DOS executable from disassembly, preserving original logic, data layouts, and naming where known.

## Language & Build

- **Source code:** C (not C++)
- **Tests:** C++17 (Google Test)
- **Build system:** CMake 3.22+
- **Toolchains:** MinGW-w64 (GCC) or MSVC (Visual Studio 2022)
- **Platform dependencies:** SDL2, SDL2_mixer

## Directory Structure

| Directory     | Purpose                        | CMake Target  |
|---------------|--------------------------------|---------------|
| `MoM/src/`    | Main game logic                | `momlib`      |
| `MoX/src/`    | Shared engine code             | `MOX`         |
| `STU/src/`    | Utility / debug code           | `STU`         |
| `src/`        | Application entry point        | `ReMoMber`    |
| `MoM/tests/`  | Unit tests for MoM             | `MOM_tests`   |
| `MoX/tests/`  | Unit tests for MoX             | `MOX_tests`   |

## Types — Always Use Fixed-Width

- Defined via `<stdint.h>` (wrapped in `MoX/src/MOX_TYPE.h`).
- **Never** use bare `int`, `short`, or `long` in game logic.
- Use `byte_ptr` / `SAMB_ptr` (`unsigned char *`) for raw memory pointers.

## Struct Layout

All game data structures must match the original 16-bit DOS memory layout:

- Always use `#pragma pack(push)` / `#pragma pack(2)` / `#pragma pack(pop)`.
- Document `sizeof` in hex and decimal.
- Comment field offsets with `/* XX */`.
- Mark alignment padding: `/* 2-byte alignment padding */`.

## Naming Conventions

| Element            | Convention                              | Example                              |
|--------------------|-----------------------------------------|--------------------------------------|
| Functions          | `Pascal_Snake_Case`                     | `Do_City_Calculations`, `Kill_Unit`  |
| Structs            | `s_` prefix                             | `struct s_UNIT`, `struct s_CITY`     |
| Enums (type)       | `e_` prefix                             | `enum e_BUILDING_TYPE`               |
| Enum values        | `UPPER_SNAKE_CASE` with short prefix    | `bt_Barracks`, `mr_Chaos`            |
| String constants   | `cnst_` prefix                          | `cnst_Alchemy`                       |
| Global arrays      | `_` prefix                              | `_UNITS`, `_CITIES`, `_ITEMS`        |
| Macros             | `UPPER_SNAKE_CASE`                      | `ST_TRUE`, `COMBAT_GRID_WIDTH`       |
| Boolean values     | `ST_TRUE` (1) / `ST_FALSE` (0)          | —                                    |
| WIP functions      | `__WIP` suffix                          | `Combat_Screen__WIP`                 |
| Stub functions     | `__STUB` suffix                         | `WIZ_Get_Astr_Power__STUB`           |

## Header File Template

- Include guard: `FILENAME_H` (uppercase, underscores).
- Always wrap declarations in `extern "C"` for C++ compatibility.
- Use relative `#include` paths from the source file.

## Comments & Disassembly Cross-References

- Preserve original disassembly references.
- Use `// TODO`, `// HACK`, `// DEDU` (deduction) tags.

## Testing Conventions

### Structure

- Framework: **Google Test** v1.14.0
- Test files: `test_<FunctionName>.cpp` in the `tests/` directory
- Tests are added as **source files** to the existing test executable (e.g., `MOM_tests`), **not** as separate CMake targets

### Test File Template

- Follow **AAA** (Arrange, Act, Assert) pattern.
- Use `EXPECT_EQ`, `EXPECT_STREQ`, `ASSERT_EQ`.
- Name test groups `FunctionName_test` with descriptive scenario names.

### Registering New Test Files

Add the `.cpp` file to the existing executable in `CMakeLists.txt`:

Do **not** add a separate `gtest_discover_tests()` call for individual source files.

## Key Design Principles

1. **Preserve original behavior** — match the original game logic exactly.
2. **Match original data layout** — structs must be the same size and alignment as the DOS binary.
3. **Document deviations** — if changing original logic, explain why in a comment.
4. **Platform abstraction** — SDL2 replaces DOS-specific code (VGA, sound, input) in `MoX/src/`.
