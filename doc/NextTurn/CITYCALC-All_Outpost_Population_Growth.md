CITYCALC-All_Outpost_Population_Growth.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr142\All_Outpost_Population_Growth.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr142\All_Outpost_Population_Growth.c     (Gemini translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> All_Outpost_Population_Growth()

---

# `All_Outpost_Population_Growth` — Walkthrough

**Location:** [CITYCALC.c:3225-3304](../../MoM/src/CITYCALC.c#L3225-L3304) (~80 lines, ends [line 3304](../../MoM/src/CITYCALC.c#L3304)).
**WZD overlay:** ovr142, p21 (drake178 IDA label: `CTY_OutpostGrowth`).
**Called from:** [`Next_Turn_Calc`](../../MoM/src/NEXTTURN.c#L829) — normal end-of-turn (Wave 9A).

> Verified against the **`.asm`** (254 lines). The `.c` is a Gemini translation of that asm; the asm bytes decide. Production is now 1:1 with the asm — the four divergences found during review (inverted outpost test, the terrain-index OGBUG that had been silently "fixed", the 16-bit grow multiply, and `Random(3)` shrink) are all resolved. (The doc-stub header's `ovr157` path was wrong; the files are in `ovr142`.)

## Purpose

Grows or shrinks each **outpost**'s fractional population (`Pop_10s`) once per turn (graduation of an outpost to a full city is *not* here — that's `Apply_City_Changes`). For each outpost it builds a `grow` percentage (food terrain + racial rate + Gaia's Blessing / Stream of Life + mineral terrain specials in the city area, scaled by the difficulty modifier) and a `shrink` percentage (base 5% + Evil Presence / Pestilence / Famine / Chaos Rift), then rolls `Random(100)` against each and nudges `Pop_10s` up by `Random(3)` and/or down by `Random(2)`.

Two preserved drake178 BUG notes ride in the header comment: it **ignores Wild Games** entirely, and it applies the **difficulty-based growth modifier to human outposts too** (not just AI). Both are OG behavior.

## Signature

```c
void All_Outpost_Population_Growth(void)
```

## Code walk

Line refs are production [CITYCALC.c](../../MoM/src/CITYCALC.c); cross-checked against `All_Outpost_Population_Growth.asm`.

1. **`EMMDATAH_Map()` + city loop** ([3236-3237](../../MoM/src/CITYCALC.c#L3236-L3237), asm:18-20) — maps the EMM page frame, then iterates `_CITIES`.
2. **Outpost filter** ([3239-3243](../../MoM/src/CITYCALC.c#L3239-L3243), asm:28-30) — `if(population != 0) continue;` — process only outposts (`population == 0`); `jz` → process in the asm.
3. **`grow` accrual** ([3244-3276](../../MoM/src/CITYCALC.c#L3244-L3276), asm:33-146):
   - `City_Food_Terrain` + racial `outpost_growth_rate` ([3244-3245](../../MoM/src/CITYCALC.c#L3244-L3245)).
   - Gaia's Blessing +20, Stream of Life +10 ([3246-3253](../../MoM/src/CITYCALC.c#L3246-L3253)).
   - `Get_Useable_City_Area` → arrays, then per useable square: `+5` for Iron/Silver, else `+10` for any other special ([3254-3276](../../MoM/src/CITYCALC.c#L3254-L3276)). **OGBUG here — see quirks.**
4. **`shrink` accrual** ([3277-3293](../../MoM/src/CITYCALC.c#L3277-L3293), asm:147-182) — base 5 + Evil Presence (+5) + Pestilence (+10) + Famine (+10) + Chaos Rift (+10).
5. **Difficulty scale + rolls** ([3294-3302](../../MoM/src/CITYCALC.c#L3294-L3302), asm:184-240) — `grow = (int32_t)outpost_growth_modifier * grow / 100`; `Random(100) <= grow` → `Pop_10s += Random(3)`; `Random(100) <= shrink` → `Pop_10s -= Random(2)`.

## OG quirks preserved (faithful — do not "fix")

- **Terrain scan indexes `[itr_cities]`, not `[itr]`** ([3258-3260](../../MoM/src/CITYCALC.c#L3258-L3260)) — the inner loop counts with `itr`, but the asm indexes the city-area arrays with the *outer* counter `_SI_itr_cities` (asm:104-108, 119-123). OG therefore re-checks the single square `wx_array[itr_cities]`/`wy_array[itr_cities]` every iteration, and reads out of bounds once `itr_cities >= CITY_AREA_SIZE (25)`. Preserved with the `/* OGBUG should use itr… */` and `/* OGBUG OOB… */` comments. Do not change to `itr`.
- **Ignores Wild Games; difficulty modifier applied to human outposts too** ([3222-3223 header](../../MoM/src/CITYCALC.c#L3222)) — preserved drake178 BUG notes; OG behavior.
- **Asymmetric roll amounts** ([3297](../../MoM/src/CITYCALC.c#L3297), [3301](../../MoM/src/CITYCALC.c#L3301)) — grow uses `Random(3)`, shrink uses `Random(2)` (asm:210 vs asm:229); matches the header comment `grow {3}` / `shrink {2}`. Faithful.

## Related references

- `…\ovr142\All_Outpost_Population_Growth.asm` — IDA Pro 5.5 disassembly (ground truth, 254 lines).
- `…\ovr142\All_Outpost_Population_Growth.c` — Gemini translation of the asm (agrees with the asm; production now matches both, with the terrain-index OGBUG deliberately preserved).
- `City_Food_Terrain`, `Get_Useable_City_Area`, `GET_TERRAIN_SPECIAL` — separate reviews. `Apply_City_Changes` handles outpost→city graduation, not this.
- Constants: `CITY_AREA_SIZE = 25`, `TS_IRON`, `TS_SILVER`, `WORLD_WIDTH = 60`, `WORLD_SIZE = 2400`; enchantment ids `GAIAS_BLESSING`, `STREAM_OF_LIFE`, `EVIL_PRESENCE`, `PESTILENCE`, `FAMINE`, `CHAOS_RIFT`.
- Tracked as **Wave 9A** in [`__TODO-NextTurn.md`](../__TODO-NextTurn.md).
