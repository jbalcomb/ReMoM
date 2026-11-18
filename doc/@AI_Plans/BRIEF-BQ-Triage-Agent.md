# Brief: BUGS_AND_QUIRKS triage agent

For an AI agent that works through a list of items from binaryalgorithm's `doc/@TODO/BUGS_AND_QUIRKS.txt` in its own git worktree,
and returns one branch of commits. Read `AGENTS.md` first: every rule there applies, and the ones below are the ones most often broken.

## Inputs

- Your item numbers (given in your task).
- `doc/@CodeReview/Bugs-And-Quirks-Locations.csv` — per item address: IDA function, ReMoM definition (`path:line`), `matched_by`.
  Or run `python3 tools/bq_locate.py --item N`.
  - `name` / `name-normalized`: reliable.
  - `header-VERIFY`: found only through the `// WZD oNNNpMM` header; about 6% are wrong. Confirm that the ReMoM function is the IDA
    function before you use it.
  - The `db` column says which EXE the address is in. MAGIC.EXE items live in MAGIC-side files (`MAPGEN.c`, `INITGAME.c`, `NewGame.c`, ...);
    a MAGIC item mapped to a WIZARDS function is a mapping error. Find the real home and say so.
- The IDA 5.5 asm, which is ground truth: `/c/STU/devel/STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovrNNN/<IDA_function>.asm`
  (MAGIC.EXE: `out/MAGIC/...`). Enum values: `/c/STU/devel/STU-Extras/Piethawn/Piethawn/in/WIZARDS.inc`.
- `doc/@CodeReview/Bugs-And-Quirks-Triage.md` — the triage table and its column meanings. **Read only. Do not edit it.**

## Per item

1. Find the ReMoM function. Read the item text and the asm for the WHOLE function, not just the line the item names.
2. Decide the verdict against the asm:
   - **yes**: ReMoM matches the asm. Add the `OGBUG` comment at the line.
   - **no**: ReMoM differs from the asm (a reconstruction error). Make ReMoM match the asm, then add the `OGBUG`.
   - **not a bug**: the asm does not do what the item says. Add no comment.
   - **stub**: ReMoM has no reconstruction of that code. Change nothing.
3. Cover the whole function:
   - Check every existing note: drake178 `; BUG:`, `BUGBUG`, `OGBUG`, `DEDU`, `¿ ... ?`.
   - Compare every statement to the asm. Fix any reconstruction error you find, even one outside the item, and report it.
   - When your `OGBUG` is more specific than an existing drake178 `BUG:` note, **delete the drake178 note** and say so.
     Don't reference it from the `OGBUG`.
4. Build. Fix every MSVC warning you introduced.

## Code rules (hard)

- **The asm is ground truth.** Fidelity is structural, 1:1 with the asm, not merely the same behaviour.
  Equivalent-but-restructured C is a deviation. Fix it.
- **Never delete faithful code** (code with an asm counterpart). To change faithful code, comment out the original and put the
  replacement below it. Non-faithful code (no asm counterpart) is commented out, not deleted, and marked `REBUG` (below).
  `DNE` (Does Not Exist) is NOT a removal tag: it is a DEDU note that the original seems to lack something it should have.
- **Mark every fixed reconstruction error `REBUG`** (the counterpart of `OGBUG`): the replacement line gets
  `/* CLAUDE */  /* REBUG */  /* asm: <the instructions it now matches> */`, under the commented-out original. Removing reconstruction-added code the asm lacks is a `REBUG` too. Do not use `REBUG` for adding
  `/* HACK */` stand-ins, or replacing non-asm placeholders with the asm's code (those keep plain `/* CLAUDE */`, or none).
- **OG bugs stay.** Never fix an original-game bug. It gets only an `OGBUG` comment:
  `/* OGBUG  <what happens, concretely> */` on the line above.
- **Uninitialised reads and out-of-bounds reads AND writes** must not be undefined behaviour in ReMoM (e.g. `_UNITS[-1]`, `_players[owner_idx]`
  with owner -1):
  - Use `Stack_Garbage()` / `Stack_Garbage_Range()` from `STU/src/STU_GARB.h`, or bound the access.
  - Mark the line `/* HACK */`, and say in the `OGBUG` what the OG actually does.
  - Pick a stand-in that keeps the OG's control flow. If the OG's garbage value is out of range (a stale index that matches no player),
    the stand-in must be out of range too, and the accesses it reaches get bounded. Never pick an in-range value that sends ReMoM
    down a path the OG would not take.
  - Do not bound a write that stays inside memory ReMoM owns the same way the OG does (e.g. an overrun from one `Near_Allocate_Next`
    block into the next lands inside `near_buffer`, as in the OG). It is faithful, not UB. Note it in the `OGBUG`.
- **Non-asm skips and guards** (code with no asm counterpart that changes behaviour, e.g. a `continue` on dead units) are removed or
  commented out so ReMoM does what the OG does; only the UB they were hiding gets bounded.
- **Missing stack slots are not evidence of anything.** Never call a local "phantom" because the asm has no slot for it.
- **No invented quirks.** Report only consequences that you can read from the asm.
- **House style:**
  - Allman braces; no line wrapping of code.
  - Vertical multi-condition `if`s, with `== ST_TRUE` / `== ST_FALSE`.
  - Use named constants (`WORLD_WIDTH`, `spl_*`, enum names), never magic numbers.
  - lower_snake_case locals; `int16_t`; `if(`, with no space.
- Original-game code files keep their encoding and line endings: CRLF or LF, UTF-8 or latin-1, as found.

## Build and test

Setup, once per worktree: DeMoM tests need the game data next to the exe. Copy the LBX files and `CONFIG.MOM` from the main
tree's `out/build/MSVC-debug/bin/Debug/` into the worktree's `out/build/MSVC-debug/bin/Debug/` (create it first).

In your worktree:
```bash
cmake --preset MSVC-debug
cmake --build --preset MSVC-debug
ctest --preset MSVC-debug -R "DeMoM_"
```
- Never use an ad-hoc build directory.
- A test failure you did not cause still gets reported, with the output.
- Do not change test goldens.
- A new test's `TIMEOUT` follows the neighbouring tests (60 for fixture runs); never a sub-second measurement plus 10%.

## Commits

- Commit on your own worktree branch only. Make one commit per item, or per group of items that share a function.
- Message: `BQ #NN: <function> - <one line>`.
- Never touch `main` or `bq-base`, and never push.

## Report (your final message)

1. **Build and test result:** the final build's warning/error lines for the files you touched, and the ctest summary line.
2. **One triage row per item, for EVERY item you were given**, ready to paste into the table (count them before you send):
   `[x] | [#NN](../@TODO/BUGS_AND_QUIRKS.txt#L<line>) | <their function> | <their description> | <their status> | [<Function>](../../MoM/src/<File>.c#L<line>) | **<yes / no / not a bug / stub>** | in | <notes: asm file and lines, what was confirmed, what was fixed>`
   - Use `[ ]` and `pending` for **stub** items.
   - Links: a function-name link points at that function's **definition line**; a link in the `OGBUG` note column points at the **OGBUG comment line**.
   - Grade **partial** when you could not compare the whole function (huge functions): say which asm range you covered.
   - Don't tick anything you didn't do.
3. **Follow-ups outside your items** (deviations you found but did not fix): give the asm file, lines and instruction text for each,
   so the claim can be checked. A follow-up without asm evidence is not a finding.
4. **Anything you were unsure of:** name the item, the asm lines, and the open question. Do not guess. An honest "unresolved" beats
   a confident wrong verdict. Your report will be checked against the asm.
