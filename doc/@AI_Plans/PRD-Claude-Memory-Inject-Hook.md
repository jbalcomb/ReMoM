# PRD — Claude Code `UserPromptSubmit` Hook: Memory Re-injection

**Status:** Draft
**Owner:** TBD
**Date:** 2026-05-03
**Tracks:** Claude Code agent reliability — defend against memory loss across context compactions

---

## Summary

Add a `UserPromptSubmit` hook (configured in `.claude/settings.local.json`) plus a small companion script that re-injects the contents of the project's `MEMORY.md` into the assistant's context on every user prompt via the hook's `additionalContext` mechanism. This compensates for the lack of a `PostCompact` / `InstructionsReloaded` hook event in Claude Code: per current Claude Code docs (verified 2026-05-03), `MEMORY.md` and the linked individual memory files are loaded at session start but are **not** guaranteed to be re-injected after the harness compacts context. The result, observed empirically in this project, is that the assistant "forgets" durable preferences mid-session and repeats mistakes the user has already corrected.

## Problem

Multiple incidents in a single recent session illustrate the failure mode:

- The assistant deleted an original `Randomize();` line in `MoM/src/LoadScr.c` while restructuring the surrounding logic. The "Editing Original Game Code" rule in `MEMORY.md` ("NEVER delete original game code. Instead, comment it out and add the new code below/after it.") is exactly the kind of rule the assistant has internalized — but only as long as it remains visible in context. The user had to catch and correct.
- The assistant wrote and committed a script (`tools/regenerate_goldens.sh`) without running it, then described it as ready. The "Prefer deterministic tools over guessing" rule is in `MEMORY.md` and was even discussed earlier in the same session. It was effectively invisible by the time the script-write moment arrived.
- The assistant added a `.c` file to `MoX/src/CMakeLists.txt` but not to `Makefile.am`, breaking the Linux autotools build. A "dual build systems" memory entry was added immediately after this incident — but the same failure mode applies: nothing forces the entry to remain in context.

The common pattern: durable rules live in `MEMORY.md`, but their *visibility* depends on harness behaviour the assistant cannot influence. As context grows and the harness compacts, the rules disappear, and the assistant cannot reliably notice the disappearance.

A solution that does not depend on the assistant's good behaviour is required.

## Goals

- Re-inject `MEMORY.md` content into the assistant's context on every user prompt, regardless of compaction state.
- Be invisible to the assistant in normal operation — no per-prompt user effort.
- Be invisible to the user — no console noise or perceptible latency.
- Stay self-contained on the user's machine: opt-in via `.claude/settings.local.json` rather than a repo-tracked `settings.json` (memory files are per-user and per-project anyway).
- Tolerate `MEMORY.md` being absent, malformed, or empty without breaking prompts.

## Non-Goals

- Modifying the format or content of `MEMORY.md`.
- Auto-loading the individual memory files linked from `MEMORY.md`. Drill-downs into specific memory files remain on-demand.
- General-purpose context preservation across compaction. The hook only addresses memory-file rehydration.
- Cross-project portability. The hook config and script paths are tied to one project's working directory. A user-global variant could be designed later.

## Functional Requirements

1. **Hook config** — added to `.claude/settings.local.json`:
   ```jsonc
   {
     "hooks": {
       "UserPromptSubmit": [
         {
           "hooks": [
             { "type": "command", "command": "<absolute-path-to-script>" }
           ]
         }
       ]
     }
   }
   ```
   `.claude/settings.local.json` is per-clone (untracked) — local to one machine.

2. **Companion script** — proposed location: `tools/inject-memory.sh` (POSIX shell, works under Git Bash / MSYS2 / WSL / real Linux).

   Behaviour:
   - Resolves the absolute path of the project's `MEMORY.md` file. Path depends on `$USERPROFILE`/`$HOME` and the Claude-Code-derived project key (e.g. `c--STU-devel-ReMoM`).
   - Reads the file. If absent or empty, emits a no-op JSON envelope and exits 0.
   - Otherwise, emits valid JSON to stdout matching the Claude Code `UserPromptSubmit` hook output schema:
     ```json
     {
       "hookSpecificOutput": {
         "hookEventName": "UserPromptSubmit",
         "additionalContext": "<MEMORY.md contents, JSON-escaped>"
       }
     }
     ```
   - JSON-escapes the content correctly (quotes, backslashes, newlines, control chars). Recommend `jq -Rs '.'` or a Python one-liner; fall back to a manual escape only if neither is available.

