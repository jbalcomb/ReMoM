CITYCALC-City_Food_WildGame.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr142\City_Food_WildGame.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr142\City_Food_WildGame.c     (Gemini translation of the .asm — second opinion)

City_Growth_Rate()
    |-> City_Maximum_Size()
        |-> City_Food_Terrain()                 CITYCALC
        |-> Map_Square_Food2()                  Terrain
        |    |-> City_Area_Square_Is_Shared()   CITYCALC
        |-> City_Food_WildGame()                CITYCALC

---

# `City_Food_WildGame` — Walkthrough

**Location:** [CITYCALC.c:1663-1710](../../MoM/src/CITYCALC.c#L1663-L1710) (~48 lines, ends [line 1710](../../MoM/src/CITYCALC.c#L1710)).
**WZD overlay:** ovr142, p07 (drake178 IDA label: `CTY_GetWildGameFood`).
**Called from:** [`City_Maximum_Size`](../../MoM/src/CITYCALC.c#L1960) (max-size food), the max-city-size food accumulation at [1904](../../MoM/src/CITYCALC.c#L1904), the required-farmer-food calc at [3077](../../MoM/src/CITYCALC.c#L3077), and the city-stat struct fill at [4535](../../MoM/src/CITYCALC.c#L4535) (`Wild_Games`).

> Verified against the **`.asm`** (128 lines). No Gemini `.c` exists for this function, so the asm bytes are the sole reference. **Production is 1:1 with the asm — no divergences.**

## Purpose

Sums the food produced by **wild-game** terrain specials across a city's usable catchment squares. Each usable square carrying the `TS_WILDGAME` special contributes **2 food** if the city owns it outright, or **1 food** (halved) if the square is shared with another city. Returns the total as whole food units.

## Signature

```c
int16_t City_Food_WildGame(int16_t city_idx)
```

Locals: `wx_array`/`wy_array` (catchment coords, `CITY_AREA_SIZE`), `useable_map_squares`, `bit_index`, `city_wp`, `food_units` (accumulator, `_DI_`), `itr` (`_SI_`), `bit_field` (`_DX_`).

## Code walk

Line refs are production [CITYCALC.c](../../MoM/src/CITYCALC.c); cross-checked against `City_Food_WildGame.asm`.

1. **City plane + catchment** ([1674-1677](../../MoM/src/CITYCALC.c#L1674-L1677), asm:18-52) — `city_wp = _CITIES[city_idx].wp`; `useable_map_squares = Get_Useable_City_Area(CITYX(), CITYY(), city_wp, &wx_array, &wy_array)` (corruption-excluded coordinate list).
2. **Accumulate** ([1679-1706](../../MoM/src/CITYCALC.c#L1679-L1706), asm:53-118) — `food_units = 0`; for each `itr` in `[0, useable_map_squares)`:
   - **Wild-game gate** ([1684](../../MoM/src/CITYCALC.c#L1684), asm:57-80) — `GET_TERRAIN_SPECIAL(wx, wy, city_wp) & TS_WILDGAME`; skip the square if unset (`jz loc_C79F9`).
   - **Shared-bit lookup** ([1687-1689](../../MoM/src/CITYCALC.c#L1687-L1689), asm:81-102) — `bit_index = wy*WORLD_WIDTH + wx`; `bit_field = city_area_shared_bits + city_wp*WORLD_SIZE`.
   - **Owned vs shared** ([1691-1702](../../MoM/src/CITYCALC.c#L1691-L1702), asm:104-112) — `Test_Bit_Field(bit_index, bit_field) == ST_FALSE` (not shared) → `food_units += 2`; else (shared) → `food_units += 1`.
3. **Return** ([1708](../../MoM/src/CITYCALC.c#L1708), asm:120-121) — `return food_units`.

## Notes (faithful — do not "fix")

- **`ST_FALSE` branch mapping** ([1691-1702](../../MoM/src/CITYCALC.c#L1691-L1702)) — the asm's `or ax,ax; jz loc_C79F6` sends the **zero** (not-shared) case to `add di, 2` (asm:111-112) and falls through on non-zero to `inc di` (asm:108). Production's `if(… == ST_FALSE) food_units += 2; else food_units += 1;` matches exactly — the `+= 2` is the shorter jump target, not the fall-through. (Flagged because this is a JZ-to-skip spot where an inversion would be easy.)
- **Shared square gives half** — a wild-game square shared with another city yields 1 food instead of 2 (the standard MoM catchment-sharing split), driven by the `city_area_shared_bits` field, not a divide.
- **No Gaia's Blessing boost** — unlike [`City_Food_Terrain`](../../MoM/src/CITYCALC.c#L1656) (which applies `×3/2` for `GAIAS_BLESSING`), wild-game food is not scaled by any enchantment. The asm has no such check; faithful, not a bug.
- **Returns whole food units** — no `×2`/`/2` scaling. Contrast `City_Food_Terrain`, which works in a doubled `food_units_x2` accumulator and divides down on return.

## Related references

- `…\ovr142\City_Food_WildGame.asm` — IDA Pro 5.5 disassembly (ground truth, 128 lines). No Gemini `.c` counterpart.
- Callees: [`Get_Useable_City_Area`](../../MoM/src/CITYCALC.c) (corruption-excluded catchment), `GET_TERRAIN_SPECIAL` (macro), `Test_Bit_Field`.
- Sibling food functions in ovr142: [`City_Food_Terrain`](../../MoM/src/CITYCALC.c#L1638) (o142p06), `Square_Food_x2`, `Map_Square_Food2`.
- Constants/fields: `CITY_AREA_SIZE`, `TS_WILDGAME`, `WORLD_WIDTH`, `WORLD_SIZE`, `city_area_shared_bits`, `ST_FALSE`, `_CITIES[].wp/wx/wy`, `GAIAS_BLESSING` (not used here).
