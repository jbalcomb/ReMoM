## Plan: Cross-Platform Platform Layer Skeleton

Build a minimal but production-oriented platform layer skeleton that separates OS concerns from game logic, uses SDL2 on Linux/macOS and native Win32 on Windows, and preserves gameplay behavior while allowing balanced modernization. The implementation should focus on extracting stable interfaces first, wiring build selection cleanly, and validating parity with smoke tests before deeper subsystem rewrites.

**Steps**
1. Phase 1 - Baseline and Boundaries
2. Confirm and document existing platform boundary symbols to preserve during extraction: Startup_Platform, Shudown_Platform, Platform_Event_Handler, Platform_Get_Millies, Platform_Sleep_Millies, Platform_Video_Update, Platform_Palette_Update.
3. Define explicit include/exclude scope for this first delivery:
- In scope: platform API headers, backend stub implementations, CMake source selection, startup wiring, compile guards, smoke tests.
- Out of scope: full graphics rewrite, save format redesign, deep UI scaling modernization.
4. Add a lightweight architecture note in the repo docs describing module ownership and call flow from main loop to platform backend.
5. Phase 2 - Introduce Platform API Surface
6. Create a dedicated platform API folder with subsystem headers for video, input, timer, audio, and filesystem designed around current call sites (not idealized future APIs).
7. Keep names and argument shapes close to existing usage where possible to reduce churn; add compatibility wrappers where signatures must differ. Depends on step 2.
8. Add backend-neutral context struct(s) for runtime state ownership (window, renderer/surface, timers, input state, audio state) to remove reliance on global backend-specific variables.
9. Add compile-time backend selector defines and central backend dispatch module that routes API calls to SDL2 or Win32 implementations.
10. Phase 3 - Backend Skeleton Implementations
11. Implement SDL2 backend skeleton files for Linux/macOS with initialization, event pumping, frame present hook, sleep/timer hook, and audio init stubs. Parallel with step 12 after step 9.
12. Implement Win32 backend skeleton files for Windows with equivalent hooks and no-op/stub behavior where migration details are pending. Parallel with step 11 after step 9.
13. Move direct backend includes out of shared headers where possible; keep backend headers private to backend implementation files.
14. Add strict compile guards so mixed backend symbols cannot compile into a single translation unit accidentally.
15. Phase 4 - Build System Integration
16. Update platform and MoX CMake wiring to choose backend source sets per host OS (SDL2 for Linux/macOS, Win32 for Windows) instead of compiling both families by default.
17. Keep existing top-level targets intact (ReMoMber, MOM_tests) and link through the same higher-level libraries so rollout is low-risk.
18. Preserve existing post-build support file behavior and test data staging while adding any needed platform-specific runtime env handling for tests.
19. Add/adjust CMake presets for Linux/macOS/Windows developer workflows that exercise the selected backend path.
20. Phase 5 - Incremental Subsystem Hookover
21. Redirect startup/shutdown in the executable entry path to use the new platform dispatch layer first; preserve legacy behavior behind compatibility wrappers. Depends on phases 2-4.
22. Hook timer and event-pump usage to platform API and validate frame pacing remains behaviorally close to current gameplay expectations.
23. Hook filesystem access behind the platform filesystem adapter at primary resource entry points (LBX and save/load wrappers) without changing file formats yet.
24. Hook audio init/play path through platform audio adapter while keeping current SOUND_ENABLED/SOUND_DISABLED behavior.
25. Keep graphics data path unchanged initially (320x200 indexed pipeline) and only route present/palette boundaries through the new API in this phase.
26. Phase 6 - Verification and Hardening
27. Add compile verification matrix for Linux, macOS, and Windows ensuring only the intended backend source set compiles per platform.
28. Add smoke tests for startup, resource loading, basic input response, and frame progression on each OS.
29. Add targeted regression checks for case-sensitive asset lookup and path handling to prevent Linux/macOS runtime failures.
30. Add logging with contextual information on backend init, backend selection, and platform failures to simplify triage.
31. Document known gaps and next-phase tasks (graphics internals extraction, input/graphics decoupling for cursor rendering, timing modernization options).

**Relevant files**
- /home/jbalcomb/devel/ReMoM/src/ReMoM.c — executable entry and platform startup/shutdown call flow.
- /home/jbalcomb/devel/ReMoM/MoX/src/MOM_PFL.h — current cross-module platform function declarations to preserve/bridge.
- /home/jbalcomb/devel/ReMoM/MoX/src/sdl2_PFL.c — SDL2 event/video/platform behavior to map into backend implementation.
- /home/jbalcomb/devel/ReMoM/MoX/src/sdl2_Timer.c — timer/frame pacing behavior to preserve during adapter insertion.
- /home/jbalcomb/devel/ReMoM/MoX/src/sdl2_Audio.c — SDL2 audio behavior for adapter routing.
- /home/jbalcomb/devel/ReMoM/MoX/src/win_PFL.cpp — Win32 platform init/event handling reference backend.
- /home/jbalcomb/devel/ReMoM/MoX/src/win_Input.c — Win32 input behavior reference.
- /home/jbalcomb/devel/ReMoM/MoX/src/win_CLK.c — Win32 timing reference.
- /home/jbalcomb/devel/ReMoM/MoX/src/GFILE.c — filesystem wrapper entry points for adapter hookover.
- /home/jbalcomb/devel/ReMoM/MoX/src/LBX_Load.c — asset loading path and filename normalization risks.
- /home/jbalcomb/devel/ReMoM/CMakeLists.txt — top-level target orchestration and test env plumbing.
- /home/jbalcomb/devel/ReMoM/MoX/src/CMakeLists.txt — backend source-set selection and linkage changes.
- /home/jbalcomb/devel/ReMoM/cmake/ReMoM_Common_Compile_Definitions.cmake — compile definitions for backend selection.
- /home/jbalcomb/devel/ReMoM/cmake/ReMoM_PostBuild_Copies.cmake — support file copy behavior to preserve.
- /home/jbalcomb/devel/ReMoM/CMakePresets.json — cross-platform developer/build presets.

**Verification**
1. Configure and build on Linux with SDL2 backend selected; verify no Win32 platform objects are compiled.
2. Configure and build on macOS with SDL2 backend selected; verify startup and event loop smoke run.
3. Configure and build on Windows with Win32 backend selected; verify SDL2-only files are excluded unless intentionally required.
4. Run MOM_tests and startup smoke on each platform and compare baseline behavior for timing/input responsiveness.
5. Validate resource load and save/load smoke paths with existing LBX and save files.
6. Validate case-sensitive filename handling on Linux/macOS using representative asset names.

**Decisions**
- Selected delivery: minimal platform layer skeleton in code first.
- Backend strategy: SDL2 for Linux/macOS, native Win32 on Windows.
- Compatibility target: balanced; preserve gameplay behavior while allowing practical modernization at platform boundaries.
- Excluded in this phase: full renderer rewrite, deep input remap redesign, save format evolution.

**Further Considerations**
1. Backend convergence path: decide whether to keep dual backend long-term or converge to SDL2 on Windows in a later phase to reduce maintenance.
2. Path normalization policy: choose canonical lowercase lookup strategy versus strict on-disk naming enforcement for assets.
3. Timing policy: choose whether to keep default 55ms frame pacing by default with optional modern cap, or shift default after parity milestone.
