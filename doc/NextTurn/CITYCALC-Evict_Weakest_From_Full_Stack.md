CITYCALC-Evict_Weakest_From_Full_Stack.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr120\UNIT_RemoveExcess.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr120\UNIT_RemoveExcess.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Apply_City_Changes()
            |-> City_Apply_Production()
                |-> Evict_Weakest_From_Full_Stack()

---

# `Evict_Weakest_From_Full_Stack` — Walkthrough

**Location:** [CITYCALC.c:1407-1445](../../MoM/src/CITYCALC.c#L1407-L1445) (~39 lines, ends [line 1445](../../MoM/src/CITYCALC.c#L1445)).
**WZD overlay:** ovr120, p26 (IDA address `ovr120:1E02`).
**On-disk OG name:** the disassembly/decompilation files retain the original drake178 name `UNIT_RemoveExcess` — IDA output naming is decoupled from the renamed production symbol.

## Purpose

A general post-unit-creation helper: whenever a routine drops a new unit onto a map square, that square's stack can exceed the `MAX_STACK` limit of 9. `Evict_Weakest_From_Full_Stack` scans every unit on the given unit's square, finds the **least valuable** one, and removes it — pushing it off to an adjacent square (owned units) or dismissing/killing it outright (neutral units).

Despite the original "RemoveExcess" name, it removes **exactly one** unit. Because each spawn adds a single unit, the square is only ever one over the cap, so shedding the weakest restores it.

"Value" is a cheap upkeep-plus-experience heuristic: `Unit_Gold_Upkeep + Unit_Mana_Upkeep + Level`. The lowest-scoring unit is the one shed. Ties resolve to the **last** unit scanned (the comparison is `<=`).

The function only acts when the stack is *over* the cap (`troop_count > MAX_STACK`); at or below the cap it does nothing.

## Called from

~13 sites — essentially any routine that spawns a unit onto a square:

| Site | Context |
|---|---|
| [`City_Apply_Production`](../../MoM/src/NEXTTURN.c#L2181) | city finishes building a unit (the NextTurn path this doc tracks). |
| [NEXTTURN.c:3482](../../MoM/src/NEXTTURN.c#L3482) | overland spell-cast unit summon during end-of-turn. |
| [HIRE.c:663](../../MoM/src/HIRE.c#L663), [940](../../MoM/src/HIRE.c#L940) | hiring heroes / mercenaries at the fortress. |
| [Combat.c:20472](../../MoM/src/Combat.c#L20472) | combat-side unit placement / summon. |
| [OverSpel.c:843](../../MoM/src/OverSpel.c#L843) | overland summon spell. |
| [Spells130.c:1578](../../MoM/src/Spells130.c#L1578), [Spells132.c:157](../../MoM/src/Spells132.c#L157) / [345](../../MoM/src/Spells132.c#L345) / [535](../../MoM/src/Spells132.c#L535) / [578](../../MoM/src/Spells132.c#L578) | spell-summoned units. |

Nearly all pass `(_units - 1)` — the slot of the just-created unit.

## Signature

```c
void Evict_Weakest_From_Full_Stack(int16_t unit_idx)
```

`unit_idx` is the just-created unit; the function reads its `wx/wy/wp` to locate the square to scan. Note `unit_idx` itself is not necessarily the unit removed — the lowest-value unit on the square is.

## Globals read / written

| Symbol | Access | Effect |
|---|---|---|
| `_UNITS[]` (`s_UNIT`) | read | `wx/wy/wp` of `unit_idx`; `Level` and `owner_idx` of each unit on the square. |

External calls: `Army_At_Square_1` (enumerate units on the square), `Unit_Gold_Upkeep`, `Unit_Mana_Upkeep`, `UNIT_LoggedPushOff` (relocate an owned unit), `Kill_Unit` (dismiss a neutral unit).

## Locals

```c
int16_t troops[10];             /* Army_At_Square_1 output buffer; ReMoM inits to 0xBB sentinels (HACK, not OG) */
int16_t unit_wp, unit_wy, unit_wx;  /* copied from _UNITS[unit_idx] — dead stores (see note) */
int16_t troop_count;            /* Army_At_Square_1 output count */
int16_t lowest_trooper_idx;     /* index of the least-valuable unit; seeded to -999 */
int16_t trooper_value;          /* per-unit upkeep+Level score */
int16_t lowest_trooper_value;   /* running minimum; seeded to 999 */
int16_t itr_troops;             /* si — scan index */
```

The `{ 0xBB, ... }` `troops[]` initializer is a ReMoM debug sentinel — OG leaves the buffer uninitialized (`Army_At_Square_1` fills the first `troop_count` entries).

## Code walk

Line refs are production [CITYCALC.c](../../MoM/src/CITYCALC.c); cross-checked against `UNIT_RemoveExcess.asm` (164 lines, on-disk OG filename).

### Setup ([1418-1423](../../MoM/src/CITYCALC.c#L1418-L1423))

Seed `lowest_trooper_idx = -999`, `lowest_trooper_value = 999` (asm:21-22). Copy `unit_wx/wy/wp` from `_UNITS[unit_idx]` (asm:23-46), then `Army_At_Square_1(UNITX(), UNITY(), UNITP(), &troop_count, &troops[0])` (asm:47-77) to enumerate the stack on that square.

> **Dead-store note (faithful):** the call passes the `UNITX()/UNITY()/UNITP()` macros, which re-read `_UNITS[unit_idx].wx/wy/wp` directly rather than the `unit_wx/wy/wp` locals just assigned. OG does exactly the same — asm:23-46 store the locals, then asm:51-74 **re-read** the struct for the call arguments, leaving the locals dead. Production's macro form matches the asm; the decompiled `.c`, which passes the locals, is the divergent (simplified) rendering.

### Over-cap guard ([1424](../../MoM/src/CITYCALC.c#L1424), asm:78-80)

`if (troop_count > MAX_STACK)` — `MAX_STACK = 9`. At or under the cap the function returns without touching anything.

### Find the least-valuable unit ([1426-1435](../../MoM/src/CITYCALC.c#L1426-L1435), asm:82-136)

For each unit on the square: `trooper_value = Unit_Gold_Upkeep + Unit_Mana_Upkeep + Level` (asm:86-120). Keep the running minimum in `lowest_trooper_value` / `lowest_trooper_idx`; the update guard is `trooper_value <= lowest_trooper_value` (asm:121-123 `jg` skips), so **ties keep the later-scanned unit**.

### Remove it ([1436-1443](../../MoM/src/CITYCALC.c#L1436-L1443), asm:137-157)

```c
if(_UNITS[lowest_trooper_idx].owner_idx != NEUTRAL_PLAYER_IDX)
{
    UNIT_LoggedPushOff(lowest_trooper_idx);
}
else
{
    Kill_Unit(lowest_trooper_idx, kt_Dismissed);
}
```

- **Non-neutral** → `UNIT_LoggedPushOff` (relocate to an adjacent square, keeping the unit alive).
- **Neutral** (`owner_idx == 5`) → `Kill_Unit(…, kt_Dismissed)` — `kt_Dismissed = 1`, dismissible/resurrectable.

The asm (asm:142-143) is `cmp owner_idx, e_NEUTRAL_PLAYER_IDX; jnz <PushOff>` — neutral falls through to `Kill_Unit(idx, 1)`, non-neutral jumps to `UNIT_LoggedPushOff`. Production's `!=`-first structure mirrors that `jnz`-to-PushOff sense exactly.

## Faithfulness notes (production correct; decompiled `.c` diverges)

Production is 1:1 with the asm throughout. `e_NEUTRAL_PLAYER_IDX = 5` (`Piethawn/in/WIZARDS.inc:9184`), matching production's `NEUTRAL_PLAYER_IDX (5)`.

One spot where the drake178 `.c` is a simplified rendering and production matches the asm — recorded so it doesn't get "corrected" toward the `.c`:

- **Square coords are re-read at the call, not passed from locals.** See the dead-store note above — production's `UNITX()/UNITY()/UNITP()` matches the asm; the `.c` passing `unit_wx/wy/wp` is the simplified rendering.

## OG quirks preserved (faithful — do not "fix")

- **Dead `unit_wx/wy/wp` stores** ([1420-1422](../../MoM/src/CITYCALC.c#L1420-L1422)) — computed then bypassed by the `UNITX()/UNITY()/UNITP()` re-reads at the call. OG does the same (asm:23-46 store, asm:51-74 re-read). Preserved.
- **`<=` tie-break keeps the later unit** ([1430](../../MoM/src/CITYCALC.c#L1430), asm:121-123) — among equal-value units the last one scanned is the one removed. Faithful.
- **Seed sentinels `-999` / `999`** ([1418-1419](../../MoM/src/CITYCALC.c#L1418-L1419)) — if the loop somehow found nothing (cannot happen when `troop_count > 9`), `lowest_trooper_idx` would stay `-999`; the over-cap guard makes that unreachable. Preserved as-is.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr120\UNIT_RemoveExcess.asm` — IDA Pro 5.5 disassembly (the authority, 164 lines). On-disk filename retains the drake178 name `UNIT_RemoveExcess`.
- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr120\UNIT_RemoveExcess.c` — drake178 decompilation (call passes locals rather than the struct re-reads the asm does; the asm and production are correct).
- `Piethawn/in/WIZARDS.inc:9184` — `e_NEUTRAL_PLAYER_IDX = 5` (authoritative IDA equate).
- [`NEXTTURN-City_Apply_Production.md`](NEXTTURN-City_Apply_Production.md) — the NextTurn-path caller; invokes `Evict_Weakest_From_Full_Stack(_units - 1)` right after building a unit.
- Constants: `MAX_STACK = 9`, `NEUTRAL_PLAYER_IDX = 5`, `kt_Dismissed = 1` ([CITYCALC.h:25](../../MoM/src/CITYCALC.h#L25)).
- Tracked as a downstream helper of Wave 9B in [`__TODO-NextTurn.md`](../__TODO-NextTurn.md).
