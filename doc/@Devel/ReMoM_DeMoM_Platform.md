
                ╔═══════════════════════════════════════════════════════════════════╗
                ║                          GAME TARGETS                             ║
                ║                                                                   ║
                ║    ┌──────────────────────────┐    ┌─────────────────────────┐    ║
                ║    │   ReMoM.exe   [1]     │    │   DeMoM.exe       [2]   │    ║
                ║    │   (windowed game)        │    │   (windowless tester)   │    ║
                ║    │   src/ReMoM.c            │    │   src/DeMoM.c           │    ║
                ║    │   src/ReMoM_Init.c       │    │   src/ReMoM_Init.c      │    ║
                ║    │   + Automation Drv. [a]  │    │   + Automation Drv. [a] │    ║
                ║    └────────────┬─────────────┘    └────────────┬────────────┘    ║
                ╚═════════════════│═══════════════════════════════│═════════════════╝
                                  │ link / call                   │ link / call
                                  ▼                               ▼
                ╔═══════════════════════════════════════════════════════════════════╗
                ║                     GAME-ENGINE LIBRARIES                         ║
                ║                                                                   ║
                ║    ┌─────────────┐      ┌─────────────┐      ┌─────────────┐      ║
                ║    │     MoM     │ ◄──► │     MoX     │ ───► │     STU     │      ║
                ║    │   /MoM/src  │      │   /MoX/src  │      │   /STU/src  │      ║
                ║    │             │      │             │      │             │      ║
                ║    │ rules, AI,  │      │ engine /    │      │ trc_prn,    │      ║
                ║    │ screens,    │      │ runtime:    │      │ dbg_prn,    │      ║
                ║    │ MAPGEN,     │      │ MOX2 main,  │      │ JSON, FIO,  │      ║
                ║    │ NEXTTURN,   │      │ Fields,     │      │ validation, │      ║
                ║    │ Combat,     │      │ Input/Mouse │      │ AI_METRICS, │      ║
                ║    │ Spells…     │      │ LBX, Save,  │      │ world dump  │      ║
                ║    │             │      │ random…     │      │             │      ║
                ║    └─────────────┘      └──────┬──────┘      └─────────────┘      ║
                ║                                │ calls Platform.h API             ║
                ╚════════════════════════════════│══════════════════════════════════╝
                                                 ▼
                ╔═══════════════════════════════════════════════════════════════════╗
                ║       PLATFORM LAYER  (one backend selected at link time)   [3]   ║
                ║       /platform/include/Platform{,_Keys,_Replay}.h                ║
                ║                                                                   ║
                ║   ┌─────────┐  ┌─────────┐  ┌─────────┐    ┌─────────────────┐    ║
                ║   │  SDL2   │  │  SDL3   │  │  Win32  │    │  Windowless [4] │    ║
                ║   │ /sdl2/  │  │ /sdl3/  │  │ /win32/ │    │  /windowless/   │    ║
                ║   │ window, │  │ window, │  │ window, │    │   no video,     │    ║
                ║   │ audio,  │  │ audio,  │  │ GDI,    │    │   no audio,     │    ║
                ║   │ input   │  │ input   │  │ input   │    │   stub timer    │    ║
                ║   └────┬────┘  └────┬────┘  └────┬────┘    └────────┬────────┘    ║
                ║        └───────────┬┴───────────┬┘                  │             ║
                ║                    ▼            ▼                   ▼             ║
                ║      ┌──────────────────────────────────────────────────────┐     ║
                ║      │            Platform_Event_Handler()    ◄── tap point │     ║
                ║      └──┬─────────────────────────────────────┬──┬──────────┘     ║
                ║         │                                     │  │                ║
                ║         ▼                                     │  ▼                ║
                ║   ┌──────────────────┐   per-frame callback ──┘ ┌──────────────┐  ║
                ║   │  Replay     [5]  │                          │Automation [6]│  ║
                ║   │ /platform/replay │                          │    Driver    │  ║
                ║   │   Replay.c       │                          │              │  ║
                ║   │ ─ record  .RMR ──┼──► file                  │ reads .hms ◄─┼─ scenario
                ║   │ ─ replay  .RMR ◄─┼─── file                  │ injects keys │  ║
                ║   │                  │                          │ + mouse      │  ║
                ║   └──────────────────┘                          └──────┬───────┘  ║
                ║                                                        │          ║
                ║     injects via Platform_Keyboard_Buffer_Add_Key_Press │          ║
                ║                 + User_Mouse_Handler                  ◄┘          ║
                ╚═══════════════════════════════════════════════════════════════════╝

