
# HeMoM — Headless Master of Magic: Testing Infrastructure

## What We Built

HeMoM is a separate executable that runs the full Master of Magic game engine without a display, menus, logos, credits, or music.  It creates or loads a game from the command line and enters `Screen_Control()` — the same game loop as the real game.

### Components

| Component | Path | Purpose |
|-----------|------|---------|
| **HeMoM executable** | `src/HeMoM.c` | Lean entry point — parse CLI, init engine, create/load game, run |
| **Shared engine init** | `src/ReMoM_Init.c` + `.h` | CONFIG.MOM, sound config, `Init_Drivers()`, `Allocate_Data_Space()`, resource loading — shared by both ReMoMber and HeMoM |
| **Headless platform backend** | `platform/headless/` | Zero-dependency platform implementation: no SDL, no Win32, no display |
| **Game config file** | `assets/ReMoM.ini` | INI file specifying new game parameters (difficulty, wizard, books, retorts, race, banner) |
| **SDL offscreen flag** | `--headless` on ReMoMber | Sets `SDL_VIDEODRIVER=offscreen` (SDL3) or `dummy` (SDL2) before SDL init |


## Why We Did It This Way

### The Problem

ReMoM reconstructs the original Master of Magic v1.31 game binary.  Testing it means running game logic — world generation, combat AI, spell casting, city management, turn processing.  These code paths are deep inside a screen-based state machine (`Screen_Control()`) that expects a window, a mouse, a keyboard, and a player clicking through 8 sub-screens just to create a game.

You cannot unit-test world generation by calling `Init_New_Game()` in isolation because it depends on 200+ globals being initialized by the full startup sequence (CONFIG.MOM, `Init_Drivers()`, `Allocate_Data_Space()`, LBX resource loading, palette setup).

### The Architecture That Made It Easy

The platform abstraction layer (`Platform.h`) was already clean: MoM and MoX never touch SDL directly — everything goes through `Platform_Video_Update()`, `Platform_Event_Handler()`, etc.  This meant we could add a headless backend by implementing ~30 stub functions with no changes to game code.

### Two Headless Approaches

**1. Pure headless backend (`USE_HEADLESS`)** — A new `platform/headless/` directory with stub implementations.  Zero external dependencies.  Best for CI pipelines, build servers, Docker containers, and platforms where SDL is not installed.

**2. SDL offscreen mode (`--headless` flag / `SDL_VIDEODRIVER=offscreen`)** — An environment variable trick that tells SDL to create an offscreen rendering surface.  Audio still works.  No code changes to the platform backend.  Best for local testing and replay where you want the full SDL stack but no visible window.

HeMoM automatically selects the right approach based on how it was built.

### Separation from ReMoMber

HeMoM is a separate executable rather than flags on ReMoMber because:

- **ReMoMber stays faithful to the original game** — its `MOM_main()` is a reconstruction of the original MAGIC.EXE/WIZARDS.EXE entry point, with all the logos, credits, menu music, and save file validation.  Adding `--newgame` / `--load` flags there would pollute the reconstruction.
- **HeMoM can be opinionated** — it skips logos, credits, and music.  It can add test-specific exits in the future (e.g., create game + save + quit, run N turns + save).
- **Shared init is extracted** — `ReMoM_Init_Engine()` contains the ~200-line engine initialization sequence that both executables need.  One source of truth, no duplication.


## How It Fits With the Overall Testing Strategy

```
Testing Pyramid for ReMoM:
                                         
    ┌─────────────────────────┐
    │   Full-game replay      │  ← HeMoM --newgame + --replay
    │   (integration / E2E)   │     Deterministic. Catches regressions
    ├─────────────────────────┤     in the full game loop.
    │   Save-file scenario    │  ← HeMoM --load SAVE.GAM + --replay
    │   (scenario testing)    │     Jump to a known game state,
    ├─────────────────────────┤     replay specific interactions.
    │   Headless new game     │  ← HeMoM --newgame ReMoM.ini
    │   (smoke test)          │     Does world generation crash?
    ├─────────────────────────┤     Does it produce a valid SAVE9.GAM?
    │   Unit tests            │  ← GoogleTest (MOM_tests, MOX_tests)
    │   (function-level)      │     Direct calls, mock data,
    └─────────────────────────┘     no engine init needed.
```

### What Each Layer Tests

**Unit tests** (existing, `tests/`) — Test individual functions like `STR_ListSeparator()`, unit creation, save file loading.  These link `momlib`/`MOX`/`STU` directly and call functions with crafted inputs.  They don't need a platform backend or engine init.

**Headless new game** (HeMoM `--newgame`) — Smoke test that the full init + world generation pipeline works end-to-end.  Catches linker errors, uninitialized globals, crashes in `Init_New_Game()`, `Initialize_Events()`, `NEWG_FinalizeTables__WIP()`.  The output is a `SAVE9.GAM` file that can be validated.

