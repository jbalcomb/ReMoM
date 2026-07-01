SETTLE-Evict_Unit.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr100\UNIT_PushOffTile.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr100\UNIT_PushOffTile.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Apply_City_Changes()
            |-> City_Apply_Production()
                |-> Evict_Weakest_Unit()
                    |-> Evict_Unit_With_Message()
                        |-> Evict_Unit()

IIF/OON:  |-> Evict_Weakest_Unit()  |-> Evict_Unit_With_Message()  |-> Evict_Unit()
XREF:
    End_Of_Combat__WIP()
    Hire_Hero_Popup()
    AI_Accept_Hero()
    Cast_Word_Of_Recall()
    Cast_Summon_Hero()
    Cast_Incarnation()
    Cast_Resurrection()
    Cast_Spell_Overland()
    City_Apply_Production()
    Determine_Offer()

---

# `Evict_Unit` — Walkthrough

**Location:** [SETTLE.c:446-532](../../MoM/src/SETTLE.c#L446-L532) (~87 lines, ends [line 532](../../MoM/src/SETTLE.c#L532)).
**WZD overlay:** ovr100, p09 (IDA segment `8000:8244C`).
**On-disk OG name:** the disassembly/decompilation files retain the original drake178 name `UNIT_PushOffTile` — IDA output naming is decoupled from the renamed production symbol.
**Called from:** [`Evict_Unit_With_Message`](../../MoM/src/CITYCALC.c#L1387) — the sole caller in production. (The header `XREF:` list is the OG's ~10 call sites; in the current reconstruction only the `Evict_Unit_With_Message` path reaches here.)

## Purpose

Relocates a unit off its current square to a nearby free square, or kills it if none exists. It searches a small neighborhood around the unit for the first square that (a) isn't the active combat square, (b) has no lair, (c) has no rival player's city, and (d) has room for the unit (`Unit_Space_At_Square > 0`). If a suitable square is found the unit is moved there and re-tower-flagged; otherwise `Kill_Unit` destroys it.

This is the relocation primitive underneath the stack-overflow eviction path ([`Evict_Unit_With_Message`](CITYCALC-Evict_Unit_With_Message.md) → here). It lives in SETTLE.c because the same "shove the occupant aside" mechanic is used when a settler founds a city on an occupied square.

## Signature

```c
void Evict_Unit(int16_t unit_idx)
```

## Globals read / written

| Symbol | Access | Effect |
|---|---|---|
| `_UNITS[unit_idx]` (`s_UNIT`) | read + mutated | reads `wx/wy/wp/owner_idx`; on success writes `wx`, `wy` to the target square (mutated further by `Units_In_Tower`); on failure destroyed by `Kill_Unit`. |
| `_combat_wx` / `_combat_wy` | read | the active combat square, excluded as a destination (OG symbol `_combat_wx__som_started_anim_ctr`). |

External calls: `Square_Has_Lair`, `Player_City_At_Square`, `Unit_Space_At_Square`, `Units_In_Tower`, `Kill_Unit`.

## Locals

```c
int16_t troops[2];                 /* 1-element push list handed to Units_In_Tower (slot 0 = unit_idx) */
int16_t target_square_wx, target_square_wy;  /* chosen destination */
int16_t unit_owner_idx, unit_wp, unit_wy, unit_wx;  /* snapshot of the unit's square/owner */
int16_t unit_spaces;               /* Unit_Space_At_Square result for the current candidate */
int16_t found_square;              /* ST_TRUE once a destination is picked */
int16_t curr_wx;                   /* itr_wx wrapped into [0, WORLD_WIDTH) */
int16_t range;                     /* 3 — neighborhood span */
int16_t wx_start, wy_start;        /* top-left of the search box (unit_wx-1, unit_wy-1) */
int16_t itr_wx, itr_wy;            /* scan indices */
```

## Code walk

Line refs are production [SETTLE.c](../../MoM/src/SETTLE.c); cross-checked against `UNIT_PushOffTile.asm` (213 lines, on-disk OG filename).

### Setup + search box ([463-479](../../MoM/src/SETTLE.c#L463-L479), asm:27-80)

Snapshot `wx/wy/wp/owner_idx`. `wy_start = unit_wy - 1`, clamped to `0` if negative (asm:62-67). `wx_start = unit_wx - 1`, **wrapped** `+= WORLD_WIDTH` if negative (asm:69-76) — note the deliberate asymmetry (clamp vs wrap), a preserved OG quirk. `range = 3`, `found_square = ST_FALSE`.

### Neighborhood scan ([480-519](../../MoM/src/SETTLE.c#L480-L519), asm:81-164)

Nested `itr_wy` / `itr_wx` loops over the 3×3 box (`itr < start + range`, matching the asm's `<` bounds), short-circuiting on `found_square`. Per candidate square:
- `itr_wy >= WORLD_HEIGHT` skips the row (asm:85-87); `itr_wx >= WORLD_WIDTH` wraps into `curr_wx = itr_wx - WORLD_WIDTH` (asm:95-103).
- **Skip the combat square** ([496-500](../../MoM/src/SETTLE.c#L496-L500), asm:105-110): proceed if `curr_wx != _combat_wx` **||** `itr_wy != _combat_wy` — i.e. skip only the single exact combat square, matching the asm's two `jnz`-to-proceed tests.
- Skip if `Square_Has_Lair != -1` or `Player_City_At_Square(...owner...) != -1` (asm:116-127); both must return `-1` (no lair, no rival city) to proceed.
- `unit_spaces = Unit_Space_At_Square(...)`; if `> 0`, set `found_square = ST_TRUE`, `target_square_wx/wy = curr_wx/itr_wy` (asm:135-143). First hit wins — `found_square` blocks any later overwrite.

### Relocate or kill ([520-530](../../MoM/src/SETTLE.c#L520-L530), asm:167-204)

`unit_spaces > 0` → write `wx/wy = target_square_wx/wy`, then `Units_In_Tower(1, &troops[0], wp)` re-flags tower occupancy (asm:167-197). Else `Kill_Unit(unit_idx, 1)` (asm:200-204). Note the guard is on `unit_spaces`, not `found_square` — equivalent here since `unit_spaces > 0` is exactly what sets `found_square`.

## OG quirks preserved (faithful — do not "fix")

- **`wy_start` clamps to 0 but `wx_start` wraps `+WORLD_WIDTH`** ([467-476](../../MoM/src/SETTLE.c#L467-L476)) — deliberate asymmetry (asm:62-76). Production preserves the drake178 `/* OGBUG */` note that range-checking belongs on the test coordinates, not on the box origin (shifting the box). Faithful — do not fix.
- **Towers of Wizardry are valid push destinations** ([525](../../MoM/src/SETTLE.c#L525)) — `Units_In_Tower` re-flags occupancy without excluding towers as targets. Preserved drake178 `/* OGBUG */` note; OG-faithful.
- **Post-loop guard on `unit_spaces`, not `found_square`** ([520](../../MoM/src/SETTLE.c#L520)) — matches asm:167 (`cmp unit_spaces, 0`). Equivalent to guarding on `found_square` here. Faithful.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr100\UNIT_PushOffTile.asm` — IDA Pro 5.5 disassembly (the authority, 213 lines). On-disk filename retains the drake178 name `UNIT_PushOffTile`.
- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr100\UNIT_PushOffTile.c` — drake178 decompilation (scan loops `<`, combat skip `== combat && continue` — matches the asm).
- [`CITYCALC-Evict_Unit_With_Message.md`](CITYCALC-Evict_Unit_With_Message.md) — the sole production caller.
- Downstream: `Square_Has_Lair`, `Player_City_At_Square`, `Unit_Space_At_Square` (defined just below in SETTLE.c), `Units_In_Tower`, `Kill_Unit` — separate reviews.
- Constants: `WORLD_WIDTH = 60`, `WORLD_HEIGHT = 40`, `range = 3`, `ST_FALSE = 0`, `ST_TRUE = 1`.
- Tracked as a downstream helper of Wave 9B in [`__TODO-NextTurn.md`](../__TODO-NextTurn.md).
