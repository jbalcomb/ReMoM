CITYCALC-Evict_Unit_With_Message.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr120\UNIT_LoggedPushOff.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr120\UNIT_LoggedPushOff.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Apply_City_Changes()
            |-> City_Apply_Production()
                |-> Evict_Weakest_Unit()
                    |-> Evict_Unit_With_Message()

---

# `Evict_Unit_With_Message` — Walkthrough

**Location:** [CITYCALC.c:1376-1400](../../MoM/src/CITYCALC.c#L1376-L1400) (~25 lines, ends [line 1400](../../MoM/src/CITYCALC.c#L1400)).
**WZD overlay:** ovr120, p25 (IDA address `ovr120:1D81`).
**On-disk OG name:** the disassembly/decompilation files retain the original drake178 name `UNIT_LoggedPushOff` — IDA output naming is decoupled from the renamed production symbol.
**Called from:** [`Evict_Weakest_Unit`](../../MoM/src/CITYCALC.c#L1472) — the non-neutral eviction branch (the sole caller).

## Purpose

A thin message-logging wrapper around `Evict_Unit`. It relocates a unit off its current square, and — if that push **destroyed** a human-owned unit (no adjacent square could take it, so `Evict_Unit` sets `owner_idx = ST_UNDEFINED`) — records the loss in the `msg_unit_lost` queue so it shows up in the human player's end-of-turn "units lost" summary.

The `…_With_Message` in the name is exactly this: `Evict_Unit` does the relocation; this wrapper adds the human-facing loss message when the push turns out to be fatal. Non-human losses and successful relocations are not logged.

Whether the owner was human is captured **before** the push (into `owner_was_human_player`), because a fatal push overwrites `owner_idx` with `ST_UNDEFINED` — after which the original owner can no longer be read.

## Signature

```c
void Evict_Unit_With_Message(int16_t unit_idx)
```

## Globals read / written

| Symbol | Access | Effect |
|---|---|---|
| `_UNITS[unit_idx]` (`s_UNIT`) | read (+ mutated by callee) | `owner_idx` read before/after the push; `type` read for the message. `Evict_Unit` mutates position / `owner_idx`. |
| `msg_unit_lost_ctr` | read + increment | lost-unit queue length, cap 20. |
| `msg_unit_lost[]` (`s_Unit_Lost_Msg_Item`) | written | `[msg_unit_lost_ctr].Unit_Type` = unit type, `.Cause` = 3. |

External calls: `Evict_Unit`.

## Locals

```c
int16_t owner_was_human_player;   /* di — captured before the push, since a fatal push clears owner_idx */
```

## Code walk

Line refs are production [CITYCALC.c](../../MoM/src/CITYCALC.c); cross-checked against `UNIT_LoggedPushOff.asm` (67 lines, on-disk OG filename).

### Capture original owner ([1378-1386](../../MoM/src/CITYCALC.c#L1378-L1386), asm:17-24)

`owner_was_human_player = (owner_idx == HUMAN_PLAYER_IDX) ? ST_TRUE : ST_FALSE`. Production writes the condition inverted (`!= HUMAN → ST_FALSE` else `ST_TRUE`) with swapped branches — equivalent to OG's `owner == HUMAN → TRUE else 0` (asm:17-24), behavior-identical.

### Push ([1387](../../MoM/src/CITYCALC.c#L1387), asm:26-28)

`Evict_Unit(unit_idx)` — relocate to an adjacent square, or destroy the unit (`owner_idx = ST_UNDEFINED`) if none is available.

### Log the loss if fatal ([1388-1399](../../MoM/src/CITYCALC.c#L1388-L1399), asm:34-61)

```c
if(
    (_UNITS[unit_idx].owner_idx == ST_UNDEFINED)
    &&
    (owner_was_human_player == ST_TRUE)
    &&
    (msg_unit_lost_ctr < 20)
)
{
    msg_unit_lost[msg_unit_lost_ctr].Unit_Type = _UNITS[unit_idx].type;
    msg_unit_lost[msg_unit_lost_ctr].Cause = 3;
    msg_unit_lost_ctr++;
}
```

Production folds OG's three nested `if`s into one `&&` chain — equivalent to the sequential `jnz/jge` guards at asm:34-39 — then writes `Unit_Type`, `Cause = 3`, and increments the counter (asm:45-61). `owner_idx` is `int8_t` and `ST_UNDEFINED = -1`, matching the asm's signed `cmp owner_idx, -1` (asm:34) — no signed/unsigned trap. `Cause = 3` denotes the push-off loss cause.

## OG quirks preserved (faithful — do not "fix")

- **Owner captured before the push** ([1378-1386](../../MoM/src/CITYCALC.c#L1378-L1386)) — a fatal `Evict_Unit` clears `owner_idx`, so the human check must be taken beforehand. Faithful.
- **Only human, only-fatal pushes are logged** — non-human losses and successful relocations produce no message (asm:34-37 gate on both `owner_idx == -1` and `flag == ST_TRUE`). Faithful.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr120\UNIT_LoggedPushOff.asm` — IDA Pro 5.5 disassembly (the authority, 67 lines). On-disk filename retains the drake178 name `UNIT_LoggedPushOff`.
- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr120\UNIT_LoggedPushOff.c` — drake178 decompilation.
- [`CITYCALC-Evict_Weakest_Unit.md`](CITYCALC-Evict_Weakest_Unit.md) — the sole caller; invokes this on the non-neutral eviction path.
- `Evict_Unit` — the underlying relocation/destroy routine (separate review).
- Constants: `HUMAN_PLAYER_IDX = 0`, `ST_UNDEFINED = -1` ([MOX_BASE.h:75](../../MoX/src/MOX_BASE.h#L75)); `msg_unit_lost` cause `3` = push-off loss.
- Tracked as a downstream helper of Wave 9B in [`__TODO-NextTurn.md`](../__TODO-NextTurn.md).