**Save-file scenario** (HeMoM `--load`) — Jump to a known game state and replay a specific sequence of inputs.  Good for testing specific game situations: a particular combat, a spell being cast, diplomacy interactions.  Create the scenario once in the real game with `--record`, then replay it forever with HeMoM.

**Full-game replay** (HeMoM `--newgame` + `--replay`) — The gold standard.  Create a game from a config, then replay a full recorded session.  The replay system records mouse position, button state, and key presses at each frame, plus the RNG seed.  If the game logic is deterministic, the replay produces identical results every time.  Any code change that alters behavior will cause the replay to diverge — which is exactly what you want for regression testing.

### Record & Replay System

The replay system (`.RMR` files) is the keystone of integration testing.  It was built before HeMoM and works independently:

- **Record**: `ReMoMber --record session.RMR` — plays the game normally, writes every input frame to a human-readable CSV file.
- **Replay**: `ReMoMber --replay session.RMR` or `HeMoM --newgame --replay session.RMR` — feeds the recorded input frames back into the engine in place of live OS events.
- **Determinism**: The RNG seed is captured at record time and restored at replay time.  If the game logic is purely deterministic given the same inputs and RNG seed, the replay reproduces the exact same game state.
- **Logging**: Recording creates `session-RECORD.log` and replay creates `session-REPLAY.log`.  These can be diffed to detect divergence.

The `.RMR` file format is human-readable CSV:
```
# ReMoM Replay v2
# random_seed=12345
# screen_scale=4
# frame_count=100
# idx,timestamp_ms,delta_ms,mouse_x,mouse_y,mouse_buttons,key_pressed,key_count,keys...
0,2000,2000,0,0,0,1,1,283
1,2100,100,160,100,1,0,0
```


## How To Use It

### Build Presets

| Preset | Backend | Use case |
|--------|---------|----------|
| `MSVC-headless-debug` | Pure headless (no SDL) | CI, build servers, no display needed |
| `clang-headless-debug` | Pure headless (no SDL) | Linux CI, Docker containers |
| `MSVC-debug` | Win32 | Local dev (HeMoM auto-sets SDL_VIDEODRIVER) |
| Any SDL preset | SDL2/SDL3 | Local dev (HeMoM auto-sets SDL_VIDEODRIVER) |

```bash
# Build headless (zero dependencies)
cmake --preset MSVC-headless-debug
cmake --build out/build/MSVC-headless-debug --config Debug --target HeMoM

# Or build alongside the normal game (HeMoM will use SDL offscreen)
cmake --preset MSVC-debug
cmake --build out/build/MSVC-debug --config Debug --target HeMoM
```

### CLI Reference

```
HeMoM --newgame [ReMoM.ini] [--replay FILE.RMR] [--record FILE.RMR]
HeMoM --load SAVE3.GAM     [--replay FILE.RMR] [--record FILE.RMR]
HeMoM --help
```

| Flag | Description |
|------|-------------|
| `--newgame [FILE]` | Create a new game from config (default: `ReMoM.ini`) |
| `--load FILE` | Load a save file (`SAVE1.GAM`..`SAVE9.GAM`, `SAVETEST.GAM`) |
| `--replay FILE` | Replay recorded input from `.RMR` file |
| `--record FILE` | Record input to `.RMR` file |
| `--help` | Show usage |

### ReMoM.ini Configuration

```ini
[Game]
difficulty = Normal      # Intro | Easy | Normal | Hard | Impossible
magic = Normal           # Weak | Normal | Powerful
landsize = Medium        # Small | Medium | Large
opponents = 3            # 0..4

[Wizard]
wizard = Merlin           # Preset name (sets default books + retort)
name = MyWizard           # Override wizard name (up to 19 chars)
race = High Men           # Barbarian | Beastmen | Dark Elf | Draconian | Dwarf
                          # Gnoll | Halfling | High Elf | High Men | Klackon
                          # Lizardman | Nomad | Orc | Troll
banner = Blue             # Blue | Green | Purple | Red | Yellow | Brown

[Books]
books_nature = 5          # Override preset book counts (0..13)
books_sorcery = 0
books_chaos = 0
books_life = 5
books_death = 0

[Retorts]
Sage Master = T           # Override preset retorts (T or F)
Alchemy = F
```

Unspecified keys use the preset wizard's defaults.  Specifying `wizard = Merlin` gives you Merlin's books (Nature 5, Life 5) and retort (Sage Master), then individual `books_*` or retort lines override those defaults.


## End-to-End Example: Regression Test for World Generation

### Goal

Verify that world generation produces a valid game state and does not crash, across all difficulty levels, with a known wizard configuration.

### Step 1: Create the config

Save as `test_worldgen.ini` in the game's working directory (next to the LBX files):