3. **Optional Windows variant** — `tools/inject-memory.cmd` or `.ps1` if the user runs hooks under cmd.exe / PowerShell instead of Git Bash. Same JSON contract.

4. **Documentation** — a brief note in `AGENTS.md` (or a new `doc/Devel-Claude-Memory-Hook.md`) describing what the hook does, where it's configured, and the activation steps.

## Acceptance Criteria

- After configuring the hook in `.claude/settings.local.json` and making the script executable:
  - Submitting any prompt in a fresh Claude Code session results in `MEMORY.md` content appearing as `additionalContext` in the next assistant turn (verifiable via Claude Code session logs / `--debug` mode).
  - Editing `MEMORY.md` and submitting a new prompt — without restarting — reflects the edit in the next turn.
  - Removing or temporarily renaming `MEMORY.md` does not break prompt submission; the assistant simply gets no extra context (and the script exits 0).
- Token-cost overhead is bounded and predictable — measured at install time, recorded in the doc.
- A test prompt issued *after* manually triggering a context compaction (`/compact`) shows that `MEMORY.md` content is still present in context on the post-compact turn.

## Implementation Notes

- Path resolution is the trickiest part. The Claude Code memory dir is at `$USERPROFILE\.claude\projects\<project-key>\memory\MEMORY.md` on Windows, `$HOME/.claude/projects/<project-key>/memory/MEMORY.md` on Unix. The `<project-key>` is derived from the absolute project path with `/` and `\` replaced by `-` and the leading drive letter `C:\` becoming `c--`. Hard to compute portably — easier to pin the absolute path in the script for now and document it as a per-machine setup detail.
- JSON escaping: prefer `jq -Rs '.'` (reads stdin, slurps as one string, JSON-escapes). If `jq` isn't available, `python -c "import json,sys; print(json.dumps(sys.stdin.read()))"` works on any system Python.
- Script latency: reading + escaping ~10 KB takes <50 ms on cold disk. Negligible per-prompt overhead.
- Token cost: current `MEMORY.md` is roughly 50 lines × ~80 chars/line ≈ 4 KB ≈ ~1000 tokens. Tolerable. Compare to the cost of one round-trip correcting a memory-failure mistake (typically 1000s of tokens of explanation + re-work).

## Risks / Open Questions

- **Token cost growth.** As `MEMORY.md` accumulates entries, per-turn cost grows linearly. Mitigations:
  - Inject only the "CRITICAL" header section, not the full file. Lighter; still catches the "must re-read after compaction" cases.
  - Periodically prune `MEMORY.md` for stale entries.
- **Privacy / sensitivity.** `MEMORY.md` may contain user-private notes the user wouldn't want surfaced verbatim in assistant output. Today's content doesn't seem sensitive, but worth flagging.
- **Path fragility.** The hard-coded `MEMORY.md` path will break if Claude Code's project-key derivation changes, or if the user moves the project. The script should fail loudly (exit 1 with a useful stderr message) when the path resolves to nothing — better than silently emitting no context.
- **Compaction-only injection.** Re-injecting on every prompt is wasteful when context is fresh. A more efficient pattern would inject only when a compaction was recently detected — but Claude Code doesn't expose a `PostCompact` hook today (per docs verified 2026-05-03). If/when one is added, this hook should be replaced by something that fires only on the compaction edge.
- **Two `settings.local.json` use cases competing.** If the user already has hook entries in this file for other reasons, a naive overwrite would lose them. The install instructions should explicitly say "merge into the existing hooks structure," not "replace."

## Dependencies

- Claude Code with `UserPromptSubmit` hook support and the documented `additionalContext` envelope (current as of 2026-05-03).
- POSIX shell (Git Bash, MSYS2 bash, or WSL bash) on Windows; native `bash` on Linux / macOS.
- `jq` strongly recommended; a Python 3 fallback is acceptable.

## Follow-ups (out of scope for v1)

- A user-global variant of the hook that resolves `MEMORY.md` paths automatically per project, installable from `~/.claude/settings.json`.
- A `PostCompact` Claude Code hook (would replace this PRD's mechanism with a more efficient one). Worth filing via `/feedback`.
- An `InstructionsReloaded` Claude Code hook covering both fresh-load and post-compaction.
- Selective injection: tag-based subsetting of `MEMORY.md` (e.g. only "CRITICAL" or "feedback" sections) to reduce token cost.
