# Plan: Logging Subsystem (STU_LOG) + Log-Curator Agent

> Source PRD: [plans/PRD-Logging-System-And-Curator-Agent.md](PRD-Logging-System-And-Curator-Agent.md) · GH issue: https://github.com/jbalcomb/ReMoM/issues/14

## Architectural decisions

Durable decisions that apply across all phases. Each phase below assumes these are in place and does not re-litigate them.

- **Module location**: `STU/src/STU_LOG.c` and `STU/src/STU_LOG.h` (next to the existing `STU_DBG.*`, shared across all targets via the `STU_` convention).
- **Public API**: severity macros `LOG_TRACE(cat, fmt, ...)`, `LOG_DEBUG`, `LOG_INFO`, `LOG_WARN`, `LOG_ERROR`, `LOG_FATAL` plus `log_init(void)`, `log_shutdown(void)`, `log_pump(void)`, and `log_flush_all(void)`. Macros capture `__FILE__`, `__LINE__`, `__FUNCTION__` at call-site.
- **Severity levels**: `TRACE` < `DEBUG` < `INFO` < `WARN` < `ERROR` < `FATAL`. `LOG_FATAL` always flushes the whole ring synchronously before returning.
- **Categories**: enum `LOG_CAT_AIMOVE`, `LOG_CAT_COMBAT`, `LOG_CAT_SAVE`, `LOG_CAT_PFL`, `LOG_CAT_IKI`, ... defined centrally in `STU_LOG.h`. New categories added only via that header.
- **Message format on disk**: one line per message: `[ISO8601] [SEV  ] [CAT] file.c:NNN function: msg`. Severity and category use fixed-width fields so awk/grep one-liners are predictable.
- **Ring buffer**: 2 MB static `char[2097152]`, single producer/consumer (engine is single-threaded), `size_t head`/`size_t tail`, write-wraps-at-boundary, silently-drops-on-would-overtake-tail.
- **Pump cadence**: `log_pump()` is publicly exposed; each target calls it at its own frame boundary (ReMoM main loop, HeMoM tick, ReMoMber loop). Pump writes at most 4 KB per call.
- **File rotation**: at `log_init`, delete `remom_log_previous.txt` if present, rename `remom_log_current.txt` → `_previous.txt`, rename `remom_log_new.txt` → `_current.txt`, open fresh `remom_log_new.txt` for writing. Files live next to the executable.
- **Runtime config**: `[Logging]` section in `ReMoM.ini` with `severity_threshold=` (one of the six levels) and one boolean per category (`AIMOVE=true`, etc.). Defaults baked in if section absent.
- **Compile-time floor**: `STU_LOG_MIN_SEVERITY` macro acts as a hard floor; runtime ini cannot lower severity below it. Release builds strip `TRACE`/`DEBUG` via the `((void)sizeof(...))` idiom (no function call, no argument evaluation).
- **Crash recovery**: `log_init` registers `atexit(log_flush_all)` and installs signal handlers (`SIGSEGV`/`SIGABRT`/`SIGFPE`/`SIGILL` on POSIX, `SetUnhandledExceptionFilter` on Win32). Handler writes a `[CRASH] <signal>` line into the ring, calls `log_flush_all`, then chains to default handler.
- **Drop accounting**: a static `dropped_since_last_pump` counter; if non-zero at pump time, `log_pump` emits a synthetic `[LOGGER] N messages dropped` line before draining real messages, then resets the counter.
- **Subagent**: `.claude/agents/log-curator.md`, modeled on the existing `hemom-tester` agent. Tools: `Read`, `Glob`, `Grep`, `Bash`, `Agent`. No `Edit`/`Write`/`NotebookEdit` — outputs are patches or markdown reports.
- **Helper scripts**: `tools/log-tools/log_triage.py`, `tools/log-tools/log_diff.py`, `tools/log-tools/log_migrate.py`. All stdlib-only Python, runnable by humans, individually unit-tested with golden fixtures under `tools/log-tools/tests/`.
- **Build system**: both CMake (`CMakePresets.json` → `MSVC-debug` etc.) and Autotools (`Makefile.am`) get updates for any new `.c` files. Per project convention, no bare `cmake -S . -B build`.

---

## Phase 1: Tracer bullet — single LOG_INFO end-to-end into ReMoM

**User stories**: 1, 11 (partial)

