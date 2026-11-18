# BRIEF — Groundwork for RL agents playing ReMoM

For the agent that handles our reinforcement-learning (RL) model work. Written 2026-11-18. Nothing here is built yet; this records the goal, the facts as of today, and the two pieces of groundwork that come first.

## The goal

Let a learning model play Master of Magic through ReMoM, as a **player that decides** — not a script. In RL terms:

| RL term | What it is in ReMoM |
|---|---|
| **environment** | **DeMoM** ("Driven Master of Magic", [src/DeMoM.c](../../src/DeMoM.c)): the game running windowless |
| **agent** | the model being trained |
| **Agent Interface** (to build) | the connection between the two: reset, step(action) → observation, reward |
| — | the **Automation Driver** ([src/Automation_Driver.c](../../src/Automation_Driver.c)) is the *scripted* player (`.hms` scenarios, for tests and demos). Same input path, but it follows a script; it does not decide |

Target the **Gymnasium** API (formerly OpenAI Gym): `reset() -> observation, info` and `step(action) -> observation, reward, terminated, truncated, info`. Most RL libraries expect it, so existing training code can run against DeMoM unchanged.

## What exists today (verified 2026-11-18)

- **Windowless runs:** DeMoM links the windowless platform backend ([platform/windowless/](../../platform/windowless/), `Platform_Windowless`): no window, no audio, input from a scenario or an `.RMR` replay.
- **Determinism:** `--seed1 N` pins the RNG ([doc/@Devel/](../@Devel/) and the DeMoM CLI help). Same seed + same inputs = same game; needed for reproducible episodes.
- **Input injection:** the Automation Driver already feeds clicks, typing and keys through the same platform entry points real input takes (`Automation_Driver_Frame()`, [src/Automation_Driver.c:923](../../src/Automation_Driver.c#L923)). An agent's actions can use the same path.
- **Game-state reading:** [src/Game_Save_Dump.c](../../src/Game_Save_Dump.c) dumps game state to text; a starting point for state-based observations.

## Gap 1 — a simulated clock (do this first)

DeMoM runs in **real time**. The windowless timer is the wall clock and its sleeps are real:

- `Platform_Get_Millies()` reads `GetTickCount64()` ([platform/windowless/windowless_Timer.c:26](../../platform/windowless/windowless_Timer.c#L26)); Linux uses `clock_gettime`.
- `Platform_Sleep_Millies()` calls `Sleep()` / `nanosleep()` ([windowless_Timer.c:47](../../platform/windowless/windowless_Timer.c#L47), [:68](../../platform/windowless/windowless_Timer.c#L68)).

So every 55 ms game tick still costs 55 ms of wall time. Training needs many games far faster than real time. Build a **simulated clock** in the windowless backend: time advances when the game asks to wait, instead of waiting. It also speeds up the DeMoM test suite. Keep it switchable (real clock for replay/visual checks), and confirm the test goldens do not change.

## Gap 2 — the Agent Interface

- **Reset:** start a game from a seed, a ReMoM.ini setup or a save (DeMoM already has `--newgame`, `--load`, `--continue`).
- **Step:** apply one action, run the game until it next needs input, return the observation.
- **Observation — two options, decide which first:**
  - *pixels:* the windowless backend still draws every frame into memory and just never presents it (`Platform_Video_Update()` is empty, [windowless_PFL.c:96](../../platform/windowless/windowless_PFL.c#L96); the frame is in `video_page_buffer[]` / `current_video_page`, [MoX/src/Video.h](../../MoX/src/Video.h)). Needs an interface to read that buffer out;
  - *game state:* structured data (units, cities, wizard resources), building on `Game_Save_Dump.c`. Much smaller and easier to learn from.
- **Action space:** UI-level (clicks/keys — general, but the agent must learn the UI) or game-level (move unit, set production, cast spell — easier to learn, but each action needs code). Decide deliberately; the UI-level path already exists via the Automation Driver's injection.
- **Reward:** not defined yet (score, turn survival, cities, …). Keep it outside the game code.
- **Episode end:** game won/lost, or a turn limit (`truncated`).

## Constraints from the project

- The baseline game code stays faithful to the original disassembly ([AGENTS.md](../../AGENTS.md)). Put the Agent Interface and the clock in the platform layer / `src/`, not in `MoM/` game code.
- Name shared modules by what they do, not by the first program that uses them (AGENTS.md naming rules).
- Say **windowless** and **Automation Driver** (AGENTS.md glossary).
