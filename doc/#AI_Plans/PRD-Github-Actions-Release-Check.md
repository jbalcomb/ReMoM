# PRD — GitHub Actions: Release Build Check

**Status:** Draft
**Owner:** TBD
**Date:** 2026-04-25
**Tracks:** Pre-push verification, layer 3 of 3 (server-side enforcement)

---

## Summary

Add a GitHub Actions workflow that runs `cmake --workflow --preset MSVC-release` on every push and pull request. Provides server-side enforcement of the same Release-build invariant verified by the IDE task and the local pre-push hook. Functions as the absolute-last-line-of-defense gate that cannot be bypassed by `git push --no-verify`.

## Problem

The IDE task (manual) and pre-push hook (local, bypassable) both depend on developer cooperation. CI is the only enforcement layer that:

1. Runs uniformly across all platforms and machines.
2. Cannot be skipped, ignored, or forgotten.
3. Produces a public, reviewable record of pass/fail per commit and per PR.
4. Catches breakage introduced by Linux clones whose pre-push hook (per the hook PRD's v1 scope) doesn't run.

The existing `.github/workflows/release.yml` is a tag-driven *packaging* workflow — it only fires on `v*` tag pushes and produces release artifacts. There is no workflow today that gates *every* push against a Release build. That's the gap this PRD addresses.

## Goals

- Run the `MSVC-release` workflow on every push to any branch and on every pull request.
- Surface the result as a GitHub status check that PR review enforces (configurable as a required check on `main` once the workflow is stable).
- Run on a Windows runner using the same toolchain (`cl.exe`, VS 2022) that local developers use, so failures are reproducible locally.
- Optionally also run a Linux job to catch portability regressions early.
- Complete in a reasonable time — faster than a full release-package build, since this gate runs continuously.

## Non-Goals

- **Replace the existing `release.yml` packaging workflow.** That one stays as-is for tagged releases.
- **Deploy artifacts.** No upload to releases, no S3, no anything. Pure pass/fail.
- **Run in lieu of the pre-push hook.** Local enforcement is still preferred (faster feedback, doesn't pollute commit history with "fix CI" commits). CI is the backstop, not the primary gate.
- **Replace the test suite or coverage workflows** (if any are added later). This is a build-and-link gate, not a comprehensive QA gate.

## Functional Requirements

1. **File location:** `.github/workflows/release-check.yml` (or similar — must not collide with the existing `release.yml`).
2. **Triggers:**
   - `push` to any branch.
   - `pull_request` (default events: opened, synchronize, reopened).
   - `workflow_dispatch` for manual reruns.
3. **Jobs:**
   - **`windows-release`:** runs on `windows-latest`, executes `cmake --workflow --preset MSVC-release` (or equivalent configure + build + test sequence), uploads logs on failure.
   - **(Optional, later) `linux-release`:** runs on `ubuntu-latest` using the `clang-debug` preset built in Release. Catches portability issues. Out of scope for v1; covered by tag-driven `release.yml` for now.
4. **Artifacts on failure:** upload the build directory log (or a tail of compile output) so contributors can debug without recreating the environment locally.
5. **Concurrency:** cancel-in-progress for the same PR / ref so that rapid pushes don't queue up duplicate builds.
6. **Caching:** cache the CMake build directory between runs to avoid full re-configure on every push. Key on the OS, generator, and preset; invalidate when `CMakeLists.txt` or `CMakePresets.json` changes.

## Acceptance Criteria

- [ ] A push to any branch triggers the `windows-release` job and surfaces a status check on the commit.
- [ ] A PR triggers the same job; passing status is visible on the PR.
- [ ] An intentionally broken commit (e.g. unguarded `trc_prn`) produces a red status check with the failing compile/link line in the workflow logs.
- [ ] A green status check correlates 1:1 with a clean local `cmake --workflow --preset MSVC-release` run.
- [ ] Workflow runs do not exceed [TBD — propose 15 minutes target] from queue to completion in steady state (after warm cache).
- [ ] Existing `release.yml` (tag-driven packaging) continues to work unchanged.
- [ ] Repository settings have `windows-release` configured as a required status check on `main` (or whatever the canonical branch is) once the workflow has run cleanly for ≥ a week.

## Implementation Notes

- The existing `release.yml` already establishes the Windows + SDL2 setup pattern (download SDL2 dev libs, configure with `-DSDL2_DIR=...`). This PRD's workflow should reuse that pattern verbatim — copy the `Download SDL2 development libraries` step and the configure step. The only material difference is the build target (`ReMoMber` only, possibly add `HeMoM`) and the lack of packaging.
- Consider extracting the SDL2-install logic into a composite action under `.github/actions/setup-sdl2/` so both workflows can reuse it. Out of scope for v1 if there's no immediate pain.
- Use `actions/checkout@v6` and `actions/cache@v4`. The CMake binary directory at `out/build/MSVC-release/` is a reasonable cache target.
- Pull-request from forks: secrets are unavailable in PR-from-fork runs by default; this workflow shouldn't need any secrets, so that's fine. Do NOT add `pull_request_target` — it has known security pitfalls and isn't needed here.
- Set `permissions:` block to the minimum required (likely just `contents: read`). No write needed for this gate.
- Failure mode: the workflow should fail fast — first step that errors aborts the job (default behavior).

## Risks / Open Questions

- **Cost.** Public GitHub repos get free Actions minutes; private repos have a quota. If the project remains private and CI burns through the quota, we may need to throttle (e.g., only run on PRs, not every push). Re-evaluate after a month of operation.
- **SDL2 download flakiness.** The `release.yml` downloads SDL2 from libsdl-org's GitHub releases. If that becomes unreliable, we may need to vendor or mirror the dev libs. Same risk as `release.yml`; not new.
- **Slow runner cold starts.** First run after a long idle period will be slow. Mitigation: cache the SDL2 download separately from the CMake build cache. Tunable later.
- **Windows-only v1.** Linux portability regressions won't be caught until the tag-driven `release.yml` runs on a tagged commit — i.e., release time. Acceptable for v1; raise as a follow-up to add the Linux job here.
- **Preset evolution.** If `MSVC-release` preset changes meaningfully (renamed, refactored), this workflow breaks. Same coupling as the IDE task and pre-push hook; acceptable.
- **Required-status-check timing.** Marking the check "required" too early — before the workflow has stabilized — will block legitimate work. Stage it: ship the workflow first, observe ≥ a week, then flip the required flag.

## Dependencies

- **PRD-VSCode-Task-Safe-To-Push.md:** independent.
- **PRD-Git-Hook-Pre-Push-Release-Check.md:** independent and complementary. CI catches what the hook misses (Linux, `--no-verify` bypass, anyone who didn't enable `core.hooksPath`).
- Existing `.github/workflows/release.yml` (for SDL2 setup pattern reuse).
- `MSVC-release` preset in `CMakePresets.json` (already exists).