### What to build

A minimum-viable logger that proves every layer of the design integrates. One severity (`LOG_INFO`), no metadata beyond a raw timestamp, no rotation, no drops, no config, no signal handlers. `log_init` opens a single output file with a fixed name; `log_pump` drains the entire ring (no 4 KB cap yet); `log_shutdown` flushes and closes. Wire `log_init`/`log_pump`/`log_shutdown` into the ReMoM frame loop, add one `LOG_INFO` call at game startup. After running ReMoM, the output file contains the message.

### Acceptance criteria

- [ ] `STU_LOG.c` / `STU_LOG.h` exist, build under MSVC-debug, build under Autotools.
- [ ] A single `LOG_INFO` macro is callable from any `.c` in the engine and produces output.
- [ ] Running ReMoM produces an output file in the executable directory containing the message from the startup call-site.
- [ ] `log_pump` is called from the ReMoM main loop (not from inside `LOG_INFO`).
- [ ] A C test program writes N messages, calls `log_pump`, reads the file back, and asserts that exactly N messages appear in order.

---

## Phase 2: Full message format + all severities + compile-time severity floor

**User stories**: 2, 9

### What to build

All six severity macros (`LOG_TRACE` through `LOG_FATAL`). Each message gets the full metadata header: ISO8601 timestamp, fixed-width severity tag, fixed-width category tag, `file:line`, function name, then the formatted message. Categories enum defined centrally. `STU_LOG_MIN_SEVERITY` macro enforced at compile time so Release builds expand stripped severities to the no-op sizeof idiom (no function call, no argument evaluation, no binary bloat). `LOG_FATAL` macro guaranteed to call `log_flush_all` synchronously after writing.

### Acceptance criteria

