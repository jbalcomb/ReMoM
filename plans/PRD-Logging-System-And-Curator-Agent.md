# PRD: ReMoM Logging Subsystem & Log-Curator Agent

> **Reference:** Builds on and supersedes [PRD-C89_Game_Logging_System_Design-Gemini.md](PRD-C89_Game_Logging_System_Design-Gemini.md). The Gemini doc defines the ring-buffer mechanics; this PRD expands it with metadata, runtime controls, dropped-message accounting, headless behavior, a full migration plan, and a custom Claude Code agent to manage all of it.

## Problem Statement

The current ReMoM logging path (`dbg_prn`, `trc_prn` in `STU/src/STU_DBG.c`) calls `vfprintf` followed by `fflush` on every message. Every log line is a synchronous disk-touching call. This was acceptable during early reconstruction, but is now load-bearing in the wrong places:

- The AI walkthrough work (AIMOVE, AIDUDES, AISPELL, AIDATA) wants high-volume per-turn / per-unit / per-stack tracing without paying frame-pacing penalties.
- Combat and main-screen rendering paths also need to be loggable without hitching.
- The HeMoM headless runner produces logs that need to be diffed across replay runs to catch behavioral regressions, but the current format has no machine-parseable structure (no severity, no category, no consistent metadata).
- When the process dies mid-frame, anything that the OS hadn't flushed is lost — but the existing `fflush`-every-call style sacrifices throughput across the board to mitigate this.
- 20+ files already call `dbg_prn`/`trc_prn`; any new design has to handle a real migration, not greenfield design.

Beyond the engine itself, manually triaging logs, hand-writing log-call additions with correct severity, and eyeballing diffs between replay runs are all chores that scale badly. We want a dedicated agent that knows the conventions and can do those chores correctly the first time.

## Solution

Two deliverables in one feature branch:

1. **A new C89 logging subsystem (`STU_LOG`)** built on a 2 MB static ring buffer with per-frame trickle-write, structured message metadata (timestamp, severity, category, file:line, function), runtime configuration via `ReMoM.ini`, drop accounting, and crash-safe flush via `atexit` + signal/SEH handlers. Replaces `dbg_prn` and `trc_prn` everywhere.

2. **A `log-curator` Claude Code subagent** (read-only tool access) plus three helper scripts under `tools/log-tools/`. The agent owns four duties: triaging live logs, helping author new log call-sites with correct severity/category, managing the dbg_prn/trc_prn → `LOG_*` migration, and diffing logs across runs (raw and semantic modes).

The new logger lands and stabilizes on a separate branch first. Migration of existing call-sites happens after the new system is proven, via the curator agent in a series of bounded, reviewable passes.

## User Stories

### Logging subsystem

1. As a ReMoM developer debugging an AI turn, I want to add a `LOG_TRACE(AIMOVE, "stack %d picked %s", id, name)` call inside a hot loop, so that I can trace behavior at high volume without measurable frame impact.
2. As a ReMoM developer, I want every log message to carry severity, category, timestamp, file:line, and function name, so that I can filter and grep logs after the fact.
3. As a ReMoM developer, I want the logger to silently drop messages when the ring buffer fills, so that the engine never blocks on disk I/O during gameplay.
4. As a ReMoM developer triaging silent gaps in logs, I want a synthetic `[LOGGER] N messages dropped since last drain` line emitted by `Pump()` whenever drops occurred, so that I can tell "nothing happened" apart from "buffer overflowed."
5. As a ReMoM developer, I want the per-frame disk write capped at 4 KB, so that a sudden burst of logging doesn't cause a stutter even if the queue is full.
6. As a ReMoM developer, I want the log to survive crashes — both clean `exit()` and `SIGSEGV` / Win32 access violations — so that the last messages before the crash are on disk.
7. As a ReMoM developer running back-to-back sessions, I want a 3-file rotation (`remom_log_new.txt` → `_current` → `_previous`) handled at startup, so that I always have the prior run available without managing files manually.
8. As a ReMoM player, I want a `[Logging]` section in `ReMoM.ini` where I can set severity threshold and per-category enables, so that I can crank up logging for a problem area without recompiling.
9. As a release-build user, I want `TRACE` and `DEBUG` severities compiled out entirely in Release, so that I pay no runtime cost (not even a function call) for stripped messages.
10. As a HeMoM headless-test author, I want the same logger to drive HeMoM output to the same `remom_log_*.txt` files, so that my replay-vs-record diffs operate on consistent format.
11. As an embedder of HeMoM in CI, I want `log_pump()` exposed publicly so I can call it at my own frame boundary, so that headless runs drain the ring deterministically.
12. As a ReMoM developer, I want to be able to call `LOG_FATAL(...)` and have the system flush the entire ring before the process terminates, so that the fatal message is never lost.
13. As a ReMoM developer, I want the `dbg_prn` and `trc_prn` symbols deleted (not left as wrappers) after migration completes, so that there is exactly one logging path in the codebase.

