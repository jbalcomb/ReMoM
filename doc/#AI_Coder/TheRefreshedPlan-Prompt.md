## Plan: Generic Simtex SDL2 Rewrite Roadmap

Formalize the current ReMoM architecture as the baseline, then refactor toward a true layered rewrite: Platform (SDL2 only) -> Generic Simtex engine -> Game adapters/data packs -> Executable. The recommended approach is evolutionary rather than greenfield: preserve proven reconstruction logic, isolate hard boundaries first, then introduce a game adapter seam so Master of Magic remains the first shipped game while MoO-family support becomes structurally possible later.

**Steps**
1. Phase 1 - Baseline and scope lock: inventory the current public seams and decide the rewrite contract. Treat Platform, MoX, MoM, and STU as candidate layers, but explicitly define which APIs survive. Reuse the existing target split in /home/jbalcomb/STU/devel/ReMoM/CMakeLists.txt, /home/jbalcomb/STU/devel/ReMoM/platform/CMakeLists.txt, /home/jbalcomb/STU/devel/ReMoM/MoX/CMakeLists.txt, /home/jbalcomb/STU/devel/ReMoM/MoM/CMakeLists.txt, and /home/jbalcomb/STU/devel/ReMoM/STU/CMakeLists.txt as the starting dependency graph.
2. Phase 1 - Document boundary violations to eliminate first. Use /home/jbalcomb/STU/devel/ReMoM/doc/MoX-SimtexGameEngine.md, /home/jbalcomb/STU/devel/ReMoM/doc/000-MoX_vs_MoM.md, and /home/jbalcomb/STU/devel/ReMoM/doc/Devel-PlatformLayer-AiPrompt.md to enumerate where MoX depends on MoM, where screen control leaks game concepts into engine code, and where platform backends still call non-platform functions. This step blocks the rest because the rewrite target is unclear until these leaks are named precisely.
3. Phase 2 - Freeze the platform contract. Make /home/jbalcomb/STU/devel/ReMoM/platform/include/Platform.h, /home/jbalcomb/STU/devel/ReMoM/platform/include/Platform_Keys.h, and /home/jbalcomb/STU/devel/ReMoM/platform/include/Platform_Replay.h the only allowed OS-facing surface. Standardize on SDL2 for the rewrite path, treat SDL3 and Win32 as secondary backends, and move any lingering SDL-specific code out of MoX or src/. This phase can run in parallel with step 4 once the platform API list is agreed.
4. Phase 2 - Define the engine core that is truly game-agnostic. Start from the MoX modules already closest to generic behavior: allocation, timers, LBX loading, graphics primitives, palette/video/page management, fonts, input routing, mouse, sound abstraction, animation, and EMM compatibility. Use /home/jbalcomb/STU/devel/ReMoM/doc/MoX-SimtexGameEngine.md and /home/jbalcomb/STU/devel/ReMoM/doc/STU_Extract.md to separate reusable engine services from MoM-only tables, enums, and structs.
5. Phase 2 - Introduce a game adapter seam. Replace the current implicit dependency on global game symbols with a formal interface owned by the game layer: screen registration and screen flow, game init/bootstrap, asset table registration, game state lifecycle, save/load hooks, simulation tick/turn entrypoints, and game-specific UI draw callbacks. The existing Screen_Control/current_screen dependency cluster is the first concrete place to cut this seam because it currently drags most screens into engine code.
6. Phase 3 - Repackage Master of Magic as the first adapter. Move MoM-specific types, enums, static data tables, spells, combat, diplomacy, AI, and UI screens behind the new game interface instead of letting them leak into MoX headers. Use /home/jbalcomb/STU/devel/ReMoM/MoM/src/INITGAME.c, /home/jbalcomb/STU/devel/ReMoM/MoM/src/NEXTTURN.c, /home/jbalcomb/STU/devel/ReMoM/MoM/src/Combat.c, and /home/jbalcomb/STU/devel/ReMoM/MoM/src/MoM_SCR.h as anchor points for bootstrap, turn loop, tactical combat, and screen ownership.
7. Phase 3 - Split monoliths only after the adapter seam exists. Prioritize /home/jbalcomb/STU/devel/ReMoM/MoM/src/Combat.c using the segment boundaries already captured in /home/jbalcomb/STU/devel/ReMoM/doc/__TODO-ProjectOverview.md. Do the same for other high-coupling modules only when each extracted file can depend on the game adapter layer instead of engine internals. This depends on steps 4 and 5.
8. Phase 4 - Normalize data ownership and formats. Separate generic container/serialization/resource code from game content definitions. Keep LBX, palettes, fonts, image decoding, and archive access in engine code; keep wizard, city, unit, spell, race, and diplomacy tables in the MoM adapter. Use the header-organization concerns in /home/jbalcomb/STU/devel/ReMoM/doc/000-MoX_vs_MoM.md to avoid repeating the current shared-header tangle.
9. Phase 4 - Add a second-game proof point at the interface level only. Do not attempt full MoO support initially. Instead, create a thin placeholder adapter with its own type namespace, bootstrap hooks, and screen registration to prove the engine boundary supports more than one game. This can run in parallel with late-stage MoM cleanup once the engine/game seam is stable.
10. Phase 5 - Align the build around the new layering. Preserve the current top-level target structure but rename or add targets so the dependency direction becomes explicit: platform -> simtex_engine -> game_mom -> app. Keep STU as shared support only if it remains dependency-clean; otherwise split generic utilities from test/debug helpers. Update both CMake and autotools only after the CMake graph is correct.
11. Phase 5 - Expand verification around architectural seams. Add tests where the rewrite can regress silently: resource loading, palette/render behavior, page flipping, input translation, new-game initialization, turn progression, save/load round-trips, and combat setup invariants. Reuse existing GoogleTest coverage in MoM/tests and MoX/tests as guardrails before broader refactors.
12. Phase 5 - Produce the contributor-facing rewrite map. Publish a maintained architecture map listing each module as Platform, Engine, Adapter, or Shared Utility, plus a migration backlog ordered by dependency risk. This is the handoff artifact for contributors and should stay synchronized with the codebase as the rewrite proceeds.