- [ ] All six severity macros work and emit messages in the documented format.
- [ ] A test asserts the exact regex shape of an emitted line including timestamp, severity, category, file:line, and function fields.
- [ ] A test built with `STU_LOG_MIN_SEVERITY=LOG_SEV_INFO` confirms `LOG_TRACE` and `LOG_DEBUG` calls produce zero output AND zero argument-evaluation side effects (e.g., a counter incremented inside the call's argument list stays at 0).
- [ ] `LOG_FATAL` followed by reading the file (without calling `log_pump`) shows the FATAL message already on disk.
- [ ] Categories enum lives in `STU_LOG.h`; adding a new category is a single-file change.

---

## Phase 3: Drop accounting + 4 KB pump cap + runtime config

**User stories**: 3, 4, 5, 8

### What to build

`log_pump` honors the 4 KB-per-call throughput cap; remainder drains over subsequent pumps. Static `dropped_since_last_pump` counter increments whenever a write would overtake `tail`; on next pump, a synthetic `[LOGGER] N messages dropped` line is emitted before the real drain and the counter resets. `[Logging]` section in `ReMoM.ini` is parsed at `log_init`: `severity_threshold=` (string, one of six levels) and one boolean per category. Runtime threshold and category mask consulted on the macro hot path. Compile-time floor still beats runtime config.

### Acceptance criteria

- [ ] A test writes 100 KB of messages, calls `log_pump` once, and asserts the file grew by at most 4 KB + the size of one `[LOGGER] N dropped` line.
- [ ] A test fills the ring beyond 2 MB and confirms the next pump produces a `[LOGGER] N messages dropped` line with the correct N.
- [ ] A test sets `severity_threshold=WARN` in a test ini, calls `log_init` against it, fires `LOG_INFO`, and confirms zero output.
- [ ] A test sets `AIMOVE=false` in a test ini and confirms `LOG_INFO(AIMOVE, ...)` produces no output while other categories still log.
- [ ] A test confirms the compile-time floor beats a permissive ini setting (e.g., ini says `TRACE` but `STU_LOG_MIN_SEVERITY=LOG_SEV_INFO`, `LOG_TRACE` still strips).
- [ ] `ReMoM.ini` ships with a `[Logging]` section example showing all keys and defaults.

---

## Phase 4: File rotation + crash recovery

**User stories**: 6, 7, 12

### What to build

`log_init` performs the 3-file rotation dance: delete `_previous` if present, rename `_current` → `_previous`, rename `_new` → `_current`, open fresh `_new`. `log_init` also registers an `atexit` handler that calls `log_flush_all`. Signal handlers installed for `SIGSEGV`, `SIGABRT`, `SIGFPE`, `SIGILL` on POSIX and `SetUnhandledExceptionFilter` on Win32. Each handler writes a `[CRASH] <signal>` line directly to the ring (bypassing the macro layer so it is signal-safe enough), calls `log_flush_all`, then chains to default handler so the process still dies. `log_flush_all` drains the entire ring synchronously, bypassing the 4 KB cap.

### Acceptance criteria

- [ ] A test creates fixture files (`_new` + `_current` + `_previous` with distinguishable contents), calls `log_init`, asserts post-rotation filenames and contents are correctly shifted.
- [ ] A test where only `_new` exists pre-init confirms it becomes `_current` and a new `_new` is created.
- [ ] A test where no log files exist pre-init confirms `log_init` succeeds and creates `_new`.
- [ ] A test calls `LOG_INFO`, then `exit(0)`, and asserts the message is on disk (atexit path).
- [ ] A platform-specific test (Win32) raises an access violation after `LOG_INFO`, confirms the message and a `[CRASH]` line appear in `_new`.
- [ ] A platform-specific test (POSIX) raises `SIGSEGV` after `LOG_INFO`, confirms same.
- [ ] A test calls `LOG_FATAL` with 200 KB of pre-existing messages in the ring and confirms all of it (plus the FATAL line) is on disk before `LOG_FATAL` returns control.

---

## Phase 5: HeMoM + ReMoMber integration

**User stories**: 10, 11 (full)

### What to build

Wire `log_init`/`log_pump`/`log_shutdown` into the HeMoM tick loop and the ReMoMber loop. Both targets produce `remom_log_*.txt` files in the same format as ReMoM. A headless HeMoM run with a curated scenario produces a log file that the existing tooling (grep, awk) can consume. Verify the rotation works correctly across consecutive runs of each target.

### Acceptance criteria

- [ ] HeMoM headless run produces `remom_log_new.txt` in the expected format.
- [ ] ReMoMber run produces `remom_log_new.txt` in the expected format.
- [ ] Running HeMoM twice in a row produces a `_current.txt` (from the first run) and a `_new.txt` (from the second), with `_previous.txt` empty or absent on the first cycle.
- [ ] Build under MSVC-debug succeeds for all targets.
- [ ] Manual verification: side-by-side comparison of a ReMoM log line and a HeMoM log line shows identical metadata structure.

---

## Phase 6: Agent skeleton + triage duty

**User stories**: 14, 19, 21, 22 (partial)

### What to build

`.claude/agents/log-curator.md` checked into the repo, modeled on the existing `hemom-tester` agent. Read-only tool permissions (`Read`, `Glob`, `Grep`, `Bash`, `Agent`). Agent description identifies the four duties and currently routes only the triage duty. First helper script `tools/log-tools/log_triage.py` lands: stdlib-only Python, reads a log file, computes counts by severity, lists ERROR/FATAL/CRASH/LOGGER lines with surrounding context, flags timestamp gaps >5s as possible hangs. Output is structured (JSON or columnar) for the agent to consume. Golden-fixture tests under `tools/log-tools/tests/`.

### Acceptance criteria

- [ ] `.claude/agents/log-curator.md` exists with read-only tool permissions explicitly listed.
- [ ] `tools/log-tools/log_triage.py` runs from the command line on a real log file and produces sensible output.
- [ ] At least three fixture log files under `tools/log-tools/tests/` cover: clean run, run with FATAL, run with dropped-message line.
- [ ] Each fixture has a corresponding test that asserts on the script's structured output (counts, list contents).
- [ ] Manual demo: invoke the agent against a real HeMoM-produced log; agent produces a one-page markdown summary that correctly identifies any FATAL/ERROR/drop events.
- [ ] Agent's prompt does not embed parsing logic — it always invokes `log_triage.py` for the analysis.

---

## Phase 7: Agent gains diff + migration-inventory + authoring duties

**User stories**: 15, 16, 17, 18, 20, 22 (full)

### What to build

Two more helper scripts land, plus the heuristic-only authoring-help duty.

`tools/log-tools/log_diff.py` — two modes: `--raw` normalizes timestamps to `<TS>` then runs unified diff; `--semantic` parses each line into `(severity, category, file:line, function, msg-template)`, masks numeric tokens in the template, builds a `Counter` of templates per `(category, severity)`, reports symmetric differences and count deltas. Default is `--semantic`.

`tools/log-tools/log_migrate.py` — greps `dbg_prn`/`trc_prn` call-sites using a regex tuned to the project's actual call patterns, applies severity heuristic (FATAL keywords → FATAL, "error"/"fail"/"unable" → ERROR, "warn"/"unexpected" → WARN, default → DEBUG) and category heuristic (from path: `MoM/src/AIMOVE.c` → AIMOVE, `platform/sdl3/` → PFL, ...). Output is a structured report listing `(file, line, current_call, proposed_LOG_call)`. Does NOT edit files.

Agent gains all four duties:
- **diff**: invokes `log_diff.py`, contextualizes (groups by category, flags new ERROR/FATAL on B side, surfaces drop-marker discrepancies).
- **migration inventory**: invokes `log_migrate.py`, presents results as a reviewable patch.
- **authoring help**: heuristic-only, no script. Agent prompt embeds a conventions table derived from `log_migrate.py`'s heuristics. When asked "what severity/category for X", agent reads surrounding code and recommends.

### Acceptance criteria

- [ ] `tools/log-tools/log_diff.py` exists, supports `--raw` and `--semantic`, defaults to `--semantic`.
- [ ] At least one fixture pair `A.log` / `B.log` under tests with both deterministic differences and benign-timing differences; raw-mode test asserts both kinds of difference appear, semantic-mode test asserts only the deterministic one appears.
- [ ] `tools/log-tools/log_migrate.py` exists, runs from the command line over the real `MoM/src/` and `STU/src/` trees, produces a non-empty inventory.
- [ ] A fixture `.c` file with handcrafted `dbg_prn`/`trc_prn` calls covering each heuristic branch; test asserts `log_migrate.py` proposes the expected severity and category for each.
- [ ] `log_migrate.py` never writes to a source file; explicit test confirms it.
- [ ] Agent prompt updated to dispatch all four duties; conventions table included for authoring help.
- [ ] Manual demo: ask the agent "what severity for a 'unit selection failed' message in `AIMOVE.c`?" — agent recommends `LOG_ERROR(AIMOVE, ...)` without invoking any script.

---

## Phase 8: Migration execution + old-logger deletion

**User stories**: 13

### What to build

Apply the migration agent across the codebase in per-subsystem batches. Each batch: agent inventories `dbg_prn`/`trc_prn` calls in one subsystem (e.g., AIMOVE, then COMBAT, then PFL, ...), proposes the `LOG_*` rewrites, human reviews and applies, batch is committed separately. Repeat until the inventory script returns empty.

Once empty: delete `dbg_prn`, `trc_prn`, `Debug_Log_Startup`, `Debug_Log_Shutdown`, `Trace_Log_Startup`, `Trace_Log_Shutdown`, `Debug_Log_File`, `Trace_Log_File`, `Log_ISO8601_DateTime` (if no remaining callers), `Debug_Log_FileName`, `Trace_Log_FileName`. `STU_DBG.c` shrinks to (or is deleted in favor of) only the `DBG_ORIG_*` diagnostic globals and `DBG_TST_*` flags (which are not logging). Update both `STU/src/CMakeLists.txt` and the project-root `Makefile.am` if `STU_DBG.c` is removed.

### Acceptance criteria

- [ ] At least one per-subsystem migration batch is committed and reviewable as a single commit.
- [ ] After all batches, `log_migrate.py` produces an empty inventory.
- [ ] `git grep -nw dbg_prn` and `git grep -nw trc_prn` return zero matches in `MoM/`, `MoX/`, `STU/`, `platform/`, `src/`, `tools/` (excluding `STU_DBG.c` if it still exists, and excluding archived docs / `.md` files that document the old API for history).
- [ ] `git grep -nw Debug_Log_File` and `git grep -nw Trace_Log_File` return zero matches outside `STU_DBG.c` (or are gone entirely if `STU_DBG.c` is deleted).
- [ ] MSVC-debug build succeeds after deletion.
- [ ] HeMoM regression run produces a non-empty `remom_log_*.txt` that triages clean (no new ERROR/FATAL introduced by the migration itself).
- [ ] `STU_DBG.h` either no longer declares `dbg_prn`/`trc_prn` or is deleted entirely.