### Log-curator agent

14. As a ReMoM developer, I want to invoke `log-curator triage <path>` on a `remom_log_current.txt`, so that I get a one-page summary of FATALs, ERRORs, dropped-message events, and crash markers without reading 200 KB by hand.
15. As a ReMoM developer adding logging to a new function, I want to ask the curator agent "what severity and category should I use here?" so that the codebase stays consistent without me memorizing the rules.
16. As a ReMoM developer in the middle of migration, I want the agent to inventory remaining `dbg_prn`/`trc_prn` call-sites and propose `(severity, category)` for each, so that I can review and apply in batches.
17. As a ReMoM developer comparing two runs (e.g., before/after an AI change), I want `log-curator diff --semantic A.log B.log`, so that benign timing/ordering differences don't drown out the real behavioral divergence.
18. As a ReMoM developer doing forensic analysis, I want `log-curator diff --raw A.log B.log` for an unfiltered unified diff with timestamps normalized, so that I can see every byte that differs.
19. As a ReMoM developer, I want the curator agent to default to read-only tools, so that running it never accidentally rewrites my source.
20. As a ReMoM developer doing the migration, I want the agent to emit a patch I can review (not edit my files directly), so that the migration stays auditable.
21. As a ReMoM project maintainer, I want the curator agent definition checked into `.claude/agents/`, so that it ships with the repo and every contributor gets the same behavior.
22. As a ReMoM project maintainer, I want the agent's helper scripts to be plain-language Python under `tools/log-tools/` (not embedded in the agent prompt), so that they are testable and runnable by humans too.

## Implementation Decisions

### `STU_LOG` module

- Lives at `STU/src/STU_LOG.c` and `STU/src/STU_LOG.h`. Naming follows the project convention of `STU_` for shared-across-targets components living in `STU/src/`. `STU_DBG.c` shrinks to (or is fully deleted in favor of) the new module after migration completes.
- Single 2 MB static `char[]` ring buffer, `head`/`tail` indices, write-wraps-at-boundary. Matches Gemini's mechanics exactly.
- Public surface is six severity macros — `LOG_TRACE(cat, fmt, ...)`, `LOG_DEBUG`, `LOG_INFO`, `LOG_WARN`, `LOG_ERROR`, `LOG_FATAL` — plus `log_init()`, `log_shutdown()`, and `log_pump()`. Macros capture `__FILE__`, `__LINE__`, `__FUNCTION__` at call-site (zero runtime cost for the captures themselves).
- In Release builds, `LOG_TRACE` and `LOG_DEBUG` expand to the same `((void)sizeof(...))` idiom used by the existing `DLOG` macro so that argument expressions are syntactically validated but never evaluated.
- Categories are an enum (`LOG_CAT_AIMOVE`, `LOG_CAT_COMBAT`, `LOG_CAT_SAVE`, `LOG_CAT_PFL`, `LOG_CAT_IKI`, ...) defined in `STU_LOG.h`. New categories are added centrally — agent will enforce the convention.
- Write logic: format to a 1 KB stack buffer with `_vsnprintf`/`vsnprintf` (preprocessor gate), prepend metadata header, copy into ring. If would-overtake-tail: increment a `dropped_since_last_pump` counter and return.
- `log_pump()`: drains up to 4 KB to disk; if any drops were recorded since last call, emits a single `[LOGGER] N messages dropped` synthetic line first.
- `log_pump()` is publicly exposed; callers (ReMoM frame loop, HeMoM tick loop, ReMoMber loop) wire it in at their own frame boundary.
- `log_fatal()` (called by `LOG_FATAL` macro after writing its own message) calls a `log_flush_all()` internal function that drains the entire ring synchronously, bypassing the 4 KB cap.

### Runtime configuration

- A new `[Logging]` section in `ReMoM.ini` with keys: `severity_threshold` (one of TRACE/DEBUG/INFO/WARN/ERROR/FATAL), and one boolean per category (`AIMOVE=true`, etc.). Defaults baked in if section absent.
- Compile-time `STU_LOG_MIN_SEVERITY` macro acts as a hard floor — runtime ini cannot lower severity below it. Lets Release strip TRACE/DEBUG cleanly.
- `log_init()` reads ini and populates a static config struct; macros consult it on the hot path.

