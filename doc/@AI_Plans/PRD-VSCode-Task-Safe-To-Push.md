# PRD — VS Code Tasks: "Safe to Push" Compound Check

**Status:** Draft
**Owner:** TBD
**Date:** 2026-04-25
**Tracks:** Pre-push verification, layer 1 of 3 (manual / IDE-side)

---

## Summary

Add a set of VS Code tasks that together verify the codebase is ready to push. Structured as **named atomic sub-tasks** (one per build-configuration, plus tests and packaging) **plus a compound task** that orchestrates them. The compound `check: safe-to-push` runs both `MSVC-debug` and `MSVC-release` builds — catching both Debug-only and Release-only breakage in one invocation. Provides a deterministic IDE-side gate that surfaces unwrapped `dbg_prn` / `trc_prn` calls (and other configuration-conditional defects) before any push.

## Problem

The repo has two relevant build configurations:

- **`MSVC-debug` (Debug, `STU_DEBUG` defined):** declarations for `dbg_prn` / `trc_prn` are visible. Calls to those symbols compile and link regardless of whether they're guarded by `#ifdef STU_DEBUG`. Conversely, code inside `#ifdef STU_DEBUG` blocks gets compiled — so a typo in a debug-only block stays hidden until you turn `STU_DEBUG` off.
- **`MSVC-release` (Release, `STU_DEBUG` not defined):** those declarations are hidden. Any unguarded call becomes an undefined symbol — compile warning + link error. Conversely, debug-only code is excluded — so a release-only conditional that depends on a missing helper goes unnoticed until someone actually builds Debug.

Today the developer only runs `MSVC-debug` interactively. Both directions of breakage (Debug-only and Release-only) are invisible unless someone manually switches presets. The result: configuration-conditional bugs accumulate silently and are discovered late.

A single-config check fixes only half the problem. A compound task that builds **both** configurations catches both directions in one invocation — and the structure scales naturally to additional gates (lint, format, license audit) as the project's pre-push policy grows.

## Goals

- Provide a set of **atomic, named sub-tasks** — one per logical step of verification (per-config build, tests, packaging) — that can each be invoked individually for fast iteration.
- Provide a **compound task `check: safe-to-push`** that orchestrates the sub-tasks via `dependsOn`. Running it should be the developer's single "am I ready to push?" gesture.
- Build **both** `MSVC-debug` and `MSVC-release` configurations — catches Debug-only and Release-only breakage in one invocation.
- Surface compile/link errors in the **Problems** panel with clickable file:line navigation. Each sub-task gets its own dedicated terminal panel so the failing step is immediately identifiable without scrolling through a unified workflow log.
- Be **compositional**: the same atoms should be reusable in additional compound gates (`check: quick`, `check: safe-to-publish`) without duplication.
- Zero infrastructure: no new dependencies, no extension installs beyond what's already in use.
- Discoverable by name from `Tasks: Run Task...` and from the Command Palette.

## Non-Goals

- **Enforcement.** This task does nothing unless invoked. Forcing it on `git push` is the pre-push hook's responsibility (see `PRD-Git-Hook-Pre-Push-Release-Check.md`).
- **Server-side gating.** Runs only on the developer's machine. CI gating is the GitHub Actions workflow's job (see `PRD-Github-Actions-Release-Check.md`).
- **Build-system changes.** All work happens at the VS Code layer; CMake is invoked unchanged.

## Functional Requirements

### Atomic sub-tasks

Add the following named tasks to `.vscode/tasks.json`. Each is a real shell task with a `command` and a `$msCompile` problem matcher:

| Task label | Command (illustrative) | Purpose |
|---|---|---|
| `build: MSVC-debug` | `cmake --build --preset MSVC-debug` | Verify Debug compiles + links |
| `build: MSVC-release` | `cmake --build --preset MSVC-release` | Verify Release compiles + links (catches unguarded `dbg_prn`/`trc_prn`) |
| `test: MSVC-release` | `ctest --preset MSVC-release` | Run the test suite against the Release build |
| `package: MSVC-release` | `cmake --build --preset MSVC-release --target package` | (Optional) verify CPack packaging step is healthy |

Each sub-task:
- Uses `"presentation": { "reveal": "always", "panel": "dedicated" }` so output is discoverable but doesn't clobber the active terminal.
- Uses `"problemMatcher": ["$msCompile"]` so errors land in the Problems panel.
- Can be invoked individually from `Tasks: Run Task...`.

### Compound task: `check: safe-to-push`

A task with no `command`, only a `dependsOn` list, that orchestrates the sub-tasks:

```jsonc
{
  "label": "check: safe-to-push",
  "dependsOn": [
    "build: MSVC-debug",
    "build: MSVC-release",
    "test: MSVC-release"
  ],
  "dependsOrder": "sequence",
  "group": { "kind": "build", "isDefault": false }
}
```