```ini
[Game]
difficulty = Hard
magic = Powerful
landsize = Large
opponents = 4

[Wizard]
wizard = Freya
race = High Elf
banner = Green

[Retorts]
Nature Mastery = T
```

### Step 2: Run HeMoM to create the game

```bash
cd out/build/MSVC-headless-debug/bin/Debug/

# Create a new game from config — no display, no menus
./HeMoM --newgame test_worldgen.ini
```

Expected output:
```
[HeMoM] SDL video driver set to offscreen
[headless] Platform started (no display, no audio)
[HeMoM] Initializing engine...
[HeMoM] Loading config: test_worldgen.ini
[HeMoM] Creating new game: difficulty=3 magic=2 landsize=2 opponents=4 wizard=Freya race=7 banner=1
[HeMoM] New game created successfully
[HeMoM] Entering Screen_Control()
```

At this point the game has:
- Generated Arcanus and Myrror world maps
- Placed nodes, towers, lairs, cities
- Created 5 wizard players (1 human + 4 AI)
- Saved `SAVE9.GAM` (the "Continue" slot)
- Entered `Screen_Control()` at `scr_Main_Screen`

The game is now waiting for input in the main game loop.  In headless mode with no replay, it will sit idle (no input events).  Press Ctrl+C to exit, or provide a `--replay` file.

### Step 3: Record a test session (one-time, using the real game)

Play the game normally with recording enabled:

```bash
./ReMoMber --record test_5turns.RMR
```

Play through: create a game as Freya, click through 5 turns (hit "Next Turn" 5 times), then quit.  The `.RMR` file captures every mouse click and key press.

### Step 4: Replay headlessly

```bash
./HeMoM --newgame test_worldgen.ini --replay test_5turns.RMR
```

This creates the same game from config, then feeds the recorded input.  The replay system restores the RNG seed from the `.RMR` header, so the world generation is identical.  The 5 "Next Turn" clicks are replayed, exercising AI turns, city production, spell research, unit movement — the full game loop.

When the replay runs out of frames, the game returns to live input (which in headless mode means no input, so it idles).

### Step 5: Validate the result

After replay completes, `SAVE9.GAM` contains the game state after 5 turns.  You can:

- **Compare save files**: `diff` or binary compare against a known-good `SAVE9.GAM` from a previous run.  Byte-identical saves mean the game logic is deterministic.
- **Compare replay logs**: The replay system writes `test_5turns-REPLAY.log`.  Diff against the original `test_5turns-RECORD.log` to detect divergence.
- **Load in the real game**: `ReMoMber` with Continue to visually inspect the result.

### Step 6: Automate in CI

```bash
#!/bin/bash
# ci_test_worldgen.sh — Smoke test: does world generation crash?

set -e

cmake --preset MSVC-headless-debug
cmake --build out/build/MSVC-headless-debug --config Debug --target HeMoM

cd out/build/MSVC-headless-debug/bin/Debug/

# Create configs for each difficulty
for diff in Intro Easy Normal Hard Impossible; do
    cat > "test_${diff}.ini" << EOF
[Game]
difficulty = ${diff}
magic = Normal
landsize = Medium
opponents = 3

[Wizard]
wizard = Merlin
race = High Men
banner = Blue
EOF

    echo "Testing difficulty=${diff}..."
    timeout 60 ./HeMoM --newgame "test_${diff}.ini" &
    HEMOM_PID=$!
    sleep 10
    kill $HEMOM_PID 2>/dev/null || true
    
    # Verify save file was created
    if [ ! -f SAVE9.GAM ]; then
        echo "FAIL: SAVE9.GAM not created for difficulty=${diff}"
        exit 1
    fi
    
    echo "PASS: difficulty=${diff}"
    rm -f SAVE9.GAM
done

echo "All world generation smoke tests passed."
```


## File Inventory

```
src/
├── ReMoM.c          Game executable entry point (unchanged game flow)
├── ReMoM_Init.c     Shared engine init (extracted from MOM_main)
├── ReMoM_Init.h     Header for shared init
└── HeMoM.c          Headless executable entry point

platform/
├── headless/
│   ├── headless_PFL.c     Lifecycle + video no-ops + input/mouse stubs
│   ├── headless_KD.c      Keyboard buffer (no OS key translation)
│   ├── headless_Timer.c   Portable timer (Win32 + POSIX)
│   ├── headless_EMM.c     EMS malloc stub
│   ├── headless_Audio.c   Audio no-op
│   └── headless_State.c   Platform shared state globals
├── sdl2/                  (existing, unchanged)
├── sdl3/                  (existing, unchanged)
├── win32/                 (existing, unchanged)
└── replay/                (existing, unchanged — works with all backends)

assets/
└── ReMoM.ini         Sample new game configuration
```
