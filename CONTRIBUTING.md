# Contributing to ReMoM

Reassembly of Master of Magic v1.31 (Simtex, 1995)

## Project Overview

ReMoM is a C-based reassembly/reconstruction of the classic DOS game Master of Magic. The codebase is organized into several libraries that mirror the original binary structure.

### Directory Structure

- `MoM/src/` — Main game logic (MoM library: `momlib`)
- `MoX/src/` — Shared engine code (MoX library: `MOX`)
- `STU/src/` — Utility/debug code (STU library)
- `src/` — Application entry point (`ReMoMber` executable)
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

- **Functions:** `PascalCase` (e.g., `City_Maximum_Size_NewGame`)
- **Local variables:** `snake_case` (e.g., `map_square_count`)
- **Global variables:** `_prefixed_snake_case` (e.g., `_world_maps`, `_CITIES`)
- **Constants/macros:** `UPPER_SNAKE_CASE` (e.g., `WORLD_WIDTH`, `NUM_PLANES`)
- **Enum values:** Context-dependent prefixed (e.g., `tt_Grasslands1`, `bt_AlchemistsGuild`)

### Testing

- Use **Google Test** framework.
- Test files: `test_<FunctionName>.cpp`
- Test fixture class: `<FunctionName>_test`
- Test names: `TEST_F(<FunctionName>_test, <DescriptiveName>)`
- Follow Arrange/Act/Assert pattern with comments.
- Wrap C headers in `extern "C"` blocks in test files