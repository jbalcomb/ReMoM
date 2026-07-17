# BRA — Endgame (Win/Lose) Testing Architecture Decision

**Status:** Decided (scaffolding-first) — working record, open questions flagged inline
**Date:** 2026-07-17
**Author role:** Business Requirements Analyst (decision record)
**Related:** `BRA-Combat-Testing.md`, `PRD-Combat-Testing.md`, `PLAN-Combat-Testing.md`

This document works through *what is involved* in finishing and testing the game's win/lose paths —
banishment, defeat, Spell of Return, Spell of Mastery — for both the human player and the AI. It
records the goal, the current-state finding that reshaped the effort, the criteria weighed, the options
for reaching each transition, and the decision to build test scaffolding first. It is a decision record
and a working scratchpad for open questions, not an implementation plan (that will be a follow-on
`PLAN-Endgame-Testing.md`).

It inherits one principle wholesale from `BRA-Combat-Testing.md`: **test code on the substrate it was
designed for.** State-transition logic is screen-free → headless/assertable; cutscenes render →
windowed/observed.

---

## 1. The ultimate goal

"Win/lose testing" is not one deliverable. Like combat, the endgame has **two natures**:

- **Endgame logic** — the state transitions: a wizard's fortress going inactive on banishment, the
  `Defeated_Wizards` bitfield recording an elimination, Spell of Return relocating a fortress, Spell of
  Mastery flagging the caster and branching to win-or-lose. Deterministic given (game state, RNG seed);
  *assertable*.
- **Endgame presentation** — the cutscenes: the limbo-fall banishment animation, the victory animation,
  the Spell-of-Mastery sphere sequence, the "you have been defeated / you are triumphant" battle
  scrolls, the game-over/Hall-of-Fame transition. *Visual*; verified by watching, not by asserting
  field values.

The goal is therefore threefold, mirroring the combat decision:

1. **Detect regressions in endgame outcomes** automatically, deterministically, on every build.
2. **Provide a harness to set up and observe a specific endgame** — to develop, debug, and visually
   verify the banishment/victory/defeat sequences.
3. **Localize which transition changed** when an outcome regression fires.

---

## 2. Current-state finding (the fact that reshaped the effort)

**The state-tracking layer is implemented and testable today. The terminal transitions are not — every
`GAME_OVER()` call is commented out, so no code path currently ends the game.** Verified in-source
2026-07-17:

### Implemented (assertable now)

