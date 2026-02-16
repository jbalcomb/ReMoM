diff --git a/c:\STU\devel\ReMoM\.github/copilot-instructions.md b/c:\STU\devel\ReMoM\.github/copilot-instructions.md
new file mode 100644
--- /dev/null
+++ b/c:\STU\devel\ReMoM\.github/copilot-instructions.md
@@ -0,0 +1,54 @@
+# GitHub Copilot Instructions for ReMoM
+
+## Project Context
+- ReMoM is a C reassembly of Master of Magic (v1.31). Behavior parity with the original game is more important than stylistic refactors.
+- Treat this as a classic/legacy C codebase with reverse-engineering constraints.
+- Prefer minimal, targeted changes over large rewrites.
+
+## Repository Architecture
+- `MoM/src`: game logic and screens (many files map to original overlays and offsets).
+- `MoX/src`: engine/platform/game-system layer (I/O, graphics, input, sound, data loading).
+- `STU/src`: support utilities and helpers.
+- `src/ReMoM.c`: executable entry point (`ReMoMber` target).
+- `tests/unit` + `tests/mocks`: GoogleTest-based unit tests and C stubs/mocks.
+
+## Coding Rules for Suggestions
+- Use C for `.c/.h` changes. Do not introduce C++ features into C modules.
+- Follow existing style:
+  - 4 spaces, no tabs.
+  - Allman braces.
+  - Keep existing naming conventions and file casing (many names mirror original binaries).
+  - Preserve CRLF-friendly formatting and avoid unnecessary whitespace churn.
+- Keep and respect reverse-engineering annotations/comments (`WZD`, `MGC`, `ovr###`, data-segment notes).
+- Do not "clean up" legacy globals, magic values, or odd control flow unless explicitly requested.
+- Prefer fixed-width types (`int16_t`, `uint16_t`, etc.) where existing code does.
+- Maintain C/C++ interop guards in headers (`extern "C"` blocks) when present.
+
+## Safety Constraints (Important)
+- Avoid behavioral changes that can break original-game parity.
+- Preserve binary/file-layout-sensitive code (`#pragma pack`, struct sizes, raw `fread/fwrite` sizes, offsets).
+- Avoid mass renames/moves of files or symbols.
+- Do not introduce new third-party dependencies unless explicitly requested.
+
+## Includes and Module Boundaries
+- Follow local include patterns already used in each module (often relative paths such as `../../MoX/src/...`).
+- Prefer using existing module APIs rather than bypassing layers.
+- When adding a new source file, also add it to the correct module `CMakeLists.txt`:
+  - `STU/src/CMakeLists.txt`
+  - `MoX/src/CMakeLists.txt`
+  - `MoM/src/CMakeLists.txt`
+  - and test `CMakeLists.txt` when applicable.
+
+## Build and Test Expectations
+- Build system is CMake; tests use GoogleTest.
+- Common commands:
+  - `cmake -S . --preset base`
+  - `cmake --build --preset base --target all`
+  - `ctest --test-dir build/base -C Debug --output-on-failure`
+- CI builds across Windows/Linux/macOS with `gcc`, `clang`, and `cl`; keep changes portable.
+- For isolated unit tests, prefer lightweight C stubs in `tests/mocks` rather than linking full game runtime.
+
+## Preferred Copilot Output
+- Produce focused patches with clear intent.
+- For new logic, suggest accompanying unit tests in `tests/unit`.
+- If behavior is uncertain, preserve current behavior and add TODO notes rather than guessing.