### Crash recovery

- `log_init()` registers an `atexit()` handler that calls `log_flush_all()`.
- `log_init()` also installs platform signal handlers: `SIGSEGV`, `SIGABRT`, `SIGFPE`, `SIGILL` on POSIX; `SetUnhandledExceptionFilter` on Win32. Each writes a `[CRASH] <signal>` line directly to the ring, then calls `log_flush_all()`, then chains to the default handler.
- Handlers are async-unsafe-tolerant only to the degree the single-threaded model allows — we accept that signal handlers calling stdio is technically UB on some platforms, but consistent with the project's single-threaded posture and acceptable for a crash-time-only path.

### File rotation

- Exact Gemini scheme: at `log_init()`, delete `remom_log_previous.txt` if present, rename `remom_log_current.txt` → `remom_log_previous.txt`, rename `remom_log_new.txt` → `remom_log_current.txt`, open fresh `remom_log_new.txt`.
- Files live in the same working directory as the executable (matches current `Trace_Log_FileName` convention).

### Message format

Each line written to disk follows this format:

```
[2026-05-25T14:32:01Z] [WARN ] [AIMOVE] AIMOVE.c:1234 AI_Stacks_Peacetime_Ocean_Movement_And_Cleanup: stack %d found no valid destination
```

Fixed-width severity tag, fixed-width category tag, file:line, function name, then the formatted message. The fixed-width fields make awk/grep one-liners predictable and make the semantic diff trivial to implement.

### Migration

- **Phase 1 (this PRD):** Land `STU_LOG` alongside the existing `dbg_prn`/`trc_prn`. New code uses `LOG_*` macros. Existing call-sites unchanged. Both loggers active. Verify on at least one feature (e.g., the active AIMOVE walkthrough work) that the new path works under real load.
- **Phase 2 (follow-up, agent-driven):** `log-curator migrate` inventories every `dbg_prn`/`trc_prn` call-site and emits a proposed mapping `(file, line, current_call, proposed_LOG_call)`. Human reviews and applies in batches per-file or per-subsystem.
- **Phase 3 (follow-up):** When inventory is empty, `dbg_prn`, `trc_prn`, `Debug_Log_*`, `Trace_Log_*` symbols and the `Debug_Log_File`/`Trace_Log_File` globals are deleted. `STU_DBG.c` either disappears or shrinks to just the `DBG_ORIG_*` diagnostic globals (which are not logging).

### `log-curator` subagent

- Lives at `.claude/agents/log-curator.md`. Modeled on the existing `hemom-tester` subagent (same general shape: trigger description, tool permissions, behavior guidelines).
- **Tool access (read-only default):** `Read`, `Glob`, `Grep`, `Bash` (for invoking helper scripts), `Agent` (for delegating subsearches). No `Edit`, `Write`, or `NotebookEdit` — outputs are patches or markdown reports for the user to apply.
- Four duties, each backed by either a helper script or a documented heuristic:
  - **Triage:** invokes `tools/log-tools/log_triage.py <path>`; agent interprets the script's structured output and writes a markdown summary.
  - **Authoring help:** when asked "what severity/category for X?", agent reads the surrounding code, consults the conventions table embedded in the agent prompt, and recommends. Heuristic-driven, no script.
  - **Migration:** invokes `tools/log-tools/log_migrate.py [--path <glob>]`; for each `dbg_prn`/`trc_prn` call-site, proposes `(severity, category)` based on filename, surrounding function name, and message-content keyword heuristics. Emits a patch.
  - **Diff:** invokes `tools/log-tools/log_diff.py [--raw|--semantic] A B`; agent contextualizes the diff (groups divergences by category, flags any new ERROR/FATAL on the B side, surfaces dropped-message-marker discrepancies).

### Helper scripts

- `tools/log-tools/log_triage.py` — single file, stdlib-only Python. Reads a log file, returns counts by severity, lists every ERROR/FATAL/CRASH/LOGGER line with surrounding context, flags any time gap >5s in timestamps as a possible hang or drop window.
- `tools/log-tools/log_migrate.py` — single file, stdlib-only Python. Greps `dbg_prn`/`trc_prn` call-sites using a regex matched to the project's actual call patterns, applies the proposed-severity heuristic (FATAL keywords → FATAL, "error"/"fail"/"unable" → ERROR, "warn"/"unexpected" → WARN, default → DEBUG; category from path: `MoM/src/AIMOVE.c` → AIMOVE, `platform/sdl3/` → PFL, etc.). Output is a structured report (JSON or columnar text) that the agent or a human can convert to actual edits.
- `tools/log-tools/log_diff.py` — single file, stdlib-only Python. Two modes: `--raw` normalizes timestamps to `<TS>` then runs unified diff. `--semantic` parses each line into `(severity, category, file:line, function, msg-template)`, masks numeric tokens in the template, builds a `Counter` of templates per (category, severity), reports symmetric differences and count deltas.

