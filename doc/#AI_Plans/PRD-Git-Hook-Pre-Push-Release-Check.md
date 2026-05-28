# PRD — Git Pre-Push Hook: Release Build Check

**Status:** Draft
**Owner:** TBD
**Date:** 2026-04-25
**Tracks:** Pre-push verification, layer 2 of 3 (local enforcement)

---

## Summary

A repository-tracked `pre-push` Git hook that aborts `git push` when the codebase fails to build in Release configuration. The hook is checked into the repo at `tools/git-hooks/pre-push` and activated per-clone via `git config core.hooksPath tools/git-hooks`. Provides automatic local enforcement of the same Release-build invariant that the VS Code task verifies on demand.

## Problem

Release-only build breakage (unguarded `dbg_prn` / `trc_prn` calls being the immediate motivator) currently has no enforcement. Developers can — and historically do — push commits that compile cleanly in `MSVC-debug` but fail in `MSVC-release`. The breakage is then either caught by CI (which is post-push, after the artifact is already public) or by a future developer trying to ship a Release build (sometimes weeks later).

A local pre-push hook closes the gap before the bad commit ever leaves the developer's machine. The IDE task is a manual ceremony; the hook is automatic.

## Goals

- Block any local `git push` whose post-push tip would fail to build with `MSVC-release`.
- Live in the repo (not `.git/hooks/`) so every clone has the same logic. Each clone enables it once via `git config core.hooksPath tools/git-hooks`.
- Provide a clear, actionable failure message that points at the failed step and suggests the corresponding remediation (e.g., "wrap your `trc_prn` call in `#ifdef STU_DEBUG`").
- Allow opt-out via `git push --no-verify` for emergencies.

## Non-Goals

- **Server-side enforcement.** That's the GitHub Actions workflow's job. The hook is local-only and bypassable.
- **Pre-commit checking.** Pre-push is sufficient; no need to slow individual commits.
- **Comprehensive QA.** The hook only verifies the Release build (and the tests/packaging that the workflow includes). It does not run linters, security scanners, or asset-license audits.
- **Auto-installation.** Adopting the hook requires the explicit `git config core.hooksPath` opt-in, documented in `AGENTS.md`. We do not silently enable hooks on `git clone` (would be a security smell).

## Functional Requirements

1. **File location:** `tools/git-hooks/pre-push` (executable, no extension — `git` requires the bare name).
2. **Shell:** `#!/usr/bin/env bash` shebang. Git ships a Bash on Windows (Git Bash) which is universally available; PowerShell-only would block Linux clones.
3. **Behavior:**
   - Read stdin (Git passes `<local-ref> <local-sha> <remote-ref> <remote-sha>` lines for each ref being pushed). For now, run the same check regardless of ref — refinement (only checking `main`-bound pushes, etc.) is a follow-up.
   - Run `cmake --workflow --preset MSVC-release` from the repo root.
   - Exit `0` to permit the push, non-zero to abort.
4. **Output:**
   - On failure: print the workflow's stderr/stdout plus a header line like `[pre-push] Release build failed — push aborted. Bypass with --no-verify if intentional.`
   - On success: print one line confirming the check passed; let `git push` continue normally.
5. **Activation documentation:** `AGENTS.md` (or `README.md`) gains a one-liner instructing each cloner to run `git config core.hooksPath tools/git-hooks` once.

## Acceptance Criteria

- [ ] After `git config core.hooksPath tools/git-hooks`, attempting `git push` from a working tree that builds cleanly in Release succeeds normally.
- [ ] On a tree where the Release build fails, `git push` aborts with a non-zero exit and prints the failing compile/link error.
- [ ] `git push --no-verify` bypasses the hook (Git default behavior; verify it remains intact).
- [ ] The hook works on Windows (Git Bash), Linux, and macOS without changes.
- [ ] AGENTS.md documents the one-time `core.hooksPath` setup.
- [ ] Reverting `core.hooksPath` (or unsetting it) restores default Git behavior — no residual side effects.

## Implementation Notes

- The hook reads stdin per the Git contract (`man githooks`, `pre-push` section). Even if we don't parse the input today, we should consume it so Git doesn't complain about a closed pipe.
- Cross-platform CMake invocation: `cmake --workflow --preset MSVC-release` is identical on all platforms. The preset itself is Windows-specific (uses `cl.exe`), so the hook implicitly only enforces on Windows clones. On Linux, the call will fail at configure time. Two options:
  - **(a)** Have the hook detect the host and call the appropriate preset (`clang-debug` Release on Linux). Adds complexity.
  - **(b)** Keep it Windows-only for v1 and document that Linux clones get no pre-push gate. CI (GitHub Actions) covers Linux server-side.
  - Recommend (b) for v1; revisit when Linux dev becomes regular.
- Build artifacts: the workflow writes to `out/build/MSVC-release/`. Hook should not delete that on success or failure — caller may want to inspect logs.
- Caching: the workflow includes a configure step; if the cache is current, that's fast. The build step is incremental. So in practice the hook adds only the cost of compiling actually-changed files plus running tests.
- Git's hook contract treats unset/non-executable hooks as "no hook." On Windows, file-mode bits are preserved by Git when `core.fileMode` is true. We should set `+x` on the file via `git update-index --chmod=+x tools/git-hooks/pre-push` before committing.

## Risks / Open Questions

- **Speed.** A multi-minute hook will lead to `--no-verify` muscle memory. Mitigations:
  - Only run the gate when pushing to `main` / a release branch (parse stdin).
  - Run only the build step, skipping tests/packaging in the hook variant — `cmake --build --preset MSVC-release` instead of `cmake --workflow`. Faster, still catches the linkage issues that motivated this PRD.
  - Add an opt-in fast mode via env var (`HOOK_FAST=1`).
- **Linux/macOS clones.** As noted under implementation, v1 only enforces on Windows. Acceptable for now; CI covers the rest.
- **Discoverability of `core.hooksPath`.** First-time cloners will skip the setup if the doc is buried. AGENTS.md placement matters; consider also a top-level `CONTRIBUTING.md` if/when the project goes public.
- **CRLF / line-ending issues on Windows.** Hook scripts must be `LF`-line-ending or Bash on Windows refuses them. `.gitattributes` should pin `tools/git-hooks/*` to `text eol=lf`.
- **Bypass discipline.** `--no-verify` is sometimes legitimate (hot-fixing CI itself), but if it becomes routine the hook is dead weight. No automated mitigation; cultural concern.

## Dependencies

- **PRD-VSCode-Task-Safe-To-Push.md:** independent. Either can ship first.
- **PRD-Github-Actions-Release-Check.md:** independent and complementary; CI catches what the hook misses (Linux, `--no-verify` cases, code that bypasses the gate).
- `MSVC-release` preset in `CMakePresets.json` (already exists).
- Git ≥ 2.9 for `core.hooksPath` support (universally available).
