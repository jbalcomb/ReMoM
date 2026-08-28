# PRD — End-Game Return-to-Menu via `setjmp`/`longjmp`

**Status:** Not taken — superseded by option B ([PRD-Endgame-Return-To-Menu-Screen-State.md](PRD-Endgame-Return-To-Menu-Screen-State.md)), which was implemented 2026-07-20 (state-based `Get_Winner()`/`magic_master_idx`, no non-local jumps). Kept for rationale/comparison.
**Owner:** TBD
**Date:** 2026-07-19
**Tracks:** End-game control flow, platform substitution for DOS `execl` (option A of two — see [PRD-Endgame-Return-To-Menu-Screen-State.md](PRD-Endgame-Return-To-Menu-Screen-State.md))

---

## Summary

Replace the four DOS `Respawn(MAGIC.EXE, …)` end-game calls with a non-local jump that collapses the call stack back to `Screen_Control()`'s dispatch loop and re-enters the Main Menu. A single `jmp_buf` armed once at the top of `Screen_Control()` is the target; each end-game site runs a small teardown, sets `current_screen = scr_Main_Menu_Screen`, and `longjmp`s to it. This is the closest behavioral analog to what `execl` did in DOS (throw away the entire stack in one operation), with the smallest possible code footprint and no audit of the intervening call chain.

## Problem

