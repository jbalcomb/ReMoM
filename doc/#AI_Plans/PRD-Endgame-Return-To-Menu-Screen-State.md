# PRD — End-Game Return-to-Menu via Screen-State / Flag (MoO2-style)

**Status:** Implemented (B1) — all SET/REACT/GUARD/prompt-stop sites wired + `Get_Winner()`; `momlib` builds clean (MSVC-debug, 2026-07-20). Pending end-to-end play-test. See "Wiring status — COMPLETE".
**Owner:** TBD
**Date:** 2026-07-19
**Tracks:** End-game control flow, platform substitution for DOS `execl` (option B of two — see [PRD-Endgame-Return-To-Menu-Longjmp.md](PRD-Endgame-Return-To-Menu-Longjmp.md))

---

## Summary

Restructure the end-game the way MoO2 did: a state-based `Get_Winner()` records the winner in `magic_master_idx` at each site where the win condition can become true, and whichever `Screen_Control` case is on the stack — the `Main_Screen` interactive loop or `Next_Turn_Proc` — reacts by routing to the Main Menu. No non-local jumps.

The trace (2026-07-19) sharpened the design in three ways:
1. There are exactly **two boundary cases**, split by `Screen_Control` case, not by a runtime "who initiated this" test — the call stack already encodes it.
2. Both conquest paths (human and AI) **converge on one function**, `Combat__WIP` (Combat.c:3920), so a single guard there is the whole correctness story (it prevents a second end-game firing on an already-over game).
3. **Spell of Mastery never resolves in the interactive loop** — human *and* AI SoM both complete under `scr_NextTurn` / `Next_Turn_Proc`. So the `Main_Screen` boundary is needed only for interactive conquest.

## Problem