- `dependsOrder: "sequence"` is required so a failing earlier step short-circuits later ones (avoids running the test suite when the build doesn't link).
- Optionally also depend on `package: MSVC-release` if packaging health is part of "safe-to-push." Recommend leaving it out of v1 — packaging adds time without adding push-safety information.

### Compositional reuse (optional, follow-on)

The same atomic sub-tasks should also support additional compound gates without duplication:

| Compound | `dependsOn` | When to run |
|---|---|---|
| `check: quick` | `build: MSVC-debug` only | per-save sanity (sub-second after warm cache) |
| `check: safe-to-push` | both builds + tests | pre-push (this PRD) |
| `check: safe-to-publish` | the above + future lint/format/license atoms | pre-release |

These are **out of scope for v1 implementation** but should not require structural changes to ship them later — that's the point of factoring sub-tasks out as atoms.

### Exit status

- Compound returns `0` only when every sub-task in `dependsOn` returns `0`.
- Any non-zero sub-task aborts the chain (under `dependsOrder: "sequence"`) and propagates non-zero.
- Failure visible in: the dedicated terminal panel of the failing sub-task, the Problems panel (with file:line), and the task-runner status indicator.

## Acceptance Criteria

- [ ] All four atomic sub-tasks appear in `Tasks: Run Task...` and can be run individually.
- [ ] Compound task `check: safe-to-push` appears in `Tasks: Run Task...`.
- [ ] Running `check: safe-to-push` on a clean checkout of `main` runs Debug build → Release build → tests in sequence and exits 0.
- [ ] Introducing an unguarded `trc_prn(...)` call (test artifact, not committed) and running `check: safe-to-push` produces a non-zero exit at the `build: MSVC-release` step. The Problems panel shows the offending file:line. Subsequent sub-tasks (`test: MSVC-release`) do not run.
- [ ] Introducing a Debug-only-syntax-error inside a `#ifdef STU_DEBUG` block produces a non-zero exit at the `build: MSVC-debug` step (verifying the Debug-side direction of the gate).
- [ ] Reverting test artifacts and re-running produces a clean pass.
- [ ] Each sub-task runs in its own dedicated terminal panel; the failing sub-task's panel is the one revealed on failure.
- [ ] No new VS Code extensions are required to make this work; CMake Tools (already in use) is sufficient.

## Implementation Notes

- The atomic build commands (`cmake --build --preset <name>`) are equivalent to running the build phase of the corresponding workflow preset, but skipping configure/test/package phases when those aren't desired. Both `MSVC-debug` and `MSVC-release` configure presets already exist in `CMakePresets.json`.
- The compound task itself has **no `command`** — only `dependsOn` and `dependsOrder`. VS Code treats this as orchestration-only.
- `dependsOrder: "sequence"` (not `"parallel"`) is required so a failed earlier step short-circuits later ones. Parallel ordering would cost wall-clock time when both builds succeed but lose the short-circuit benefit and clutter terminal panels with steps that were doomed anyway.
- Initial run will take longer (full configure + both full builds); subsequent runs incremental against the per-preset binary directory.
- Optional but recommended: bind the compound to a keystroke via `keybindings.json` (e.g., `Ctrl+Alt+R`) so it's reachable without menu navigation.
- The shell field should match the user's terminal (typically PowerShell on Windows). Consider `"type": "shell"` with no explicit shell so VS Code uses the workspace default.
- Compound-task-specific limitation: VS Code does not aggregate problem-matcher output across sub-tasks into a single status. The Problems panel shows the union of all sub-task findings; the developer reads the dedicated terminal panel of the failing sub-task to identify which step broke. This is acceptable — and is in fact a feature, since it preserves which step found which problem.

## Risks / Open Questions

- **Build duration.** Both Debug and Release builds run sequentially. First-time cost is "Debug full build + Release full build"; steady-state cost is incremental on whichever files changed. Acceptable for an explicit "I'm about to push" gate, less so for "I want to check this every few minutes." Mitigation: have `check: quick` (Debug only) for fast iteration; reserve the compound for actual pre-push moments.
- **Naming.** "Safe to push" implies more than the task currently verifies (linkage and tests only — no license headers, secrets, asset cleanliness, etc.). The compositional design above (`check: safe-to-publish` as a superset) preserves the option to grow the gate without renaming. To revisit alongside broader pre-push policy.
- **Sub-task naming convention.** Recommended pattern: `<verb>: <preset>` (e.g., `build: MSVC-release`). Alternatives like `build:MSVC-release` (no space) are valid; pick one and apply consistently. Stable convention matters more than which one — once committed, the pre-push hook and CI workflow can reference these labels.
- **Cross-preset variants.** Long-term, similar atomic sub-tasks for `clang-debug` and `MSVC-nosound-debug` may be warranted, plus a `check: safe-to-publish` compound that includes them. Out of scope for v1; the structure accommodates them without rework.
- **Sub-task observability vs noise.** Each sub-task gets its own dedicated terminal panel, which is great for identifying failures but creates panel clutter on success. VS Code's `revealProblems: "onProblem"` option can suppress reveal on success — worth specifying explicitly in v1.

## Dependencies

- VS Code (any recent version).
- CMake Tools or generic CMake CLI on `PATH`.
- `MSVC-release` preset in `CMakePresets.json` (already exists).