## Testing Decisions

A good test exercises behavior at a module's public boundary. For `STU_LOG`, the public boundary is "messages in, file contents out (after pump)" — tests must never reach into the ring buffer internals or assert on `head`/`tail` indices.

- **Module under test:** `STU_LOG`.
- **Test harness:** a small C89 test program that calls `log_init` against a temp directory, fires a series of `LOG_*` calls, calls `log_pump()`, then reads back `remom_log_new.txt` and asserts on its contents. Lives under existing test conventions for this repo.
- **Specific behaviors to test:**
  - Basic round-trip: a single `LOG_INFO` survives pump and appears in the file with correct metadata fields.
  - Rotation: a fresh `log_init` with all three files present renames correctly, opens new file, doesn't lose previous data.
  - Drop accounting: filling the ring beyond capacity produces a `[LOGGER] N messages dropped` synthetic line on next pump, with the correct N.
  - Pump cap: writing 100 KB worth of messages and pumping once writes at most 4 KB; remaining drains over subsequent pumps.
  - Severity filter: setting `severity_threshold=WARN` causes `LOG_INFO` calls to produce no output.
  - Crash flush: a deliberate `LOG_FATAL` followed by `exit(1)` produces a file ending in the FATAL line (atexit path).
- **Helper scripts:** each gets a pytest-style test file under `tools/log-tools/tests/` with golden inputs (a few short fixture logs checked into the test directory). Tests assert on the structured output of each script, not on its stdout formatting.
- **Out of scope for automated tests:** the subagent itself. Agent behavior is exercised by humans via the four documented invocations; we rely on the helper scripts (which are tested) for correctness of the underlying analysis.

Prior art in this repo: HeMoM tests under the HeMoM headless infrastructure already use a "drive an entry point with curated inputs, assert on outputs" pattern; the `STU_LOG` test follows the same shape.

## Out of Scope

- **Multi-threaded logging.** The Gemini PRD's single-threaded constraint stands. No mutexes, no atomics. If a future target ever needs MT logging, that is a separate PRD.
- **Network/remote logging.** Disk-only. No syslog, no journald, no HTTP sinks.
- **Structured (JSON/protobuf) output.** Lines are human-readable plain text with fixed-width fields. The semantic-diff script does the structured parsing on the consumption side, not at write time.
- **Log compression/archival.** Beyond the 3-file rotation, retention is not the logger's job.
- **Migration of the `DLOG` macro.** `DLOG` already gates on `STU_DEBUG`. Whether it gets folded into `LOG_DEBUG` is a follow-up call once the new system is proven.
- **The `DBG_ORIG_*` diagnostic globals** in `STU_DBG.h` are not logging — they are reference pointers for debugging save-file divergence. They stay where they are.
- **HeMoM's existing test-recording infrastructure.** This PRD makes the logger work with HeMoM; it does not redesign HeMoM's own .RMR replay format.
- **Agent auto-triggering.** The `log-curator` agent is user-invoked only. Whether to add a hook that auto-runs `log_triage` after each HeMoM run is a separate decision.

## Further Notes

- The 4 KB/frame throughput cap (≈240 KB/sec at 60fps) is per the Gemini PRD. If profiling shows we're consistently filling the ring faster than this drains, the cap is the first knob to revisit — but the silently-drop-on-overflow behavior means the engine stays healthy either way.
- The signal-handler-calling-stdio caveat is intentional. A strict-correctness alternative would be to write the ring's raw bytes via `write(2)` / `WriteFile` in the handler, but this requires teaching the handler the ring's wrap logic. We accept the simpler approach and document it.
- The curator agent's "authoring help" duty is the squishiest one — it depends on a conventions table that doesn't exist yet. Bootstrapping plan: agent's first version contains a starter table derived from the migration script's heuristics; the table is refined as humans accept/reject its recommendations during Phase 2.
- The decision to ship the agent as a subagent + helper scripts (rather than embedding logic in the agent prompt) means the deterministic work (parsing, counting, diffing) is human-runnable and unit-testable. The agent provides judgment and synthesis on top.
- This PRD intentionally does not commit to a feature-branch name; the user names branches.
