CITYCALC-City_Growth_Rate.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr142\City_Growth_Rate.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr142\City_Growth_Rate.c     (Gemini translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Apply_City_Changes()        NEXTTURN
            |-> City_Growth_Rate()      CITYCALC
                |-> City_Maximum_Size() CITYCALC

---

# `City_Growth_Rate` — Walkthrough

**Location:** [CITYCALC.c:2573-2717](../../MoM/src/CITYCALC.c#L2573-L2717) (~145 lines, ends [line 2717](../../MoM/src/CITYCALC.c#L2717); Doxygen block precedes the body).
**WZD overlay:** ovr142, p15 (drake178 IDA label: `CTY_GetPopGrowth`).
**Called from:** `Apply_City_Changes` and `City_Screen_Draw2__WIP` (per the asm XREF comment).

> Verified against the **`.asm`** (401 lines). The `.c` is a Gemini translation of that asm; the asm bytes decide. **Production is 1:1 with the asm — no divergences.** Builds clean (Debug, MSVC). The three drake178 `BUGBUG` notes are faithful OG behavior and stay.

## Purpose

Returns a city's per-turn **population growth rate** (may be negative). Short-circuits for empty / at-max / zero-max / starving cities, then computes a base rate from the food headroom, adds Granary / Farmers' Market bonuses, doubles for Stream of Life and Population Boom, applies a percentage `population_growth_modifier` (Housing project, Dark Rituals, Sawmill, Builders' Hall), and finally scales by neutral / AI-difficulty rules and floors at 0. The header comment warns it "contains multiple bugs that can prevent negative total growth from being applied" — see the preserved BUGBUGs.

## Signature

```c
int16_t City_Growth_Rate(int16_t city_idx)
```

Locals: `maximum_size`, `population_growth_modifier`, `population_growth_rate` (`_DI_`).

## Code walk

Line refs are production [CITYCALC.c](../../MoM/src/CITYCALC.c); cross-checked against `City_Growth_Rate.asm`. Each short-circuit uses `goto Done` to skip the shared modifier/owner/floor block, mirroring the asm's direct `@@JmpDone` / `@@JmpJmpDone_Return_*` jumps.

1. **Empty city** ([2581-2585](../../MoM/src/CITYCALC.c#L2581-L2585), asm:20-25) — `population == 0` → growth `0`, `goto Done`.
2. **`maximum_size = City_Maximum_Size(city_idx)`** ([2586](../../MoM/src/CITYCALC.c#L2586), asm:27-32).
3. **At max (BUG #1)** ([2589-2593](../../MoM/src/CITYCALC.c#L2589-L2593), asm:33-42) — `population == maximum_size` → growth `0`, `goto Done`.
4. **Zero max** ([2595-2599](../../MoM/src/CITYCALC.c#L2595-L2599), asm:44-55) — `maximum_size == 0` → growth `-population`, `goto Done` (skips the floor, so the negative survives).
5. **Starving** ([2601-2605](../../MoM/src/CITYCALC.c#L2601-L2605), asm:57-94) — `food_units < population` → growth `(food_units - population) * 5`, `goto Done` (negative survives).
6. **Base growth** ([2608](../../MoM/src/CITYCALC.c#L2608), asm:96-125) — `((maximum_size - population) + 1) / 2 + _race_type_table[race].Growth_Mod` (the asm's `cwd; sub ax,dx; sar ax,1` is signed `/ 2`).
7. **Granary +2 / Farmers' Market +3** ([2610-2626](../../MoM/src/CITYCALC.c#L2610-L2626), asm:126-158) — `bldg_status[GRANARY]` (`0x1D`) and `[FARMERS_MARKET]` (`0x1E`), each `bs_Built` or `bs_Replaced`.
8. **At 25 (BUG #2)** ([2629-2633](../../MoM/src/CITYCALC.c#L2629-L2633), asm:159-167) — `population >= MAX_CITY_POPULATION (25)` → growth `0`, `goto Done`.
9. **Stream of Life ×2 / Population Boom ×2** ([2635-2643](../../MoM/src/CITYCALC.c#L2635-L2643), asm:169-191).
10. **Modifier accrual** ([2650-2692](../../MoM/src/CITYCALC.c#L2650-L2692), asm:193-301) — `population_growth_modifier = 0`; `-25` Dark Rituals; then **only if `construction == bt_Housing`**: `+50` at pop 1 else `((pop − rebels − farmers) × 100 / pop)`, `+10` Sawmill (`0x0F`), `+15` Builders' Hall (`0x20`).
11. **At 25 in Housing (BUG #3)** ([2688-2691](../../MoM/src/CITYCALC.c#L2688-L2691), asm:302-310) — inside the Housing branch, `population >= 25` → growth `0`.
12. **Apply modifier + owner scaling + floor** ([2697-2714](../../MoM/src/CITYCALC.c#L2697-L2714), asm:312-390) — reached by non-Housing and Housing-`pop<25` alike (the asm's shared `loc_C8B28`): `rate = rate × (modifier + 100) / 100`; neutral → `/2` then `0` if `population >= (_difficulty+1)×2`; other AI → `× difficulty_modifiers_table[_difficulty].population_growth / 100`; human unscaled; `SETMIN(rate, 0)`.
13. **Return** ([2715-2716](../../MoM/src/CITYCALC.c#L2715-L2716), asm:388-393).

## OG quirks preserved (faithful — do not "fix")

- **BUG #1** ([2589](../../MoM/src/CITYCALC.c#L2589), asm:38-42) — `population == maximum_size` forces growth `0`, so a city exactly at max can never shrink. drake178-flagged; matches asm.
- **BUG #2** ([2629](../../MoM/src/CITYCALC.c#L2629), asm:165-167) — `population >= 25` forces growth `0` before the enchantment/modifier passes. drake178-flagged; matches asm.
- **BUG #3** ([2688](../../MoM/src/CITYCALC.c#L2688), asm:308-310) — inside the Housing branch, `population >= 25` again forces `0` "even with zero workers and no buildings." drake178-flagged; matches asm. It sets `population_growth_rate = 0` and falls through the shared block (rather than `goto Done`); functionally identical, since `0` propagates through `(0×(mod+100))/100`, the neutral `/2`, the AI scale, and `SETMIN(0,0)`.
- **Signed `/ 2` idioms** ([2608](../../MoM/src/CITYCALC.c#L2608) base, [2701](../../MoM/src/CITYCALC.c#L2701) neutral) — the asm's `cwd; sub ax,dx; sar ax,1` is C `/ 2` truncating toward zero. Faithful.
- **Granary/Farmers'/Sawmill/Builders' each accept `bs_Built` OR `bs_Replaced`** — matches the asm's two-compare `jz/jnz` per building.

## Related references

- `…\ovr142\City_Growth_Rate.asm` — IDA Pro 5.5 disassembly (ground truth, 401 lines).
- `…\ovr142\City_Growth_Rate.c` — Gemini translation of the asm.
- Callees: [`City_Maximum_Size`](../../MoM/src/CITYCALC.c#L1912) ([`CITYCALC-City_Maximum_Size.md`](CITYCALC-City_Maximum_Size.md)), [`City_Rebel_Count`](../../MoM/src/CITYCALC.c).
- Constants/fields: `_CITIES[].{population, food_units, farmer_count, race, construction, owner_idx, bldg_status[], enchantments[]}`, `MAX_CITY_POPULATION = 25`, `GRANARY = 0x1D`, `FARMERS_MARKET = 0x1E`, `SAWMILL = 0x0F`, `BUILDERS_HALL = 0x20`, `bt_Housing`, `bs_Built`, `bs_Replaced`, `STREAM_OF_LIFE`, `DARK_RITUALS`, `NEUTRAL_PLAYER_IDX = 5`, `HUMAN_PLAYER_IDX = 0`, `_race_type_table[].Growth_Mod`, `difficulty_modifiers_table[].population_growth`, `events_table->{Population_Boom_Status, Population_Boom_Data}` — building indices in [MOM_DEF.h](../../MoX/src/MOM_DEF.h), `MAX_CITY_POPULATION` in [MOX_DEF.h:718](../../MoX/src/MOX_DEF.h#L718).
