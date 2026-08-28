
//GHCP+GPT54



I’m loading the saved plan text and then I’ll try to place it into an untitled Markdown editor with the requested name so you can refine it directly.
I have the plan source in session memory; the next step is creating the requested untitled editor buffer, and if the untitled URI is unsupported by the file tool I’ll fall back to the closest viable alternative and state that precisely.
I’ve got the exact saved plan text. I’m now attempting the untitled-file creation directly with the requested URI-style name.
Created the untitled prompt buffer as requested:
untitled:plan-genericSimtexSdl2RewriteRoadmap.prompt.md
It contains the plan as-is, without frontmatter, ready for refinement.





The current direction is clearer: formalize the existing architecture as the basis for a clean rewrite, with a broader goal of a generic Simtex-style engine in C99 + SDL2. I’m checking whether the repo already contains concrete multi-game or engine-separation design notes before I draft the plan.

Read memory

I found concrete architecture debt already documented by the repo: platform boundary violations, a dependency-heavy screen system, and a clear path to splitting monoliths like Combat.c. I’m pulling the most relevant docs now so the plan can point at specific seams instead of staying generic.

The plan is going to center on extracting a true generic engine from what is currently a game-specific reconstruction, rather than pretending this is a blank slate. One last check: I’m verifying the actual library/target boundaries in the build so the rewrite roadmap matches how the repo already compiles.

Plan: Generic Simtex SDL2 Rewrite Roadmap

Use the current ReMoM codebase as the factual baseline, then refactor toward a clean layered architecture instead of pretending this is a blank-slate project. The recommended path is evolutionary: lock the boundaries first, extract a truly generic engine second, then move Master of Magic behind a formal game adapter so multi-game support becomes structurally possible later.

Steps

Phase 1: Baseline the actual architecture and freeze scope.
Reuse the current target split in CMakeLists.txt, CMakeLists.txt, CMakeLists.txt, CMakeLists.txt, and CMakeLists.txt as the starting dependency graph.

Phase 1: Enumerate the boundary violations that must be removed first.
The key sources are MoX-SimtexGameEngine.md, 000-MoX_vs_MoM.md, and Devel-PlatformLayer-AiPrompt.md.
These already document the main problems:
engine code depending on game screens,
shared headers mixing generic and MoM-specific types,
platform backends still reaching into non-platform code.

Phase 2: Freeze the platform contract.
Treat Platform.h, Platform_Keys.h, and Platform_Replay.h as the only OS-facing surface.
Standardize the rewrite path on SDL2, and treat SDL3 and Win32 as secondary backends that must conform to that contract rather than shape it.

Phase 2: Define the actual generic engine.
Start from the MoX modules that are already close to reusable engine services:
allocation,
timers,
archive loading,
graphics primitives,
palette/video/page management,
fonts,
input routing,
mouse,
sound abstraction,
animation,
DOS memory compatibility.
Audit MOX_BASE.h, MOX_TYPE.h, and MOX_DEF.h to split engine-wide definitions from game-specific ones.

Phase 2: Introduce a formal game adapter seam.
Replace implicit global coupling with explicit game-owned hooks for:
screen registration and flow,
bootstrap and shutdown,
asset table registration,
save/load lifecycle,
strategic turn entry,
combat entry,
game-specific UI callbacks.
The first seam to cut is the Screen_Control/current_screen cluster described in MoX-SimtexGameEngine.md.

Phase 3: Repackage Master of Magic as the first adapter.
Move MoM-specific state, data tables, spells, diplomacy, AI, and screens behind the adapter boundary.
The anchor files are INITGAME.c, NEXTTURN.c, Combat.c, and MoM/src/MoM_SCR.h.

Phase 3: Split monoliths only after the engine/game seam exists.
The documented Combat split in __TODO-ProjectOverview.md is the right roadmap, but it should happen after the ownership boundaries are clean.
Otherwise the rewrite just redistributes coupling across more files.

Phase 4: Normalize data ownership.
Keep LBX loading, palettes, fonts, image decoding, archive access, and generic containers in the engine.
Keep wizard, unit, city, race, spell, diplomacy, and campaign tables in the MoM adapter.
Use 000-MoX_vs_MoM.md as the guide for avoiding another mixed-header design.

Phase 4: Add a second-game proof point at the interface level only.
Do not build full MoO support yet.
Create a thin placeholder adapter with its own types, bootstrap hooks, and screen registration so the engine proves it can host more than one game.

Phase 5: Align the build around explicit layering.
Keep the current top-level structure but make the dependency direction obvious:
platform -> simtex_engine -> game_mom -> app.
Keep STU only where it remains dependency-clean; otherwise separate runtime utilities from debug/test helpers.

Phase 5: Expand architectural verification.
Reuse existing tests in CMakeLists.txt and CMakeLists.txt, then add seam-focused coverage for:
resource loading,
palette/render behavior,
input translation,
new-game init,
turn progression,
save/load round-trips,
combat setup invariants.

Phase 5: Publish and maintain a contributor rewrite map.
Label every module as Platform, Engine, Adapter, or Shared Utility, and keep the migration backlog ordered by dependency risk.

Relevant files

CMakeLists.txt — current top-level dependency graph and backend selection
BUILDING.md — SDL2-first cross-platform build assumptions
Platform.h — primary platform contract
Platform_Keys.h — input abstraction contract
Platform_Replay.h — replay/recording contract
MOX_BASE.h — core engine base layer candidate
MOX_TYPE.h — shared types to audit for leakage
MOX_DEF.h — shared defines/enums to split
MoM/src/MoM_SCR.h — current screen boundary
INITGAME.c — bootstrap/new-game lifecycle
NEXTTURN.c — strategic loop entry
Combat.c — major monolith to split later
000-MoX_vs_MoM.md — header and ownership problems
MoX-SimtexGameEngine.md — engine dependency issues
__TODO-ProjectOverview.md — concrete refactor backlog
Devel-PlatformLayer-AiPrompt.md — platform cleanup targets
STU_Extract.md — extraction/resource patterns
README.md — project intent and current delivery goal
Verification

Confirm the build graph in the current CMake files matches the intended dependency direction before any refactor begins.
Build an architecture matrix listing every module as Platform, Engine, MoM Adapter, or Shared Utility.
Require engine-only targets to compile without direct MoM headers after each boundary-cut milestone.
Run existing MoX and MoM tests after each major architecture step.
Run a playable smoke test after each adapter milestone: launch, start new game, advance one turn, enter combat, save, reload, exit.
For the second-game proof point, verify the app can select a different adapter without the engine target depending on MoM symbols.
Decisions

Base the plan on the current repository, not on a disconnected greenfield design.
Use C99 + SDL2 as the primary rewrite target.
Include architecture boundaries, migration order, build shape, and multi-game extensibility.
Exclude asset replacement, gameplay redesign, and full Master of Orion implementation in the first pass.
Sequence the work as boundaries first, adapters second, decomposition third.
I also saved this plan in session memory so it can be revised cleanly if you want to iterate on it.

Natural next steps:

Narrow this into a first milestone plan focused only on engine/game boundary extraction.
Turn this into a contributor roadmap with concrete work items by subsystem.
Stress-test the plan specifically against MoO support to see where the generic-engine assumption breaks.
