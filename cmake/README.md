# CMake Helpers (ReMoM)

This folder contains shared helper modules used by project targets.

## Why this was done

- Reduce repeated CMake blocks across `ReMoMber`, `momlib`, `MOX`, `STU`, and test targets.
- Keep compile behavior consistent across targets and presets.
- Make support-file copying declarative so each target states only what it needs.
- Lower maintenance risk when changing warning policy, debug defines, or runtime-file staging.

## How it works

- Top-level `CMakeLists.txt` includes the helper modules once.
- Target CMake files call helper functions after target creation.
- The helpers expand to normal `target_compile_definitions()`, `target_compile_options()`, and `add_custom_command(TARGET ... POST_BUILD ...)` calls.
- Generator expressions (for example `$<$<CONFIG:Debug>:...>`) are evaluated per target and per configuration, so `Debug` gets debug-only defines.

## Shared compile definitions

File: `ReMoM_Common_Compile_Definitions.cmake`

Function:
- `ReMoM_Apply_Common_Compile_Definitions(<target>)`

Applies project-wide definitions to a target:
- `_CRT_SECURE_NO_WARNINGS=TRUE`
- `STU_DEBUG=TRUE` in `Debug`
- `STU_TRACE=TRUE` in `Debug`
- `_STU_SDL2=TRUE` in `Debug`

Notes:
- Why: these were duplicated across multiple targets and could drift.
- How: one function call emits the same definition set for each target.

## Shared compile options

File: `ReMoM_Common_Compile_Options.cmake`

Function:
- `ReMoM_Apply_Common_Compile_Options(<target>)`

Applies compiler-family warning options to a target:
- Clang option set
- GCC option set
- MSVC option set

Notes:
- Why: warning policy should be consistent and maintained in one place.
- How: helper checks compiler family and applies the matching option list.

## Shared post-build support-file copies

File: `ReMoM_PostBuild_Copies.cmake`

Function:
- `ReMoM_Add_Target_Support_Files(<target> [flags...])`

Available flags:
- `COPY_SDL2`          — copy `SDL2.dll` (Windows)
- `COPY_SDL2_MIXER`    — copy `SDL2_mixer.dll` (Windows, only when audio is enabled)
- `COPY_SDL2D`         — copy `SDL2d.dll` (Windows)
- `COPY_GMOCK`         — copy `gmock.dll` (Windows)
- `COPY_LBX`           — copy all root `*.lbx` files

Notes:
- Why: post-build copy blocks were repeated and hard to keep in sync.
- How: each flag maps to a conditional post-build copy command; `COPY_SDL2_MIXER` additionally respects `DISABLE_AUDIO`.

### Example

```cmake
ReMoM_Add_Target_Support_Files(MOM_tests
    COPY_SDL2
    COPY_SDL2_MIXER
    COPY_SDL2D
    COPY_GMOCK
    COPY_LBX
)
```

## Typical target pattern

```cmake
ReMoM_Apply_Common_Compile_Definitions(MyTarget)
ReMoM_Apply_Common_Compile_Options(MyTarget)
ReMoM_Add_Target_Support_Files(MyTarget COPY_SDL2 COPY_SDL2D)
```
