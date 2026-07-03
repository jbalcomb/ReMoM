NEXTTURN-Cool_Off_Volcanoes.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr140\Cool_Off_Volcanoes.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr140\Cool_Off_Volcanoes.c     (Gemini translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Cool_Off_Volcanoes()       NEXTTURN
            |-> Set_Terrain_Type_Mountain()
            |-> Volcano_Counts()

---

# `Cool_Off_Volcanoes` — Walkthrough

**Location:** [NEXTTURN.c:4525-4552](../../MoM/src/NEXTTURN.c#L4525-L4552) (~28 lines, ends [line 4552](../../MoM/src/NEXTTURN.c#L4552)).
**WZD overlay:** ovr140 (o140p26).
**Called from:** [`Next_Turn_Calc`](../../MoM/src/NEXTTURN.c#L616) — normal end-of-turn, call site [NEXTTURN.c:796](../../MoM/src/NEXTTURN.c#L796) (Wave 8C).

> Verified against the **`.asm`** (75 lines). The `.c` is a Gemini translation of that asm; the asm bytes decide. **Production is 1:1 with the asm — no divergences, no deviations.**

## Purpose

Once per turn, every Volcano tile on both planes has a small chance to cool into a Mountain. After the sweep, `Volcano_Counts` re-tallies volcano ownership (the per-wizard volcano bookkeeping used for the Volcano overland spell's power/decay).

## Signature

```c
void Cool_Off_Volcanoes(void)
```

Locals: `terrain_type`, `itr_wp`, `itr_wy` (`_DI_`), `itr_wx` (`_SI_`).

## Code walk

Line refs are production [NEXTTURN.c](../../MoM/src/NEXTTURN.c); cross-checked against `Cool_Off_Volcanoes.asm`.

1. **Triple loop over the whole map** ([4532-4536](../../MoM/src/NEXTTURN.c#L4532-L4536), asm:13-67) — `itr_wp` (plane, `< NUM_PLANES` = asm literal `2`, asm:66) outer, `itr_wx` (`< WORLD_WIDTH`, asm:62) middle, `itr_wy` (`< WORLD_HEIGHT`, asm:58) inner. Loop nesting/order match the asm exactly.
2. **Read + mask the terrain type** ([4538](../../MoM/src/NEXTTURN.c#L4538), asm:25-41) — the asm computes the `_world_maps` word offset (`itr_wp * WORLD_SIZE + itr_wy * WORLD_WIDTH + itr_wx`, in words), loads the raw terrain word, and takes `% _TerType_Count` (the `div` remainder in `dx`) to strip the high flag bits down to the base terrain id. Production expresses this as `GET_TERRAIN_TYPE(itr_wx, itr_wy, itr_wp) % _TerType_Count`.
3. **Cool a Volcano** ([4540-4546](../../MoM/src/NEXTTURN.c#L4540-L4546), asm:42-54) — if `terrain_type == tt_Volcano` (asm:42-43) and `Random(100) < 3` (asm:44-49), call `Set_Terrain_Type_Mountain(itr_wx, itr_wy, itr_wp)`. The asm pushes `itr_wp`, `itr_wy`, `itr_wx` (right-to-left cdecl) → argument order `(itr_wx, itr_wy, itr_wp)`, matching production.
4. **Re-tally** ([4551](../../MoM/src/NEXTTURN.c#L4551), asm:68) — `Volcano_Counts()` after the sweep.

## Faithful (do not "fix")

- **`Random(100) < 3` is a 2% chance, not 3%.** `Random(n)` is **1-based** (`1..100`), so `< 3` fires only on results `1` or `2` → `2/100`. Faithful to asm:44-49 (`cmp ax, 3; jge skip`). Do not "correct" the literal to make it read as 3%.
- **`% _TerType_Count` mask** — the raw `_world_maps` word carries terrain flags in its high bits; the modulo reduces it to the base terrain id before the `tt_Volcano` compare (asm:38-41). Faithful.

## Related references

- `…\ovr140\Cool_Off_Volcanoes.asm` — IDA Pro 5.5 disassembly (ground truth, 75 lines).
- `…\ovr140\Cool_Off_Volcanoes.c` — Gemini translation of the asm.
- Callees: `Set_Terrain_Type_Mountain`, `Volcano_Counts` ([WZD_o143.c:100](../../MoM/src/WZD_o143.c#L100)), `Random` (1-based `1..n`).
- Constants/fields: `NUM_PLANES = 2`, `WORLD_WIDTH`, `WORLD_HEIGHT`, `WORLD_SIZE`, `_TerType_Count`, `tt_Volcano`, `_world_maps`, `GET_TERRAIN_TYPE`.
- Tracked as **Wave 8C** in [`__TODO-NextTurn.md`](../__TODO-NextTurn.md).
