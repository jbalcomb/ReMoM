# Logging and Instrumentation — what is built in, how to turn it on, who reads it

ReMoM's logging and instrumentation serves four separate purposes. They have different audiences, different on/off switches, and different answers
to "is it in the Release build?". This page is the map. When you add a log line or a probe, decide first which of the four it belongs to.

| Purpose | Who reads it | In Release? | Compile-time switch | Run-time switch |
|---|---|---|---|---|
| [1. Run-time logging](#1-run-time-logging) | anyone diagnosing a run; crash reports | yes | `STU_LOG_MIN_SEVERITY` (INFO outside Debug) | `[Logging]` in `ReMoM.ini` |
| [2. Debug code](#2-debug-code-stu_debug) | a developer at a debugger | no | `STU_DEBUG` (Debug config only) | some knobs are plain variables |
| [3. OG matchup](#3-og-matchup) | the STU-DOSBox compare tools | opt-in | `STU_CALL_TRACE`, `STU_GD_CAPTURE` | INI `CALL_TRACE`; `REMOM_GD_CAPTURE` |
| [4. Catalogs](#4-catalogs-pictures-and-inputs) | `tools/asset_catalog`, `tools/field_catalog`, `rmr2hms` | opt-in | `STU_CLOG` | `--record`; `REMOM_CLOG_FIELDS` |

Performance instrumentation (`PERF_INSTRUMENT` + `REMOM_PERF`) is a fifth, separate system; see
[PRD-Performance-Management.md](../@AI_Plans/PRD-Performance-Management.md). It follows the same pattern used here: its own compile switch, its own
run-time gate, never tied to `STU_DEBUG`.

---

## 1. Run-time logging

`LOG_TRACE` / `LOG_DEBUG` / `LOG_INFO` / `LOG_WARN` / `LOG_ERROR` / `LOG_FATAL` ([STU_LOG.h](../../STU/src/STU_LOG.h)), written to
`remom_log_new.txt` (rotated to `_current` / `_previous`) in `REMOM_LOG_DIR`, else the directory the caller sets, else the CWD.

- **Compile time:** `STU_LOG_MIN_SEVERITY`. Severities below it become a sizeof-noop: no call, no argument evaluation. Every non-Debug config sets it
  to `LOG_SEV_INFO` ([CMakeLists.txt](../../CMakeLists.txt), next to the `STU_DEBUG` line), as the logging PRD requires
  ([PRD-Logging-System-And-Curator-Agent.md:39](../@AI_Plans/PRD-Logging-System-And-Curator-Agent.md#L39)): Release has no TRACE or DEBUG lines.
  Debug has no floor. The call trace (§3a) and `[GD]` lines (§3b) have their own write paths, so the floor does not strip them.
- **Run time:** the `[Logging]` section of `ReMoM.ini` ([assets/ReMoM.ini](../../assets/ReMoM.ini)): `severity_threshold` (default TRACE; the
  shipped INI sets DEBUG) and one `CATEGORY = true|false` line per category (an unlisted category is on). The compile-time floor wins over the INI.
- **Categories** are the `log_category` enum in [STU_LOG.h](../../STU/src/STU_LOG.h). A few are not general logging:
  `CALL_TRACE` is the OG-matchup call trace (§3); `COMBAT_TEST` is test-only (read by `tools/check_combat_legality.py`); `INTEGRITY` is runtime
  integrity checkpoints and should stay on.
- **What belongs here:** things worth seeing from a Release run — warnings, errors, integrity failures, the high-level story of a run at INFO.
  Chatty developer detail is TRACE/DEBUG, which compiles out of Release; it needs no `#ifdef STU_DEBUG` around it.
- `dbg_prn()` / `trc_prn()` ([STU_DBG.h](../../STU/src/STU_DBG.h)) are the older synchronous print paths. They stay in the STU library; game code no
  longer calls them (the Phase 8 migration to `LOG_*` is complete).

## 2. Debug code (`STU_DEBUG`)

`STU_DEBUG` is defined for the Debug config only ([CMakeLists.txt:198](../../CMakeLists.txt#L198)). Everything under `#ifdef STU_DEBUG` is gone
from Release: debug draws and hot-keys, asserts and break-points, debug triggers (`DBG_trigger_*`, `DBG_debug_flag` in
[STU_DBG.c](../../STU/src/STU_DBG.c)), the MoM-aware dumpers in [STU_DBG_MoM.c](../../STU/src/STU_DBG_MoM.c), and DeMoM's `--ai-metrics`.

- `REMOM_DIAGNOSTIC` ([CMakeLists.txt:89](../../CMakeLists.txt#L89)) builds the Debug config against the redistributable CRT, as
  `ReMoM_diagnostic.exe`, so a tester without Visual Studio can run it.
- **What belongs here:** code that only makes sense while developing — it changes behavior, draws on screen, stops the program, or costs too much
  to carry. Debug-only *logging* does not need an `#ifdef STU_DEBUG`: the Release floor already removes TRACE and DEBUG lines.

### The OG's own release flag — not `STU_DEBUG`

The original game has its own debug switch: `release_flag`, set by `Release_Version()` and read by `Check_Release_Version()`
([capture.c](../../MoX/src/capture.c)). This is original code with an asm counterpart, not ReMoM debug code. `ReMoM_Init_Engine()` calls
`Release_Version()` ([ReMoM_Init.c:448](../../src/ReMoM_Init.c#L448)), and both ReMoM and DeMoM go through it, so the flag is set in **every**
build, Debug included. The OG debug features it guards are therefore off everywhere, for example:

- the Main_Screen debug hot-keys Alt-K, Alt-A, Alt-N, Alt-T and Shift-3 ([MainScr.c](../../MoM/src/MainScr.c)); Alt-P has no release test in the
  original, so it stays live;
- the fixed debug seed (`Set_Random_Seed(10039)`) at the end of `Init_Overland` ([LoadScr.c:994](../../MoM/src/LoadScr.c#L994)), and the fixed seed
  1000 in `PreInit_Overland` ([LoadScr.c](../../MoM/src/LoadScr.c)), whose `--seed1` / `--seed2` override now runs after the original check;
- `Cast_Spell_Target_Error` ([AISPELL.c](../../MoM/src/AISPELL.c)) and the F11 key in [capture.c](../../MoX/src/capture.c), which return at once.

`STU_DEBUG` does not change this flag. Turning the OG debug features on for a Debug build would mean not calling `Release_Version()` in Debug, and
that would also turn on the `Init_Overland` fixed seed, which re-seeds the random sequence after every `Init_Overland` call.

## 3. OG matchup

Reverse-engineering evaluation: a ReMoM run and an OG-MoM run under STU-DOSBox write logs in the same text format, and tools in STU-Extras
(STU-DOSBox) diff them. Both formats are contracts with those tools; do not change a byte.

### 3a. Call trace — `[FN-ENTER]` / `[FN-EXIT]`

`TRACE_ENTER()` / `TRACE_EXIT()` and their `_NAMED` / `_NOTE` forms ([STU_LOG.h](../../STU/src/STU_LOG.h)), aligned by name and `rng_call` against
the OG trace by `merge-remom-trace.py` / `compare-call-paths.py` (ReMoM-CallTrace-PRD.md, in STU-Extras).

- **Compile time:** `STU_CALL_TRACE` — its own switch, independent of `STU_LOG_MIN_SEVERITY` and of the rest of `STU_DEBUG`. It is on in Debug
  (it defaults to 1 when `STU_DEBUG` is defined) and off in Release unless the CMake option `REMOM_CALL_TRACE` is ON.
- **Run time:** `severity_threshold = TRACE` and the `CALL_TRACE` category on, in `[Logging]`.
- Tests: [test_STU_LOG.cpp](../../STU/tests/test_STU_LOG.cpp) pins the text; [test_STU_LOG_strip.cpp](../../STU/tests/test_STU_LOG_strip.cpp)
  proves it survives an INFO floor; [test_STU_LOG_calltrace_off.cpp](../../STU/tests/test_STU_LOG_calltrace_off.cpp) proves it is a no-op when off.

Running a matchup on the faster Release build:

```sh
cd /c/STU/devel/ReMoM
cmake --preset MSVC-release -DREMOM_CALL_TRACE=ON
cmake --build --preset MSVC-release
# in the ReMoM.ini the run uses, [Logging]:  severity_threshold = TRACE   (and CALL_TRACE not set to false)
```

`REMOM_CALL_TRACE` is a cached CMake option: it stays ON for that build directory until you reconfigure with `-DREMOM_CALL_TRACE=OFF`.

### 3b. Game-data capture — `[GD]`

State-array dumps ([STU_GD.h](../../STU/src/STU_GD.h), [STU_GD_MoM.h](../../STU/src/STU_GD_MoM.h)), driven by the generated `gd_*_fields.h` field
maps so they diff field-for-field against the OG probe (compare-gd).

- **Compile time:** `STU_GD_CAPTURE` — defaults on with `STU_DEBUG`, off in Release unless the CMake option `REMOM_GD_CAPTURE` is ON (cached, like
  `REMOM_CALL_TRACE`). Every `[GD]` line goes through `STU_GD_WRITE` ([STU_GD.h](../../STU/src/STU_GD.h)), not `LOG_TRACE`, so the Release floor
  does not strip it.
- **Run time:** the `REMOM_GD_CAPTURE` environment variable set, non-empty and not `0`. The `[GD]` lines are TRACE severity, so
  `severity_threshold = TRACE` too.

## 4. Catalogs (pictures and inputs)

Inventories of the game's graphics and input fields, for graphics upscaling, screen rearranging, and naming clicks in `rmr2hms` / HMS scripts. This
is data collection, not debugging, so its run-time parts live in their own STU module, [STU_CLOG](../../STU/src/STU_CLOG.h), with their own switch.

- **Compile time:** `STU_CLOG` — on in Debug (it defaults to 1 when `STU_DEBUG` is defined), off in Release unless the CMake option `REMOM_CLOG` is
  ON. Like `REMOM_CALL_TRACE`, the option is cached for its build directory.
- **Run time:** the record log is written only with `--record`; the `FIELDADD` / `FIELDSNAPSHOT` rows only with the `REMOM_CLOG_FIELDS` environment
  variable set, and the draw and palette rows only with `REMOM_CLOG_DRAW` set (each: non-empty, not `0`).
- STU_CLOG holds no game types. The hooks that read game data (`Clog_Field_Added` and `Dump_Fields_CSV` in [Fields.c](../../MoX/src/Fields.c)) stay
  in the game code under `#if STU_CLOG`.

Running a catalog recording on the Release build:

```sh
cd /c/STU/devel/ReMoM
cmake --preset MSVC-release -DREMOM_CLOG=ON
cmake --build --preset MSVC-release
```

### 4a. Picture catalog

- **Part A — every sprite and its size: done, static.** [tools/asset_catalog](../../tools/asset_catalog/README.md) parses `assets/*.LBX` (no game
  run) into `tools/fields/assets.fwv`.
- **Part B — where each sprite is drawn: built (B1), palettes partly (B4).** [PLAN-Draw-Catalog.md](../@AI_Plans/PLAN-Draw-Catalog.md).
  `LBX_Load_Entry` names each loaded buffer, `FLIC_Draw` adds each draw to a ring of recent draws, and the record log stamps each click with
  `draw=NAME.LBX:entry@(x1,y1)-(x2,y2)` for the most recently drawn sprite under it. With the `REMOM_CLOG_DRAW` environment variable set, every draw
  is a row in `remom_clog_draw.tsv` and every palette load a row in `remom_clog_palette.tsv`, joined on the palette generation. The color-effect
  flags are not built yet.

### 4b. Input catalog

- **Static part: done.** [tools/field_catalog](../../tools/field_catalog/README.md) parses every `Add_*Field` call site into
  `tools/fields/catalog.fwv`; about half the fields get a click point with no game run.
- **Runtime part:** recording with `--record NAME.RMR` writes `NAME-RECORD.log`, and each click gets a `field[i]=(x1,y1)-(x2,y2)` stamp (the
  `Replay_Log_Field_Hit` callbacks in [ReMoM.c](../../src/ReMoM.c) and [DeMoM.c](../../src/DeMoM.c)). `tools/field_catalog/runtime.py` harvests the
  rectangles into `runtime_geometry.fwv`.
  - The rectangles are written in every build.
  - The `@file:line` origin stamp after each rectangle — the key that joins it to the catalog — comes from the STU_CLOG field-origin table, so it
    is there only in an `STU_CLOG` build. A recording from a plain Release build has rectangles that cannot be tied to a call site.
- **Every registered field:** with `REMOM_CLOG_FIELDS=1`, `Clog_Field_Added` / `Dump_Fields_CSV` ([Fields.c](../../MoX/src/Fields.c)) write
  `FIELDADD` / `FIELDSNAPSHOT` rows to `remom_clog_fields.tsv` in the log directory, and
  [fields_merge.py](../../tools/fields_merge.py) merges them with the static catalog. The trace switches on when Main_Screen first adds its fields.
  Main_Screen re-adds its fields on every loop pass, so the file grows quickly; that is why it is off unless asked for.

---

## One log file per purpose

Each purpose writes its own file, so a tool or a person reads only the data meant for it:

- **Run-time logging** — `remom_log_new.txt` (rotated to `_current` / `_previous`).
- **Catalogs** — STU_CLOG's own files (`remom_clog_*.tsv`) plus the per-recording `*-RECORD.log`; see
  [PLAN-Draw-Catalog.md §7](../@AI_Plans/PLAN-Draw-Catalog.md#7-output-files--one-per-purpose).
- **OG matchup** — not separated yet: the call trace and the `[GD]` lines go into the general log as the `CALL_TRACE` category and `[GD]`-prefixed
  TRACE lines (open item 1).

## Open items

1. **A matchup log file.** Move the call trace and the `[GD]` lines out of the general log into their own file. Blocked on the compare tools: per
   their docs in STU-Extras (`ReMoM-CallTrace-PRD.md`, `GD-Capture-Points.md`), `merge-remom-trace.py` / `compare-call-paths.py` pick the newest of
   `remom_log_new/current/previous.txt` by modification time with no path argument, and compare-gd reads `remom_log_new.txt`. The tool sources are
   not in STU-Extras; the docs place them in `/home/jbalcomb/STU_DBWD/dosbox-capture/prng-port/baselines/` on a Linux box. They have to learn the
   new file name in the same change. (compare-gd's documented `[GD]` regex is found anywhere in a line, so the log header is not needed.)