**Relevant files**
- /home/jbalcomb/STU/devel/ReMoM/CMakeLists.txt — current top-level dependency graph and backend selection
- /home/jbalcomb/STU/devel/ReMoM/BUILDING.md — current SDL2-first build and cross-platform assumptions
- /home/jbalcomb/STU/devel/ReMoM/platform/include/Platform.h — target platform contract to freeze
- /home/jbalcomb/STU/devel/ReMoM/platform/include/Platform_Keys.h — input abstraction contract
- /home/jbalcomb/STU/devel/ReMoM/platform/include/Platform_Replay.h — replay/recording contract that may belong at platform boundary
- /home/jbalcomb/STU/devel/ReMoM/MoX/src/MOX_BASE.h — core base types/macros candidate for generic engine
- /home/jbalcomb/STU/devel/ReMoM/MoX/src/MOX_TYPE.h — shared type definitions to audit for game leakage
- /home/jbalcomb/STU/devel/ReMoM/MoX/src/MOX_DEF.h — shared enums/defines to split into generic vs game-specific
- /home/jbalcomb/STU/devel/ReMoM/MoM/src/MoM_SCR.h — current screen ownership boundary
- /home/jbalcomb/STU/devel/ReMoM/MoM/src/INITGAME.c — bootstrap/new-game lifecycle anchor
- /home/jbalcomb/STU/devel/ReMoM/MoM/src/NEXTTURN.c — strategic simulation/turn loop anchor
- /home/jbalcomb/STU/devel/ReMoM/MoM/src/Combat.c — high-value monolith to decompose after interfaces exist
- /home/jbalcomb/STU/devel/ReMoM/doc/000-MoX_vs_MoM.md — documented header and module-boundary issues
- /home/jbalcomb/STU/devel/ReMoM/doc/MoX-SimtexGameEngine.md — engine ownership and Screen_Control dependency problem
- /home/jbalcomb/STU/devel/ReMoM/doc/__TODO-ProjectOverview.md — concrete module-splitting backlog, especially Combat.c
- /home/jbalcomb/STU/devel/ReMoM/doc/Devel-PlatformLayer-AiPrompt.md — platform boundary cleanup tasks
- /home/jbalcomb/STU/devel/ReMoM/doc/STU_Extract.md — resource/data extraction patterns useful for genericization
- /home/jbalcomb/STU/devel/ReMoM/MoM/tests/CMakeLists.txt — current game-side test structure to reuse
- /home/jbalcomb/STU/devel/ReMoM/MoX/tests/CMakeLists.txt — current engine-side test structure to reuse

**Verification**
1. Confirm the build graph matches the intended dependency direction by reviewing the active CMake targets in /home/jbalcomb/STU/devel/ReMoM/CMakeLists.txt and each subproject CMakeLists before any refactor starts.
2. Create an architecture matrix listing every current source module as Platform, Engine, MoM Adapter, or Shared Utility; flag each illegal dependency crossing for migration.
3. Require that engine-only targets compile without direct inclusion of MoM headers after each boundary-cut milestone.
4. Run the existing unit/integration tests in MoX/tests and MoM/tests after each major phase, especially after platform cleanup and screen-control extraction.
5. Validate a full playable smoke test after each adapter milestone: launch, load assets, start new game, advance at least one turn, enter one combat, save, reload, and exit cleanly.
6. For the second-game proof point, verify that the app can switch adapters at startup without MoM symbols being required by the engine target.

**Decisions**
- Use the current repository as the factual baseline instead of inventing a greenfield architecture unrelated to working code.
- Standardize the rewrite path on C99 + SDL2; SDL3 and Win32 stay outside the primary architectural plan unless they constrain API design.
- Included scope: engine/game boundary definition, module migration order, build/test strategy, and multi-game extensibility seam.
- Excluded scope: replacing proprietary art/audio/assets, redesigning Master of Magic gameplay, and implementing full Master of Orion support in the first rewrite pass.
- Recommended sequence: boundaries first, adapters second, module decomposition third. Reversing this order will increase churn and duplicate effort.

**Further Considerations**
1. The repo contains both SDL2-first and SDL3-first documentation; decide which one is authoritative before contributors start reshaping the platform layer so the rewrite does not chase two backends at once.
2. If code generation is eventually used for versioned structs/enums across MoM and MoO, keep that out of the first boundary-refactor milestone; otherwise architecture work will stall on tooling.
3. STU should remain a small shared-utility layer only if its debug/test helpers can stay isolated from runtime engine dependencies.