### Footnotes

 * [1] ReMoM — the windowed game executable (the user-facing build of the ReMoM project). Linked against momlib + MOX + STU + Platform where Platform is whichever windowed backend the preset chose (SDL2 / SDL3 / Win32).

 * [2] DeMoM — Driven Master of Magic: windowless, driven by a scenario, a replay or (later) an agent. Formerly HeMoM. A lean entry point hardwired to Platform_Windowless, regardless of which windowed backend the preset selected. Supports --newgame, --load SAVE.GAM, --record game.RMR, --replay game.RMR.

 * [3] Platform layer — a single contract (Platform.h / Platform_Keys.h / Platform_Replay.h) with four implementations under /platform/{sdl2,sdl3,win32,windowless}/. The engine never talks to SDL/Win32 directly; everything funnels through the contract.

 * [4] Windowless mode — Platform_Windowless provides no-op video/audio, a stub timer/EMM, and a programmatic input source. It's what makes deterministic CI runs and --continue save tests possible.

 * [5] Record & Replay — sits at the Platform_Event_Handler() boundary. On record, every engine-visible input frame is appended to a human-readable .RMR file (with a header line # random_seed=<seed> so the run is reproducible). On replay, the file is the input source — OS events are ignored. platform/replay/Replay.c:1-13

 * [6] Automation Driver — src/Automation_Driver.c. Reads a .hms scenario (wait, key, click, next_turn, quit, …), and on each frame injects synthetic keys/mouse via the same platform entry points a real user's input takes. Linked into both ReMoM and DeMoM, so the exact same scenario can run windowed or windowless. src/Automation_Driver.h:1-19

 * [a] Automation Driver is compiled into both targets; it stays dormant unless a scenario is loaded.


Program-flow paths

1. Regular ReMoM usage (a human plays the game)

User keyboard/mouse  →  SDL3 (or SDL2/Win32) backend
                     →  Platform_Event_Handler()      ← Automation Driver idle, Replay idle
                     →  MoX (Input/Mouse/Fields, MOX2 main loop)
                     →  MoM (current screen: MainMenu → MainScr → CityScr → …)
                     →  back through MoX → Platform → SDL → screen + audio

2. DeMoM scripted for testing (CI / regression)

DeMoM.exe --newgame ReMoM.ini --seed N  (+ optional --record game.RMR or Automation Driver scenario file)
   →  ReMoM_Init  (no logos, no menu, no music)
   →  Skip menus, fabricate NewGame state, jump into Screen_Control()
   →  Frame loop:
        Platform_Windowless.Event_Handler()
          → AHP_Frame()         injects scripted key/click  [6]
          → Replay (record)     appends frame to .RMR        [5]
          → MoX → MoM advances turns / screens
   →  On finish: Game_Save_Dump → assertion file
                 (NONDET-tagged, since save format has uninit regions)
   →  Exit code drives ctest pass/fail.

3. ReMoM script — visual confirmation that a script does what we think

ReMoM.exe --replay game.RMR     (or load the same .hms via Automation Driver)
   →  SDL3 backend:   real window, real audio, real timing
   →  Platform_Event_Handler():
        Replay (replay-mode) feeds the recorded frames        [5]
          — OS events are ignored; the .RMR is authoritative.
        OR AHP_Frame() injects the same scripted actions      [6]
   →  Engine and rendering proceed normally — you watch the
      AI / human-script drive the UI on screen, in real time,
      and visually verify the windowless run wasn't lying.
Same scenario file, three execution modes — that's the point of putting both Replay and Automation Driver at the Platform boundary instead of inside MoX or MoM.
