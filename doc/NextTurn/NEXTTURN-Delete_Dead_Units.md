NEXTTURN-Delete_Dead_Units.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr060\Delete_Dead_Units.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr060\Delete_Dead_Units.c

Next_Turn_Proc()
    |-> Delete_Dead_Units()
Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Delete_Dead_Units()

---

# `Delete_Dead_Units` — Walkthrough

**Location:** [NEXTTURN.c:161-197](../../MoM/src/NEXTTURN.c#L161-L197) (~37 lines, ends [line 197](../../MoM/src/NEXTTURN.c#L197)).
**WZD overlay:** ovr060, p01 (IDA `loc_52976`).
**Called from:** `Next_Turn_Proc` (the outer-wrapper bookends), `Next_Turn_Calc` (pre-sim and post-AI-turn), and `Loaded_Game_Update` / `Loaded_Game_Update_WZD` — per the tracker, ~5× per turn.

## Purpose

Compacts the global `_UNITS[]` array by removing every dead/invalid unit and shifting the survivors down to close the gaps. A unit is "dead" when its `owner_idx` falls outside the valid player range `[HUMAN_PLAYER_IDX (0), NEUTRAL_PLAYER_IDX (5)]` — dead units are marked with `owner_idx = ST_UNDEFINED (-1)`, and anything `> 5` is likewise invalid.

Because removing a slot shifts every later unit's index down by one, the function also fixes up the per-wizard **hero back-references** (`_players[].Heroes[].unit_idx`): any hero pointing at a unit above the deleted slot is decremented so it still points at the same unit. `_units` is decremented for each removal.

## Signature

```c
void Delete_Dead_Units(void)
```

## Globals read / written

| Symbol | Access | Effect |
|---|---|---|
| `_units` (`int16_t`) | read + mutated | loop bound; decremented per removed unit. |
| `_UNITS[]` (`s_UNIT[]`) | read + compacted | `owner_idx`/`type` read; `Delete_Structure` shifts the tail down over the removed slot. |
| `_num_players` (`int16_t`) | read | hero-fixup outer loop bound. |
| `_players[].Heroes[].unit_idx` | read + mutated | decremented when it references a unit above the removed slot. |

External calls: `Delete_Structure` (generic array-compaction primitive).

## Locals

```c
int16_t unit_type;    /* set from _UNITS[itr_units].type — dead store, never read (faithful; OG does the same) */
int16_t itr_heroes;   /* hero-fixup inner index */
int16_t itr_units;    /* si — outer scan index */
int16_t itr_players;  /* di — hero-fixup outer index */
```

## Code walk

Line refs are production [NEXTTURN.c](../../MoM/src/NEXTTURN.c); cross-checked against `Delete_Dead_Units.asm` (117 lines).

### Scan + delete loop ([168-188](../../MoM/src/NEXTTURN.c#L168-L188), asm:15-109)

A `while (itr_units < _units)` loop. Per unit:
- `unit_type = _UNITS[itr_units].type` (asm:24-26) — a dead store, kept faithful.
- **Dead test** ([172-176](../../MoM/src/NEXTTURN.c#L172-L176), asm:32-41): `owner_idx < HUMAN_PLAYER_IDX (0)` **||** `owner_idx > NEUTRAL_PLAYER_IDX (5)`. `owner_idx` is `int8_t`, so `< 0` catches `ST_UNDEFINED (-1)`; the two `cmp`/`jl`/`jg`-to-delete tests match the asm.
- **On dead** ([178-185](../../MoM/src/NEXTTURN.c#L178-L185), asm:43-102): `Delete_Structure(itr_units, &_UNITS[0], sizeof(s_UNIT), _units)` shifts the tail down; then the hero-fixup double loop decrements every `_players[p].Heroes[h].unit_idx` that is `!= ST_UNDEFINED && > itr_units` (asm:52-100); then `_units -= 1` and **`continue`** — re-checking the unit that shifted into the current slot without advancing `itr_units`.
- **On live** ([187](../../MoM/src/NEXTTURN.c#L187), asm:104-105): `itr_units++`.

### Debug assertion ([190-195](../../MoM/src/NEXTTURN.c#L190-L195))

A ReMoM-only `#ifdef STU_DEBUG` block asserting no surviving unit has `owner_idx == ST_UNDEFINED`. Not in OG — instrumentation.

## Faithfulness notes

Production is behaviorally 1:1 with the asm. Two rendering details worth recording:

- **The `continue` is a production restructure — it is not in the asm.** The asm has no `continue`; it uses conditional jumps. The delete branch ends `dec [_units]; jmp @@Loop_Units` (asm:101-102) — jumping to the loop test without incrementing `itr_units` — while the live branch does `inc itr_units` (asm:104-105) then falls into the loop test. That control flow can be written equally as the OG `.c`'s `if(dead){…} else {itr_units++;}` (delete block falls out of the loop body; `else` increments) **or** as production's `if(dead){…; continue;} itr_units++;`. Both compile to the same jumps; neither is "more" faithful than the other. Production's early-`continue` (flagged `/* COPILOT */`) is an equivalent-but-restructured rendering, not a literal match to the asm's branch shape.
- **`unit_type` dead store** ([171](../../MoM/src/NEXTTURN.c#L171)) — assigned then never read, in both production and OG (asm:24-26). Preserved.

## OG quirks preserved (faithful — do not "fix")

- **`unit_type` dead store** ([171](../../MoM/src/NEXTTURN.c#L171)) — see above; OG computes it too and never uses it. Not a bug.
- **Hero fixup uses `> itr_units` (strict)** ([181](../../MoM/src/NEXTTURN.c#L181), asm:80-81 `jle` skips) — a hero pointing *at* the deleted slot is not decremented (that hero's unit is the one being removed); only references *above* it shift down. Faithful.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr060\Delete_Dead_Units.asm` — IDA Pro 5.5 disassembly (the authority, 117 lines).
- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr060\Delete_Dead_Units.c` — drake178 decompilation (renders the loop as `if/else`; equivalent to production's `continue`).
- `Delete_Structure` — generic array-element removal / tail-shift primitive (separate review).
- Constants: `HUMAN_PLAYER_IDX = 0`, `NEUTRAL_PLAYER_IDX = 5`, `ST_UNDEFINED = -1`, `NUM_HEROES`.
- Tracked as **Wave 1A** in [`__TODO-NextTurn.md`](../__TODO-NextTurn.md).