| Piece | Location | Notes |
|---|---|---|
| `Change_City_Ownership` → conquest bridge | [City_ovr55.c:730](../../MoM/src/City_ovr55.c#L730) → [:780](../../MoM/src/City_ovr55.c#L780) | calls `WIZ_Conquer__WIP` |
| `WIZ_Conquer__WIP` — counts loser cities, drives banish path | [CONQUEST.c:207](../../MoM/src/CONQUEST.c#L207) | WIP but functional for state |
| `_FORTRESSES[loser].active = ST_FALSE` on banishment | [CONQUEST.c:267](../../MoM/src/CONQUEST.c#L267) | the concrete "wizard out" mutation |
| `Defeated_Wizards` bit set when **human** conquers | [CONQUEST.c:497](../../MoM/src/CONQUEST.c#L497) | AI-conqueror path does NOT set it — see §7 |
| "banishes" vs "defeats" message by city count | [CONQUEST.c:551](../../MoM/src/CONQUEST.c#L551) / [:555](../../MoM/src/CONQUEST.c#L555) | `cnst_Conquest_Msg3` / `_Msg4` |
| `Cast_Spell_Of_Return` relocates fortress + capital | [Spells132.c:1697](../../MoM/src/Spells132.c#L1697) | dispatched from [OverSpel.c:1423](../../MoM/src/OverSpel.c#L1423) |
| `Spell_Of_Mastery` sets `GAME_SoM_Cast_By`, branches | [SPLMASTR.c:2202](../../MoM/src/SPLMASTR.c#L2202) / [:2209](../../MoM/src/SPLMASTR.c#L2209) | dispatched from [OverSpel.c:811](../../MoM/src/OverSpel.c#L811) |
| `Spell_Of_Mastery_Lose` (AI cast SoM → human loses) | [SPLMASTR.c:1887](../../MoM/src/SPLMASTR.c#L1887) | entered at [SPLMASTR.c:2217](../../MoM/src/SPLMASTR.c#L2217) |
| `Eliminated_Opponent` (per-battle wipe check) | [Combat.c:17074](../../MoM/src/Combat.c#L17074) | feeds `_combat_winner` |
| Battle scroll "defeated" / "triumphant" | [Combat.c:22452](../../MoM/src/Combat.c#L22452) / [:22446](../../MoM/src/Combat.c#L22446) | presentation |

### Stubbed / commented (blocked — cannot assert a terminal ending yet)

| Function | Location | State |
|---|---|---|
| `GAME_IsWon__STUB` — "no active AI left" | [CONQUEST.c:942](../../MoM/src/CONQUEST.c#L942) | body is `return ST_FALSE;` |
| `WIZ_Banishment__STUB` — banish/return decision, spell awards | [CONQUEST.c:1003](../../MoM/src/CONQUEST.c#L1003) | body is `return ST_FALSE;` |
| `GAME_PlayVictoryAnim__STUB` | [CONQUEST.c:957](../../MoM/src/CONQUEST.c#L957) | empty |
| `GAME_LimboFallAnim__STUB` | [CONQUEST.c:976](../../MoM/src/CONQUEST.c#L976) | empty |
| `SoM_Started__STUB` — "X has started casting SoM" warning | [SPLMASTR.c:1987](../../MoM/src/SPLMASTR.c#L1987) | partial (draw loop empty) |
| `GAME_OVER()` call sites | CONQUEST.c:430 / :445, SPLMASTR.c:1946 / :2340 | all `// GAME_OVER();` / `// SPELLY` |

**Consequence for testing:** we can pin the *intermediate* state the game moves through (fortress
active flag, `Defeated_Wizards`, `casting_spell_idx`, fortress relocation, `GAME_SoM_Cast_By`), but we
cannot yet assert "the game ended and player X won" because the code never reaches that. The tests we
build now characterize the implemented layer and stand as **red markers** the eventual stub-finishing
work will turn green.

---

## 3. Criteria considered

| Criterion | Why it matters |
|---|---|
| **Determinism** | A win/lose test that varies run-to-run under a fixed seed is not a regression test. Endgame paths draw RNG (banishment forfeit rolls, spell awards) — seed pinning is mandatory. |
| **Automatability** | Can it run pass/fail in CI / pre-push without a human? |
| **Substrate fidelity** | Logic headless, cutscenes windowed. Do not force a victory animation through `Platform_Headless` (the exact mistake `BRA-Combat-Testing.md` records). |
| **Reachability** | *New for endgame:* how do you get the game into a near-defeat / mid-banishment / about-to-win state cheaply and deterministically? This is the crux (§4). |
| **Reuse** | HeMoM fixtures, `savedump` + `check_save_fields`, `.hms`/`.RMR`, `refresh_asserts.py`. |
| **Observability** | Can a developer *watch* the banishment/victory sequence, not just read a field diff? |
| **Maintenance cost** | Seed-stream sensitivity; golden churn; brittleness to unrelated RNG-draw changes. |

### Criteria excluded

- **Pixel-exact cutscene comparison** — out of scope; observed, optionally replay-log compared.
- **Exhaustive per-spell coverage** — structural coverage of the 8 endgame cells first.

---

## 4. The reachability question (the crux)

The decisive new question versus combat testing: **how do we drive the game to each endgame transition,
headlessly and deterministically, without playing (or rendering) a full game?**

### Option A — Patch-and-observe
Binary-patch a save into the *post-condition* (fortress already inactive, `Defeated_Wizards` bit
already set), load it, and assert the consumers behave (Magic screen draws a grey gem, next-turn skips
the wizard, etc.).

- **Pros:** trivial, fully headless, deterministic, no combat needed.
- **Cons:** tests the *readers* of the state, not the *transition that produces it*. Won't catch a
  regression in `WIZ_Conquer__WIP` itself.

### Option B — Patch-to-brink + drive a real capture
Patch a wizard down to one city, then script an actual combat that captures it, letting
`Change_City_Ownership → WIZ_Conquer__WIP` fire naturally.

- **Pros:** exercises the real transition end-to-end.
- **Cons:** needs the tactical/strategic combat substrate (windowed per `BRA-Combat-Testing.md`);
  brittle; slow; couples endgame tests to combat determinism.

### Option C — HeMoM direct-invoke hook (recommended for the logic layer)
Add a headless entry mode to `HeMoM.c`, **exactly analogous to the existing `--combat` mode**
([HeMoM.c `--combat`](../../src/HeMoM.c)), that loads a patched save and calls one endgame entry point
directly — `WIZ_Conquer__WIP`, `Cast_Spell_Of_Return`, `Spell_Of_Mastery`, or `GAME_IsWon__STUB` — then
dumps the resulting save for `check_save_fields`.

- **Pros:** exercises the real transition function on the correct (screen-free) substrate;
  deterministic; reuses `g_load_save_gam_name_override`, `--dump-save`, `--seed1`; no combat coupling.
- **Cons:** invokes the function out of its normal call context — must patch a *self-consistent*
  precondition (e.g. the loser genuinely owns the city being lost) or the function reads stale state.
  Functions that branch into cutscene draws must be entered with rendering guarded/skipped.
- **Verdict:** the right harness for the logic layer. Cutscenes stay in the windowed layer (Option-B-ish
  observation), never asserted headlessly.

**Chosen:** **C for the state-transition layer, patch-and-observe (A) where a transition's normal call
context is too entangled to synthesize, windowed observation for cutscenes.** B is available but not a
default.

---

## 5. Decision — three layers

| Layer | Nature tested | Substrate | Harness | Determinism / mode | When run |
|---|---|---|---|---|---|
| **Endgame-state regression** | Transition *logic* (banish, defeat, return, mastery flags) | Headless | HeMoM endgame direct-invoke hook + patched save + dump + `check_save_fields` | Seeded, pass/fail | Every build / pre-push / CI |
| **Endgame-cutscene harness** | *Presentation* (limbo fall, victory anim, SoM sphere, scrolls) | Windowed (real `Platform`) | `demo_vga`-style target / `ReMoMber --scenario` | Observed; `.RMR` replay for repro | Development, debugging, visual verification |
| **Endgame predicates** | Individual decisions (`GAME_IsWon`, banish-vs-defeat) | None (unit test) | GTest, synthesized wizard/city state | Seeded, pass/fail | Every build — *after* the stubs are reconstructed |

Guiding principle (inherited): **test code on the substrate it was designed for.**

---

## 6. Scenario matrix (8 cells)

Subject × condition. "Blocked-by-stub" names what the cell *cannot* yet assert.

| # | Subject | Condition | Trigger (layer) | Assert now (implemented) | Blocked-by-stub |
|---|---|---|---|---|---|
| 1 | Human | Banished (non-last city lost) | `WIZ_Conquer__WIP` (C) | `fortress[H].active=0`; human still owns ≥1 city | `GAME_LimboFallAnim`, `GAME_OVER` |
| 2 | AI | Banished | `WIZ_Conquer__WIP` (C) | `fortress[AI].active=0`; human `Defeated_Wizards` bit set | `WIZ_Banishment` decision |
| 3 | Human | Defeated (last city lost) | `WIZ_Conquer__WIP` (C) | `fortress[H].active=0`; human owns 0 cities | `GAME_OVER` (human loss) |
| 4 | AI | Defeated (last AI) → **elimination win** | `WIZ_Conquer__WIP` + `GAME_IsWon` (C) | AI `active=0`; last-AI precondition | `GAME_IsWon__STUB`, `GAME_PlayVictoryAnim`, `GAME_OVER` |
| 5 | Human | Spell of Return | `Cast_Spell_Of_Return` (C) | fortress/capital relocated to target; **verify `active` reset** (§7 bug) | — |
| 6 | AI | Spell of Return | `Cast_Spell_Of_Return` (C, AI path) | AI fortress relocated | — |
| 7 | Human | Spell of Mastery (win) | `Spell_Of_Mastery` (C) | `GAME_SoM_Cast_By=H`; SoM enchantment/relations set | victory finalize, `GAME_OVER` |
| 8 | AI | Spell of Mastery (human loses) | `Spell_Of_Mastery` (C) | `GAME_SoM_Cast_By=AI`; `Spell_Of_Mastery_Lose` reached | `GAME_LimboFallAnim`, `GAME_OVER` |

State fields and their save offsets (player *i* base `P = 2536 + 1224*i`, all little-endian; from
[Game_Save_Dump.c](../../src/Game_Save_Dump.c)): `casting_spell_idx` `P+0x52` (Return=214, Mastery=213),
`Defeated_Wizards` `P+0x354`, gold `P+0x356`, mana `P+0x25C`; fortress *i* `26104+4*i` byte `+3`
(`active`); unit *k* `46900+32*k` byte `+3` (`owner_idx`); city *k* `35500+114*k` byte `+0x12`.

---

## 7. Open questions / gaps (need user input or verification)

1. **Spell of Return does not reappear to reset `_FORTRESSES[player_idx].active = ST_TRUE`.** A wizard
   who returns may remain flagged inactive. Is this an OGBUG (preserve, characterize) or a
   reconstruction gap (the reset was in the disassembly and got dropped)? Scenario 5 is designed to
   surface it. **Decision needed before finishing the stubs**, not before scaffolding.
2. **`Defeated_Wizards` bit is set only for the human conqueror** ([CONQUEST.c:497](../../MoM/src/CONQUEST.c#L497)).
   Is the AI-conqueror omission OG-faithful, or does the bit belong on every conqueror? Affects
   scenario 2's assertion and any future `GAME_IsWon` reconstruction.
3. **Sequencing of the stub-finishing follow-on.** Reconstructing `GAME_IsWon__STUB` is the smallest
   unblock that makes the *elimination win* (scenario 4) assertable end-to-end. Worth doing first in the
   follow-on `PLAN`, or leave all stubs for one batch?
4. **Base fixture per scenario.** Reuse `assets/SAVECMBT.GAM`, or capture a purpose-built multi-wizard
   near-endgame seed so the last-city / last-AI preconditions are natural rather than heavily patched?
5. **Cutscene observation scope.** Do we want `.RMR` captures of the victory / limbo-fall sequences now
   (as observe-only assets), or defer until the finalizers are reconstructed and there is something to
   watch past the current stub?

---

## 8. What did and did not fit the goal

**Fit:**
- Endgame-state transitions via a HeMoM direct-invoke hook: deterministic, headless, reuses the whole
  save-dump/assert pipeline. Clean fit for regression detection of the implemented layer.
- Windowed cutscene observation modeled on `demo_vga` / `ReMoMber --scenario`: clean fit for watching
  the sequences once their finalizers exist.

**Did not fit:**
- Asserting a terminal "game won/lost" headlessly *today* — blocked by the `GAME_OVER` stubs. Recorded
  as blocked, not forced.
- Driving every endgame via full scripted playthroughs — too slow/brittle; reachability is solved by
  patching + direct-invoke, not by playing the whole game.

---

## 9. Lesson carried from BRA-Combat-Testing

Match each nature to its designed substrate; don't collapse them into one mold. The endgame adds a
second trap beyond the combat one: because the terminal transitions are stubbed, it is tempting to
"just finish them" to have something to test. The scaffolding-first decision resists that — it builds
the deterministic rig against the implemented layer, so that when the stubs *are* reconstructed the
regression net already exists and the finishing work is validated as it lands, not after.

---

## Addendum — Knowledge graph

### Mermaid

```mermaid
graph TD
    subgraph Conquest["Conquest / elimination path"]
        CCO["Change_City_Ownership<br/>City_ovr55.c:730"] --> WCW["WIZ_Conquer__WIP<br/>CONQUEST.c:207"]
        WCW --> FA["write _FORTRESSES[loser].active = FALSE<br/>CONQUEST.c:267"]
        WCW --> WCQ["WIZ_Conquest__WIP<br/>CONQUEST.c:471"]
        WCQ --> DWB["set Defeated_Wizards bit (human only)<br/>CONQUEST.c:497"]
        WCW -.delegates.-> WBS["WIZ_Banishment__STUB<br/>CONQUEST.c:1003"]
        WCW --> GIW["GAME_IsWon__STUB<br/>CONQUEST.c:942"]
        GIW -.win.-> GPV["GAME_PlayVictoryAnim__STUB<br/>CONQUEST.c:957"]
        WCW -.human loss.-> GLF["GAME_LimboFallAnim__STUB<br/>CONQUEST.c:976"]
        GPV -.-> GOV["GAME_OVER()  (commented)"]
        GLF -.-> GOV
    end

    subgraph Combat["Per-battle result"]
        EO["Eliminated_Opponent<br/>Combat.c:17074"] --> CW["_combat_winner"]
        CW --> CCO
    end

    subgraph Spells["Overland spell paths"]
        OSP["OverSpel dispatch"] --> CSR["Cast_Spell_Of_Return<br/>Spells132.c:1697"]
        OSP --> SOM["Spell_Of_Mastery<br/>SPLMASTR.c:2202"]
        CSR --> RELO["relocate fortress + capital<br/>(active reset? — gap)"]
        SOM --> SCB["set GAME_SoM_Cast_By<br/>SPLMASTR.c:2209"]
        SOM -.AI cast.-> SOL["Spell_Of_Mastery_Lose<br/>SPLMASTR.c:1887"]
        SOM -.human win.-> GOV
        SOL -.-> GOV
        SBS["SBookScr.c:556"] -.warning.-> SST["SoM_Started__STUB<br/>SPLMASTR.c:1987"]
    end

    CSTATE["casting_spell_idx == spl_Spell_Of_Return<br/>(banished/returning proxy)"] -.gates.-> WCW
    CSTATE -.gates.-> SOM

    classDef stub stroke-dasharray:5 5,stroke:#c0392b,color:#c0392b;
    classDef commented fill:#fdecea,stroke:#c0392b,stroke-dasharray:5 5,color:#c0392b;
    class WBS,GIW,GPV,GLF,SST stub;
    class GOV commented;
```

### Node table

| id | kind | file:line | status |
|---|---|---|---|
| `Change_City_Ownership` | function | [City_ovr55.c:730](../../MoM/src/City_ovr55.c#L730) | impl |
| `WIZ_Conquer__WIP` | function | [CONQUEST.c:207](../../MoM/src/CONQUEST.c#L207) | WIP (functional for state) |
| `WIZ_Conquest__WIP` | function | [CONQUEST.c:471](../../MoM/src/CONQUEST.c#L471) | WIP |
| `_FORTRESSES[].active` | field (`s_FORTRESS` +0x03) | [MOM_DAT.h:2062](../../MoX/src/MOM_DAT.h#L2062) | impl (alive/in-game flag) |
| `Defeated_Wizards` | field (`s_WIZARD` +0x354) | [MOM_DAT.h:1522](../../MoX/src/MOM_DAT.h#L1522) | impl (elimination bitfield) |
| `casting_spell_idx` | field (`s_WIZARD` +0x52) | [MOM_DAT.h:1477](../../MoX/src/MOM_DAT.h#L1477) | impl (banished/returning proxy) |
| `WIZ_Banishment__STUB` | function | [CONQUEST.c:1003](../../MoM/src/CONQUEST.c#L1003) | stub (`return ST_FALSE`) |
| `GAME_IsWon__STUB` | function | [CONQUEST.c:942](../../MoM/src/CONQUEST.c#L942) | stub (`return ST_FALSE`) |
| `GAME_PlayVictoryAnim__STUB` | function | [CONQUEST.c:957](../../MoM/src/CONQUEST.c#L957) | stub (empty) |
| `GAME_LimboFallAnim__STUB` | function | [CONQUEST.c:976](../../MoM/src/CONQUEST.c#L976) | stub (empty) |
| `GAME_OVER()` | function | CONQUEST.c:430/445, SPLMASTR.c:1946/2340 | commented at all call sites |
| `Eliminated_Opponent` | function | [Combat.c:17074](../../MoM/src/Combat.c#L17074) | impl |
| `OverSpel` dispatch | function | [OverSpel.c:811](../../MoM/src/OverSpel.c#L811) / [:1423](../../MoM/src/OverSpel.c#L1423) | impl |
| `Cast_Spell_Of_Return` | function | [Spells132.c:1697](../../MoM/src/Spells132.c#L1697) | impl (active-reset gap) |
| `Spell_Of_Mastery` | function | [SPLMASTR.c:2202](../../MoM/src/SPLMASTR.c#L2202) | impl (cutscene); finalize commented |
| `Spell_Of_Mastery_Lose` | function | [SPLMASTR.c:1887](../../MoM/src/SPLMASTR.c#L1887) | impl; finalize commented |
| `SoM_Started__STUB` | function | [SPLMASTR.c:1987](../../MoM/src/SPLMASTR.c#L1987) | stub (partial) |
| `GAME_SoM_Cast_By` | global | [SPLMASTR.h:34](../../MoM/src/SPLMASTR.h#L34) | impl |
| `spl_Spell_Of_Mastery` = 213 | spell id | [Spellbook.h:239](../../MoM/src/Spellbook.h#L239) | impl |
| `spl_Spell_Of_Return` = 214 | spell id | [Spellbook.h:240](../../MoM/src/Spellbook.h#L240) | impl |

### Edge table

| from | to | relation |
|---|---|---|
| `Eliminated_Opponent` | `_combat_winner` → `Change_City_Ownership` | produces / triggers |
| `Change_City_Ownership` | `WIZ_Conquer__WIP` | calls |
| `WIZ_Conquer__WIP` | `_FORTRESSES[].active` | writes FALSE (banish) |
| `WIZ_Conquer__WIP` | `WIZ_Conquest__WIP` | calls (message + bit) |
| `WIZ_Conquest__WIP` | `Defeated_Wizards` | writes bit (human conqueror only) |
| `WIZ_Conquer__WIP` | `WIZ_Banishment__STUB` | delegates (blocked) |
| `WIZ_Conquer__WIP` | `GAME_IsWon__STUB` | calls (blocked) |
| `GAME_IsWon__STUB` | `GAME_PlayVictoryAnim__STUB` → `GAME_OVER()` | dispatches-to (blocked/commented) |
| `WIZ_Conquer__WIP` | `GAME_LimboFallAnim__STUB` → `GAME_OVER()` | dispatches-to (blocked/commented) |
| `OverSpel` dispatch | `Cast_Spell_Of_Return` | dispatches-to |
| `OverSpel` dispatch | `Spell_Of_Mastery` | dispatches-to |
| `Cast_Spell_Of_Return` | fortress/capital fields | writes (relocate; active-reset gap) |
| `Spell_Of_Mastery` | `GAME_SoM_Cast_By` | writes |
| `Spell_Of_Mastery` | `Spell_Of_Mastery_Lose` | calls when caster is AI |
| `Spell_Of_Mastery` / `Spell_Of_Mastery_Lose` | `GAME_OVER()` | dispatches-to (commented) |
| `casting_spell_idx == Return` | `WIZ_Conquer__WIP`, `Spell_Of_Mastery` | gates (skip banished/returning wizard) |
