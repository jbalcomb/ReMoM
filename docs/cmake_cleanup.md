# CMake Build System Cleanup

## Current Architecture

### Dependency Graph

```
ReMoMber (executable)
    ↓
[momlib, MOX, STU, Platform, SDL3::SDL3]

momlib (static, 78 sources)
    ↓
[MOX, STU, Platform]

MOX (static, 54 sources)
    ↓
[Platform]

STU (static, 18 sources)
    ↓
(no dependencies - standalone utility)

Platform (static, 19 sources)
    ↓
[SDL3::SDL3, SDL3_mixer::SDL3_mixer (optional)]
```

### Active CMakeLists.txt Files (13)

| File | Purpose |
|------|---------|
| CMakeLists.txt | Root — project setup, SDL3, GoogleTest, compiler flags, subdirs |
| src/CMakeLists.txt | ReMoMber executable, asset copying |
| platform/CMakeLists.txt | Platform static lib (SDL3 abstraction layer) |
| STU/CMakeLists.txt | Passthrough to STU/src and STU/tests |
| STU/src/CMakeLists.txt | STU static lib |
| STU/tests/CMakeLists.txt | STU unit tests |
| MoX/CMakeLists.txt | Passthrough to MoX/src and MoX/tests |
| MoX/src/CMakeLists.txt | MOX static lib |
| MoX/tests/CMakeLists.txt | MOX unit tests (6 tests) |
| MoM/CMakeLists.txt | Passthrough to MoM/src and MoM/tests |
| MoM/src/CMakeLists.txt | momlib static lib |
| MoM/tests/CMakeLists.txt | MoM unit tests (26 tests) |
| tests/CMakeLists.txt | Root-level tests (hello_test) |

### CMake Presets (6)

| Preset | Generator | Toolchain | Notes |
|--------|-----------|-----------|-------|
| base | Ninja | Clang (MinGW) | — |
| MSVC-debug | VS2022 x64 | MSVC | Debug |
| MSVC-nosound-debug | VS2022 x64 | MSVC | Debug, DISABLE_AUDIO=TRUE |
| Clang-Ninja-debug | Ninja | Clang | Debug |
| Clang-Ninja-coverage | Ninja | Clang | Coverage instrumentation |
| clang-nosound-debug | Ninja | Clang | Linux, no sound |

### Design Notes

- Only the Platform library links SDL3 directly; everything else goes through Platform
- GoogleTest v1.17.0 fetched via FetchContent with EXCLUDE_FROM_ALL
- SDL3_mixer is optional, controlled by `DISABLE_AUDIO` cmake variable
- DLL copying happens at configure-time (not build-time)
- Game assets (LBX files, CONFIG.MOM) copied post-build from /assets/

---

## Issues

### HIGH — Orphaned IDE-Specific CMakeLists (3 duplicates)

**Files:**
- IDE/VS2022-SDL2/MoM/tests/CMakeLists.txt
- IDE/VS2022-SDL3/MoM/tests/CMakeLists.txt
- IDE/VS2022-Static/MoM/tests/CMakeLists.txt

These are identically duplicated across three VS2022 variants. They reference SDL2 (not SDL3), link GTest::gmock differently than the main build, have older test file lists, and use their own DLL/asset copy commands. Almost certainly stale.

### HIGH — Hard-Coded SDL3 Path

Root CMakeLists.txt lines 35 and 38 hard-code `C:/devellib/SDL3-3.4.2`. Not portable, breaks on any other machine or CI.

### HIGH — Broken Install Rules

Root CMakeLists.txt lines 82-105 reference `SDL3_DLL_FILE_PATH` and `SDL3_MIXER_DLL_FILE_PATH` which are never defined. The install target is non-functional.

### MEDIUM — Commented-Out Dead Code

- Line 4: `# set(ENV{CTEST_PARALLEL_LEVEL} 2)` with comment "noworkie?"
- Lines 15-18: Commented CTest parallel configuration
- Lines 83-84: Commented install targets

### MEDIUM — Undefined REMOM_TEST_PATH_ENV

Referenced in tests/CMakeLists.txt and STU/tests/CMakeLists.txt but never defined anywhere in the CMake files. May have been lost during a refactor.

### MEDIUM — Overly Broad momlib PUBLIC Include

momlib declares `${PROJECT_SOURCE_DIR}/src` as a PUBLIC include directory, exposing the entire /src tree to any consumer of momlib.

### MEDIUM — Inconsistent Test Setup

- Some tests link GTest::gmock, others don't
- gtest_discover_tests() usage varies
- No shared pattern/module for test configuration

### LOW — No Custom .cmake Modules

Repeated patterns (DLL copying, test setup, source file listing) could be factored into reusable .cmake modules.

### LOW — Global Warning Suppression

`/wd4068` (suppress unknown pragma warning) applied globally with no explanatory comment.

### LOW — Unused win32 Platform Directory

platform/win32/ exists with source files but is not referenced in any CMakeLists.txt. Appears abandoned or not yet wired in.

---

## TODO

- [ ] Remove or archive the 3 orphaned IDE-specific CMakeLists files
- [ ] Parameterize the SDL3 path (cache variable with default, or proper find_package)
- [ ] Fix or remove broken install rules (define the DLL path variables or delete the section)
- [ ] Remove commented-out dead code from root CMakeLists.txt
- [ ] Define REMOM_TEST_PATH_ENV or remove references to it
- [ ] Tighten momlib PUBLIC include directory scope
- [ ] Standardize test CMakeLists patterns (gmock linking, gtest_discover_tests)
- [ ] Add comment explaining /wd4068 suppression
- [ ] Decide fate of platform/win32/ directory (wire in or remove)
- [ ] Consider extracting repeated patterns into .cmake modules
