---
applyTo: "**"
---

Ignore the Register annotations. (e.g., `_DI_`, `_SI_`, `_CX_`, etc.) They are just for reference when disassembling in IDA Pro 5.5.

# Project general coding standards

## Style Guide
- Use Allman style braces
- Wrap block comments at 180 characters
- Use "square", not "tile"

## Naming Conventions
- Use lower_snake_case for variable names  (e.g., )
- Use Proper_Snake_Case for function names  (e.g., Simulate_World_Map_Generation)
- Use UPPER_SNAKE_CASE for manifest constants and macro names
- Name shared modules by what they do, not by which target first uses them. A file/symbol prefix that names one consumer (e.g., `HeMoM_*` on code that is also linked into ReMoMber) reads as a layering violation and misleads readers. Reserve target-specific naming for code that genuinely cannot be linked into another target (e.g., main entry points, target-specific glue).

## Error Handling
- Always log errors with contextual information

## Verification — prefer deterministic tools over guessing
- When a question can be answered deterministically by running a tool, run the tool. Do not read source and claim "this should work" without verification.
- Compiler/linker is the authority for: will it compile, will it link, are types right, are includes correct. Build the affected target with the project's CMake preset (`cmake --build --preset MSVC-debug`) — never use ad-hoc build directories or bare `cmake -S . -B build`.
- `git log --diff-filter=A --format='%h %ai %an %s' -- <path>` and `git blame -L start,end <path>` are the authority for who introduced a piece of code and when. Do not assume.
- Word-boundary `grep` over the appropriate file types is near-canonical for C symbol cross-references. IDE "Find All References" without a clean compile_commands.json is also just text search — be honest about the limitation.
- Static analysis: MSVC compiler warnings are first-class — treat them like errors.
- Existing valgrind suppressions live in `valgrind-suppressions-all.supp`.
- The action loop is: change → run the smallest validating tool → read the output → fix and repeat → only then summarize. If a build wasn't run, say so explicitly ("unverified — please build to confirm") rather than implying validation that didn't happen.

## Pre-push gate (optional but recommended)
A repo-tracked `pre-push` Git hook lives at `tools/git-hooks/pre-push`. It mirrors the VS Code `check: safe-to-push` task: builds Debug, builds Release (catches unguarded `dbg_prn` / `trc_prn` and other Debug-only-symbol leakage), then runs the test suite. Short-circuits on first failure.

One-time setup per clone:
```
git config core.hooksPath tools/git-hooks
```

Bypass for emergencies:
```
git push --no-verify
```

The same gate is also wired up as the VS Code task `check: safe-to-push` (`Tasks: Run Task...`) for on-demand IDE-side runs. See `doc/PRDs/PRD-Git-Hook-Pre-Push-Release-Check.md` and `doc/PRDs/PRD-VSCode-Task-Safe-To-Push.md`.

## Glossary of Terms
- Dasm stands for Disassembly  (the disassembley of the Borland C++ 3.0 code from IDA Pro 5.5)
- DBG stands for Debug
- DEDU stands for Deduce  (like a to-do list item where some code or logic needs figured out or validated and then documented)
- DNE stands for Does Not Exist  (usually mean it seems like it should based on some similar code elsewhere or a bug or SimTex's coding style)
- IDK stands for I don't know
- IDGI stands for I don't get it
- NIU stands for Not In Use
- PFL stands for Platform
- OGBUG stands for Original Game Bug  (do not try to fix these in the base-line ReMoM code-base)
- src stands for Source
- dst stands for Destination
- sgmt stands for Segment
- ofst stands for Offset