Same root cause as option A: the DOS end-game ends in [`Respawn()`](../../MoX/src/EXIT.c#L137) → `execl("MAGIC.EXE", …)`, discarding the whole stack. ReMoM can't relaunch an EXE, and the four `Respawn` sites sit deep under `Screen_Control()`. Re-calling the menu nests a new game beneath the unfinished turn → eventual stack overflow; merely setting `current_screen` keeps a finished game processing as each frame returns.

**Prior art.** MoO2 resolves the winner inside its Next-Turn code, so a game-over there falls back into the screen loop; its net-code `Client_Next_Turn_` just sets `_current_screen`. MoO2 does **not** appear to handle "the current player destroys the last opponent during their own turn" in `Change_Colony_Ownership_` — the interactive/human-initiated case. MoM needs that case, which is exactly why the design has a second (human-side) boundary in `Main_Screen`.

**Where MoM triggers.** Detection already lives at the city-capture point: [`Change_City_Ownership`](../../MoM/src/City_ovr55.c#L730) → [`Resolve_Wizard_Conquest`](../../MoM/src/CONQUEST.c#L191) (fires on human-loss or `CP_Is_Dead()` win), plus the Spell-of-Mastery resolvers. Rule of thumb: **when a city is captured or razed, check whether that was the last city of the last opponent (win) or the human's last city (loss).**

## Unwind boundaries (by `Screen_Control` case)

| Case on stack | What reaches the trigger under it | Boundary / return point |
|---|---|---|
| `scr_Main_Screen` | **Interactive human conquest only** — human moves a stack onto the last enemy fortress | `Main_Screen`'s `while (leave_screen_flag == ST_FALSE)` — [MainScr.c:921](../../MoM/src/MainScr.c#L921) |
| `scr_NextTurn` | **Everything else** — all AI conquest, all neutral rampage capture, and **all** Spell-of-Mastery completion (human *and* AI) | `Next_Turn_Proc` — [NEXTTURN.c:300](../../MoM/src/NEXTTURN.c#L300) → back to the `scr_NextTurn` case |

Because the stack context selects the boundary, each SET site just records `magic_master_idx = Get_Winner()`; each boundary checks it independently and only the one actually executing can react. No initiator detection required.

## Set / react / guard checklist (`magic_master_idx = Get_Winner()`)

Line-verified 2026-07-19. **Model:** [`Get_Winner()`](../../MoM/src/NEXTTURN.c#L4756) (DNE in MoM — ported from MoO2 `Get_Winner_`) evaluates the current board: it checks `GAME_SoM_Cast_By` (the Spell-of-Mastery winner, MoO2's `_council_winner`) first, then the wizard-elimination conditions, and returns the winning player index or `ST_UNDEFINED`. [`magic_master_idx`](../../MoM/src/NEXTTURN.c#L86) (default `ST_UNDEFINED`) holds that result. Three kinds of site:

- **SET** — `magic_master_idx = Get_Winner();` wherever the win condition can newly become true (distributed — one per condition, for debuggability).
- **REACT** — at the two boundaries, `if (magic_master_idx != ST_UNDEFINED)` → route to the menu / end-game.
- **GUARD [correctness]** — the deep presenter early-returns when a winner already exists, so a second capture the same turn can't play a second end-game.

### SET sites (one per condition)

| Condition | Site | Covers |
|---|---|---|
| Overland combat resolves | `Move_Units`, right after `combat_result = Combat__WIP(...)` — [MainScr.c:5413](../../MoM/src/MainScr.c#L5413) | **all** combat captures — human interactive, AI, neutral, and rampage (rampage runs *inside* `Combat__WIP`). Call unconditionally — either the attacker-won (Combat.c:4184) or defender (4248) branch can eliminate. |
| Rebellion event | `Event_Twiddle`, right after `Change_City_Ownership` — [EVENTS.c:996](../../MoM/src/EVENTS.c#L996) | a wizard losing their fortress/last city to rebellion |
| Human SoM completes | after `Cast_Spell_Overland(_human_player_idx)` — [NEXTTURN.c:461](../../MoM/src/NEXTTURN.c#L461) | human Spell-of-Mastery win (`GAME_SoM_Cast_By` set inside `Spell_Of_Mastery`) |
| AI SoM completes | after `Cast_Spell_Overland(player_idx)` — [AIDUDES.c:267](../../MoM/src/AIDUDES.c#L267) | AI Spell-of-Mastery win/loss |

The `Move_Units` combat set is shared by the human and AI/neutral combat paths (all funnel through `Move_Units → Combat__WIP`), so it reads as "after any overland battle, re-check the winner" — one line feeding both boundaries. Rampage and neutral capture ride this same set (they resolve inside `Combat__WIP`); only the two non-combat conditions (rebellion, SoM) need their own SET.

### REACT sites (the two boundaries)

- **HP — `Main_Screen`.** After the input-handling block closes ([MainScr.c:1917](../../MoM/src/MainScr.c#L1917)), before the redraw guard: `if (magic_master_idx != ST_UNDEFINED) { current_screen = scr_Main_Menu_Screen; leave_screen_flag = ST_TRUE; }`. Must live here — `leave_screen_flag` is a `Main_Screen` local. Catches the human's interactive conquest (the only trigger under `scr_Main_Screen`).
- **CP/NP — end of `Next_Turn_Proc`.** [NEXTTURN.c:465](../../MoM/src/NEXTTURN.c#L465): `if(magic_master_idx != ST_UNDEFINED) { current_screen = scr_Main_Menu_Screen; } else { current_screen = scr_Main_Screen; }` — this *replaces* the original `current_screen = scr_Main_Screen` (preserved commented at NEXTTURN.c:436) and must sit **after** the human-SoM SET at :461, so it sees every SET. Catches everything else — AI/neutral conquest, rebellion, human & AI SoM — since all complete within this `Next_Turn_Proc` call. (No dispatcher-side check at MOM_SCR.c:394 is needed; 465 sets `current_screen` before the function returns.)

### GUARD — no double presentation [correctness]

- `Resolve_Wizard_Conquest` entry — [CONQUEST.c:206](../../MoM/src/CONQUEST.c#L206) — `if (magic_master_idx != ST_UNDEFINED) return;`. All capture routes funnel through it (combat 4184/4248/4263/4270, rampage 22724, rebellion 994) because `Change_City_Ownership` ([City_ovr55.c:769-782](../../MoM/src/City_ovr55.c#L769)) only calls it on genuine elimination (**old owner non-neutral AND (fortress OR last city)**). One guard here stops a second Win/Lose animation once a winner exists. **This guard is REQUIRED** — the AI-conquest-then-rebellion case double-presents without it (rebellion's `Event_Twiddle` runs after `AI_Next_Turn` in `Next_Turn_Calc`, with no guard between).
- `Spell_Of_Mastery` entry — [SPLMASTR.c:2215](../../MoM/src/SPLMASTR.c#L2215), before `GAME_SoM_Cast_By = player_idx` — covers both SoM outcomes (`Spell_Of_Mastery_Lose` is only reached from here).

### Optional prompt-stop (UX latency, not correctness) — wired 2026-07-20

Because REACT is at the turn boundary, the model **lets the current turn finish** and evaluates once — no mid-stack unwinding is required for correctness (the GUARD prevents double-present). Trade-off: after an AI kills the human mid-turn, the Lose screen plays deep (as OG did right before `Respawn`), then the remaining AI/NP turns would still compute before the menu. These short-circuits kill that pause. All keyed `if (magic_master_idx != ST_UNDEFINED)`:

| # | Site | Action | Purpose |
|---|---|---|---|
| 1 | `AI_Execute_Orders` unit-loop top — [SETTLE.c:164](../../MoM/src/SETTLE.c#L164) | `break` | stop the current player's remaining units — CP **and** NP (shared function). The key one. |
| 2 | `AI_Next_Turn` movement loop top — [AIDUDES.c:339](../../MoM/src/AIDUDES.c#L339) | `break` | stop remaining AI players' movement/combat |
| 3 | `AI_Next_Turn` planning loop top — [AIDUDES.c:218](../../MoM/src/AIDUDES.c#L218) | `break` | stop remaining AI players' planning (AI SoM @267 can end it here) |
| 4 | after `AI_Execute_Orders(NEUTRAL)` — [AIDUDES.c:381](../../MoM/src/AIDUDES.c#L381) | `return` | if the neutral turn ended it, skip `Make_Raiders`/`Make_Monsters`/cleanup |

They all unwind *to* the REACT at NEXTTURN.c:465 (they don't bypass it); the cheap `Next_Turn_Proc` tail still runs and its own presenters are GUARDed.

**Not wired (optional):** a `return` before the Neutral phase (~AIDUDES.c:363, before `Player_All_Colony_Autobuild(NEUTRAL)`) to also skip the cheap NPC colony/farmer work when the game ended during the AI turns — pure optimization; the downstream `AI_Execute_Orders(NEUTRAL)` already breaks via #1 and #4 catches the tail.

### Wiring status — COMPLETE (builds clean, MSVC-debug, 2026-07-20)

All sites wired (`/* EOG_HACK */` markers) and `momlib` compiles/links clean:
- **SET ×4** — MainScr.c:5413 (combat), EVENTS.c:996 (rebellion), NEXTTURN.c:461 (human SoM), AIDUDES.c:267 (AI SoM).
- **REACT ×2** — MainScr.c:1917 (HP), NEXTTURN.c:465 (CP/NP, replaces the original `current_screen = scr_Main_Screen`, preserved commented at :436).
- **GUARD ×2** — `Resolve_Wizard_Conquest` (CONQUEST.c:206), `Spell_Of_Mastery` (SPLMASTR.c:2215).
- **Prompt-stop ×4** — SETTLE.c:164, AIDUDES.c:218/339/381.
- **`Get_Winner()`** — NEXTTURN.c:4762, using `N_Living_Players()`/`N_Living_Human_Players()`; SoM short-circuit via `GAME_SoM_Cast_By != ST_UNDEFINED`, else `(N_Living_Human_Players() == 0) || (N_Living_Players() == 1)` ⇒ human eliminated (loss) or sole survivor (win). Idempotent (state-derived).
- **State** — `magic_master_idx` defined at NEXTTURN.c:87, declared `extern` in NEXTTURN.h:24; the four OG `Respawn(…)` calls commented `// DONT`.

**Open (behavioral verification, not wiring):**
- Play-test the five endgames (AI-turn win, interactive win, conquest loss, SoM win, SoM loss) end-to-end to the menu, and N consecutive games for flat stack usage. Consider a HeMoM scenario.
- Sanity-check the `spl_Spell_Of_Return` liveness nuance (helpers count `_FORTRESSES.active` only; a returning wizard counts as dead — moot for the human's terminal fortress-loss, but confirm).

## Two sub-variants (where the presentation lives)

- **B1 — presentation stays put (recommended).** `Win_Animation`/`Lose_Animation`/`End_Of_Game_Score` keep running at the deep site (faithful to the disassembly). The SET sites record `magic_master_idx`; the boundary routes to the menu. Smallest deviation.
- **B2 — full MoO2 restructure.** Move the presentation into a new `scr_End_Game` case in `Screen_Control`; the trigger records only result + game-over. Cleanest separation, but relocates faithful-to-asm calls (a deliberate fidelity deviation) and is larger.

## Goals

- From all four sites, unwind to `Screen_Control()` and re-enter the Main Menu via ordinary returns — no `longjmp`.
- Cover win and loss of both normal conquest and Spell of Mastery, including the human-initiated interactive-capture case MoO2's colony code omits.
- Carry the outcome in `magic_master_idx` (set via `Get_Winner()`, with `GAME_SoM_Cast_By` supplying the SoM winner), reacted-to at the two boundaries.
- Stop dead-game processing promptly (no further city transfers / combat / turn advancement, and crucially no second end-game trigger).
- Preserve fidelity: OG `Respawn(…)` stays commented; new lines `/* CLAUDE HACK */`; (B2) relocated presentation annotated as a deviation.

## Non-Goals

- **Emulating `execl` / process relaunch.**
- **Non-local jumps** (that's [option A](PRD-Endgame-Return-To-Menu-Longjmp.md)).
- **Reworking save/autosave-on-exit.**
- **(B1) Moving the end-game presentation** — only B2 does that.

## Functional Requirements

1. **Winner state.** Global `int16_t magic_master_idx` ([NEXTTURN.c:86](../../MoM/src/NEXTTURN.c#L86)) — default `ST_UNDEFINED` (game ongoing), else the winning player index. Reset to `ST_UNDEFINED` at New Game and Continue/Load entry (alongside `GAME_SoM_Cast_By = ST_UNDEFINED` at [MOM_SCR.c:176](../../MoM/src/MOM_SCR.c#L176)/:207).
2. **`Get_Winner()` body** ([NEXTTURN.c:4756](../../MoM/src/NEXTTURN.c#L4756)). Per the MoM win/loss rules: return `GAME_SoM_Cast_By` if a Spell-of-Mastery winner is set (MoO2's `_council_winner` short-circuit); else the human if all opponents are eliminated (a win); else a loss indicator if the human is eliminated; else `ST_UNDEFINED`. Pure function of game state — no side effects.
3. **SET sites** (`magic_master_idx = Get_Winner();`). The four condition sites from the checklist: `Move_Units` after `Combat__WIP` @[MainScr.c:5403](../../MoM/src/MainScr.c#L5403) (all combat — human/AI/neutral/rampage), `Event_Twiddle` after `Change_City_Ownership` @[EVENTS.c:994](../../MoM/src/EVENTS.c#L994) (rebellion), and after `Cast_Spell_Overland` at [NEXTTURN.c:449](../../MoM/src/NEXTTURN.c#L449) (human SoM) and [AIDUDES.c:262](../../MoM/src/AIDUDES.c#L262) (AI SoM).
4. **REACT boundaries.** HP — `Main_Screen`, between [MainScr.c:1911](../../MoM/src/MainScr.c#L1911) and :1915: `if (magic_master_idx != ST_UNDEFINED) { current_screen = scr_Main_Menu_Screen; leave_screen_flag = ST_TRUE; }`. CP/NP — `scr_NextTurn` case after `Next_Turn_Proc` returns ([MOM_SCR.c:382](../../MoM/src/MOM_SCR.c#L382)): `if (magic_master_idx != ST_UNDEFINED) current_screen = scr_Main_Menu_Screen;`.
5. **GUARD (no double presentation).** `Resolve_Wizard_Conquest` entry ([CONQUEST.c:191](../../MoM/src/CONQUEST.c#L191)) and the two SoM presenters: `if (magic_master_idx != ST_UNDEFINED) return;`. Covers every capture route (combat, rampage, rebellion) since all funnel through `Resolve_Wizard_Conquest`.
6. **Trigger sites (deep).** Comment out OG `Respawn(…)` at the four sites; presentation stays (B1) and the site just `return;`s — routing is the boundaries' job. Do **not** set `current_screen` deep.
7. **State lifecycle.** `magic_master_idx` cleared to `ST_UNDEFINED` exactly where a new game begins, so it can't suppress the next game's first turn.
8. **Optional prompt-stop.** If post-presentation compute is noticeable, add the earlier trace guards (checklist "Optional prompt-stop"), all keyed `magic_master_idx != ST_UNDEFINED`.

## Acceptance Criteria

- [ ] Win-by-elimination during an AI turn, win-by-elimination during the human's own turn (interactive), loss-by-conquest, SoM-win, and SoM-loss each return to the Main Menu; a subsequent New Game plays normally.
- [ ] **No second end-game animation** plays when a further capture happens the same turn (the `Resolve_Wizard_Conquest` entry GUARD short-circuits once `magic_master_idx` is set).
- [ ] The interactive last-AI-kill case (Main_Screen boundary) works — the case MoO2's colony code omits.
- [ ] N consecutive games show flat stack usage.
- [ ] `magic_master_idx` is `ST_UNDEFINED` at the start of every new game's first turn.
- [ ] Every SET, REACT, and GUARD site in the checklist is present.
- [ ] OG `Respawn(…)` preserved as comments; new lines `/* CLAUDE HACK */`; (B2) relocation annotated.
- [ ] Debug and Release both build clean.

## Implementation Notes

- **The `Resolve_Wizard_Conquest` GUARD is the linchpin.** Every capture route funnels through it, so one entry guard makes the correctness property (no double-presentation) hold regardless of how promptly outer frames unwind. Everything in "Optional prompt-stop" is latency, not correctness.
- **Let-the-turn-finish.** Because REACT is at the turn boundary, the model does not unwind mid-turn by default. That's the big simplification over a flag-and-guard scheme: the AI turn completes on a valid board (a wizard just has no cities), then the boundary evaluates once.
- **Presentation timing (B1).** For an AI kill, the Win/Lose animation plays mid-AI-turn — which is where OG played it (right before `Respawn`). B2 moves it to loop level at a fidelity cost.
- **Fidelity ledger.** B1 adds `Get_Winner()` (new, MoO2-ported) + `magic_master_idx` SET/REACT/GUARD lines (new `/* CLAUDE HACK */` control flow justified by the platform substitution) and keeps every faithful call in place. B2 relocates `Win/Lose_Animation`/`End_Of_Game_Score`; log that in the walkthrough docs.
- **State vs. flags.** `magic_master_idx` is *winner state* (return to menu, holds who won) — distinct from `quit_flag` (exit process, `Screen_Control`) and `leave_screen_flag` (leave this screen, `Main_Screen` local). On the HP side the REACT sets `leave_screen_flag`; keep `magic_master_idx` separate from `quit_flag`.

## Risks / Open Questions

- **`Get_Winner()` return contract (primary open item).** It must encode three outcomes unambiguously: human won (all opponents eliminated), human lost (human's fortress/last city gone — even with multiple AIs left), and ongoing (`ST_UNDEFINED`). The REACT/GUARD sites are trivial; the semantics live entirely in this function's body. Nail the human-lost-but-AIs-remain case.
- **SET-site completeness.** Only four SET sites, but all four must be present or a path silently never returns to menu. The combat SET (`Move_Units@5403`) is shared and covers human/AI/neutral/rampage; the two non-combat conditions (rebellion, SoM) each need their own SET.
- **Neutral / rampage / rebellion capture — RESOLVED (2026-07-19).** All route through `Change_City_Ownership → Resolve_Wizard_Conquest`, so the `Resolve_Wizard_Conquest` entry GUARD covers double-presentation regardless of path; the combat SET at `Move_Units@5403` records the winner for neutral capture and rampage (both resolve inside `Combat__WIP`), and rebellion has its own SET at EVENTS.c:994. Rampage additionally refuses fortress cities (capital guard @Combat.c:22718). No capture path is left without a SET or GUARD.
- **Let-the-turn-finish side effects.** With no mid-turn unwind, an AI turn continues after `magic_master_idx` is set; verify nothing in the remainder of `Next_Turn_Proc` (or `Move_Units`' own post-`Combat__WIP` work at MainScr.c:5403+) assumes the just-eliminated player still exists. If it does, promote the relevant "Optional prompt-stop" guard to mandatory.
- **B2 fidelity cost.** Relocating presentation trades disassembly fidelity for architecture; default to B1.

## Comparison to option A (`longjmp`)

| | [longjmp](PRD-Endgame-Return-To-Menu-Longjmp.md) | This PRD (screen-state / flag) |
|---|---|---|
| Code surface | 1 buffer + 1 `setjmp` + teardown + 4 site edits | Flag + `Combat__WIP` guard + ~7 unwind guards + boundary consumption + (B2) new case |
| Fidelity to `execl` | High (one-shot stack collapse) | Medium (structured unwind ≠ what DOS did) |
| Non-local control flow | Yes | No |
| Primary risk | Skipped cleanup on abandoned frames | Missing an unwind guard → latency (not correctness, given the `Combat__WIP` guard) |
| Effort | Low | Medium (bounded — chains fully enumerated) |
| Best when | Near-term fix; faithful to `execl`; minimal surface | Team policy forbids `longjmp`; wants a structured flow reusing existing screen machinery |

**Recommendation:** option A remains the smallest, most `execl`-faithful near-term fix. This option is now well-scoped: the unwind terminates at two existing boundaries, correctness rests on one `Combat__WIP` guard, and the remaining guards are an enumerated latency concern. Prefer B1; reserve B2 for when the fuller MoO2 separation is explicitly wanted.
