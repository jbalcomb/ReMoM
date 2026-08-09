---
applyTo: "**"
---

Ignore the Register annotations. (e.g., `_DI_`, `_SI_`, `_CX_`, etc.) They are just for reference when disassembling in IDA Pro 5.5.

# Project general coding standards

## Editing Original Game Code — 100% fidelity to the disassembly is the target (hard rule)
- **The target is 100% fidelity to the disassembly.** The asm is ground truth. "Original code" means the reconstruction that is *faithful to the asm*.
- **NEVER delete code that is faithful to the asm** (anything with an asm counterpart) — not a line, a declaration, a comment, commented-out scratch, or whitespace. There is no "it's dead / unused / stale / scratch / cleaner" excuse. If faithful code must change, **comment out the original and add the replacement below it**, and mark changes `/* CLAUDE */`.
- **VERIFY against the asm before classifying anything as phantom.** Faithful-but-ugly code, `; `-style asm-derived comments, and reconstruction notes are NOT phantom — preserve them. When you cannot verify no-asm-counterpart, do NOT delete — ask.
- **Deleting and style-matching are two COMPLETELY DIFFERENT concerns.** Style conformance (renaming a reconstructed local to `lower_snake_case`, whitespace/formatting) removes nothing — allowed. Deleting faithful code is forbidden.

## Style Guide
- Use Allman style braces
- Wrap block comments at 180 characters
- Use "square", not "square"

## Naming Conventions
- Use lower_snake_case for variable names  (e.g., )
- Use Proper_Snake_Case for function names  (e.g., Simulate_World_Map_Generation)
- Use UPPER_SNAKE_CASE for manifest constants and macro names
- Name shared modules by what they do, not by which target first uses them. A file/symbol prefix that names one consumer (e.g., `HeMoM_*` on code that is also linked into ReMoM) reads as a layering violation and misleads readers. Reserve target-specific naming for code that genuinely cannot be linked into another target (e.g., main entry points, target-specific glue).

## Filenames
- One period per filename, reserved for the extension. Use underscores to separate stem components — `hemom_seed12345_stderr.log`, not `hemom.seed12345.stderr.log` or `hemom_seed12345.stderr.log`. Multiple dots confuse tools (shell glob behaviour, IDE syntax detection, archive extractors) and obscure which suffix is actually the extension.
- Use real, recognized extensions (`.log`, `.txt`, `.csv`, `.json`). Do not use words like `raw`, `trace`, `out`, or `dump` as extensions — they aren't recognized by editors/tools and trigger "unknown file type" handling. If you need to distinguish kinds of output, encode that in the stem (`*_stderr.log`, `*_rng.log`), not the extension.

## Shell and scripting (all agents)
- **Commands given to the user are Bash (Git Bash on Windows).** Use `/c/STU/devel/ReMoM/...` paths, real Unix tools, and `./ReMoM.exe`. Never hand over `C:\...` paths, PowerShell/cmd forms, or a prose description where a command belongs. This holds regardless of what an agent harness reports as the "primary" shell.
- **All scripting defaults to Python, invoked as `python3`.** Not bare `python`, not `py`, not a sed/awk one-liner, not PowerShell.
- **If `python3` cannot be found, STOP** — halt and ask. Do not fall back to `python`, `py`, or a shell equivalent. On this project's Windows boxes `python` is Python **2.7** while `python3` is **3.x**, so a silent fallback runs the wrong interpreter and produces confidently wrong results (a Python 2 run of a Python 3 script reports zero matches rather than failing, which reads as a data problem and wastes the time it takes to chase one). The Python 2 tell in output: `print("a:", x)` renders as `('a:', x)` — if you see that, fix the interpreter, don't rewrite the script.

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

The hook only runs when at least one ref being pushed targets `refs/heads/main`. Pushes to feature branches skip the gate so iterative work doesn't pay the full build+test cost. Run the VS Code `check: safe-to-push` task manually on a feature branch when you want to verify locally before opening a merge.

One-time setup per clone:
```
git config core.hooksPath tools/git-hooks
```

Bypass for emergencies (only meaningful on a main-bound push, since other pushes already skip the gate):
```
git push --no-verify
```

The same gate is also wired up as the VS Code task `check: safe-to-push` (`Tasks: Run Task...`) for on-demand IDE-side runs. See `doc/#AI_Plans/PRD-Git-Hook-Pre-Push-Release-Check.md` and `doc/#AI_Plans/PRD-VSCode-Task-Safe-To-Push.md`.

## PRDs and Plans
- Product requirements documents (`PRD-*.md`) and implementation plans live in `doc/#AI_Plans/`.

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
- OG-MoM stand for Original Master of Magic (1994)

## Vetting third-party dependencies (all agents)
Before recommending, installing, or adding ANY external binary, library, package, or tool — including
anything from winget/choco/vcpkg/`FetchContent`, and any DLL committed to the repo — follow
`doc/#Devel/Dependency-Vetting.md`. Run `python3 tools/vet_dependency.py --repo OWNER/NAME
[--upstream OWNER/NAME --author "Exact Name"] [--tier A|B|C]` for the checkable facts.

Two rules that are easy to break:
- **Never produce a trust score** (no 0-100, no letter grade, no low/medium/high). Report verified
  facts, flags, and the questions you could not answer. An unchecked item is a gap, never a pass.
- **Classify blast-radius tier first** (A ships to users / B builds the product / C dev-only scratch).
  Scrutiny must be proportional. Do not write a rigorous analysis of a dev tool while binaries that
  ship to users go unexamined.

Note that these checks would all have passed xz-utils (CVE-2024-3094); only build transparency would
have caught it. Read the policy doc rather than trusting the checklist.