In DOS, an end-of-game outcome ends by calling [`Respawn()`](../../MoX/src/EXIT.c#L137), which finishes with `execl("MAGIC.EXE", …)` — replacing the running process image. `execl` never returns, so the entire call stack (mid-turn AI, combat resolution, city-ownership transfer) is discarded and the game restarts from the menu on a fresh stack.

ReMoM cannot relaunch an EXE. The four sites that call `Respawn` are all reached ~6+ frames deep:

| Site | File / line | Outcome | Descent |
|---|---|---|---|
| Human conquered | [CONQUEST.c:334](../../MoM/src/CONQUEST.c#L334) | loss | `Screen_Control` → `scr_NextTurn` → `Next_Turn_Proc` → combat → `Change_City_Ownership` → `Resolve_Wizard_Conquest` |
| Last AI eliminated | [CONQUEST.c:343](../../MoM/src/CONQUEST.c#L343) | win | (same as above) |
| Spell of Mastery loss | [SPLMASTR.c:1952](../../MoM/src/SPLMASTR.c#L1952) *(already commented)* | loss | `Screen_Control` → … → `Spell_Of_Mastery` → `Spell_Of_Mastery_Lose` |
| Spell of Mastery win | [SPLMASTR.c:2346](../../MoM/src/SPLMASTR.c#L2346) *(already commented)* | win | `Screen_Control` → … → `Spell_Of_Mastery` |

Naively re-calling `Main_Menu_Screen()` (or re-entering `Screen_Control()`) from any of these sites **nests** a fresh game beneath the unfinished turn. Repeated across enough games, the stack overflows. Setting `current_screen` alone does not help either: as each intermediate frame returns it keeps processing a game that is already over (transferring cities, resolving combat, advancing the turn).

The requirement is: from any of the four sites, **unwind cleanly to the `Screen_Control` loop** and re-enter the menu, without executing any more of the dead game's turn logic.

## Goals

- From all four end-game sites, return control to the top of the `Screen_Control()` dispatch loop with `current_screen = scr_Main_Menu_Screen`, on a fully unwound stack.
- Cover win **and** loss of both normal conquest and Spell of Mastery.
- Preserve fidelity: the original `Respawn(…)` call stays in-source as a comment at each live site; the new behavior is marked `/* CLAUDE HACK */`.
- Keep the change surface minimal — no modification of the intervening combat / turn-resolution frames.
- Perform the still-relevant portions of `Respawn`'s DOS cleanup (audio, mouse, stray file handles) before jumping, so no resource leaks or lingering audio across games.

## Non-Goals

- **Restructuring the descent path.** That is option B ([PRD-Endgame-Return-To-Menu-Screen-State.md](PRD-Endgame-Return-To-Menu-Screen-State.md)). This PRD deliberately avoids touching `Next_Turn_Proc`, combat resolvers, `Change_City_Ownership`, or the SoM resolvers.
- **Process relaunch.** We do not emulate `execl`; the menu re-entry is in-process.
- **Save-on-exit / autosave semantics.** Out of scope; unchanged from today.
- **Moving the end-game presentation.** `Win_Animation` / `Lose_Animation` / `End_Of_Game_Score` continue to run at the deep site exactly where the disassembly has them, before the jump.

## Functional Requirements

1. **Jump buffer.** A file-scope `jmp_buf g_end_game_return;` in `MoM/src/MOM_SCR.c`, exposed via `MOM_SCR.h` (`extern`) so the end-game sites in `CONQUEST.c` and `SPLMASTR.c` can reach it.
2. **Arm point.** `Screen_Control()` ([MOM_SCR.c:68](../../MoM/src/MOM_SCR.c#L68)) calls `setjmp(g_end_game_return)` once, immediately before the `while (quit_flag == ST_FALSE)` loop. `Screen_Control`'s frame is a strict ancestor of every trigger site and lives for the whole program, so one arming is valid for every jump. `quit_flag` is (re)initialized to `ST_FALSE` **after** the `setjmp` so a returning `longjmp` cannot land on a stale value.
3. **Teardown helper.** `End_Game_Teardown()` (new, in `MOM_SCR.c` or `EXIT.c`) performs the parts of `Respawn` that still have a live SDL2 equivalent — stop music, `Audio_Uninit`, reset the system mouse, `fcloseall()` — and **omits** the DOS-only steps (`int86` handle-close loop, `EMM_Release_Resd`, `execl`).
4. **Site edits.** At each of the four sites, the OG `Respawn(…)` line is retained as a comment and replaced by:
   ```c
   /* CLAUDE HACK */ End_Game_Teardown();
   /* CLAUDE HACK */ current_screen = scr_Main_Menu_Screen;
   /* CLAUDE HACK */ longjmp(g_end_game_return, 1);
   ```
   (The two SPLMASTR sites are already commented out; they gain the live HACK block. The two CONQUEST sites have their live `Respawn(…)` commented first.)
5. **Menu re-entry state.** On return, the `scr_Main_Menu_Screen` case runs as normal. `GAME_SoM_Cast_By` is already reset to `ST_UNDEFINED` when a new game is subsequently entered (Continue/Load cases at [MOM_SCR.c:176](../../MoM/src/MOM_SCR.c#L176), :207); no additional reset required for correctness of the *menu*, but see Implementation Notes on per-game globals.

## Acceptance Criteria

- [ ] Winning by eliminating the last AI returns to the Main Menu; a subsequent New Game plays normally.
- [ ] Losing (human's last city/fortress conquered) returns to the Main Menu.
- [ ] Winning and losing via Spell of Mastery both return to the Main Menu.
- [ ] Playing and ending N consecutive games (win or lose) shows no unbounded stack growth (verify stack pointer / high-water mark is flat across games, e.g. under HeMoM or a debugger watch).
- [ ] No audio continues playing into the menu after an end-game; the mouse cursor is responsive; no file-handle leak (`fcloseall` ran).
- [ ] The original `Respawn(…)` call is present as a comment at all four sites; new lines are marked `/* CLAUDE HACK */`.
- [ ] Debug and Release both build clean (`cmake --build --preset MSVC-debug`, then the Release leg of the safe-to-push gate).

## Implementation Notes

- **Why `longjmp` is the faithful analog.** `execl` collapsed the stack in a single operation; `longjmp` does the same minus the image reload. Because this is C (no destructors / RAII on the skipped frames), nothing on the abandoned path *needs* to run — which is exactly the DOS situation, where `Respawn` did its cleanup explicitly *before* `execl` precisely because no unwinding would occur. `End_Game_Teardown()` reproduces that explicit pre-cleanup.
- **`setjmp` placement / local-variable caveat.** Per C89, non-`volatile` automatic locals in `Screen_Control` modified between `setjmp` and `longjmp` have indeterminate values after the return. `Screen_Control`'s locals are all re-initialized at the top of each loop iteration (and `quit_flag` right after `setjmp`), so this is a non-issue; document it so no future edit relies on a local surviving the jump.
- **Per-game memory.** Blocks allocated during the abandoned turn (block allocator, input fields) are not individually freed by the jump. This mirrors `execl`, which discarded them wholesale. The block-allocator arena is reset by the next New Game / Continue load, so there is no *persistent* leak across games; the transient high-water within a single game is unchanged. If a specific per-game global is found to carry stale state into the menu, reset it in `End_Game_Teardown()` rather than auditing the descent.
- **Header hygiene.** `MOM_SCR.h` gains `#include <setjmp.h>` and `extern jmp_buf g_end_game_return;`. `CONQUEST.c` and `SPLMASTR.c` already include the screen header path; confirm `setjmp.h` visibility at those sites.
- **HeMoM.** The headless harness enters `Screen_Control()` too ([HeMoM.c:1352](../../src/HeMoM.c#L1352)); the same arming works there. For headless combat-only runs that skip `Screen_Control`, the end-game sites are not reached, so no special handling is needed — but verify a headless full-game-to-victory run unwinds correctly.

## Risks / Open Questions

- **Skipped `finally`-style work on the abandoned path.** If any frame between a trigger and `Screen_Control` performs cleanup that must happen even on game-over (e.g. flushing a log, releasing a platform lock), `longjmp` skips it. Audit is light (the path is combat/turn resolution, which has no such obligation today), but should be spot-checked. This is the single substantive risk and the main thing option B avoids.
- **Interaction with an in-progress interactive combat screen.** The conquest sites can be reached from interactive combat (`Combat.c` call sites), not only AI turn resolution. Confirm that jumping out mid-combat-screen leaves the platform/video pages in a state the menu re-initializes cleanly (the `scr_Main_Menu_Screen` case does `Load_Palette`/`Apply_Palette`; `End_Game_Teardown` handles mouse/audio). Likely fine because the win/lose animation + score screen already fully repaint before the jump.
- **`End_Game_Teardown` scope.** Exact membership (which `Respawn` steps have live equivalents vs. are DOS-only) needs a short pass over `EXIT.c:137`'s body and the current `__STUB` status of `Stop_Music__STUB` / `Audio_Uninit__STUB`.
- **Testability of the stack-flatness criterion.** Needs a chosen measurement (debugger watch on `SP`, or a cheap high-water probe) — coordinate with [BRA-Endgame-Testing.md](BRA-Endgame-Testing.md).

## Comparison to option B (screen-state / flag)

| | This PRD (longjmp) | [Screen-state / flag](PRD-Endgame-Return-To-Menu-Screen-State.md) |
|---|---|---|
| Code surface | 1 buffer + 1 `setjmp` + 1 teardown helper + 4 site edits | Every frame on the descent (audit `Change_City_Ownership`'s 6 callers, combat resolvers, `Next_Turn_Proc`, SoM resolvers) |
| Fidelity to `execl` | High (stack-collapse in one op) | Medium (structured unwind, not what DOS did) |
| Non-local control flow | Yes (`longjmp`) | No |
| Main risk | Skipped cleanup on abandoned frames | Missing a guard → a finished game keeps processing |
| Effort | Low | Medium–High |

**Recommendation:** ship this (option A) as the near-term fix — minimal, faithful to `execl`, no descent audit. Adopt option B only if the team wants to eliminate non-local jumps as a matter of policy and is willing to pay the descent-audit cost.
